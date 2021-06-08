// SPDX-License-Identifier: GPL-2.0
/*
 *  bluetea/lib/string.c
 *
 *  String library
 *
 *  Copyright (C) 2021  Ammar Faizi
 */


#include <bluetea/lib/string.h>



__no_inline char *strtriml(char *str, size_t len)
{
	size_t i;
	char *start = str, *end;

	if (len == 0)
		return str;

	i = 0;
	/* Trim leading whitespace. */
	while (is_ws(*str)) {
		str++;
		i++;
		if (i >= len)
			return str;
	}

	/* All spaces. */
	if (*str == '\0')
		return str;

	/* Trim trailing whitespace. */
	end = start + len - 1;
	while ((end > str) && is_ws(*end))
		end--;

	end[1] = '\0';
	return str;
}


__no_inline char *strtrim(char *str)
{
	return strtriml(str, strlen(str));
}
