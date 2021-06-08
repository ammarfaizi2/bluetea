// SPDX-License-Identifier: GPL-2.0
/*
 *  bluetea/tests/001_string/001_string.c
 *
 *  Test case for string library.
 *
 *  Copyright (C) 2021  Ammar Faizi
 */

#include <stdio.h>
#include <string.h>
#include <bluetea/bluetest.h>
#include <bluetea/lib/string.h>


static BLUETEST(001_string, test_strtriml_001)
{
	TQ_START;

	{
		/* Test simple spaces. */
		char *ret = NULL;
		char my_str[] = "   012345   ";

		TQ_VOID(ret = strtriml(my_str, strlen(my_str)));

		/* It must return to the first char which is not space. */
		TQ_ASSERT(ret == (my_str + 3));

		/* It must not remove the leading spaces. */
		TQ_ASSERT(ret[-1] == ' ');
		TQ_ASSERT(ret[-2] == ' ');
		TQ_ASSERT(ret[-3] == ' ');

		/* It must only remove the first trailing space. */
		TQ_ASSERT(ret[6] == '\0');
		TQ_ASSERT(ret[7] == ' ');
		TQ_ASSERT(ret[8] == ' ');
	}

	{
		/* Test variant of spaces. */
		char *ret = NULL;
		char my_str[] = "\t\v\r012345 \f\n";

		TQ_VOID(ret = strtriml(my_str, strlen(my_str)));

		/* It must return to the first char which is not space. */
		TQ_ASSERT(ret == (my_str + 3));

		/* It must not remove the leading spaces. */
		TQ_ASSERT(ret[-1] == '\r');
		TQ_ASSERT(ret[-2] == '\v');
		TQ_ASSERT(ret[-3] == '\t');

		/* It must only remove the first trailing space. */
		TQ_ASSERT(ret[6] == '\0');
		TQ_ASSERT(ret[7] == '\f');
		TQ_ASSERT(ret[8] == '\n');
	}

	TQ_RETURN;
}


bluetest_entry_t test_entry[] = {
	FN_BLUETEST(001_string, test_strtriml_001),
	NULL
};
