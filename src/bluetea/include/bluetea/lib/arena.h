// SPDX-License-Identifier: GPL-2.0
/*
 *  src/bluetea/include/bluetea/lib/arena.h
 *
 *  Arena library header
 *
 *  Copyright (C) 2021  Ammar Faizi
 */


#ifndef BLUETEA__LIB__ARENA_H
#define BLUETEA__LIB__ARENA_H

#include <bluetea/base.h>

__no_inline int ar_init(void *ar_buf, size_t size);
__no_inline size_t ar_capacity(void);
__no_inline void *ar_alloc(size_t len);
__no_inline void *ar_strdup(const char *str);
__no_inline void *ar_strndup(const char *str, size_t inlen);

#endif /* #ifndef BLUETEA__LIB__ARENA_H */
