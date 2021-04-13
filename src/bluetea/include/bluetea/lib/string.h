// SPDX-License-Identifier: GPL-2.0
/*
 *  src/bluetea/include/bluetea/lib/string.h
 *
 *  String library header
 *
 *  Copyright (C) 2021  Ammar Faizi
 */

#ifndef BLUETEA__LIB__STRING_H
#define BLUETEA__LIB__STRING_H

/*
 * Trim a null terminated C string.
 *
 * - The length of `char *str` must be known be the caller.
 * - `size_t *res_len` returns the length of trimmed string.
 * - This trim function doesn't shift any char.
 * - This function returns a pointer to the trimmed position.
 */
char *trim_len(char *str, size_t len, size_t *res_len);

#endif
