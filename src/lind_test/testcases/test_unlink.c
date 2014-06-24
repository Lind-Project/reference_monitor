/*
 * test_ulink.c
 *
 *  Created on: Jun 22, 2014
 *      Author:  Ali Gholami
 */


#include "testcases.h"

int main(int argc, char **argv) {
	char cwd[MAXBUF];
	char *dest = strcat(getcwd(cwd, sizeof(cwd)),"/testfiles/test_unlink.txt");
	test_unlink(dest);
	return 0;
}

void test_unlink(char *dest) {
	if (unlink(dest) == -1) {
			fprintf(stderr, "unlink(%s) error \n", dest);
			return;
		}

	fprintf(stderr, "unlinked(%s) \n", dest);
}
