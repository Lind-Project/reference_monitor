/*
 * lind_util.c
 *
 *  Created on: June 16, 2014
 *      Author: Ali Gholami, Benson Tsai
 */

#include "lind_util.h"


char *trim(char * str)
{
	char *s1 = str, *s2 = &str[strlen(str) - 1];

	while ((isspace(*s2)) && (s2 >= s1))
		s2--;
	*(s2 + 1) = '\0';

	while ((isspace(*s1)) && (s1 < s2))
		s1++;
	strcpy(str, s1);

	return str;
}

char *get_substring(char *string, int position, int length)
{
	char *pointer;
	int c;
	pointer = malloc(length + 1);
	if (pointer == NULL) {
		fprintf(stderr, "Unable to allocate memory.\n");
		exit(EXIT_FAILURE);
	}

	for (c = 0; c < position - 1; c++)
		string++;

	for (c = 0; c < length; c++) {
		*(pointer + c) = *string;
		string++;
	}
	*(pointer + c) = '\0';
	return pointer;
}

char *get_lind_config()
{



	const char *name = "LIND_MONITOR";
	const char *appendStr = "/src/main/lind_monitor/configs/monitor.cfg";

	const char *path = getenv(name);

	if (path == NULL) {
		fprintf(stderr, "Could not load the Lind config file %s. \n", path);
		exit(-1);
	}

	char *monitor_home = malloc(strlen(path) + strlen(appendStr));

	strcpy(monitor_home, path);
	strcpy(monitor_home, appendStr);

	fprintf(stdout, "final path %s", monitor_home);
	return monitor_home;
}
