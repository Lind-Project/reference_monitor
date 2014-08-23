/*
 * 	test_fstat.c
 *
 *  Created on: Jun 18, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main(int argc, char **argv)
{
	char *path = strcat(get_testfiles_dir(),"/test_read.txt");
	test_fstat(path);
	return 0;
}


void test_fstat(char *path)
{
	int fd = open(path, O_RDONLY);

	if (fd < 0){
		fprintf(stderr, "Could not open %s \n", path);
		return;
	}

	/* http://codewiki.wikidot.com/c:system-calls:stat */
    struct stat fileStat;
    if(fstat(fd, &fileStat) < 0)
        return ;
    fprintf(stdout, "Information for %s\n", path);
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "File Size: \t\t%d bytes\n", (int) fileStat.st_size);
    fprintf(stdout, "Number of Links: \t%d\n", (int) fileStat.st_nlink);
    fprintf(stdout, "File inode: \t\t%d\n",  (int) fileStat.st_ino);

    fprintf(stdout, "File Permissions: \t");
    printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
    fprintf(stdout, "\n\n");

    fprintf(stdout, "The file %s a symbolic link\n", (S_ISLNK(fileStat.st_mode)) ? "is" : "is not");

	if (close(fd)!= 0){
		fprintf(stderr, "close() error \n");
		return;
	}
}
