/*
 * test_getdents.c
 *
 *  Created on: Jun 24, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

/* taken from http://man7.org/linux/man-pages/man2/getdents.2.html */

struct linux_dirent {
	long           d_ino;
	off_t          d_off;
	unsigned short d_reclen;
	char           d_name[];
};

#define BUF_SIZE 1024


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
	char d_type;

	fprintf(stdout, "'FIRST!!!\n");
	fd = open(path, O_RDONLY | O_DIRECTORY);

	if (fd == -1) {
		fprintf(stderr, "open(%s) error \n", path);
		return;
	}

	  fprintf(stdout, "'BEFORE!!!\n");

	for ( ; ; ) {
	  nread = syscall(SYS_getdents, fd, buf, BUF_SIZE);
	  fprintf(stdout, "HERE!!!\n");

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
		  fprintf(stdout, "%8ld  ", d->d_ino);
		  d_type = *(buf + bpos + d->d_reclen - 1);
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
