/*
 * test_sendto_recvfrom.c
 *
 *  Created on: Jun 25, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

#define BUF_SIZE 500

int main(int argc, char **argv)
{
	pthread_t server, client;

	const char *message1 = "Server thread";
	const char *message2 = "Client thread";
	int ret1, ret2;

	ret1 = pthread_create(&server, NULL, test_recvfrom, (void*) message1);

	if (ret1) {
		fprintf(stderr, "Error - pthread_create() return code: %d\n",
				ret1);
		return 1;
	}

	ret2 = pthread_create(&client, NULL, test_sendto, (void*) message2);

	if (ret2) {
		fprintf(stderr, "Error - pthread_create() return code: %d\n",
				ret2);
		return 1;
	}

	fprintf(stdout, "pthread_create() for server returns: %d\n", ret1);
	fprintf(stdout, "pthread_create() for client returns: %d\n", ret2);

	pthread_join(server, NULL);
	pthread_join(client, NULL);

	return 0;
}

void * test_recvfrom()
{
	int sockfd, nread;
	struct sockaddr_in add, client_add;
	socklen_t len;
	char mesg[MAXBUF];

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&add, sizeof(add));
	add.sin_family = AF_INET;
	add.sin_addr.s_addr = htonl(INADDR_ANY);
	add.sin_port = htons(SENDRECV_SERVER_PORT);
	bind(sockfd, (struct sockaddr *) &add, sizeof(add));

	len = sizeof(client_add);
	nread = recvfrom(sockfd, mesg, MAXBUF, 0, (struct sockaddr *) &client_add,
			&len);

	if (nread == -1) {
		fprintf(stderr, "recvfrom() error \n");
	}

	nread = sendto(sockfd, mesg, nread, 0, (struct sockaddr *) &client_add,
			sizeof(client_add));

	if (nread == -1) {
		fprintf(stderr, "sendto() error \n");
	}

	mesg[nread] = '\0';

	fprintf(stdout, "Lind server: received message %s \n", mesg);

	if (close(sockfd) != 0) {
		fprintf(stderr, "close() error \n");
		return NULL;
	}

	 return (void *) 1;
}

void *test_sendto()
{

	int sockfd;
	struct sockaddr_in add;
	char send_msg[] = "Lind sendto/recvfrom test \n";
	char recv_msg[MAXBUF];
	int nread;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&add, sizeof(add));
	add.sin_family = AF_INET;
	add.sin_addr.s_addr = inet_addr(LOCAL_SERVER);
	add.sin_port = htons(SENDRECV_SERVER_PORT);

	nread = sendto(sockfd, send_msg, strlen(send_msg), 0, (struct sockaddr *) &add,
			sizeof(add));

	if (nread == -1) {
		fprintf(stderr, "sendto() error \n");
		return NULL;
	}

	nread = recvfrom(sockfd, recv_msg, MAXBUF, 0, NULL, NULL);

	if (nread == -1) {
		fprintf(stderr, "recvfrom() error \n");
		return NULL;
	}

	recv_msg[nread] = '\0';

	fprintf(stdout, "Lind client: received message %s \n", recv_msg);

	if (close(sockfd) != 0) {
		fprintf(stderr, "close() error \n");
		return NULL;
	}

	 return (void *) 1;
}
