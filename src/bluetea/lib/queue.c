// SPDX-License-Identifier: GPL-2.0
/*
 *  bluetea/lib/queue.c
 *
 *  Queue library
 *
 *  Copyright (C) 2021  Ammar Faizi
 */

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <bluetea/lib/queue.h>


static inline bt_qnode_t *node_create(size_t len)
{
	void *orig, *user;

	orig = malloc(
		sizeof(bt_qnode_t) + len + 1ul	/* Node size        */
		+ 0x3full			/* Alignment backup */
		+ sizeof(uint8_t)		/* Origin track     */
	);
	if (unlikely(!orig))
		return NULL;

	user = (void *)(
		(
			(uintptr_t)orig
			+ 0x3full 		/* Alignment backup. */
			+ sizeof(uint8_t)	/* Origin track.     */
		)
		& ~0x3full /* Fix the alignment. */
	);

	/* Store how many shift is done after fixing the alignment. */
	((uint8_t *)user)[-1] = (uint8_t)((uintptr_t)user - (uintptr_t)orig);

	return user;
}


static void bt_node_destroy_ignore_ref(bt_qnode_t *node)
{
	void *orig, *user = node;
	uint8_t shift;

	if (unlikely(!node))
		return;

	memcpy(&shift, (void *)((uintptr_t)user - 1), sizeof(shift));
	orig = (void *)((uintptr_t)node - (uintptr_t)shift);
	free(orig);
}


void bt_node_destroy(bt_qnode_t *node)
{
	void *orig, *user = node;
	bt_qnode_t *next, *prev;
	uint8_t shift;

	if (unlikely(!node))
		return;


	if (unlikely(node->next || node->prev)) {
		printf("  FATAL: Deleting referenced node! Issuer: %s:%d\n",
			__FILE__, __LINE__);
		abort();
	}


	memcpy(&shift, (void *)((uintptr_t)user - 1), sizeof(shift));
	orig = (void *)((uintptr_t)node - (uintptr_t)shift);
	free(orig);
}


void bt_node_destroy_ref(bt_queue_t *q, bt_qnode_t *node)
{
	void *orig, *user = node;
	uint8_t shift;

	if (unlikely(!node))
		return;

	if (unlikely(q->count == 0)) {
		printf(" Deleted wrong node! Issuer: %s:%d\n", __FILE__,
			__LINE__);
		abort();
	}

	if (node == q->head && node == q->tail) {
		assert(q->count == 1);
		assert(node->prev == NULL);
		assert(node->next == NULL);
		q->head = NULL;
		q->tail = NULL;
	} else if (node == q->head) {
		assert(node->prev == NULL);
		assert(node->next);
		q->head = q->head->next;
		q->head->prev = NULL;
	} else if (node == q->tail) {
		assert(node->next == NULL);
		assert(node->prev);
		q->tail = q->tail->prev;
		q->tail->next = NULL;
	} else {
		bt_qnode_t *next, *prev;
		next = node->next;
		prev = node->prev;
		assert(next);
		assert(prev);
		next->prev = prev;
		prev->next = next;
	}

	memcpy(&shift, (void *)((uintptr_t)user - 1), sizeof(shift));
	orig = (void *)((uintptr_t)node - (uintptr_t)shift);
	free(orig);
	q->count--;
}


void *bt_node_get_data(bt_qnode_t *node)
{
	return node->data;
}


bt_qnode_t *bt_queue_enqueue(bt_queue_t *q, const void *data, size_t len)
{
	bt_qnode_t *node;

	if (unlikely(q->count >= q->capacity)) {
		errno = EAGAIN;
		return NULL;
	}


	node = node_create(len + 1ul);
	if (unlikely(!node)) {
		errno = ENOMEM;
		return NULL;
	}


	node->next = NULL;
	node->prev = NULL;
	node->len  = len;
	memcpy(node->data, data, len);
	q->count++;

	if (!q->head && !q->tail) {
		/*
		 * It's an empty queue.
		 */
		q->head = node;
		q->tail = node;
	} else {
		/*
		 * Add queue to the tail.
		 */
		assert(q->tail);
		assert(q->head);
		node->prev = q->tail;
		q->tail->next = node;
		q->tail = q->tail->next;
	}

	return node;
}


void bt_queue_destroy(bt_queue_t *q)
{
	bt_qnode_t *tmp, *node;

	if (unlikely(q->count == 0))
		goto out;

	assert(q->head);
	assert(q->tail);

	node = q->head;
	while (node) {
		tmp = node;
		node = node->next;
		bt_node_destroy_ignore_ref(tmp);
	}
out:
	memset(q, 0, sizeof(*q));
}


bt_qnode_t *bt_queue_dequeue(bt_queue_t *__restrict__ q)
{
	bt_qnode_t *ret;

	if (unlikely(q->count == 0)) {
		errno = EAGAIN;
		return NULL;
	}

	ret = q->head;
	if ((q->head = q->head->next))
		q->head->prev = NULL;

	q->count--;
	ret->next = NULL;
	ret->prev = NULL;
	return ret;
}
