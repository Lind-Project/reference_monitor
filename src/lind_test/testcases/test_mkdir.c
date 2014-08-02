/*
 * test_mkdir.c
 *
 *  Created on: Jun 11, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"


int main(int argc, char **argv)
{
	char *path = strcat(get_testfiles_dir(),"/tmp_lind_test");
	test_mkdir(path);
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
		fprintf(stderr, "cleanup(%s) directory error \n", path);
	}

}
