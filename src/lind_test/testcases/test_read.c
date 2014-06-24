/*
 * test_read.c
 *
 *  Created on: Jun 10, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main(int argc, char **argv)
{
	char cwd [4096];
	char *path = "/testfiles/test_read.txt";
	test_read(strcat(getcwd(cwd, sizeof(cwd)), path));
	return 0;
}

void test_read(char *path)
{
	int fd;
	int nread;

	char buff[MAXBUF];

	fd = open(path, O_RDONLY);

	if (fd < 0){
		fprintf(stderr, "Could not open %s \n", path);
		return;
	}

	while ((nread = read(fd, buff, 4096)) > 0){
		fprintf(stdout, "%s", buff);
	}

	fprintf(stdout, "\n");
	close(fd);

	fprintf(stdout, "read(%s) finished \n", path);

}

