/*
 * test_socket.c
 *
 *  Created on: Jun 20, 2014
 *      Author:  Ali Gholami
 *
 */

#include "testcases.h"

int main(int argc, char **argv)
{
	test_getsockopt();
	return 0;
}

/* https://support.sas.com/documentation/onlinedoc/sasc/doc750/html/lr2/zsockopt.htm */
void test_getsockopt()
{
	int optlen, gs, socktype, s;

	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s == -1) {
		fprintf(stderr, "socket not created \n");
		return;
	}

	/* Ask for the socket type. */
	optlen = sizeof(socktype);
	gs = getsockopt(s, SOL_SOCKET, SO_TYPE, &socktype, &optlen);
	if (gs == -1) {
		fprintf(stderr, "could not get sockopt \n");
		return;
	}
	switch (socktype) {
	case SOCK_STREAM:
		fprintf(stdout, "Stream socket.\n");
		break;
	case SOCK_DGRAM:
		fprintf(stdout, "Datagram socket.\n");
		break;
	case SOCK_RAW:
		fprintf(stdout, "Raw socket.\n");
		break;
	default:
		fprintf(stdout, "Unknown socket type.\n");
		break;
	}
}
