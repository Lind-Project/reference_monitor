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
	/* taken from  https://support.sas.com/documentation/onlinedoc/sasc/doc700/html/lr2/zockname.htm */
	int sockfd, sa_len;
	struct sockaddr_in sa;

	sa_len = sizeof(sa);

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	if (getsockname(sockfd, &sa, &sa_len) == -1) {
		fprintf(stderr, "getsockname() failed \n");
		return;
	}

   if (close(sockfd)!= 0){
       fprintf(stderr, "close() error \n");
   	   return;
	}

	fprintf(stdout, "Local IP address is: %s\n", inet_ntoa(sa.sin_addr));
  	fprintf(stdout, "Local port is: %d\n", (int) ntohs(sa.sin_port));

}
