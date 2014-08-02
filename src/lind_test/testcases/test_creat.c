/*
 * test_creat.c
 *
 *  Created on: Jun 21, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main() {

	char *path = strcat(get_testfiles_dir(),"/test_creat.txt");
	test_creat(path);
	return 0;
}

void test_creat(char *path)
{
	int fd;
	fd = creat(path, S_IREAD | S_IWRITE);

	if (fd == -1){
		fprintf(stderr, "error in opening %s \n", path);
		return;
	}


	if (close(fd)!= 0){
	       fprintf(stderr, "close() error \n");
	   	   return;
	}

	fprintf(stdout, "%s opened successfully for read/write access \n", path);
}
