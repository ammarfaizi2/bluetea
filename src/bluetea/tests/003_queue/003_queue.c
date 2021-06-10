
#include <stdio.h>

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
	TQ_ASSERT(bt_queue_enqueue(&q, "test", sizeof("test")));
	TQ_ASSERT(bt_queue_count(&q) == 0);
	TQ_RETURN;
}


bluetest_entry_t test_entry[] = {
	FN_BLUETEST(003_queue, queue_init_must_be_empty),
	NULL
};
