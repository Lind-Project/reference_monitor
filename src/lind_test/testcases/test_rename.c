/*
 * test_rename.c
 *
 *  Created on: Jun 11, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main(int argc, char **argv)
{
	char *path = strcat(get_testfiles_dir(),"/test_rename.txt");
	char *renamed_path = strcat(get_testfiles_dir(),"/test_renamed.txt");

	test_rename(path, renamed_path);
	return 0;
}

void test_rename(char *old_path, char *new_path)
{
	int ret;

	ret = rename(old_path, new_path);

	if (ret <0) {
		fprintf(stderr, "could not rename %s to %s \n", old_path, new_path);
		return;
	}

	ret = rename(new_path, old_path);

	if (ret <0) {
		fprintf(stderr, "could not rename %s to %s \n", old_path, new_path);
		return;
	}

	fprintf(stdout, "renamed (%s to %s) \n ", old_path , new_path);
}
