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

/* https://support.sas.com/documentation/onlinedoc/sasc/doc700/html/lr2/zockname.htm */

void test_getsockname()
{
	int s;
	struct sockaddr_in sa;
	int sa_len;
	sa_len = sizeof(sa);

	s = socket(AF_INET, SOCK_DGRAM, 0);

	if (getsockname(s, &sa, &sa_len) == -1) {
		fprintf(stderr, "getsockname() failed \n");
		return;
	}

  	fprintf(stdout, "Local IP address is: %s\n", inet_ntoa(sa.sin_addr));
  	fprintf(stdout, "Local port is: %d\n", (int) ntohs(sa.sin_port));
}
