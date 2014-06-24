/*
 * test_select.c
 *
 *  Created on: Jun 23, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int max(int lhs, int rhs) {
	if (lhs > rhs)
		return lhs;
	else
		return rhs;
}

int main(int argc, char **argv) {
	char cwd[MAXBUF];
	char *src = strcat(getcwd(cwd, sizeof(cwd)),"/testfiles/test_read.txt");
	char *dest = strcat(getcwd(cwd, sizeof(cwd)), "/testfiles/test_write.txt");
	test_select(src, dest);
	return 0;
}

void test_select(char *src, char *dest) {

	int fd1;
	fd1 = open(src, O_APPEND);
	if (fd1 < 0) {
		fprintf(stderr, "open(%s) error \n", src);
		return;
	}

	int fd2;
	fd2 = open(dest, O_APPEND);
	if (fd2 < 0) {
		fprintf(stderr, "open(%s) error \n", dest);
		return;
	}

	char buffer[MAXBUF];
	int readp = 0;
	int writep = 0;
	int eof = FALSE;

	for (;;) {
		fd_set readfds, writefds;
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);

		int ravail, wavail;
		if (readp < writep) {
			ravail = writep - readp - 1;
			wavail = sizeof buffer - writep;
		} else {
			ravail = sizeof buffer - readp;
			wavail = readp - writep;
		}

		if (!eof && ravail)
			FD_SET(fd1, &readfds);

		if (wavail)
			FD_SET(fd2, &writefds);
		else if (eof)
			break;

		int rc = select(max(fd1, fd2) + 1, &readfds, &writefds, NULL, NULL);

		if (rc == -1)
			break;

		if (FD_ISSET(fd1, &readfds)) {
			ssize_t nread = read(fd1, &buffer[readp], ravail);
			if (nread < 1)
				eof = TRUE;
			readp = readp + nread;
		}

		if (FD_ISSET(fd2, &writefds)) {
			ssize_t nwritten = write(fd2, &buffer[writep], wavail);
			if (nwritten < 1)
				break;
			writep = writep + nwritten;
		}

		if (readp == sizeof buffer && writep != 0)
			readp = 0;

		if (writep == sizeof buffer)
			writep = 0;
	}

}

