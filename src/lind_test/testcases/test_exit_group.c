/*
 * test_exit_group.c
 *
 *  Created on: Jun 24, 2014
 *      Author:  Ali Gholami
 */


#include "testcases.h"

int main(int argc, char **argv)
{
	test_exit_group();
	return 0;
}

void test_exit_group()
{
	int status;
	pid_t pid;
	pid = fork ();

	if (pid < 0){
		fprintf(stderr, "could not create a process \n");
		return;
	}

	if (pid == 0 )  {
		status = 2;
		fprintf(stdout, "parent exit_group status= %d \n", status);
		exit(status);
	} else  {
		wait(&status);
		fprintf(stdout, "child exit_group status = %d \n", status);
	}

}
