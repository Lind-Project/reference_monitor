/*
 * test_dup.c
 *
 *  Created on: Jun 23, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main()
{
	char cwd[MAXBUF];
	char *path = strcat(getcwd(cwd, sizeof(cwd)), "/testfiles/test_read.txt");
	test_dup(path);

	return 0;
}

void test_dup(char *path)
{
	int fd;
	fd = open(path, O_WRONLY | O_CREAT, S_IREAD | S_IWRITE);
	if (fd == -1) {
		fprintf(stderr, "%s \n", path);
		return;
	}

	if (close(1) != 0) {
		fprintf(stderr, "close() error \n");
		return;
	}

	dup(fd);

	if (close(fd) != 0) {
		fprintf(stderr, "close() error \n");
		return;
	}

	fprintf(stdout, "Lind dup executed successfully!\n");
}

