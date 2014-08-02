/*
 * test_poll.c
 *
 *  Created on: Jun 24, 2014
 *      Author:  Ali Gholami
 */
#include "testcases.h"

int main(int argc, char **argv)
{
	char *path1 = strcat(get_testfiles_dir(),"/dev0");
	char *path2 = strcat(get_testfiles_dir(),"/dev1");
	test_poll(path1, path2);
	return 0;
}

void test_poll(char *path1, char * path2)
{
	/* taken from http://stackoverflow.com/questions/12444679/poll-function-in-c-how-does-it-work*/
	struct pollfd fds[2];
	int timeout_msecs = -1;
	int ret;
	int i;
	fds[0].fd = open(path1, O_RDONLY);
	fds[1].fd = open(path2, O_RDONLY);

	if (fds[0].fd < 0) {
		fprintf(stderr, "open(%s) error \n", path1);
		return;
	}

	if (fds[1].fd < 0) {
		fprintf(stderr, "open(%s) error \n", path2);
		return;
	}

	fds[0].events = POLLOUT | POLLWRBAND;
	fds[1].events = POLLOUT | POLLWRBAND;

	int flag = 1;

	while (flag) {
		ret = poll(fds, 2, timeout_msecs);

		if (ret > 0) {
			/* An event on one of the fds has occurred. */
			for (i = 0; i < 2; i++) {
				if (fds[i].revents & POLLWRBAND) {
					/* Priority data may be written on device number i. */
					fprintf(stdout,
							"Priority Data may be written on device number %d POLLWRBAND\n",
							i);
				}

				if (fds[i].revents & POLLOUT) {
					/* Data may be written on device number i. */
					fprintf(stdout,
							"Data may be written on device number %d POLLOUT\n",
							i);
				}

				if (fds[i].revents & POLLHUP) {
					/* A hangup has occurred on device number i. */
					fprintf(stdout,
							"A hangup has occurred on device number %d\n", i);
				}
				flag = 0;
			}
		}
	}

	if (close(fds[0].fd) != 0){
		fprintf(stderr, "close() error \n");
		return;
	}

	if (close(fds[1].fd) != 0){
		fprintf(stderr, "close() error \n");
		return;
	}
}
