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
	int newfd = STDERR_FILENO;

	fd = open("/var/log/apache2/error.log",O_WRONLY|O_CREAT|O_APPEND|O_CLOEXEC, 0666);
	if (fd == -1) {
		fprintf(stderr, "%s \n", path);
		return;
	}
	printf("1BEFORE\n");
	//close(newfd);
	write(fd,"1ERR LOOK IN THIS FILE\n\n\n\n\n\n\n\n",24);
	printf("1AFTER\n");


	int ret = syscall(SYS_dup3, fd, newfd, 0);

	printf("2BEFORE\n");
	//close(newfd);
	write(fd,"2ERR LOOK IN THIS FILE\n\n\n\n\n\n\n\n",24);
	printf("2AFTER\n");

	printf("2BEFORE\n");
	//close(newfd);
	write(newfd,"3ERR LOOK IN THIS FILE\n\n\n\n\n\n\n\n",24);
	printf("2AFTER\n");


	if (ret != newfd ){
		fprintf(stderr, "dup3() error \n");
		perror("dup3");
		return;
	}

	if (close(fd) != 0) {
		fprintf(stderr, "close(%d) error \n", fd);
		return;
	}

	if (close(newfd) != 0) {
		fprintf(stderr, "close(%d) error \n", newfd);
		return;
	}

	fprintf(stderr, "Test!\n");
	fprintf(stdout, "Lind dup3 executed successfully!\n");

}
