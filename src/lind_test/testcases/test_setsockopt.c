/*
 * test_setsockopt.c
 *
 *  Created on: Jun 24, 2014
 *      Author:  Ali Gholami
 */


#include "testcases.h"

int main(int argc, char *argv[]) {

	pthread_t server, client;

	const char *message1 = "Server thread";
	const char *message2 = "Client thread";
	int ret1, ret2;

	ret1 = pthread_create(&server, NULL, test_setsockopt, (void*) message1);

	if (ret1) {
		fprintf(stderr, "Error - pthread_create() return code: %d\n", ret1);
		return 1;
	}

	ret2 = pthread_create(&client, NULL, test_client, (void*) message2);

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

void *test_setsockopt() {

	/* taken from http://www.tenouk.com/Module41b.html */
	int len, rc, sockfd, sd2;
	char buffer[MAXBUF];

	struct sockaddr_in addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	int keepcnt = 5;
	int keepidle = 30;
	int keepintvl = 120;

	if (sockfd < 0) {
		fprintf(stderr, "socket() error \n");
		return;
	}

	if(setsockopt(sockfd, SOL_TCP, TCP_DEFER_ACCEPT, &keepcnt, sizeof(int)) < 0 ){
		fprintf(stderr, "setsockopt failed \n");
		return;
	}

	if(setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPIDLE, &keepidle, sizeof(int)) <0){
		fprintf(stderr, "setsockopt failed \n");
		return;
	}

	if(setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPINTVL, &keepintvl, sizeof(int)) <0){
		fprintf(stderr, "setsockopt failed \n");
		return;
	}


	/* Bind the socket */
	memset(&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(LISTEN_SERVER_PORT);

	rc = bind(sockfd, (struct sockaddr *) &addr, sizeof(addr));

	if (rc < 0) {
		fprintf(stderr, "bind() error \n");
		close(sockfd);

	}

	rc = listen(sockfd, 5);

	if (rc < 0) {
		fprintf(stderr, "listen() error \n");
		close(sockfd);

	}

	sd2 = accept(sockfd, NULL, NULL);

	if (sd2 < 0) {
		fprintf(stderr, "accept() error \n");
		close(sockfd);

	}

	rc = recv(sd2, buffer, sizeof(buffer), 0);

	if (rc <= 0) {
		fprintf(stderr, "recv() error \n");
		close(sockfd);
		close(sd2);


	}

	len = rc;

	rc = send(sd2, buffer, len, 0);

	if (rc <= 0) {
		fprintf(stderr, "send() error \n");
		close(sockfd);
		close(sd2);

	}

	close(sd2);
	close(sockfd);

}

void * test_client()
{
	int len, rc;
	int sockfd;

	char send_buf[MAXBUF] = "this is  a test from Lind client \n";
	char recv_buf[MAXBUF];
	struct sockaddr_in addr;

	/* Create an AF_INET stream socket */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	int keepcnt = 5;
	int keepidle = 30;
	int keepintvl = 120;

	if (sockfd < 0) {
		fprintf(stderr, "client - socket() error \n");
		return;
	}

	if(setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPCNT, &keepcnt, sizeof(int)) < 0 ){
		fprintf(stderr, "setsockopt failed \n");
		return;
	}

	if(setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPIDLE, &keepidle, sizeof(int)) <0){
		fprintf(stderr, "setsockopt failed \n");
		return;
	}

	if(setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPINTVL, &keepintvl, sizeof(int)) <0){
		fprintf(stderr, "setsockopt failed \n");
		return;
	}

	memset(&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(LISTEN_SERVER_PORT);


	rc = connect(sockfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in));
	if (rc < 0) {
		fprintf(stderr, "client - connect() error \n");
	}

	len = send(sockfd, send_buf, strlen(send_buf) + 1, 0);

	if (len != strlen(send_buf) + 1) {
		fprintf(stderr, "client - send() error \n");
	}

	fprintf(stdout, "%d bytes sent \n", len);
	len = recv(sockfd, recv_buf, sizeof(recv_buf), 0);

	if (len != strlen(send_buf) + 1) {
		fprintf(stderr, "client - recv() error \n");
	}

	fprintf(stdout, "server %s \n", recv_buf);
	fprintf(stdout, "%d bytes received \n", len);

	 if (close(sockfd)!= 0){
		       fprintf(stderr, "close() error \n");
		}
}
