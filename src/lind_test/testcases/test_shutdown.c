/*
 * test_chown.c
 *
 *  Created on: Jun 19, 2014
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
	dest.sin_port = htons(5000);
	//dest.sin_addr.s_addr = inet_addr(SERVER_ADDR);
	dest.sin_addr.s_addr = INADDR_ANY;

	/* Initialize socket structure */
	bzero( &dest, sizeof(dest));

	int ret;
    if ((ret = bind(sockfd, (struct sockaddr *) &dest,
                          sizeof(dest)) < 0)){
         fprintf(stderr, "ERROR on binding  \n");
         return;
    }

    shutdown(sockfd, SHUT_RD);
	close(sockfd);
	 fprintf(stdout, "Shutdown successfully %d \n", ret);
}
