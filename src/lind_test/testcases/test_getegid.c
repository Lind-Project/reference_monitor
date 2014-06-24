/*
 * test_getegid.c
 *
 *  Created on: Jun 23, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main(int argc, char **argv)
{
	test_getegid();
	return 0;
}

void test_getegid()
{
	gid_t egid = getegid();
	assert( egid == TEST_GETGID);
	fprintf(stdout, "Current effective group id number is %d \n", egid);
}
