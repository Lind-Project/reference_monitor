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
	struct sockaddr_in addr, ss;
	unsigned int len;

	sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sock_fd < 0){
		fprintf(stderr, "socket(%d) = error \n", sock_fd);
		return;

	}

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(7001);

	bind(sock_fd, (struct sockaddr*) &addr, sizeof(addr));

	len = sizeof(ss);

	if (getsockname(sock_fd, (struct sockaddr*) &ss, &len) < 0) {
		fprintf(stderr, "getsockname() = error \n");
		return;
	}

	fprintf(stdout, "Local IP address is: %s\n ",inet_ntoa(ss.sin_addr));
	fprintf(stdout, "Local port is: %d\n", (int) ntohs(ss.sin_port));

	if (close(sock_fd) < 0){
		fprintf(stderr, "close(%d) = error \n", sock_fd);
		return;
	}
}
