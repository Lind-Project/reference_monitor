/*
 * test_dup3.c
 *
 *  Created on: Jun 11, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main(int argc, char **argv)
{
	test_dup3();
	return 0;
}

void test_dup3()
{

	/***********************************************************************
	* Code listing from "Advanced Linux Programming," by CodeSourcery LLC  *
	* Copyright (C) 2001 by New Riders Publishing                          *
	* See COPYRIGHT for license information.                               *
	***********************************************************************/

	int fds[2];
	pid_t pid;
	pipe(fds);

	pid = fork();

	if (pid ==  0) {
		close(fds[1]);
		/* Connect the read end of the pipe to standard input.  */
		syscall (SYS_dup3, fds[0], STDIN_FILENO, FD_CLOEXEC);
		/* Replace the child process with the "sort" program.  */
		execlp("sort", "sort", NULL);
	} else {
		/* This is the parent process.  */
		FILE* stream;
		/* Close our copy of the read end of the file descriptor.  */
		close(fds[0]);
		/* Convert the write file descriptor to a FILE object, and write
		 to it.  */
		stream = fdopen(fds[1], "w");
		fprintf(stream, "This is a dup3() test.\n");
		fflush(stream);
		close(fds[1]);
		/* Wait for the child process to finish.  */
		waitpid(pid, NULL, 0);
	}
}
