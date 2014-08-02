/*
 * test_socketpair.c
 *
 *  Created on: Jun 24, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main(int argc, char **argv) {
	test_socketpair();
	return 0;
}

void test_socketpair() {
	int sockets[2];

	int ret = socketpair(AF_UNIX, SOCK_STREAM, 0, sockets);

	if ( ret < 0) {
		fprintf(stderr, "opening stream socket pair \n");
		return;
	}

	fprintf(stdout, "socketpair() opend successfully %d \n", ret);

	close(sockets[0]);
	close(sockets[1]);

}
