/*
 * test_getpeername.c
 *
 *  Created on: Jun 25, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main(int argc, char **argv)
{
	test_getpeername();
	return 0;
}

void test_getpeername()
{
	int sockfd, ret;
	socklen_t len;

	struct sockaddr_in dest;

	struct sockaddr_storage peer_addr;

	char ipstr[INET6_ADDRSTRLEN];
	int port;

	len = sizeof(peer_addr);

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf(stderr, "socket() error %d \n", sockfd);
	}

	bzero(&dest, sizeof(dest));

	dest.sin_family = AF_INET;
	dest.sin_port = htons(HTTP_TEST_PORT);
	dest.sin_addr.s_addr = inet_addr(SERVER_ADDR);


	if ((ret = connect(sockfd, (struct sockaddr*) &dest, sizeof(dest))) != 0) {
		fprintf(stderr, "connect() error %d  \n", ret);
		return;
	}

	ret = getpeername(sockfd, (struct sockaddr*) &peer_addr, &len);

	if (ret < 0 ){
		fprintf(stderr, " getpeername() error \n");
		return;
	}


	if (peer_addr.ss_family == AF_INET) {
	    struct sockaddr_in *sock = (struct sockaddr_in *) &peer_addr;
	    port = ntohs(sock->sin_port);
	    inet_ntop(AF_INET, &sock->sin_addr, ipstr, sizeof ipstr);
	} else {
	    struct sockaddr_in6 *sock1 = (struct sockaddr_in6 *) &peer_addr;
	    port = ntohs(sock1->sin6_port);
	    inet_ntop(AF_INET6, &sock1	->sin6_addr, ipstr, sizeof ipstr);
	}

	fprintf(stdout, "connected peer address = %s \n", ipstr);
	fprintf(stdout, "connect peer port =   %d \n", port);

	if (close(sockfd)!= 0){
		 fprintf(stderr, "close() error \n");
  	     return;
 	}

}
