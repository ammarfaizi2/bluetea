
#include <stdio.h>
#include <stdlib.h>
#include <bluetea/bluetest.h>

#define DONT_INLINE_BT_QUEUE
#include <bluetea/lib/queue.h>


static BLUETEST(003_queue, queue_init_must_be_empty)
{
	TQ_START;

	bt_queue_t q;
	TQ_ASSERT(bt_queue_init(&q, 100) == &q);
	TQ_ASSERT(bt_queue_count(&q) == 0);
	TQ_RETURN;
}


static BLUETEST(003_queue, enqueue_must_inc_count)
{
	TQ_START;
	bt_queue_t q;
	TQ_ASSERT(bt_queue_init(&q, 100) == &q);
	TQ_ASSERT(bt_queue_count(&q) == 0);
	TQ_ASSERT(bt_queue_enqueue(&q, "test", sizeof("test")));
	TQ_ASSERT(bt_queue_count(&q) == 1);
	TQ_ASSERT(bt_queue_enqueue(&q, "qwer", sizeof("qwer")));
	TQ_ASSERT(bt_queue_count(&q) == 2);
	TQ_ASSERT(bt_queue_enqueue(&q, "asdf", sizeof("asdf")));
	TQ_ASSERT(bt_queue_count(&q) == 3);
	TQ_VOID(bt_queue_destroy(&q));
	TQ_RETURN;
}


static BLUETEST(003_queue, dequeue_must_dec_count_and_must_be_fifo)
{
	TQ_START;

	bt_queue_t q;
	bt_qnode_t *nodes[3] = {NULL};
	TQ_ASSERT_S(bt_queue_init(&q, 100) == &q);
	TQ_ASSERT_S(bt_queue_count(&q) == 0);

	TQ_ASSERT_S(nodes[0] = bt_queue_enqueue(&q, "testq", sizeof("testq")));
	TQ_ASSERT(bt_node_get_len(nodes[0]) == sizeof("testq"));
	TQ_ASSERT(bt_node_get_data(nodes[0]) == nodes[0]->data);
	TQ_ASSERT_S(bt_queue_count(&q) == 1);

	TQ_ASSERT_S(nodes[1] = bt_queue_enqueue(&q, "qwer", sizeof("qwer")));
	TQ_ASSERT(bt_node_get_len(nodes[1]) == sizeof("qwer"));
	TQ_ASSERT(bt_node_get_data(nodes[1]) == nodes[1]->data);
	TQ_ASSERT_S(bt_queue_count(&q) == 2);

	TQ_ASSERT_S(nodes[2] = bt_queue_enqueue(&q, "asd", sizeof("asd")));
	TQ_ASSERT(bt_node_get_len(nodes[2]) == sizeof("asd"));
	TQ_ASSERT(bt_node_get_data(nodes[2]) == nodes[2]->data);
	TQ_ASSERT_S(bt_queue_count(&q) == 3);

	TQ_ASSERT(bt_queue_dequeue(&q) == nodes[0]);
	TQ_ASSERT(!memcmp(nodes[0]->data, "testq", nodes[0]->len));
	TQ_ASSERT(bt_queue_count(&q) == 2);

	TQ_ASSERT(bt_queue_dequeue(&q) == nodes[1]);
	TQ_ASSERT(!memcmp(nodes[1]->data, "qwer", nodes[1]->len));
	TQ_ASSERT(bt_queue_count(&q) == 1);

	TQ_ASSERT(bt_queue_dequeue(&q) == nodes[2]);
	TQ_ASSERT(!memcmp(nodes[2]->data, "asd", nodes[2]->len));
	TQ_ASSERT(bt_queue_count(&q) == 0);

	TQ_VOID(bt_node_destroy(nodes[0]));
	TQ_VOID(bt_node_destroy(nodes[1]));
	TQ_VOID(bt_node_destroy(nodes[2]));

	TQ_VOID(bt_queue_destroy(&q));
	TQ_RETURN;
}


