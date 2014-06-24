/*
 * test_chown.c
 *
 *  Created on: Jun 19, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main(int argc, char **argv)
{
	char cwd[MAXBUF];
	char *path = strcat(getcwd(cwd, sizeof(cwd)), "/testfiles/test_read.txt");
	test_lstat(path);
	return 0;
}


void test_lstat(char *path)
{
	/* taken from http://man7.org/linux/man-pages/man2/stat.2.html */
	struct stat sb;

	if (lstat(path, &sb) == -1) {
		fprintf(stderr, "stat failed %s \n", path);
		return;
	}

	fprintf(stdout, "File type:              \n ");

	switch (sb.st_mode & S_IFMT) {
		case S_IFBLK:
			fprintf(stdout, "block device\n");
			break;
		case S_IFCHR:
			fprintf(stdout, "character device\n");
			break;
		case S_IFDIR:
			fprintf(stdout, "directory\n");
			break;
		case S_IFIFO:
			fprintf(stdout, "FIFO/pipe\n");
			break;
		case S_IFLNK:
			fprintf(stdout, "symlink\n");
			break;
		case S_IFREG:
			fprintf(stdout, "regular file\n");
			break;
		case S_IFSOCK:
			fprintf(stdout, "socket\n");
			break;
		default:
			fprintf(stdout, "unknown?\n");
			break;
	}

	fprintf(stdout, "I-node number:            %ld\n", (long) sb.st_ino);

	fprintf(stdout, "Mode:                     %lo (octal)\n",
			(unsigned long) sb.st_mode);

	fprintf(stdout, "Link count:               %ld\n", (long) sb.st_nlink);
	fprintf(stdout, "Ownership:                UID=%ld   GID=%ld\n",
			(long) sb.st_uid, (long) sb.st_gid);

	fprintf(stdout, "Preferred I/O block size: %ld bytes\n",
			(long) sb.st_blksize);
	fprintf(stdout, "File size:                %lld bytes\n",
			(long long) sb.st_size);
	fprintf(stdout, "Blocks allocated:         %lld\n",
			(long long) sb.st_blocks);

	fprintf(stdout, "Last status change:       %s", ctime(&sb.st_ctime));
	fprintf(stdout, "Last file access:         %s", ctime(&sb.st_atime));
	fprintf(stdout, "Last file modification:   %s", ctime(&sb.st_mtime));

}
