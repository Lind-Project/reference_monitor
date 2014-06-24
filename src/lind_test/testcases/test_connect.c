/*
 * test_access.c
 *
 *  Created on: Jun 20, 2014
 *      Author:  Ali Gholami
 */
#include "testcases.h"

int main(int argc, char **argv)
{
	test_connect();
	return 0;
}

void test_connect()
{
	int sockfd;
	struct sockaddr_in dest;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf(stderr, "socket() error %d \n", sockfd);
	}

	bzero(&dest, sizeof(dest));

	dest.sin_family = AF_INET;
	dest.sin_port = htons(HTTP_TEST_PORT);
	dest.sin_addr.s_addr = inet_addr(SERVER_ADDR);

	int ret;

	if ((ret = connect(sockfd, (struct sockaddr*) &dest, sizeof(dest))) != 0) {
		fprintf(stderr, "connect() error %d  \n", ret);
		return;
	}

	fprintf(stdout, "successfully accepted connection \n");
}
