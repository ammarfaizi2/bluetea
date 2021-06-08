// SPDX-License-Identifier: GPL-2.0
/*
 *  bluetea/bluetest/bluetest.c
 *
 *  BlueTest (Unit Test Framework for C project)
 *
 *  Copyright (C) 2021  Ammar Faizi
 *
 *
 *  Usage:
 *  1. Compile the unit test and link it with libbluetest.so
 *  2. Run the unit test with the following arguments:
 *       ./test_name do_test valgrind --leak-check=full --show-leak-kinds=all \
 *                 --track-origins=yes
 *  3. The main() function in libbluetest.so will spawn valgrind with the
 *     following arguments:
 *       execve("valgrind", {"valgrind", "--leak-check=full", ... (vg args),
 *              "do_test_internal"}, environ);
 */

#include <poll.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <alloca.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <stdbool.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "bluetest.h"

extern char **environ;
extern bluetest_entry_t test_entry[];
static bluetest_data_t g_data;
static bool __will_run_test = false;
static int __parent_pipe_wr_fd = -1;


bool will_run_test(void)
{
	return __will_run_test;
}


void filename_resolve(char *buf, size_t bufsiz, const char *filename,
		      size_t len)
{
	char __fn0[len], __fn1[len];

	memcpy(__fn0, filename, len);
	memcpy(__fn1, filename, len);

	snprintf(buf, bufsiz, "%s/%s", basename(dirname(__fn0)),
		 basename(__fn1));
}


bool print_test(bool is_success, const char *func, const char *file, int line)
{
	if (is_success) {
		printf("\x1b[32mTest passed\x1b[0m: %s in %s line %d\n", func,
			file, line);
	} else {
		printf("\x1b[31mTest failed\x1b[0m: %s in %s line %d\n", func,
			file, line);
	}
	return is_success;
}


bool print_test_s(bool is_success, const char *func, const char *file, int line)
{
	if (is_success) {
		/* Pass */
	} else {
		printf("\x1b[31mTest failed\x1b[0m: %s in %s line %d\n", func,
			file, line);
	}
	return is_success;
}


static void sig_handler(int sig)
{
	int err;
	ssize_t write_ret;
	bluetest_data_t data;

	/*
	 * Panic!
	 *
	 * Report total passed tests!
	 */
	data = g_data;
	write_ret = write(__parent_pipe_wr_fd, &data, sizeof(data));
	if (write_ret < 0) {
		err = errno;
		printf("Error: write(): %s\n", strerror(err));
	}

	signal(sig, SIG_DFL);
	printf("Panic!\n");
	if (sig == SIGSEGV) {
		printf("  SIGSEGV caught!\n");
		printf("  You crashed the program memory!\n");
		printf("  Segmentation Fault (core dumped)\n");
	} else
	if (sig == SIGABRT) {
		printf("SIGABRT caught!\n");
		printf("Aborted (core dumped)\n");
	}

	printf("===============================================\n");
	raise(sig);
	exit(1);
}


static int fd_set_nonblock(int fd)
{
	int err;
	int flags;

	flags = fcntl(fd, F_GETFL, 0);
	if (flags < 0) {
		err = errno;
		printf("Error: fcntl(%d, F_GETFL, 0): %s\n", fd, strerror(err));
		return -err;
	}

	flags = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	if (flags < 0) {
		err = errno;
		printf("Error: fcntl(%d, F_SETFL, %d): %s\n", fd, flags,
			strerror(err));
		return -err;
	}

	return flags;
}


