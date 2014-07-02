/*
 * test_unlink.c
 *
 *  Created on: Jun 22, 2014
 *      Author:  Ali Gholami
 */


#include "testcases.h"

int main(int argc, char **argv) {

	char *path = strcat(get_testfiles_dir(),"/test_unlink.txt");
	test_unlink(path);
	return 0;
}

void test_unlink(char *dest) {
	if (unlink(dest) == -1) {
			fprintf(stderr, "unlink(%s) error \n", dest);
			return;
		}

	fprintf(stderr, "unlinked(%s) \n", dest);
}
