/*
 * test_getsockname.c
 *
 *  Created on: Jun 20, 2014
 *      Author:  Ali Gholami
 *
 */

#include "testcases.h"

int main(int argc, char **argv)
{
	test_getsockname();
	return 0;
}

void test_getsockname()
{
	int sock_fd;
	struct sockaddr_in addr;
	unsigned int len;
	int bind_ret, gsn_ret;

	sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sock_fd < 0){
		fprintf(stderr, "failed socket() = %d\n", sock_fd);
		return;

	}

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(7001);

	if ((bind_ret = bind(sock_fd, (struct sockaddr*) &addr, sizeof(addr))) <0) {
		fprintf(stderr, "error in bind(%d) = %d\n", sock_fd, bind_ret);
		return;
	}

	len = sizeof(addr);

	if ((gsn_ret = getsockname(sock_fd, (struct sockaddr*) &addr, &len)) < 0) {
		fprintf(stderr, "error in getsockname(%d, %s %d) = %d\n", sock_fd, inet_ntoa(addr.sin_addr), len, gsn_ret);
		return;
	}

	fprintf(stdout, "Local IP address is: %s\n ",inet_ntoa(addr.sin_addr));
	fprintf(stdout, "Local port is: %d\n", (int) ntohs(addr.sin_port));

	if (close(sock_fd) < 0){
		fprintf(stderr, "close(%d) = error \n", sock_fd);
		return;
	}
}
