/*
 * test_chown.c
 *
 *  Created on: Jun 19, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main(int argc, char **argv)
{
	test_shutdown();
	return 0;
}

void test_shutdown()
{

	int sockfd;
	struct sockaddr_in dest;

	/*---Open socket for streaming---*/
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf(stderr, "Socket error %d \n", sockfd);
	}


    dest.sin_family = AF_INET;
	dest.sin_port = htons(SHDOWN_SERVER_PORT);
	dest.sin_addr.s_addr = INADDR_ANY;

	/* Initialize socket structure */
	bzero( &dest, sizeof(dest));

	int ret;
    if ((ret = bind(sockfd, (struct sockaddr *) &dest,
                          sizeof(dest)) < 0)){
         fprintf(stderr, "bind(%d) error  \n", SHDOWN_SERVER_PORT);
         return;
    }

    shutdown(sockfd, SHUT_RD);
	close(sockfd);

	fprintf(stdout, "Shutdown successfully %d \n", ret);
}
