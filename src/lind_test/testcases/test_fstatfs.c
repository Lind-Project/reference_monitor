/*
 * test_fstatfs.c
 *
 *  Created on: Jun 18, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main(int argc, char **argv) {
	char *path = get_testfiles_dir();
	test_fstatfs(path);
	return 0;
}

void test_fstatfs(char *path) {
	/* taken from http://man7.org/linux/man-pages/man2/stat.2.html */
	struct statfs sb;
	int ret;

	int fd = open(path, O_RDONLY);

	if (fd < 0) {
		fprintf(stderr, "open(%s) error \n", path);
		return;
	}

	if ((ret = fstatfs(fd, &sb)) == -1) {
		fprintf(stderr, "stat(%s) failed \n", path);
		return;
	}

	 if (close(fd)!= 0){
		 fprintf(stderr, "close() error \n");
	     return;
	 }

	 fprintf(stdout, "fstatfs()= %d \n ", ret);

}
