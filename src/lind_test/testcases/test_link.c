/*
 * test_link.c
 *
 *  Created on: Jun 22, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main(int argc, char **argv)
{

	char *src = strcat(get_testfiles_dir(),"/test_link.txt");
	char *dest = strcat(get_testfiles_dir(),"/test_ulink.txt");
	test_link(src, dest);
	return 0;
}

void test_link(char *src, char * dest)
{

	if ((link(src, dest)) == -1) {
		fprintf(stderr, " could not link %s to %s \n", src, dest);
		return;
	}

	if ((unlink(dest)) == -1) {
		fprintf(stderr, " could not unlink %s \n", dest);
		return;
	}

	fprintf(stdout, "linked %s to %s \n", src, dest);
}

