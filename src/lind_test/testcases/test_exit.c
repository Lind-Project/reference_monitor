/*
 * test_chown.c
 *
 *  Created on: Jun 23, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"


int main(int argc, char **argv)
{
	test_exit();
	return 0;
}

void test_exit()
{
	int status;
	pid_t pid;
	pid = fork ();
	if (pid == 0 )  {
		fprintf(stdout, "parent exiting status= %d \n", status);
		exit (status);
	} else  {
		wait(&status);
		fprintf(stdout, "child exit status = %d \n", status);
	}
}
