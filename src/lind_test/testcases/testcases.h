/*
 * test_ugid.h
 *
 *  Created on: Jun 17, 2014
 *      Author:  Ali Gholami
 */

#ifndef TEST_UGID_H_
#define TEST_UGID_H_


/* connection port */
#define LOCAL_SERVER 			"127.0.0.1"

#define FTP_TEST_PORT        	21
#define HTTP_TEST_PORT       	80
#define ACCEPT_SERVER_PORT 		13001
#define BIND_SERVER_PORT 		13002
#define SHDOWN_SERVER_PORT 		13003
#define LISTEN_SERVER_PORT 		13004
#define SENDRECV_SERVER_PORT 	13005
#define UDP_TEST_PORT 			13006
#define WRONG_PORT 				10000000003
#define WRONG_ADDRESS 			10000000003
#define SERVER_ADDR     		"128.122.119.202"     /* NYU server */
#define SERVER_URL     			"http://www.google.com"     /* Google server */

#define MAXBUF          4096

#define TRUE   1
#define FALSE  0

#define ROOT_GROUP 	0

#define TEST_GETUID 1000
#define TEST_GETGID 1000
#define TEST_GETPID 1000

//#include "../../../src/main/lind_monitor/lind_util.h"
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
#include <dirent.h>
#include <sys/syscall.h>
#include <grp.h>
#include <netinet/tcp.h>
#include <ifaddrs.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <scsi/sg.h>
#include <resolv.h>
#include <poll.h>
#include <spawn.h>

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
void test_curl(char *url);
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
void test_dup2();
void test_dup3();
void test_pread64(char *path);
void test_pwritev(char *path);
void test_getdents(const char *path);
void test_socketpair();
void test_setsockopt();
void test_setgroups();
void test_exit_group();
void test_fcntl(char *path);
void test_flock(char *path);
void test_ioctl(char *path);
void test_poll(char *path1, char *path2);
void test_getifaddrs();
void test_getpeername();
void *test_sendto();
void *test_recvfrom();
void *test_sendmsg();
void *test_recvmsg();
void test_openat(char *path);
void test_mmap(char* path);


char *get_testfiles_dir()
{

	const char *name = "LIND_MONITOR";
	const char *appendStr = "/src/lind_test/utf/testfiles";

	char *path = getenv(name);

	if (path == NULL) {
		fprintf(stderr, "Could not load the Lind config file %s. \n", path);
		exit(-1);
	}

	char *monitor_home = (char *) malloc ((strlen (path) + strlen(appendStr) +1) * sizeof(char));

	strcpy(monitor_home, path);
	strcat(monitor_home, appendStr);

	return monitor_home;
}


#endif /* TEST_UGID_H_ */
