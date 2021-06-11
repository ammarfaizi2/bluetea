
#include <stdio.h>
#include <stdlib.h>
#include <bluetea/bluetest.h>

#define DONT_INLINE_BT_QUEUE
#include <bluetea/lib/queue.h>


static BLUETEST(003_queue, init_must_be_empty)
{
	TQ_START;
	bt_queue_t q;
	TQ_ASSERT(bt_queue_init(&q, 100) == &q);
	TQ_ASSERT(bt_queue_count(&q) == 0);
	TQ_RETURN;
}


static BLUETEST(003_queue, enqueue_must_increment)
{
	TQ_START;
	bt_queue_t q;
	TQ_ASSERT_S(bt_queue_init(&q, 100) == &q);
	TQ_ASSERT_S(bt_queue_count(&q) == 0);

	TQ_ASSERT(bt_queue_enqueue(&q, "12345", sizeof("12345")));
	TQ_ASSERT(bt_queue_count(&q) == 1);

	TQ_ASSERT(bt_queue_enqueue(&q, "54321", sizeof("54321")));
	TQ_ASSERT(bt_queue_count(&q) == 2);

	TQ_ASSERT(bt_queue_enqueue(&q, "11111", sizeof("11111")));
	TQ_ASSERT(bt_queue_count(&q) == 3);

	for (size_t i = 3; i < 100; i++) {
		TQ_ASSERT_DYN_S(bt_queue_enqueue(&q, "22222", sizeof("22222")));
		TQ_ASSERT_DYN_S(bt_queue_count(&q) == i + 1);
	}
	TQ_VOID(bt_queue_destroy(&q));
	TQ_RETURN;
}


static BLUETEST(003_queue, dequeue_must_decrement)
{
	TQ_START;
	bt_queue_t q;
	bt_qnode_t *node = NULL;
	TQ_ASSERT_S(bt_queue_init(&q, 100) == &q);
	TQ_ASSERT_S(bt_queue_count(&q) == 0);

	TQ_ASSERT_S(bt_queue_enqueue(&q, "12345", sizeof("12345")));
	TQ_ASSERT_S(bt_queue_count(&q) == 1);

	TQ_ASSERT_S(bt_queue_enqueue(&q, "54321", sizeof("54321")));
	TQ_ASSERT_S(bt_queue_count(&q) == 2);

	TQ_ASSERT_S(bt_queue_enqueue(&q, "11111", sizeof("11111")));
	TQ_ASSERT_S(bt_queue_count(&q) == 3);

	for (size_t i = 3; i < 100; i++) {
		TQ_ASSERT_DYN_S(bt_queue_enqueue(&q, "22222", sizeof("22222")));
		TQ_ASSERT_DYN_S(bt_queue_count(&q) == i + 1);
	}

	TQ_ASSERT(node = bt_queue_dequeue(&q));
	TQ_VOID(bt_qnode_delete(node));
	TQ_ASSERT(bt_queue_count(&q) == 99);

	TQ_ASSERT(node = bt_queue_dequeue(&q));
	TQ_VOID(bt_qnode_delete(node));
	TQ_ASSERT(bt_queue_count(&q) == 98);

	TQ_ASSERT(node = bt_queue_dequeue(&q));
	TQ_VOID(bt_qnode_delete(node));
	TQ_ASSERT(bt_queue_count(&q) == 97);

	for (size_t i = 96; i; i--) {
		TQ_ASSERT_DYN_S(node = bt_queue_dequeue(&q));
		TQ_VOID(bt_qnode_delete(node));
		TQ_ASSERT_DYN_S(bt_queue_count(&q) == i);
	}

	TQ_ASSERT(node = bt_queue_dequeue(&q));
	TQ_VOID(bt_qnode_delete(node));
	TQ_ASSERT(bt_queue_count(&q) == 0);
	TQ_VOID(bt_queue_destroy(&q));
	TQ_RETURN;
}


