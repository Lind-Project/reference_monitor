/*
 * test_gid.c
 *
 *  Created on: Jun 6, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main(int argc, char **argv)
{
	test_getgid();
	return 0;
}

void test_getgid()
{
	uid_t gid = getgid();
	assert( gid == TEST_GETUID);
	fprintf(stdout, "Current group id number is %d \n", gid);
}

