/*
 * test_socket.c
 *
 *  Created on: Jun 18, 2014
 *      Author:  Ali Gholami
 *
 */

#include "testcases.h"

/*  Taken from: http://www.cs.utah.edu/~swalton/listings/sockets/programs/ */

#define PORT_TEST       80            /* connection port */
#define SERVER_ADDR     "173.194.121.34"     /* Google server */
#define MAXBUF          4096

int socket_OK = 0;

void sigpipe_handler()
{
	fprintf(stdout, "SIGPIPE caught\n");
	socket_OK = 0;
}

int main(int argc, char **argv)
{
	test_socket();
	return 0;
}

void test_socket()
{
	int sockfd;
	struct sockaddr_in dest;
	char buffer[MAXBUF];

	int counter = 0;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf(stderr, "Socket error %d \n", sockfd);
	}

	bzero(&dest, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(80);
	dest.sin_addr.s_addr = inet_addr(SERVER_ADDR);


	signal(SIGPIPE, sigpipe_handler);

	int ret;

	if ((ret = connect(sockfd, (struct sockaddr*) &dest, sizeof(dest))) != 0) {
		fprintf(stderr, "Connect error %d  \n", ret);
		return;
	}

	socket_OK = 1;

	while (1) {
		if (!socket_OK)
			break;

		send(sockfd, "test\n", 5, 0);
		fprintf(stdout, "message sent\n");
		bzero(buffer, MAXBUF);
		recv(sockfd, buffer, sizeof(buffer), 0);
		fprintf(stdout, "[%6d]:%s", ++counter, buffer);
		sleep(1);
	}

	close(sockfd);

	fprintf(stdout, "Connect closed \n");
}
