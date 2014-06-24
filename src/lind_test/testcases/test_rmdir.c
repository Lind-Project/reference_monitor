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
	char *path = strcat(getcwd(cwd, sizeof(cwd)),"/testfiles/tmp_lind_test");
	test_rmdir(path);
	return 0;
}

void test_rmdir(char *path){

	int ret = mkdir(path, S_IRWXU);
	fprintf(stdout, "Create directory %s \n", path);

	if (ret == -1) {
		fprintf(stderr, "chown(%s) error \n", path);
	}

	if (rmdir(path) == -1) {
		fprintf(stderr, "rmdir(%s) error \n", path);
	}

	fprintf(stdout, "rmdir(%s)  \n", path);

}
