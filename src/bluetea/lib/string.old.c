// SPDX-License-Identifier: GPL-2.0
/*
 *  src/bluetea/lib/string.c
 *
 *  String library
 *
 *  Copyright (C) 2021  Ammar Faizi
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <bluetea/base.h>
#include <bluetea/lib/string.h>

#define HEQ(C) ((*head) == (C))
#define TEQ(C) ((*tail) == (C))


char *trim_len(char *head, size_t len, size_t *res_len)
{
	char *tail  = &(head[len - 1]);
	bool move_t = false;

	while ((len > 0) && (HEQ(' ') || HEQ('\n') || HEQ('\r') || HEQ('\v'))) {
		head++;
		len--;
	}

	while ((len > 0) && (TEQ(' ') || TEQ('\n') || TEQ('\r') || TEQ('\v'))) {
		tail--;
		len--;
		move_t = true;
	}

	if ((len > 0) && move_t)
		*(tail + 1) = '\0';

	if (res_len != NULL)
		*res_len = len;

	return head;
}


char *trim_len_cpy(char *head, size_t len, size_t *res_len)
{
	char *start = head;
	char *tail  = &(head[len - 1]);
	bool move_h = false;

	while ((len > 0) && (HEQ(' ') || HEQ('\n') || HEQ('\r') || HEQ('\v'))) {
		head++;
		len--;
		move_h = true;
	}

	while ((len > 0) && (TEQ(' ') || TEQ('\n') || TEQ('\r') || TEQ('\v'))) {
		tail--;
		len--;
	}

	if (move_h) {
		if (len > 0)
			memmove(start, head, len);

		*(start + len) = '\0';
	}

	if (res_len != NULL)
		*res_len = len;

	return start;
}


char *trim(char *str)
{
	return trim_len(str, strlen(str), NULL);
}


char *trim_cpy(char *str)
{
	return trim_len_cpy(str, strlen(str), NULL);
}


char *trunc_str(char *str, size_t n)
{
	size_t len = strnlen(str, n);

	if (len < n)
		return str;

	str[n] = '\0';
	return str;
}


void *memzero_explicit(void *s, size_t n)
{
	return memset(s, '\0', n);
}


int memcmp_explicit(const void *s1, const void *s2, size_t n)
{
	return memcmp(s1, s2, n);
}
