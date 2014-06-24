/*
 * test_chown.c
 *
 *  Created on: Jun 11, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main(int argc, char **argv)
{
	char cwd [MAXBUF];
	char *path = "/testfiles/test_read.txt";
	test_open(strcat(getcwd(cwd, sizeof(cwd)), path));
	return 0;
}

void test_open(char *path)
{
	int fd;
	fd = open(path, O_APPEND);
	if (fd < 0){
		fprintf(stderr, "Could not open %s \n", path);
		return;
	}

	close(fd);
}

