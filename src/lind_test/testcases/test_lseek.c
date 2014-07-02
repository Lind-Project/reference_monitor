/*
 * test_lseek.c
 *
 *  Created on: Jun 23, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main(int argc, char **argv)
{
	char *path = strcat(get_testfiles_dir(),"/test_lseek.txt");
	test_lseek(path);
	return 0;
}

void test_lseek(char *path)
{
   int fd;
   long position;

   fd = open(path, O_RDONLY);
   if (fd != -1){
      position = lseek(fd, 0L, 1);
      if (position != -1)
         fprintf(stdout, "The length of %s is %ld bytes.\n", path, position);
      else
         fprintf(stderr, "lseek() error  \n");
      }
   else {
      fprintf(stderr, "can't read %s \n", path);
   }

   if (close(fd)!= 0){
  	       fprintf(stderr, "close() error \n");
  	   	   return;
  	}
}