static BLUETEST(003_queue, maintain_increment_and_decrement)
{
	TQ_START;
	bt_queue_t q;
	bt_qnode_t *node = NULL;
	TQ_ASSERT_S(bt_queue_init(&q, 100) == &q);
	TQ_ASSERT_S(bt_queue_count(&q) == 0);

	TQ_ASSERT(bt_queue_enqueue(&q, "11111", sizeof("11111")));
	TQ_ASSERT(bt_queue_count(&q) == 1);

	TQ_ASSERT(bt_queue_enqueue(&q, "11111", sizeof("11111")));
	TQ_ASSERT(bt_queue_count(&q) == 2);

	TQ_ASSERT(bt_queue_enqueue(&q, "11111", sizeof("11111")));
	TQ_ASSERT(bt_queue_count(&q) == 3);

	TQ_ASSERT(node = bt_queue_dequeue(&q));
	TQ_VOID(bt_qnode_delete(node));
	TQ_ASSERT(bt_queue_count(&q) == 2);

	TQ_ASSERT(bt_queue_enqueue(&q, "11111", sizeof("11111")));
	TQ_ASSERT(bt_queue_count(&q) == 3);

	TQ_ASSERT(node = bt_queue_dequeue(&q));
	TQ_VOID(bt_qnode_delete(node));
	TQ_ASSERT(bt_queue_count(&q) == 2);

	TQ_ASSERT(bt_queue_enqueue(&q, "11111", sizeof("11111")));
	TQ_ASSERT(bt_queue_count(&q) == 3);

	TQ_ASSERT(bt_queue_enqueue(&q, "11111", sizeof("11111")));
	TQ_ASSERT(bt_queue_count(&q) == 4);

	TQ_ASSERT(bt_queue_enqueue(&q, "11111", sizeof("11111")));
	TQ_ASSERT(bt_queue_count(&q) == 5);

	for (size_t i = 0; i < 50; i++) {
		TQ_ASSERT_DYN_S(bt_queue_enqueue(&q, "11111", sizeof("11111")));
		TQ_ASSERT_DYN_S(bt_queue_count(&q) == 5 + i + 1);
	}

	for (size_t i = 0; i < 50; i++) {
		TQ_ASSERT_DYN_S(node = bt_queue_dequeue(&q));
		TQ_VOID(bt_qnode_delete(node));
		TQ_ASSERT_DYN_S(bt_queue_count(&q) == 55 - i - 1);
	}
	TQ_VOID(bt_queue_destroy(&q));
	TQ_RETURN;
}


static BLUETEST(003_queue, get_data_and_len_from_node)
{
	TQ_START;
	bt_queue_t q;
	bt_qnode_t *node = NULL;
	TQ_ASSERT_S(bt_queue_init(&q, 100) == &q);
	TQ_ASSERT_S(bt_queue_count(&q) == 0);

	TQ_ASSERT_S(node = bt_queue_enqueue(&q, "12345", sizeof("12345")));
	TQ_ASSERT_S(bt_queue_count(&q) == 1);
	TQ_ASSERT(bt_qnode_len(node) == sizeof("12345"));
	TQ_ASSERT(!memcmp(bt_qnode_data(node), "12345", sizeof("12345")));

	TQ_ASSERT_S(node = bt_queue_enqueue(&q, "4321", sizeof("4321")));
	TQ_ASSERT_S(bt_queue_count(&q) == 2);
	TQ_ASSERT(bt_qnode_len(node) == sizeof("4321"));
	TQ_ASSERT(!memcmp(bt_qnode_data(node), "4321", sizeof("4321")));

	TQ_ASSERT_S(node = bt_queue_enqueue(&q, "111", sizeof("111")));
	TQ_ASSERT_S(bt_queue_count(&q) == 3);
	TQ_ASSERT(bt_qnode_len(node) == sizeof("111"));
	TQ_ASSERT(!memcmp(bt_qnode_data(node), "111", sizeof("111")));
	TQ_VOID(bt_queue_destroy(&q));
	TQ_RETURN;
}


