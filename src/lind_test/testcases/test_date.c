/*
 * test_date.c
 *
 *  Created on: Jun 6, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main(int argc, char **argv)
{
	test_date();
	return 0;
}

void test_date()
{
	time_t current_time;
    char* c_time_string;

    current_time = time(NULL);

    if (current_time == ((time_t)-1)){
        fprintf(stderr, "Failure to compute the current time.");

    }

    c_time_string = ctime(&current_time);

    if (c_time_string == NULL){
        fprintf(stderr, "Failure to convert the current time.");
    }

    fprintf(stdout, "Current time is %s", c_time_string);
}

