/*
 * test_mkdir.c
 *
 *  Created on: Jun 11, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main(int argc, char **argv)
{
	char cwd1 [MAXBUF];
	char cwd2 [MAXBUF];
	char *path = "/testfiles/test_read.txt";
	char *renamed_path = "/testfiles/test_renamed.txt";

	test_rename(strcat(getcwd(cwd1, sizeof(cwd1)), path), strcat(getcwd(cwd2, sizeof(cwd2)), renamed_path));
	return 0;
}

void test_rename(char *old_path, char *new_path)
{
	int ret = rename(old_path, new_path);

	if (ret <0) {
		fprintf(stderr, "could not rename %s to %s \n", old_path, new_path);
		return;
	}

	fprintf(stdout, "renamed (%s to %s) \n ", old_path , new_path);
}
