
#include <stdio.h>
#include <bluetea/bluetest.h>


static BLUETEST(002_stack, test_trim)
{
	TQ_START;

	TQ_ASSERT(1);

	TQ_RETURN;
}


bluetest_entry_t test_entry[] = {
	FN_BLUETEST(002_stack, test_trim),
	NULL
};
