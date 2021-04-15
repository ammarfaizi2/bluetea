// SPDX-License-Identifier: GPL-2.0
/*
 *  src/bluetea/tests/005_getopt/005_getopt.c
 *
 *  Test case for getopt
 *
 *  Copyright (C) 2021  Ammar Faizi
 */

#include <bluetea/teatest.h>
#include <bluetea/lib/getopt.h>


static TEATEST(005_arena, simple_long_opt)
{
	TQ_START;
	static const char *short_opt = NULL;
	static const struct bt_getopt_long long_opt[] = {
		{"bind-addr",	REQUIRED_ARG,	'H'},
		{"bind-port",	REQUIRED_ARG,	'P'},
		{"daemon",	NO_ARG,		'D'},
		GETOPT_LONG_STRUCT_END,
	};
	static const char *argv[] = {
		"server",
		"--bind-addr",
		"0.0.0.0",
		"--bind-port",
		"55555",
		"--daemon",
		"--unknown-option",
		NULL
	};
	int argc = (sizeof(argv) / sizeof(*argv)) - 1;
	struct bt_getopt_wr wr = {
		argc, argv, short_opt, long_opt, NULL, 0
	};

	TQ_ASSERT(bt_getopt(&wr) == BT_GETOPT_NO_OPT);
	TQ_ASSERT(!memcmp(wr.retval, "server", sizeof("server")));
	TQ_ASSERT(wr.retval == argv[0]);

	TQ_ASSERT(bt_getopt(&wr) == 'H');
	TQ_ASSERT(!memcmp(wr.retval, "0.0.0.0", sizeof("0.0.0.0")));
	TQ_ASSERT(wr.retval == argv[2]);

	TQ_ASSERT(bt_getopt(&wr) == 'P');
	TQ_ASSERT(!memcmp(wr.retval, "55555", sizeof("55555")));
	TQ_ASSERT(wr.retval == argv[4]);

	TQ_ASSERT(bt_getopt(&wr) == 'D');
	TQ_ASSERT(wr.retval == NULL);

	TQ_ASSERT(bt_getopt(&wr) == BT_GETOPT_UNKNOWN);
	TQ_ASSERT(wr.retval == NULL);

	TQ_RETURN;
}


static TEATEST(005_arena, simple_short_opt)
{
TQ_START;
	static const char *short_opt = "H:";
	static const struct bt_getopt_long long_opt[] = {
		{"bind-addr",	REQUIRED_ARG,	'H'},
		{"bind-port",	REQUIRED_ARG,	'P'},
		{"daemon",	NO_ARG,		'D'},
		GETOPT_LONG_STRUCT_END,
	};
	static const char *argv[] = {
		"server",
		"--bind-addr",
		"0.0.0.0",
		"--bind-port",
		"55555",
		"--daemon",
		"--unknown-option",
		NULL
	};
	int argc = (sizeof(argv) / sizeof(*argv)) - 1;
	struct bt_getopt_wr wr = {
		argc, argv, short_opt, long_opt, NULL, 0
	};

	TQ_ASSERT(bt_getopt(&wr) == BT_GETOPT_NO_OPT);
	TQ_ASSERT(!memcmp(wr.retval, "server", sizeof("server")));
	TQ_ASSERT(wr.retval == argv[0]);

	TQ_ASSERT(bt_getopt(&wr) == 'H');
	TQ_ASSERT(!memcmp(wr.retval, "0.0.0.0", sizeof("0.0.0.0")));
	TQ_ASSERT(wr.retval == argv[2]);

	TQ_ASSERT(bt_getopt(&wr) == 'P');
	TQ_ASSERT(!memcmp(wr.retval, "55555", sizeof("55555")));
	TQ_ASSERT(wr.retval == argv[4]);

	TQ_ASSERT(bt_getopt(&wr) == 'D');
	TQ_ASSERT(wr.retval == NULL);

	TQ_ASSERT(bt_getopt(&wr) == BT_GETOPT_UNKNOWN);
	TQ_ASSERT(wr.retval == NULL);

	TQ_RETURN;
}


extern const test_entry_t test_entry_arr[];
const test_entry_t test_entry_arr[] = {
	FN_TEATEST(005_arena, simple_long_opt),
	NULL
};
