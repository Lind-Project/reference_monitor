/*
 * test_socket.c
 *
 *  Created on: Jun 18, 2014
 *      Author:  Ali Gholami
 *
 */

#include "testcases.h"


int main(int argc, char **argv)
{
	test_socket();
	return 0;
}

void test_socket()
{
	/* taken from: http://www.cs.utah.edu/~swalton/listings/sockets/programs/ */
	int sockfd;
	struct sockaddr_in dest;
	char buffer[MAXBUF];

	int counter = 0;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf(stderr, "socket() error %d \n", sockfd);
	}

	bzero(&dest, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(HTTP_TEST_PORT);
	dest.sin_addr.s_addr = inet_addr(SERVER_ADDR);

	int socket_flag = 0;

	int ret;

	if ((ret = connect(sockfd, (struct sockaddr*) &dest, sizeof(dest))) != 0) {
		fprintf(stderr, "connect() error %d  \n", ret);
		return;
	}

	socket_flag = 1;

	while (1) {
		if(!socket_flag)
			break;
		send(sockfd, "test\n", 5, 0);
		fprintf(stdout, "message sent\n");
		bzero(buffer, MAXBUF);
		recv(sockfd, buffer, sizeof(buffer), 0);
		fprintf(stdout, "[%6d]:%s", ++counter, buffer);
		sleep(1);
	}

   if (close(sockfd)!= 0){
	   fprintf(stderr, "close() error \n");
	   return;
	}
}