static BLUETEST(003_queue, must_be_fifo)
{
	TQ_START;
	bt_queue_t q;
	bt_qnode_t *nodes[100] = {NULL};
	TQ_ASSERT_S(bt_queue_init(&q, 5) == &q);
	TQ_ASSERT_S(bt_queue_count(&q) == 0);

	TQ_ASSERT_S(nodes[0] = bt_queue_enqueue(&q, "11111", sizeof("11111")));
	TQ_ASSERT_S(nodes[1] = bt_queue_enqueue(&q, "2222", sizeof("2222")));
	TQ_ASSERT_S(nodes[2] = bt_queue_enqueue(&q, "333", sizeof("333")));
	TQ_ASSERT_S(nodes[3] = bt_queue_enqueue(&q, "44", sizeof("44")));
	TQ_ASSERT_S(nodes[4] = bt_queue_enqueue(&q, "5", sizeof("5")));

	TQ_ASSERT(bt_queue_dequeue(&q) == nodes[0]);
	TQ_ASSERT_S(!memcmp(bt_qnode_data(nodes[0]), "11111", sizeof("11111")));
	TQ_ASSERT_S(bt_qnode_len(nodes[0]) == sizeof("11111"));
	TQ_VOID(bt_qnode_delete(nodes[0]));

	TQ_ASSERT(bt_queue_dequeue(&q) == nodes[1]);
	TQ_ASSERT_S(bt_qnode_len(nodes[1]) == sizeof("2222"));
	TQ_ASSERT_S(!memcmp(bt_qnode_data(nodes[1]), "2222", sizeof("2222")));
	TQ_VOID(bt_qnode_delete(nodes[1]));

	TQ_ASSERT(bt_queue_dequeue(&q) == nodes[2]);
	TQ_ASSERT_S(bt_qnode_len(nodes[2]) == sizeof("333"));
	TQ_ASSERT_S(!memcmp(bt_qnode_data(nodes[2]), "333", sizeof("333")));
	TQ_VOID(bt_qnode_delete(nodes[2]));

	TQ_ASSERT(bt_queue_dequeue(&q) == nodes[3]);
	TQ_ASSERT_S(!memcmp(bt_qnode_data(nodes[3]), "44", sizeof("44")));
	TQ_ASSERT_S(bt_qnode_len(nodes[3]) == sizeof("44"));
	TQ_VOID(bt_qnode_delete(nodes[3]));

	TQ_ASSERT(bt_queue_dequeue(&q) == nodes[4]);
	TQ_ASSERT_S(!memcmp(bt_qnode_data(nodes[4]), "5", sizeof("5")));
	TQ_ASSERT_S(bt_qnode_len(nodes[4]) == sizeof("5"));
	TQ_VOID(bt_qnode_delete(nodes[4]));

	TQ_VOID(bt_queue_destroy(&q));
	TQ_RETURN;
}


static BLUETEST(003_queue, detach)
{
	TQ_START;
	bt_queue_t q;
	bt_qnode_t *node = NULL;
	bt_qnode_t *nodes[100] = {NULL};
	TQ_ASSERT_S(bt_queue_init(&q, 100) == &q);
	TQ_ASSERT_S(bt_queue_count(&q) == 0);

	TQ_ASSERT_S(nodes[0] = bt_queue_enqueue(&q, "11111", sizeof("11111")));
	TQ_ASSERT_S(nodes[1] = bt_queue_enqueue(&q, "2222", sizeof("2222")));
	TQ_ASSERT_S(nodes[2] = bt_queue_enqueue(&q, "333", sizeof("333")));
	TQ_ASSERT_S(nodes[3] = bt_queue_enqueue(&q, "44", sizeof("44")));
	TQ_ASSERT_S(nodes[4] = bt_queue_enqueue(&q, "5", sizeof("5")));

	TQ_ASSERT(q.head == nodes[0]);
	TQ_ASSERT(node = bt_qnode_detach(&q, nodes[0]));
	TQ_ASSERT(node == nodes[0]);
	/*
	 * Detached node must not have `next` and `prev`!
	 */
	TQ_ASSERT(node->next == NULL);
	TQ_ASSERT(node->prev == NULL);
	TQ_VOID(bt_qnode_delete(node));
	/*
	 * As we detached the head, the head must be the second queue.
	 */
	TQ_ASSERT(q.head == nodes[1]);

	TQ_VOID(bt_queue_destroy(&q));
	TQ_RETURN;
}


bluetest_entry_t test_entry[] = {
	FN_BLUETEST(003_queue, init_must_be_empty),
	FN_BLUETEST(003_queue, enqueue_must_increment),
	FN_BLUETEST(003_queue, dequeue_must_decrement),
	FN_BLUETEST(003_queue, maintain_increment_and_decrement),
	FN_BLUETEST(003_queue, get_data_and_len_from_node),
	FN_BLUETEST(003_queue, must_be_fifo),
	FN_BLUETEST(003_queue, detach),
	NULL
};
