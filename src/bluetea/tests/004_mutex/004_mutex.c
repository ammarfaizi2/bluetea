
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <bluetea/bluetest.h>

#define DONT_INLINE_BT_MUTEX
#include <bluetea/lib/mutex.h>


struct basic_test_t {
	bool		start;
	struct bt_mutex	lock;
	void		*arg;
};


static void *basic_test_thread(void *arg)
{
	struct basic_test_t *data = arg;
	size_t *num_p = data->arg;
	data->start = true;
	for (size_t i = 0; i < (100000000ull / 10000ull); i++) {
		bt_mutex_lock(&data->lock);
		for (size_t j = 0; j < 10000ull; j++) {
			(*num_p)++;
			__asm__ volatile("":"+r"(data)::"memory");
		}
		bt_mutex_unlock(&data->lock);
	}
	return arg;
}


static BLUETEST(004_mutex, basic_test)
{
	TQ_START;
	size_t num = 0;
	pthread_t tr;
	struct basic_test_t data;
	memset(&tr, 0, sizeof(tr)); // Shut the clang up!
	data.arg = &num;
	data.start = false;
	TQ_ASSERT(!bt_mutex_init(&data.lock, NULL));
	TQ_ASSERT(!pthread_create(&tr, NULL, basic_test_thread, &data));
	TQ_IF_RUN {
		while (!data.start)
			__asm__ volatile("rep;\n\tnop;");

		for (size_t i = 0; i < (100000000ull / 10000ull); i++) {
			bt_mutex_lock(&data.lock);
			for (size_t j = 0; j < 10000ull; j++) {
				num++;
				__asm__ volatile("":"+m"(num)::"memory");
			}
			bt_mutex_unlock(&data.lock);
		}
	}
	TQ_ASSERT(!pthread_join(tr, NULL));
	TQ_ASSERT(num == 100000000ull * 2ull);
	TQ_ASSERT(!bt_mutex_destroy(&data.lock));
	TQ_RETURN;
}


bluetest_entry_t test_entry[] = {
	FN_BLUETEST(004_mutex, basic_test),
	NULL
};
