/*
 * test_mkdir.c
 *
 *  Created on: Jun 11, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"


int main(int argc, char **argv)
{
	char cwd [MAXBUF];
	char *path = "/testfiles/tmp_lind_test";
	test_mkdir(strcat(getcwd(cwd, sizeof(cwd)), path));
	return 0;
}

void test_mkdir(char *path)
{
	int ret = mkdir(path, S_IRWXU);
	if (ret == -1) {
		fprintf(stderr, "mkdir(%s) failed \n", path);
	}

	ret = rmdir(path);
	if (ret == -1) {
		fprintf(stderr, "Could not cleanup directory %s \n", path);
	}

}