static void print_info(int ret, uint32_t total_tests, uint32_t passed_tests)
{
	int fd;
	double acc;

	if (total_tests == 0 || passed_tests == 0) {
		acc = 0.0;
	} else {
		acc = ((double)passed_tests) / ((double)total_tests) * 100.0;
	}

	fd = open("test_lock.tmp", O_RDWR | O_CREAT, S_IRWXU);
	if (fd < 0) {
		int err = errno;
		printf("Error: open(\"test_lock.tmp\"): %s", strerror(err));
	}

	if (fd > 0)
		flock(fd, LOCK_EX);

	printf("==================================================\n");
	printf("\t\tSummary\n");
	printf("--------------------------------------------------\n");
	printf("   Last return value\t: %d\n", ret);
	printf("   Your accuracy\t: %.2f %c\n", acc, '%');
	printf("   Earned point\t\t: %u of %u\n", passed_tests, total_tests);
	printf("==================================================\n");

	if (fd > 0)
		flock(fd, LOCK_UN);
	close(fd);
}


static int handle_wait(pid_t child_pid, int pipe_fd[2])
{
	int err;
	int wret;
	int wstatus;
	int exit_code;
	ssize_t read_ret;
	struct pollfd fds[1];
	bluetest_data_t	data = {0, 0};


	wret = wait(&wstatus);
	if (!(WIFEXITED(wstatus) && (wret == child_pid))) {
		printf("Unknown error, please contact Ammar F");
		printf("Please also tell to him, how did you get into this error");
		return -1;
	}

	exit_code = WEXITSTATUS(wstatus);
	fds[0].fd = pipe_fd[0];
	fds[0].events = POLLIN;
	fds[0].revents = 0;

	while (true) {
		int poll_ret = poll(fds, 1, 0);
		if (poll_ret < 0) {
			/* poll() errors */
			exit_code = err = errno;
			printf("Error: poll(): %s\n", strerror(err));
			break;
		}

		if (poll_ret == 0)
			/* poll() timedout */
			break;

		read_ret = read(pipe_fd[0], &data, sizeof(data));
		if (read_ret > 0)
			/* read() success */
			continue;

		if (read_ret == 0)
			/* Pipe has been closed and unreadable. */
			break;

		err = errno;
		if (err != EAGAIN) {
			/* read() errors */
			exit_code = err;
			printf("Error: read(): %s\n", strerror(err));
			break;
		}

		/* read() returns EAGAIN */
	}


	if (exit_code == 0) {
		/*
		 * All green!
		 */
		goto out_exit;
	}


	printf("\x1b[31mTEST FAILED!\x1b[0m\n");
	printf("Exit code: %d\n", exit_code);
	if (exit_code == 99) {
		printf("Error from valgrind detected\n");
		printf("Please read the error message from valgrind to diagnose "
		       "this problem\n");
		printf("Reading valgrind backtrace is not trivial, please be "
		       "serious!\n");
	}

out_exit:
	print_info(exit_code, data.n_test, data.n_pass);
	return exit_code;
}


static int spawn_valgrind(int argc, char *argv[])
{
	int err;
	int ret = 0;
	int fw_argc;
	int pipe_fd[2];
	char **fw_argv;
	pid_t child_pid;
	char pipe_fd_arg[16];

	/* (argc & 0xfu) allows max argc up to 63 */
	fw_argc  = (uint8_t)argc & 0x3fu;

	/* +1 to pass pipe fd */
	fw_argc += 1;

	if (pipe(pipe_fd) < 0) {
		int err = errno;
		printf("Error: pipe(): %s\n", strerror(err));
		return err;
	}

	err = fd_set_nonblock(pipe_fd[0]);
	if (err < 0) {
		ret = -err;
		goto out;
	}

	err = fd_set_nonblock(pipe_fd[1]);
	if (err < 0) {
		ret = -err;
		goto out;
	}

	/* We share the `write pipe fd` to child via argument */
	snprintf(pipe_fd_arg, sizeof(pipe_fd_arg), "%d", pipe_fd[1]);

	fw_argv = alloca((size_t)(fw_argc + 1) * sizeof(*fw_argv));

	/* Copy arguments */
	memmove(&fw_argv[0], &argv[2], (size_t)(fw_argc - 3) * sizeof(*argv));
	fw_argv[fw_argc - 3] = argv[0];
	fw_argv[fw_argc - 2] = "do_test_internal";
	fw_argv[fw_argc - 1] = pipe_fd_arg;
	fw_argv[fw_argc - 0] = NULL;


	child_pid = fork();
	if (child_pid < 0) {
		err = errno;
		printf("Error: fork(): %s\n", strerror(err));
		ret = err;
		goto out;
	}


	if (child_pid == 0) {
		/*
		 * We are the child process!
		 */
		execve(fw_argv[0], fw_argv, environ);
		err = errno;
		printf("Error: execve(\"%s\"): %s\n", fw_argv[0],
			strerror(err));
		goto out;
	}

	ret = handle_wait(child_pid, pipe_fd);
out:
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	return ret;
}


