
/*
 * test_getpid.c
 *
 *  Created on: Jun 10, 2014
 *      Author:  Ali Gholami
 */


#include "testcases.h"

int main(int argc, char **argv)
{
	test_getpid();
	return 0;
}

void test_getpid()
{
	pid_t pid = getpid();
	if (pid == -1) {
		fprintf(stderr, "Could not get the pid, %d \n", pid);
	}
}
