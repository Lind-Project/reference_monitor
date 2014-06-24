/*
 * test_close.c
 *
 *  Created on: Jun 11, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"


int main(int argc, char **argv)
{
	char cwd [MAXBUF];
	char *path = strcat(getcwd(cwd, sizeof(cwd)), "/testfiles/test_read.txt");
	test_close(path);
	return 0;
}

void test_close(char *path)
{
	int fd;
	fd = open(path, O_RDONLY);

	if (fd < 0){
		fprintf(stderr, "Could not open %s \n", path);
		return;
	}

	int ret = close(fd);

	if (ret == -1) {
		fprintf(stderr, "Could not close the file %s \n ", path);
		return;
	}
}

