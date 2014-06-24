/*
 * test_ulink.c
 *
 *  Created on: Jun 22, 2014
 *      Author:  Ali Gholami
 */


#include "testcases.h"

int main(int argc, char **argv) {
	char cwd[MAXBUF];
	char *dest = "/testfiles/test_unlink.txt";

	test_unlink(strcat(getcwd(cwd, sizeof(cwd)), dest));
	return 0;
}

void test_unlink(char *dest) {
	if (unlink(dest) == -1) {
			fprintf(stderr, " could not unlink %s \n", dest);
			return;
		}

	fprintf(stderr, "unlinked(%s) \n", dest);
}
