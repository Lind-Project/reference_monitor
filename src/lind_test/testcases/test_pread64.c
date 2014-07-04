/*
 * test_pread64.c
 *
 *  Created on: Jun 24, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main(int argc, char **argv)
{
	char *path = strcat(get_testfiles_dir(),"/test_pread64.txt");
	test_pread64(path);
	return 0;
}

void test_pread64(char *path)
{
	  int ret, rc, fd;
	  off_t  off = 10;

	  char buffer[MAXBUF];

	  if ((fd = open64(path, O_RDONLY)) < 0){
		  fprintf(stderr,"open64(%s) error \n", path);
	  	  return;
	  }

	   ret = pread64(fd, buffer, ((sizeof(buffer)-1)-off), off);
	   buffer[ret] = 0x00;

	   fprintf(stdout, "pread64  \n %s \n", buffer);

	   if (close(fd)!= 0){
	       fprintf(stderr, "close() error \n");
	   	   return;
	   }
}
