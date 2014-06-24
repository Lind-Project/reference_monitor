/*
 * test_exit.c
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

	if (pid < 0){
		fprintf(stderr, "coud not create a proccess \n");
		return;
	}

	if (pid == 0 )  {
		fprintf(stdout, "parent exit statu s= %d \n", status);
		exit (status);
	} else  {
		wait(&status);
		fprintf(stdout, "child exit status = %d \n", status);
	}
}