static BLUETEST(003_queue, queue_run_out_capacity)
{
	TQ_START;

	bt_queue_t q;
	bt_qnode_t *nodes[5] = {NULL};
	TQ_ASSERT_S(bt_queue_init(&q, 5) == &q);
	TQ_ASSERT_S(bt_queue_count(&q) == 0);

	TQ_ASSERT_S(nodes[0] = bt_queue_enqueue(&q, "1111", sizeof("1111")));
	TQ_ASSERT_S(bt_queue_count(&q) == 1);
	TQ_ASSERT_S(nodes[1] = bt_queue_enqueue(&q, "2222", sizeof("2222")));
	TQ_ASSERT_S(bt_queue_count(&q) == 2);
	TQ_ASSERT_S(nodes[2] = bt_queue_enqueue(&q, "3333", sizeof("3333")));
	TQ_ASSERT_S(bt_queue_count(&q) == 3);
	TQ_ASSERT_S(nodes[3] = bt_queue_enqueue(&q, "4444", sizeof("4444")));
	TQ_ASSERT_S(bt_queue_count(&q) == 4);
	TQ_ASSERT_S(nodes[4] = bt_queue_enqueue(&q, "5555", sizeof("5555")));
	TQ_ASSERT_S(bt_queue_count(&q) == 5);

	TQ_ASSERT(
		bt_queue_enqueue(&q, "6666", sizeof("6666")) == NULL &&
		errno == EAGAIN
	);
	TQ_ASSERT(bt_queue_count(&q) == 5);
	TQ_ASSERT(
		bt_queue_enqueue(&q, "6666", sizeof("6666")) == NULL &&
		errno == EAGAIN
	);
	TQ_ASSERT(bt_queue_count(&q) == 5);
	TQ_ASSERT(
		bt_queue_enqueue(&q, "6666", sizeof("6666")) == NULL &&
		errno == EAGAIN
	);
	TQ_ASSERT(bt_queue_count(&q) == 5);
	TQ_ASSERT(
		bt_queue_enqueue(&q, "6666", sizeof("6666")) == NULL &&
		errno == EAGAIN
	);
	TQ_ASSERT(bt_queue_count(&q) == 5);

	TQ_ASSERT_S(bt_queue_dequeue(&q) == nodes[0]);
	TQ_ASSERT_S(bt_queue_count(&q) == 4);
	TQ_VOID(bt_node_destroy(nodes[0]));

	TQ_ASSERT_S(bt_queue_dequeue(&q) == nodes[1]);
	TQ_ASSERT_S(bt_queue_count(&q) == 3);
	TQ_VOID(bt_node_destroy(nodes[1]));

	/* 
	 * Must be able to enqueue again after we dequeue some.
	 */
	TQ_ASSERT(bt_queue_enqueue(&q, "6666", sizeof("6666")));
	TQ_ASSERT(bt_queue_count(&q) == 4);

	TQ_ASSERT(bt_queue_enqueue(&q, "7777", sizeof("7777")));
	TQ_ASSERT(bt_queue_count(&q) == 5);

	TQ_ASSERT(
		bt_queue_enqueue(&q, "8888", sizeof("8888")) == NULL &&
		errno == EAGAIN
	);
	TQ_ASSERT(bt_queue_count(&q) == 5);

	TQ_ASSERT(
		bt_queue_enqueue(&q, "8888", sizeof("8888")) == NULL &&
		errno == EAGAIN
	);
	TQ_ASSERT(bt_queue_count(&q) == 5);

	TQ_ASSERT(
		bt_queue_enqueue(&q, "8888", sizeof("8888")) == NULL &&
		errno == EAGAIN
	);
	TQ_ASSERT(bt_queue_count(&q) == 5);

	TQ_VOID(bt_queue_destroy(&q));
	TQ_RETURN;
}


static BLUETEST(003_queue, iterate_queue)
{
	TQ_START;
	bt_queue_t q;
	bt_qnode_t *nodes[5] = {NULL};
	memset(&q, 0, sizeof(q));
	TQ_ASSERT(bt_queue_init(&q, 100) == &q);
	TQ_ASSERT(bt_queue_count(&q) == 0);
	TQ_ASSERT(nodes[0] = bt_queue_enqueue(&q, "test", sizeof("test")));
	TQ_ASSERT(bt_queue_count(&q) == 1);
	TQ_ASSERT(nodes[1] = bt_queue_enqueue(&q, "qwer", sizeof("qwer")));
	TQ_ASSERT(bt_queue_count(&q) == 2);
	TQ_ASSERT(nodes[2] = bt_queue_enqueue(&q, "asdf", sizeof("asdf")));
	TQ_ASSERT(bt_queue_count(&q) == 3);
	TQ_ASSERT(nodes[3] = bt_queue_enqueue(&q, "asdf", sizeof("asdf")));
	TQ_ASSERT(bt_queue_count(&q) == 4);
	TQ_ASSERT(nodes[4] = bt_queue_enqueue(&q, "asdf", sizeof("asdf")));
	TQ_ASSERT(bt_queue_count(&q) == 5);
	TQ_IF_RUN {
		size_t i = 0;
		bt_queue_for_each(&q) {
			bt_qnode_t *n = __node;
			TQ_ASSERT_DYN(n == nodes[i]);
			i++;
		}
	}
	TQ_VOID(bt_queue_destroy(&q));
	TQ_RETURN;
}


