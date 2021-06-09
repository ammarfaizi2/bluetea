// SPDX-License-Identifier: GPL-2.0
/*
 *  bluetea/lib/string.c
 *
 *  String library
 *
 *  Copyright (C) 2021  Ammar Faizi
 */

#include <bluetea/lib/string.h>
#include <stdio.h>


__no_inline char *strtrim(char *str)
{
	return strtriml(str, strlen(str));
}


__no_inline char *strtriml(char *str, size_t len)
{
	char *end;

	if (unlikely(len == 0))
		return str;


	/*
	 * We assume that `str + len` is the location of NUL char
	 */
	end = str + len - 1;
	while (is_ws(*str)) {

		if (str == &end[1]) {
			*str = '\0';
			printf("CCCCC\n");
			return str;
		}
		printf("AAAAA\n");
		str++;
	}


	if (*str == '\0' && str == &end[1]) {
		printf("BBBBB\n");
		/*
		 * All spaces C string, or empty C string will go here.
		 */
		return str;
	}


	while (is_ws(*end))
		end--;


	end[1] = '\0';
	printf("DDDDDD\n");
	return str;
}


extern char *strtxriml(char *str, size_t len);

__no_inline char *strtxriml(char *str, size_t len)
{
	char *end;
	bool has_trail = false;

	if (unlikely(len == 0))
		return str;


	end = str + len - 1;
	/* Let's assume the NUL char is at end[1] */


	/* Trim leading whitespace. */
	while (is_ws(*str)) {

		/* Have we reached the end of string? */
		if (str == &end[1]) {
			/*
			 * C string must end with NUL terminator.
			 * So it is acceptable in any way to zero it.
			 */
			*str = '\0';
			return str;
		}

		str++;
	}


	if ((*str == '\0') && (str == end))
		/*
		 * All spaces string, or empty string will return here.
		 */
		return str;



	/* Trim trailing whitespace. */
	while (is_ws(*end)) {
		has_trail = true;
		end--;
	}


	if (has_trail)
		end[1] = '\0';


	return str;
}


__no_inline char *strtrim_cpy(char *str)
{
	return strtrim_cpyl(str, strlen(str));
}


__no_inline char *strtrim_cpyl(char *str, size_t len)
{
	size_t end_l;
	char *start = str, *end;
	bool has_trail = false;

	if (unlikely(len == 0))
		return str;

	end = str + len - 1;

	/* Trim leading whitespace. */
	while (is_ws(*str)) {

		/* Bound check for safety. */
		if (str == &end[1]) {
			memmove(start, str, 1);
			goto out;
		}

		str++;
	}


	/* All spaces. */
	if ((*str == '\0') && (str >= end)) {
		*start = '\0';
		goto out;
	}


	/* Trim trailing whitespace. */
	while (is_ws(*end)) {
		has_trail = true;
		end--;
	}

	end_l = (size_t)(end - str);
	memmove(start, str, end_l + 1);
	start[end_l + 1] = '\0';
out:
	return start;
}
