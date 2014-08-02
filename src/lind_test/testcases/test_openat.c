/*
 * test_openat.c
 *
 *  Created on: Jul 10, 2014
 *      Author:  Ali Gholami
 */


#include "testcases.h"

int main(int argc, char **argv)
{
	char *path = strcat(get_testfiles_dir(),"/test_open.txt");
	test_openat(path);
	return 0;
}

void test_openat(char *path)
{

	   int fd;
	   mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	   int fd_dir = AT_FDCWD;
	   fd = openat(fd_dir, path, O_RDONLY, mode);
	   if(fd == -1){
		   fprintf(stderr, "	[testcases] openat(%d, %s, %d, %d) = %d  error \n", fd_dir, path, O_RDONLY, mode, fd);
	    	return ;
	    }

	   fprintf(stdout, "[testcases] openat(%d, %s, %d, %d) = %d \n", fd_dir, path, O_RDONLY, mode, fd);

	   if (close(fd)!= 0){
		   fprintf(stderr, "[testcases] close(%s) error \n", path);
    	   return;
	   }

}
