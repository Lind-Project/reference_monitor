/*
 * test_chown.c
 *
 *  Created on: Jun 20, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main(int argc, char **argv)
{
	char *path = "/etc";
	test_statfs(path);
	return 0;
}


void test_statfs(char *path)
{
	/* taken from http://man7.org/linux/man-pages/man2/stat.2.html */
	struct statfs sb;
	int ret;

	if ((ret=statfs(path, &sb))  == -1) {
		fprintf(stderr, "stat() failed %s \n", path);
		return;
	}

	fprintf(stdout, " statfs returned          %d \n ", ret);

}
