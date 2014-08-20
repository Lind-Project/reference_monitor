/*
 * test_dup3.c
 *
 *  Created on: Jun 11, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"


int main(int argc, char **argv)
{
	char *path = strcat(get_testfiles_dir(),"/test_read.txt");
	test_dup3(path);
	return 0;
}

void test_dup3(char* path)
{

	int fd;
	int newfd = 240;
	fd = open(path, O_WRONLY | O_CREAT, S_IREAD | S_IWRITE);
	if (fd == -1) {
		fprintf(stderr, "%s \n", path);
		return;
	}

	int ret = syscall(SYS_dup3, fd, newfd, O_CLOEXEC);

	if (ret != newfd ){
		fprintf(stderr, "dup3() error \n");
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

	fprintf(stdout, "Lind dup3 executed successfully!\n");

}
