/*
 * test_dup.c
 *
 *  Created on: Jun 23, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main()
{
	char cwd [MAXBUF];
	char *path = "/testfiles/test_read.txt";

	test_dup(strcat(getcwd(cwd, sizeof(cwd)), path));

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
	close(1);
	dup(fd);
	close(fd);

	fprintf(stdout, "Lind dup executed successfully!\n");
}

