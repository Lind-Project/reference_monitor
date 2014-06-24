/*
 * test_access.c
 *
 *  Created on: Jun 20, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main(int argc, char *argv[]) {

	pthread_t server, client;

	const char *message1 = "Server thread";
	const char *message2 = "Client thread";
	int ret1, ret2;

	ret1 = pthread_create(&server, NULL, test_accept, (void*) message1);

	if (ret1) {
		fprintf(stderr, "Error - pthread_create() return code: %d\n", ret1);
		return 1;
	}

	ret2 = pthread_create(&client, NULL, test_accept_client, (void*) message2);

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

void * test_accept()
{
	/* taken from http://www.tenouk.com/Module41b.html */
	int sd1, sd2, rc;
	struct sockaddr_in addr;

	sd1 = socket(AF_INET, SOCK_STREAM, 0);

	if (sd1 < 0) {
		fprintf(stderr, "socket() error \n");

	}

	memset(&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(ACCEPT_SERVER_PORT);

	rc = bind(sd1, (struct sockaddr *) &addr, sizeof(addr));

	if (rc < 0) {
		fprintf(stderr, "bind() error \n");
		close(sd1);

	}

	rc = listen(sd1, 5);

	if (rc < 0) {
		fprintf(stderr, "listen() error \n");
		close(sd1);

	}

	sd2 = accept(sd1, NULL, NULL);

	if (sd2 < 0) {
		fprintf(stderr, "accept() error \n");
		close(sd1);

	}

	close(sd2);
	close(sd1);
}

void *test_accept_client()
{
	int rc;
	int sockfd;
	struct sockaddr_in addr;

	/* Create an AF_INET stream socket */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0) {
		fprintf(stderr, "client - socket() error \n");

	}

	memset(&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(ACCEPT_SERVER_PORT);


	rc = connect(sockfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in));

	if (rc < 0) {
		fprintf(stderr, "client - connect() error \n");
		close(sockfd);

	}

	fprintf(stdout, "connection accepted \n");
	close(sockfd);
}
