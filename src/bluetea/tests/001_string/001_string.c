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

#include "strtrim.h"
#include "strtrim_cpy.h"

bluetest_entry_t test_entry[] = {
	FN_BLUETEST(001_string, test_strtriml),
	FN_BLUETEST(001_string, test_strtrim),
	FN_BLUETEST(001_string, test_strtrim_cpyl),
	FN_BLUETEST(001_string, test_strtrim_cpy),
	NULL
};
