/*
 * test_flock.c
 *
 *  Created on: Jun 24, 2014
 *      Author:  Ali Gholami
 */


#include "testcases.h"

int main(int argc, char **argv)
{
	char *path = strcat(get_testfiles_dir(),"/test_flock.txt");
	test_flock(path);
	return 0;
}

void test_flock(char *path)
{
	int ret, fd;

	char buf[20];
	size_t nbytes;
	ssize_t bytes_written;

	strcpy(buf, "This is a test\n");
	nbytes = strlen(buf);


	fd = open(path, O_WRONLY|O_APPEND);

	if (fd < 0 ){
		fprintf(stderr, "open(%s) error \n", path);
		return;
	}

	ret = flock(fd, LOCK_EX);

	if (ret < 0 ){
		fprintf(stderr, "flock(%s) error \n", path);
		return;
	}

	fprintf(stdout, "flock() locked \n");

	bytes_written = write(fd, buf, nbytes);

	if (bytes_written < 0 ){
		fprintf(stderr, "write(%s) error \n", path);
		return;
	}

	ret = flock(fd, LOCK_UN);

	if (ret < 0 ){
		fprintf(stderr, "flock(%s) error \n", path);
		return;
	}

	fprintf(stdout, "flock() unlocked \n");

	ret = close(fd);

	if (ret < 0 ){
		fprintf(stderr, "close(%s) error \n", path);
		return;
	}
}
