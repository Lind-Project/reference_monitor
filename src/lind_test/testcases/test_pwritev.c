/*
 * test_pwrtiev.c
 *
 *  Created on: Jun 24, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main(int argc, char **argv)
{
	char *path = strcat(get_testfiles_dir(),"/test_pwritev.txt");
	test_pwritev(path);
	return 0;
}

void test_pwritev(char *path)
{

   ssize_t nwritten;
   int ret, rc, fd;
   off_t  off = 10;

   char *str0 = "Lind";
   char *str1 = "pwritev () test \n";
   struct iovec iov[2];

   if ((fd = open64(path, O_WRONLY)) < 0){
	   fprintf(stderr,"open64(%s) error \n", path);
	   return;
   }

   iov[0].iov_base = str0;
   iov[0].iov_len = strlen(str0);

   iov[1].iov_base = str1;
   iov[1].iov_len = strlen(str1);

   nwritten = pwritev(fd, iov, 2, off);

   if (close(fd)!= 0){
       fprintf(stderr, "close() error \n");
   	   return;
   }
}
