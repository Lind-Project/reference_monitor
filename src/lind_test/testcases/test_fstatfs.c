/*
 * test_chown.c
 *
 *  Created on: Jun 18, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main(int argc, char **argv)
{
	char *path = "/etc";
	test_fstatfs(path);
	return 0;
}

/* taken from http://man7.org/linux/man-pages/man2/stat.2.html */
void test_fstatfs(char *path)
{
	struct statfs sb;
	int fd = open(path, O_RDONLY);
		if (fd < 0){
			fprintf(stderr, "Could not open %s \n", path);
			return;
		}
	int ret;
	if ((ret=fstatfs(fd, &sb)) == -1) {
		fprintf(stderr, "stat failed %s \n", path);
		return;
	}

	fprintf(stdout, "fstatfs   returned       %d \n ", ret);
	close(fd);
}
