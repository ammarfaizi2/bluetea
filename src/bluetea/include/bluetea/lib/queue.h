// SPDX-License-Identifier: GPL-2.0
/*
 *  bluetea/include/bluetea/lib/queue.h
 *
 *  Queue library header
 *
 *  Copyright (C) 2021  Ammar Faizi
 */


#ifndef BLUETEA__LIB__ARENA_H
#define BLUETEA__LIB__ARENA_H

#include <stdalign.h>
#include <bluetea/base.h>

#ifdef DONT_INLINE_BT_QUEUE
#  define QUE_INLINE __no_inline
#else
#  define QUE_INLINE __inline
#endif


typedef struct bt_queue_t_ bt_queue_t;
typedef struct bt_qnode_t_ bt_qnode_t;

typedef struct bt_qnode_t_ {
	bt_qnode_t		*next;
	bt_qnode_t		*prev;
	size_t			len;
	alignas(64) char	data[];
} bt_qnode_t;


typedef struct bt_queue_t_ {
	bt_qnode_t		*head;
	bt_qnode_t		*tail;
	uint32_t		count;
	uint32_t		capacity;
} bt_queue_t;


static QUE_INLINE bt_queue_t *bt_queue_init(bt_queue_t *q, uint32_t capacity)
{
	q->head     = NULL;
	q->tail     = NULL;
	q->count    = 0;
	q->capacity = capacity;
	return q;
}


static QUE_INLINE uint32_t bt_queue_count(bt_queue_t *q)
{
	return q->count;
}


static QUE_INLINE size_t bt_qnode_len(bt_qnode_t *node)
{
	return node->len;
}


extern bt_qnode_t *bt_qnode_create(size_t len);


extern bt_qnode_t *bt_queue_enqueue(bt_queue_t *q, const void *data,
				    size_t len);


extern void bt_qnode_delete(bt_qnode_t *q);


extern bt_qnode_t *bt_qnode_detach(bt_queue_t *q, bt_qnode_t *node);


extern void bt_queue_destroy(bt_queue_t *q);


extern bt_qnode_t *bt_queue_dequeue(bt_queue_t *q);


extern void *bt_qnode_data(bt_qnode_t *node);


#undef QUE_INLINE

#endif /* #ifndef BLUETEA__LIB__ARENA_H */
