/*
 * test_read.c
 *
 *  Created on: Jun 10, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main(int argc, char **argv)
{
	char cwd[4096];
	char *path = strcat(getcwd(cwd, sizeof(cwd)),
			"/testfiles/test_pread64.txt");
	test_write(path);
	return 0;
}

void test_write(char *path)
{
	char buf[20];
	size_t nbytes;
	ssize_t bytes_written;

	strcpy(buf, "This is a test\n");
	nbytes = strlen(buf);

	int fd;
	fd = open(path, O_WRONLY);

	if (fd < 0) {
		fprintf(stderr, "open(%s) error \n", path);
		return;
	}

	bytes_written = write(fd, buf, nbytes);

	if (bytes_written == -1) {
		fprintf(stderr, "write failed \n");
		return;
	}

	if (close(fd) != 0){
		fprintf(stderr, "close() error \n");
		return;
	}

	fprintf(stdout, "\n");
	fprintf(stdout, "--- write() finished \n");

}

