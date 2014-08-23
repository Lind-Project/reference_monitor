/*
 * test_geteuid.c
 *
 *  Created on: Jun 23, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main(int argc, char **argv)
{
	test_geteuid();
	return 0;
}

void test_geteuid()
{
	uid_t euid = geteuid();
	fprintf(stdout, "Current effective user id number is: %d \n", euid);
}
