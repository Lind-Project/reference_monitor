/*
 * test_sendmsg_recvmsg.c
 *
 *  Created on: Jun 25, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main(int argc, char **argv) {
	pthread_t server, client;

	const char *message1 = "Server thread";
	const char *message2 = "Client thread";
	int ret1, ret2;

	ret1 = pthread_create(&server, NULL, test_recvmsg, (void*) message1);

	if (ret1) {
		fprintf(stderr, "Error - pthread_create() return code: %d\n", ret1);
		return 1;
	}

	ret2 = pthread_create(&client, NULL, test_sendmsg, (void*) message2);

	if (ret2) {
		fprintf(stderr, "Error - pthread_create() return code: %d\n", ret2);
		return 1;
	}

	fprintf(stdout, "pthread_create() for server returns: %d\n", ret1);
	fprintf(stdout, "pthread_create() for client returns: %d\n", ret2);

	pthread_join(server, NULL);
	pthread_join(client, NULL);

	return 0;
}

void * test_recvmsg() {
	int ret;
	int serversd;

	struct msghdr msg;
	struct sockaddr_in addr;

	serversd = socket(AF_INET, SOCK_STREAM, 0);

	if (serversd < 0) {
		fprintf(stderr, "socket() failed \n");
		return;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(UDP_TEST_PORT);

	ret = bind(serversd, (struct sockaddr *) &addr, sizeof(addr));

	if (ret < 0) {
		fprintf(stderr, "bind() failed \n");
		close(serversd);
		return;
	}

	ret = listen(serversd, 5);

	if (ret < 0) {
		fprintf(stderr, "listen() failed \n");
		close(serversd);
	}

	ret = accept(serversd, NULL, NULL);

	if (ret < 0) {
			fprintf(stderr, "accept() failed \n");
			close(serversd);
			return;
		}

	memset(&msg, 0, sizeof(msg));
	memset(&msg, 0, sizeof(msg));

	ret = recvmsg(serversd, &msg, 0);
	if (ret < 0) {
			fprintf(stderr, "recvmsg() failed \n");
			close(serversd);
		}

	if (close(serversd) != 0){
			fprintf(stderr, "close() error \n");
		}
}

void *test_sendmsg() {

	int ret, len;
	int clientsd;

	char buffer[MAXBUF];

	struct iovec iov[1];
	struct msghdr msg;

	memset(&msg, 0, sizeof(msg));
	memset(iov, 0, sizeof(iov));
	iov[0].iov_base = buffer;
	iov[0].iov_len = sizeof(buffer);
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;

	ret = sendmsg(clientsd, &msg, 0);

	if (ret < 0) {
		fprintf(stderr, "sendmsg() failed \n");
		close(clientsd);
	}

	if (close(clientsd) != 0){
			fprintf(stderr, "close() error \n");
	}

}
