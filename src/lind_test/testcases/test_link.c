/*
 * test_link.c
 *
 *  Created on: Jun 22, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main(int argc, char **argv) {

	char cwd[MAXBUF];
	char *src = "/testfiles/test_link.txt";
	char *dest = "/testfiles/test_unlink.txt";

	test_link(strcat(getcwd(cwd, sizeof(cwd)), src), strcat(getcwd(cwd, sizeof(cwd)), dest));
	return 0;
}

void test_link(char *src, char * dest) {
	if ((link(src, dest)) == -1) {
		fprintf(stderr, " could not link %s to %s \n", src, dest);
		return;
	}

	fprintf(stdout, "linked %s to %s \n", src, dest);

}

