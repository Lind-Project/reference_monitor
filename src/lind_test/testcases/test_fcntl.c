/*
 * test_fcntl.c
 *
 *  Created on: Jun 24, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"


void test_read_lock(char *path);
void test_write_lock(char *path);

int main(int argc, char **argv)
{
	char *path = strcat(get_testfiles_dir(),"/test_fcntl.txt");
	test_fcntl(path);
	return 0;
}

void test_fcntl(char *path)
{
	test_read_lock(path);
	test_write_lock(path);
}

void test_read_lock(char *path)
{
	int fd;
	struct flock lock, locked;

	fd = open(path, O_RDONLY);

	if(fd < 0 ){
		fprintf(stderr, "open(%s) error \n", path);
		return;
	}

	lock.l_type = F_RDLCK;
	lock.l_start = 0;
	lock.l_whence = SEEK_SET;
	lock.l_len = 50;
	locked = lock;

	int ret = fcntl(fd, F_GETLK, &lock);

	if (ret < 0 ){
		fprintf(stderr, "fcntl(%s) error \n", path);
		return;
	}

	if(lock.l_type == F_WRLCK){

		fprintf(stdout,"Process %d has a write lock already!\n", lock.l_pid);
		return;

	}else if(lock.l_type == F_RDLCK){

		fprintf(stderr, "File is write-locked by process %d.\n", lock.l_pid);
		return;

	}else{

		ret = fcntl(fd, F_SETLK, &locked);

		if(ret < 0 ){
			fprintf(stderr, "fcntl(%s) error \n", path);
			return;
		}
	}

	if (close(fd) != 0){
		fprintf(stderr, "close() error \n");
		return;
	}
}

void test_write_lock(char *path)
{
	int fd;
	struct flock lock, locked;

	fd = open(path, O_RDWR);

	if(fd < 0 ){
		fprintf(stderr, "open(%s) error \n", path);
		return;
	}

	fprintf(stdout, "open(%s) \n", path);


	lock.l_type = F_WRLCK;
	lock.l_start = 0;
	lock.l_whence = SEEK_SET;
	lock.l_len = 0;
	locked = lock;

	int ret = fcntl(fd, F_GETLK, &lock);

	if (ret < 0 ){
		fprintf(stderr, "fcntl(%s) error \n", path);
		return;
	}

	if(lock.l_type == F_WRLCK){

		fprintf(stdout,"Process %d has a write lock already!\n", lock.l_pid);
		return;

	}else if(lock.l_type == F_RDLCK){

		fprintf(stderr, "Process %d has a read lock already!\n", lock.l_pid);
		return;

	}else{

		ret = fcntl(fd, F_SETLK, &locked);

		if(ret < 0 ){
			fprintf(stderr, "fcntl(%s) error \n", path);
			return;
		}
	}

	if (close(fd) != 0){
		fprintf(stderr, "close() error \n");
		return;
	}
}