static BLUETEST(003_queue, iterate_queue2)
{
	TQ_START;
	bt_queue_t q;
	bt_qnode_t *nodes[5] = {NULL};
	TQ_ASSERT(bt_queue_init(&q, 100) == &q);
	TQ_ASSERT(bt_queue_count(&q) == 0);
	TQ_ASSERT(nodes[0] = bt_queue_enqueue(&q, "test", sizeof("test")));
	TQ_ASSERT(bt_queue_count(&q) == 1);
	TQ_ASSERT(nodes[1] = bt_queue_enqueue(&q, "qwer", sizeof("qwer")));
	TQ_ASSERT(bt_queue_count(&q) == 2);
	TQ_ASSERT(nodes[2] = bt_queue_enqueue(&q, "asdf", sizeof("asdf")));
	TQ_ASSERT(bt_queue_count(&q) == 3);
	TQ_IF_RUN {
		size_t i = 0;
		bt_queue_for_each_head(&q) {
			bt_qnode_t *n = __node;
			TQ_ASSERT_DYN(n == nodes[i]);
			i++;
		}
	}
	TQ_ASSERT(bt_queue_dequeue(&q) == nodes[0]);
	TQ_ASSERT(bt_queue_count(&q) == 2);
	TQ_VOID(bt_node_destroy(nodes[0]));
	TQ_IF_RUN {
		size_t i = 1;
		bt_queue_for_each_head(&q) {
			bt_qnode_t *n = __node;
			TQ_ASSERT_DYN(n == nodes[i]);
			i++;
		}
	}
	TQ_ASSERT(bt_queue_dequeue(&q) == nodes[1]);
	TQ_ASSERT(bt_queue_count(&q) == 1);
	TQ_VOID(bt_node_destroy(nodes[1]));
	TQ_IF_RUN {
		size_t i = 2;
		bt_queue_for_each_head(&q) {
			bt_qnode_t *n = __node;
			TQ_ASSERT_DYN(n == nodes[i]);
			i++;
		}
	}
	TQ_VOID(bt_queue_destroy(&q));
	TQ_RETURN;
}


static BLUETEST(003_queue, iterate_queue3)
{
	TQ_START;
	bt_queue_t q;
	bt_qnode_t *nodes[5] = {NULL};
	TQ_ASSERT(bt_queue_init(&q, 100) == &q);
	TQ_ASSERT(bt_queue_count(&q) == 0);
	TQ_ASSERT(nodes[0] = bt_queue_enqueue(&q, "test", sizeof("test")));
	TQ_ASSERT(bt_queue_count(&q) == 1);
	TQ_ASSERT(nodes[1] = bt_queue_enqueue(&q, "qwer", sizeof("qwer")));
	TQ_ASSERT(bt_queue_count(&q) == 2);
	TQ_ASSERT(nodes[2] = bt_queue_enqueue(&q, "asdf", sizeof("asdf")));
	TQ_ASSERT(bt_queue_count(&q) == 3);
	TQ_IF_RUN {
		size_t i = 2;
		bt_queue_for_each_tail(&q) {
			bt_qnode_t *n = __node;
			TQ_ASSERT_DYN(n == nodes[i]);
			i--;
		}
	}
	TQ_ASSERT(bt_queue_dequeue(&q) == nodes[0]);
	TQ_ASSERT(bt_queue_count(&q) == 2);
	TQ_VOID(bt_node_destroy(nodes[0]));
	TQ_IF_RUN {
		size_t i = 2;
		bt_queue_for_each_tail(&q) {
			bt_qnode_t *n = __node;
			TQ_ASSERT_DYN(n == nodes[i]);
			i--;
		}
	}
	TQ_ASSERT(bt_queue_dequeue(&q) == nodes[1]);
	TQ_ASSERT(bt_queue_count(&q) == 1);
	TQ_VOID(bt_node_destroy(nodes[1]));
	TQ_IF_RUN {
		size_t i = 2;
		bt_queue_for_each_tail(&q) {
			bt_qnode_t *n = __node;
			TQ_ASSERT_DYN(n == nodes[i]);
			i++;
		}
	}
	TQ_VOID(bt_queue_destroy(&q));
	TQ_RETURN;
}


