/*
 * test_access.c
 *
 *  Created on: Jun 20, 2014
 *      Author:  Ali Gholami
 */
#include "testcases.h"

int main(int argc, char **argv)
{
	test_bind();
	return 0;
}

void test_bind( )
{
	int sockfd;
	struct sockaddr_in dest;

	/* open a streaming socket */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf(stderr, "Socket error %d \n", sockfd);
		return;
	}

	/* initalize the socket structure */
    dest.sin_family = AF_INET;
	dest.sin_port = htons(BIND_SERVER_PORT);
	dest.sin_addr.s_addr = INADDR_ANY;

	bzero( &dest, sizeof(dest));

	int ret;
    if ((ret = bind(sockfd, (struct sockaddr *) &dest,
                          sizeof(dest)) < 0)){
         fprintf(stderr, "binding error  \n");
         return;
    }


	if (close(sockfd)!= 0){
		       fprintf(stderr, "close() error \n");
	   	   return;
		}

    fprintf(stdout, "bind() successfully %d \n", ret);

}
