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
	fd = open(path, O_RDONLY);
	if (fd < 0){
		fprintf(stderr, "open(%s) error \n", path);
		return;
	}

	 if (close(fd)!= 0){
	     fprintf(stderr, "close(%s) error \n", path);
	     return;
	}

	fprintf(stdout, "open(%s) successfully \n", path);
}

