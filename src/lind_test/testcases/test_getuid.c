/*
 * test_getuid.c
 *
 *  Created on: Jun 6, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main(int argc, char **argv)
{
	test_getuid();
	return 0;
}

void test_getuid()
{
	gid_t uid = getuid();
	assert(uid == TEST_GETUID);
	fprintf(stdout, "Current user id number is: %d \n", uid);
}
