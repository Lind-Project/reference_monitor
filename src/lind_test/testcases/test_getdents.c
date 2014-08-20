/*
 * test_getdents.c
 *
 *  Created on: Jun 24, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

/* taken from http://man7.org/linux/man-pages/man2/getdents.2.html */

struct linux_dirent {
	long d_ino;
	off_t d_off;
	unsigned short d_reclen;
	char d_name[];
	// There is a zero padding byte here before the type.  However, C doesn't allow you to reference struct items after a char[] (for obvious reasons)
/*	unsigned long           d_ino;
	unsigned long           d_off;
	unsigned short 		    d_reclen;
	char           			*d_name;*/
//	char					pad;
//	char 		   			d_type;
};

#define BUF_SIZE 4096


int main(int argc, char **argv)
{
	test_getdents(get_testfiles_dir());
	return 0;
}

void test_getdents(const char *path)
{
	int fd, nread;
	char buf[BUF_SIZE];
	struct linux_dirent *d;
	int bpos;

	int i;
	char *printbuf;

	fd = open(path, O_RDONLY | O_DIRECTORY);

	if (fd == -1) {
		fprintf(stderr, "open(%s) error \n", path);
		return;
	}

	for ( ; ; ) {
	  nread = syscall(SYS_getdents, fd, buf, BUF_SIZE);

	  if (nread == -1) {
			fprintf(stderr, "getdents(%s) error \n", path);
			return;
	  }

	  if (nread == 0)
		  break;

	  fprintf(stdout, "--------------- nread=%d ---------------\n", nread);
	  fprintf(stdout,"i-node#  file type  d_reclen  d_off   d_name\n");
	  for (bpos = 0; bpos < nread;) {

		  d = (struct linux_dirent *) (buf + bpos);
		  printf("Buffer size: %d\n", d->d_reclen);
		  // print the bytes in the dir entry...
		  printbuf= (char *)d;
		  for (i=0; i<d->d_reclen; i++) {
			  printf("%02x ",(unsigned char) *(printbuf+i));
		  }
		  printf("\n");
		  fprintf(stdout, "%8ld  ", d->d_ino);
		char d_type = *(buf + bpos + d->d_reclen - 1);
		  fprintf(stdout,"raw d_type '%d' ", d_type);

		  fprintf(stdout,"%-10s ", (d_type == DT_REG) ?  "regular" :
						   (d_type == DT_DIR) ?  "directory" :
						   (d_type == DT_FIFO) ? "FIFO" :
						   (d_type == DT_SOCK) ? "socket" :
						   (d_type == DT_LNK) ?  "symlink" :
						   (d_type == DT_BLK) ?  "block dev" :
						   (d_type == DT_CHR) ?  "char dev" : "???");
		  fprintf(stdout,"%4d %10lld  %s\n", d->d_reclen,
				  (long long) d->d_off, d->d_name);
		  bpos += d->d_reclen;
	  }
	}
}
