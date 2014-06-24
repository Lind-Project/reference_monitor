/*
 * test_access.c
 *
 *  Created on: Jun 20, 2014
 *      Author:  Ali Gholami
 */



#include "testcases.h"

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
	test_connect();
	return 0;
}

void test_connect()
{
	int sockfd;
	struct sockaddr_in dest;

	/*---Open socket for streaming---*/
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf(stderr, "Socket error %d \n", sockfd);
	}

	/*---Initialize server address/port struct---*/
	bzero(&dest, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(80);
	dest.sin_addr.s_addr = inet_addr(SERVER_ADDR);


	// instal sigpipe handler
	signal(SIGPIPE, sigpipe_handler);
	int ret;
	/*---Connect to server---*/
	if ((ret = connect(sockfd, (struct sockaddr*) &dest, sizeof(dest))) != 0) {
		fprintf(stderr, "Connect error %d  \n", ret);
		return;
	}

	fprintf(stdout, "Accept Connection \n");
}
