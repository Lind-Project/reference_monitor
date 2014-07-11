/*
 * test_open.c
 *
 *  Created on: Jun 11, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main(int argc, char **argv)
{
	char *path = strcat(get_testfiles_dir(),"/test_open.txt");
	test_open(path);
	return 0;
}

void test_open(char *path)
{
	int fd;

	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	fd = open(path, O_RDONLY, mode);
	if (fd < 0){
		fprintf(stderr, "[testcases] open(%s) error \n", path);
		return;
	}

	 if (close(fd)!= 0){
	     fprintf(stderr, "[testcases] close(%s) error \n", path);
	     return;
	}

	fprintf(stdout, "[testcases] open(%s) = %d \n", path, fd);
}

