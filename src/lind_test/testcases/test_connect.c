/*
 * test_connect.c
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
		fprintf(stderr, "socket() = %d error \n", sockfd);
		return;
	}

	bzero(&dest, sizeof(dest));

	dest.sin_family = AF_INET;
	dest.sin_port = htons(HTTP_TEST_PORT);
	dest.sin_addr.s_addr = inet_addr(SERVER_ADDR);

	int ret;

	if ((ret = connect(sockfd, (struct sockaddr*) &dest, sizeof(dest))) != 0) {
		fprintf(stderr, "connect() = %d error \n", ret);
		return;
	}

	if ((ret=close(sockfd))!= 0){
		 fprintf(stderr, "close() = %d error \n", ret);
  	     return;
 	}

	fprintf(stdout, "successfully connected \n");
}
