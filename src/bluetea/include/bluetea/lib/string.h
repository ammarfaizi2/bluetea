// SPDX-License-Identifier: GPL-2.0
/*
 *  bluetea/include/bluetea/lib/arena.h
 *
 *  String library header
 *
 *  Copyright (C) 2021  Ammar Faizi
 */


#ifndef BLUETEA__LIB__ARENA_H
#define BLUETEA__LIB__ARENA_H

#include <ctype.h>
#include <bluetea/base.h>

extern char *strtrim(char *str);
extern char *strtriml(char *str, size_t len);

extern char *strtrim_move(char *str);
extern char *strtriml_move(char *str, size_t len);

extern size_t htmlspecialcharsl(char *__restrict__ out, size_t outlen,
				const char *__restrict__ in, size_t inlen);

#if 0
inline static bool is_ws(char c)
{
	return (c == ' ') || (c == '\n') || (c == '\t') || (c == '\v') ||
		(c == '\r') || (c == '\f');
}
#endif

#define is_ws(c) isspace((unsigned char)(c))

#endif /* #ifndef BLUETEA__LIB__ARENA_H */
