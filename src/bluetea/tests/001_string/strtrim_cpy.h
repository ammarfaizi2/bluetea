// SPDX-License-Identifier: GPL-2.0
/*
 *  bluetea/tests/001_string/strtrim_cpy.h
 *
 *  Test case for string library.
 *
 *  Copyright (C) 2021  Ammar Faizi
 */

#include <bluetea/lib/string.h>

/*
 * Test strtrim_cpyl (explicit length).
 */
static BLUETEST(001_string, test_strtrim_cpyl)
{
	TQ_START;

	{
		/* Test simple spaces. */
		char *ret = NULL;
		char my_str[] = "   012345   ";

		TQ_VOID(ret = strtrim_cpyl(my_str, strlen(my_str)));

		/*
		 * It must return the same pointer with first argument.
		 */
		TQ_ASSERT(ret == my_str);

		TQ_ASSERT(!memcmp(my_str, "012345", sizeof("012345")));

		/*
		 * It should not alter un-needed area.
		 */
		TQ_ASSERT(!memcmp(my_str + 6, "\0""45   ", sizeof("\0""45   ")));
	}


	{
		/* Test shorter length than NUL terminator. */
		char *ret = NULL;
		char my_str[] = "   012345   ";

		TQ_VOID(ret = strtrim_cpyl(my_str, 8));

		/*
		 * It must return the same pointer with first argument.
		 */
		TQ_ASSERT(ret == my_str);

		TQ_ASSERT(!memcmp(my_str, "01234", sizeof("01234")));

		/*
		 * It must not alter un-needed area.
		 */
		TQ_ASSERT(!memcmp(my_str + 5, "\0""345   ", sizeof("\0""345   ")));
	}

	TQ_RETURN;
}


/*
 * Test strtrim_cpy (implicit length).
 */
static BLUETEST(001_string, test_strtrim_cpy)
{
	TQ_START;
	TQ_RETURN;
}
