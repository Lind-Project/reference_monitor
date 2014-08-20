/*
 * test_dup2.c
 *
 *  Created on: Jun 23, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main(int argc, char **argv)
{
	char *path = strcat(get_testfiles_dir(),"/test_read.txt");
	test_dup2(path);
	return 0;
}

void test_dup2(char* path)
{

	int fd;
	int newfd = 230;
	fd = open(path, O_WRONLY | O_CREAT, S_IREAD | S_IWRITE);
	if (fd == -1) {
		fprintf(stderr, "%s \n", path);
		return;
	}

	int ret = dup2(fd, newfd);

	if (ret != newfd ){
		fprintf(stderr, "dup2() error \n");
		return;
	}

	if (close(fd) != 0) {
		fprintf(stderr, "close() error \n");
		return;
	}

	if (close(newfd) != 0) {
		fprintf(stderr, "close() error \n");
		return;
	}

	fprintf(stdout, "Lind dup2 executed successfully!\n");

}

