/*
 * test_creat.c
 *
 *  Created on: Jun 21, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main() {
	char cwd [MAXBUF];
	char *path = "/testfiles/test_read.txt";
	test_creat(strcat(getcwd(cwd, sizeof(cwd)), path));
	return 0;
}


void test_creat(char *path)
{

	int fd;
	fd = creat(path, S_IREAD | S_IWRITE);
	if (fd == -1){
		fprintf(stderr, "Error in opening datafile.dat\n");
		return;
	}
	else {
		fprintf(stdout, "datafile.dat opened for read/write access\n");
		fprintf(stdout, "datafile.dat is currently empty\n");
	}
	close(fd);

}