static int handle_user(int argc, char *argv[])
{
	{
		char vg_path[0x100u] = {0};	/* Valgrind path. */

		strncpy(vg_path, argv[2], sizeof(vg_path));
		if (strncmp(basename(vg_path), "valgrind", 8)) {
			printf("argv[1] must be a path to valgrind\n");
			return EINVAL;
		}
	}

	return spawn_valgrind(argc, argv);
}


static int init_test(const char *pipe_wr_fd_str, const bluetest_entry_t *entry)
{
	int err;
	int pipe_wr_fd;
	bluetest_data_t	data = {0, 0};

	signal(SIGSEGV, sig_handler);
	signal(SIGABRT, sig_handler);
	__parent_pipe_wr_fd = pipe_wr_fd = atoi(pipe_wr_fd_str);
	printf("Initializing test (pipe_wr_fd: %d)...\n", pipe_wr_fd);

	while (*entry) {
		ssize_t write_ret;
		__will_run_test = false;
		g_data = data;

		/*
		 * This call is just calculate total tests.
		 */
		(*entry++)(&data.n_test, &data.n_pass);

		/*
		 * Report total point to parent to prevent misinformation
		 * when panic.
		 */
		write_ret = write(pipe_wr_fd, &data, sizeof(data));
		if (write_ret < 0) {
			err = errno;
			printf("Error: write(): %s\n", strerror(err));
		}
	}

	return 0;
}


static int run_test(const bluetest_entry_t *entry)
{
	int err;
	bluetest_data_t	data = {0, 0};
	int pipe_wr_fd = __parent_pipe_wr_fd;

	signal(SIGSEGV, sig_handler);
	signal(SIGABRT, sig_handler);
	printf("Running test (pipe_wr_fd: %d)...\n", pipe_wr_fd);

	while (*entry) {
		ssize_t write_ret;
		__will_run_test = true;
		g_data = data;

		/*
		 * This call is just calculate total tests.
		 */
		(*entry++)(&data.n_test, &data.n_pass);

		/*
		 * Report total point to parent to prevent misinformation
		 * when panic.
		 */
		write_ret = write(pipe_wr_fd, &data, sizeof(data));
		if (write_ret < 0) {
			err = errno;
			printf("Error: write(): %s\n", strerror(err));
		}
	}

	return 0;
}


/*
 * Usage:
 *
 * ./program do_test valgrind vgarg1 vgarg2 ...
 */
int main(int argc, char *argv[])
{
	int exit_code = 0;


	if (argc == 1)
		goto out_invalid;


	if (!strncmp(argv[1], "do_test", 9)) {
		/*
		 * User action.
		 */
		exit_code = handle_user(argc, argv);
		goto out;
	}


	if (!strncmp(argv[1], "do_test_internal", 17)) {
		/*
		 * Framework action.
		 */

		if (argc != 3)
			goto out_invalid;

		exit_code = init_test(argv[2], test_entry);
		if (exit_code != 0)
			goto out;

		exit_code = run_test(test_entry);
		goto out;
	}


out_invalid:
	exit_code = EINVAL;
	printf("Invalid argument\n");
out:
	return exit_code;
}
