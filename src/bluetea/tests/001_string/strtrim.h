// SPDX-License-Identifier: GPL-2.0
/*
 *  bluetea/tests/001_string/strtrim.h
 *
 *  Test case for string library.
 *
 *  Copyright (C) 2021  Ammar Faizi
 */

#include <bluetea/lib/string.h>

/*
 * Test strtriml (explicit length)
 */
static BLUETEST(001_string, test_strtriml)
{
	TQ_START;

	{
		/* Test simple spaces. */
		char *ret = NULL;
		char my_str[] = "   012345   ";

		TQ_VOID(ret = strtriml(my_str, strlen(my_str)));

		/*
		 * It must return a pointer to the first char which
		 * is not space.
		 */
		TQ_ASSERT(ret == (my_str + 3));

		/* It must not remove the leading spaces. */
		TQ_ASSERT(ret[-1] == ' ');
		TQ_ASSERT(ret[-2] == ' ');
		TQ_ASSERT(ret[-3] == ' ');

		/* It must not change the non-space characters. */
		TQ_ASSERT(!memcmp(ret, "012345", 6));

		/* It must only remove the first trailing space. */
		TQ_ASSERT(ret[6] == '\0');
		TQ_ASSERT(ret[7] == ' ');
		TQ_ASSERT(ret[8] == ' ');
		TQ_ASSERT(ret[9] == '\0');
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

		/* It must not change the non-space characters. */
		TQ_ASSERT(!memcmp(ret, "012345", 6));

		/* It must only remove the first trailing space. */
		TQ_ASSERT(ret[6] == '\0');
		TQ_ASSERT(ret[7] == '\f');
		TQ_ASSERT(ret[8] == '\n');
		TQ_ASSERT(ret[9] == '\0');
	}


	{
		/* Test no trailing/leading spaces. */
		char *ret = NULL;
		char my_str[] = "AAAAAAAA   AAAAAAAA";

		TQ_VOID(ret = strtriml(my_str, strlen(my_str)));

		TQ_ASSERT(ret == my_str);
		TQ_ASSERT(!memcmp(ret, "AAAAAAAA   AAAAAAAA", sizeof(my_str)));
	}


	{
		/* Test all spaces. */
		char *ret = NULL;
		char my_str[] = "      \t\v\n\r\f";

		TQ_VOID(ret = strtriml(my_str, strlen(my_str)));

		TQ_ASSERT(ret == (my_str + sizeof(my_str) - 1));
		TQ_ASSERT(!memcmp(my_str, "      \t\v\n\r\f", sizeof(my_str)));
	}


	{
		/* Test empty string. */
		char *ret = NULL;
		char my_str[] = "\0\0\0\0\0";

		TQ_VOID(ret = strtriml(my_str + 2, strlen(my_str + 2)));

		TQ_ASSERT(ret == (my_str + 2));
		TQ_ASSERT(!memcmp(my_str, "\0\0\0\0\0", 6));
	}

	TQ_RETURN;
}


/*
 * Test strtrim (implicit length).
 */
static BLUETEST(001_string, test_strtrim)
{
	TQ_START;
	/*
	 * No need so many tests, we trust in strtrim
	 * as long as strtriml does the job correctly.
	 * Because strtrim internally calls strtriml.
	 */
	char *ret = NULL;
	char my_str[] = "   012345   ";
	TQ_VOID(ret = strtrim(my_str));
	TQ_ASSERT(ret == my_str + 3);
	TQ_ASSERT(!memcmp(ret - 3, "   ", 3));
	TQ_ASSERT(!memcmp(ret + 6, "\0  ", 3));
	TQ_RETURN;
}
