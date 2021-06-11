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

	next = node->next;
	prev = node->prev;
	if (prev)
		prev->next = next;
	if (next)
		next->prev = prev;

	memcpy(&shift, (void *)((uintptr_t)user - 1), sizeof(shift));
	orig = (void *)((uintptr_t)node - (uintptr_t)shift);
	free(orig);
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
	for (node = q->head; node;) {
		tmp = node;
		node = node->next;
		bt_node_destroy_ignore_ref(tmp);
	}
out:
	memset(q, 0, sizeof(*q));
	return;
}


bt_qnode_t *bt_queue_dequeue(bt_queue_t *q)
{
	bt_qnode_t *ret;

	if (unlikely(q->count == 0)) {
		errno = EAGAIN;
		return NULL;
	}

	ret = q->head;
	q->head = q->head->next;
	q->count--;

	ret->next = NULL;
	ret->prev = NULL;
	return ret;
}
