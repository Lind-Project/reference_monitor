/*
 * test_sleep.c
 *
 *  Created on: Jun 6, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main(int argc, char **argv)
{
	/* sleep x seconds*/
	test_sleep(5);
	return 0;
}

void test_sleep(int sleep_time)
{

	time_t start, end;
	time (&start);

	int ret = sleep(sleep_time);

	if (ret == -1) {
		fprintf(stderr, "Could not sleep \n");
	}
	time(&end);

	double delay = difftime(end, start);

	if (delay!= sleep_time) {
		fprintf(stderr, "Did not sleep correctly %f \n", delay);
	}

}
