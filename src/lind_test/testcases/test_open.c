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
	char *path = strcat(getcwd(cwd, sizeof(cwd)),"/testfiles/test_read.txt");
	test_open(path);
	return 0;
}

void test_open(char *path)
{
	int fd;
	fd = open(path, O_APPEND);
	if (fd < 0){
		fprintf(stderr, "open(%s) error \n", path);
		return;
	}

	 if (close(fd)!= 0){
	     fprintf(stderr, "close() error \n");
	     return;
	}

	fprintf(stdout, "open(%s) successfully \n", path);
}

