/*
 * test_creat.c
 *
 *  Created on: Jun 21, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main() {
	char cwd [MAXBUF];
	char *path = "/testfiles/test_creat.txt";
	test_creat(strcat(getcwd(cwd, sizeof(cwd)), path));
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

	close(fd);

	fprintf(stdout, "%s opened successfully for read/write access \n", path);
}
