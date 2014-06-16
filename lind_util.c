/*
 * lind_util.c
 *
 *  Created on: June 16, 2014
 *      Author:  Ali Gholami, Benson Tsai
 */

#include <stdlib.h>
#include <limits.h>
#include "lind_util.h"

char *trim(char * str) {
	char *s1 = str, *s2 = &str[strlen(str) - 1];

	while ((isspace(*s2)) && (s2 >= s1))
		s2--;
	*(s2 + 1) = '\0';

	while ((isspace(*s1)) && (s1 < s2))
		s1++;
	strcpy(str, s1);

	return str;
}

char *get_substring(char *string, int position, int length) {
	char *pointer;
	int c;
	pointer = malloc(length + 1);
	if (pointer == NULL) {
		printf("Unable to allocate memory.\n");
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

char *get_lindcwd(){
	char* cfgDir[PATH_MAX];
	char* appendStr = "/configs/monitor.cfg";
	getcwd(cfgDir, PATH_MAX);
	strcat(cfgDir, appendStr);
	return cfgDir;
}
