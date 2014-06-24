/*
 * test_chown.c
 *
 *  Created on: Jun 23, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main(int argc, char **argv)
{
	char cwd[4096];
	char *path = "/testfiles/test_lseek.txt";
	test_lseek(strcat(getcwd(cwd, sizeof(cwd)), path));
	return 0;
}

void test_lseek(char *path)
{
   int fd;
   long position;

   fd = open(path, O_RDONLY);
   if (fd != -1){
      position = lseek(fd, 0L, 2);  /* seek 0 bytes from end-of-file */
      if (position != -1)
         fprintf(stdout, "The length of datafile.dat is %ld bytes.\n", position);
      else
         fprintf(stderr, "lseek() error  \n");
      }
   else {
      fprintf(stderr, "can't read %s \n", path);
   }

    close(fd);
}

