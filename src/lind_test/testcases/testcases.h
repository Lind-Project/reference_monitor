/*
 * test_ugid.h
 *
 *  Created on: Jun 17, 2014
 *      Author:  Ali Gholami
 */

#ifndef TEST_UGID_H_
#define TEST_UGID_H_

#define SERVER_ADDR     "173.194.121.34"

/* connection port */
#define PORT_TEST       80

 /* Google server */
#define MAXBUF          4096

#define TRUE   1
#define FALSE  0

#define TEST_GETUID 1000
#define TEST_GETGID 1000
#define TEST_GETPID 1000


#include <limits.h>
#include <signal.h>
#include <sys/statfs.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/wait.h>
#include <sys/un.h>
#include <pthread.h>

void test_getuid();
void test_getgid();
void test_geteuid();
void test_getegid();
void test_getpid();
void test_date();
void test_chown(char *path, uid_t uid, gid_t gid);
void test_rmdir(char *path);
void test_mkdir(char *path);
void test_rename(char *path, char *rename);
void test_sleep(int time);
void test_read(char *path);
void test_write(char *path);
void test_open(char *path);
void test_close(char *path);
void test_curl();
void test_socket();
void test_stat(char *path);
void test_fstat(char *path);
void test_lstat(char *path);
void test_statfs(char *path);
void test_fstatfs(char *path);
void test_access(char *path);
void test_connect();
void *test_accept();
void *test_accept_client();
void test_bind();
void test_select(char *src, char *dest);
void test_lseek(char *path);
void test_send();
void test_recv();
void test_link(char *src, char * dest);
void test_unlink(char *dest);
void test_exit();
void test_shutdown();
void *test_listen();
void *test_client();
void test_getsockname();
void test_getsockopt();
void test_creat();
void test_dup();


#endif /* TEST_UGID_H_ */
