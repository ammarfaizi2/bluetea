// SPDX-License-Identifier: GPL-2.0
/*
 *  bluetea/tests/001_string/strtrim.h
 *
 *  Test case for string library.
 *
 *  Copyright (C) 2021  Ammar Faizi
 */

#include <string.h>
#include <bluetea/lib/string.h>


/*
 * Test htmlspecialchars
 */
static BLUETEST(001_string, test_htmlspecialcharsl)
{
	TQ_START;
	{
		size_t len = 0;
		const char str[] = "Hello World";
		const char esc[] = "Hello World";
		char out[sizeof(esc)] = {0};

		TQ_ASSERT_S(len = htmlspecialcharsl(out, sizeof(out), str, strlen(str)));
		TQ_ASSERT(len == strlen(esc));
		TQ_ASSERT(!memcmp(out, esc, sizeof(out)));
	}


	{
		size_t len = 0;
		const char str[] = "<a href=\"https://www.google.com/test?a=1&b=2&c=3&d=4\"><b>Hello World!</p></a>";
		const char esc[] = "&lt;a href=&quot;https://www.google.com/test?a=1&amp;b=2&amp;c=3&amp;d=4&quot;&gt;&lt;b&gt;Hello World!&lt;/p&gt;&lt;/a&gt;";
		char out[sizeof(esc)] = {0};

		TQ_ASSERT_S(len = htmlspecialcharsl(out, sizeof(out), str, strlen(str)));
		TQ_ASSERT(len == strlen(esc));
		TQ_ASSERT(!memcmp(out, esc, sizeof(out)));
	}


	TQ_RETURN;
}
