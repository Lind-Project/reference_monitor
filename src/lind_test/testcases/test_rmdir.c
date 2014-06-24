/*
 * test_rmdir.c
 *
 *  Created on: Jun 6, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main(int argc, char **argv)
{
	char cwd [MAXBUF];
	char *path = "/testfiles/tmp_lind_test";
	test_rmdir(strcat(getcwd(cwd, sizeof(cwd)), path));
	return 0;
}

void test_rmdir(char *path){

	int ret = mkdir(path, S_IRWXU);
	fprintf(stdout, "Create directory %s \n", path);

	if (ret == -1) {
		fprintf(stderr, "Could not create directory to test chown() %s \n", path);
	}

	if (rmdir(path) == -1) {
		fprintf(stderr, "Removed directory %s \n", path);
	}
}
