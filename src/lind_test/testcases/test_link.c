/*
 * test_link.c
 *
 *  Created on: Jun 22, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main(int argc, char **argv)
{

	char cwd1[MAXBUF];
	char cwd2[MAXBUF];
	char *src = strcat(getcwd(cwd1, sizeof(cwd1)), "/testfiles/test_link.txt");
	char *dest =  strcat(getcwd(cwd2, sizeof(cwd2)), "/testfiles/test_unlink.txt");
	test_link(src, dest);
	return 0;
}

void test_link(char *src, char * dest)
{

	if ((link(src, dest)) == -1) {
		fprintf(stderr, " could not link %s to %s \n", src, dest);
		return;
	}

	fprintf(stdout, "linked %s to %s \n", src, dest);
}