static BLUETEST(003_queue, iterate_queue_and_destroy)
{
	TQ_START;
	bt_queue_t q;
	bt_qnode_t *nodes[100] = {NULL};
	TQ_ASSERT(bt_queue_init(&q, 100) == &q);
	TQ_ASSERT(bt_queue_count(&q) == 0);

	for (size_t i = 0; i < 100; i++) {
		char buf[32];
		snprintf(buf, sizeof(buf), "test=%zu", i);
		TQ_ASSERT_S(bt_queue_count(&q) == i);
		TQ_ASSERT_S(nodes[i] = bt_queue_enqueue(&q, buf, strlen(buf) + 1));
		TQ_ASSERT_S(bt_queue_count(&q) == i + 1);
	}

	for (size_t i = 0; i < 100; i++) {
		TQ_ASSERT(
			bt_queue_enqueue(&q, "xxxxx", sizeof("xxxxx")) == NULL &&
			errno == EAGAIN
		);
		TQ_ASSERT_S(bt_queue_count(&q) == 100);
	}

	TQ_IF_RUN {
		size_t i;

		i = 99;
		bt_queue_for_each_tail(&q) {
			bt_qnode_t *n = __node;
			TQ_ASSERT_DYN_S(n == nodes[i]);
			i--;
		}

		i = 0;
		bt_queue_for_each_head(&q) {
			bt_qnode_t *n = __node;
			TQ_ASSERT_DYN_S(n == nodes[i]);
			i++;
		}
	}

	bt_node_destroy_ref(&q, nodes[50]);
	TQ_ASSERT(bt_queue_count(&q) == 99);
	TQ_IF_RUN {
		size_t i;

		i = 99;
		bt_queue_for_each_tail(&q) {
			bt_qnode_t *n = __node;
			TQ_ASSERT_DYN_S(n == nodes[i]);
			i--;
			if (i == 50)
				i--;
		}

		i = 0;
		bt_queue_for_each_head(&q) {
			bt_qnode_t *n = __node;
			TQ_ASSERT_DYN_S(n == nodes[i]);
			i++;
			if (i == 50)
				i++;
		}
	}

	bt_node_destroy_ref(&q, nodes[49]);
	TQ_ASSERT(bt_queue_count(&q) == 98);
	bt_node_destroy_ref(&q, nodes[48]);
	TQ_ASSERT(bt_queue_count(&q) == 97);
	bt_node_destroy_ref(&q, nodes[47]);
	TQ_ASSERT(bt_queue_count(&q) == 96);
	bt_node_destroy_ref(&q, nodes[46]);
	TQ_ASSERT(bt_queue_count(&q) == 95);

	TQ_IF_RUN {
		size_t i;

		i = 99;
		bt_queue_for_each_tail(&q) {
			bt_qnode_t *n = __node;
			TQ_ASSERT_DYN_S(n == nodes[i]);
			i--;
			if (i == 50)
				i -= 5;
		}

		i = 0;
		bt_queue_for_each_head(&q) {
			bt_qnode_t *n = __node;
			TQ_ASSERT_DYN_S(n == nodes[i]);
			i++;
			if (i == 46)
				i += 5;
		}
	}

	TQ_IF_RUN {
		size_t j = 94;
		for (size_t i = 0; i < sizeof(nodes) / sizeof(*nodes); i++) {
			if (i < 46 && i < 51)
				continue;
			bt_node_destroy_ref(&q, nodes[i]);
			TQ_ASSERT_DYN(bt_queue_count(&q) == j--);
		}
	}

	TQ_VOID(bt_queue_destroy(&q));
	TQ_RETURN;
}


bluetest_entry_t test_entry[] = {
	FN_BLUETEST(003_queue, queue_init_must_be_empty),
	FN_BLUETEST(003_queue, enqueue_must_inc_count),
	FN_BLUETEST(003_queue, dequeue_must_dec_count_and_must_be_fifo),
	FN_BLUETEST(003_queue, queue_run_out_capacity),
	FN_BLUETEST(003_queue, iterate_queue),
	FN_BLUETEST(003_queue, iterate_queue2),
	FN_BLUETEST(003_queue, iterate_queue3),
	FN_BLUETEST(003_queue, iterate_queue_and_destroy),
	NULL
};
