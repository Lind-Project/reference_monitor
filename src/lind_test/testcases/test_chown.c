/*
 * test_chown.c
 *
 *  Created on: Jun 11, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

void setup(char *path)
{
	int ret = mkdir(path, S_IRWXU);
	if (ret == -1) {
		fprintf(stderr, "Could not create directory to test chown() %s \n", path);
	}
}

void cleanup(char *path)
{
	int ret = rmdir(path);
	if (ret == -1) {
		fprintf(stderr, "Could not cleanup directory %s \n", path);
	}
}

int main()
{
	char *path = "tmp_lind_test";

	fprintf(stdout, "Create directory %s \n", path);
	setup(path);

	test_chown(path, TEST_GETUID, TEST_GETGID);

	fprintf(stdout, "Cleanup directory %s \n", path);
	cleanup(path);

	return 0;
}

void test_chown(char *path,  uid_t uid, gid_t gid)
{
	int ret = chown(path, uid, gid);
	if (ret == -1){
		fprintf(stderr, "Could not change the ownership of %s to uid: %d  gid: %d \n", path, uid, gid);
	}

	fprintf(stdout, "Changed the ownership of %s to uid: %d  gid: %d \n", path, uid, gid);
}
