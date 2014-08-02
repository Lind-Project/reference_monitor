/*
 * test_setgroups.c
 *
 *  Created on: Jun 24, 2014
 *      Author:  Ali Gholami
 */


#include "testcases.h"

int main(int argc, char **argv)
{
	test_setgroups();
	return 0;
}

void test_setgroups()
{

	long ngroups_max = sysconf(_SC_NGROUPS_MAX) + 1;

	gid_t *aux_groups = (gid_t *) malloc(ngroups_max * sizeof(gid_t));

	int ngroups = getgroups(ngroups_max, aux_groups);

	if (ngroups > 0){
	    int dst = 0;
	    int i;

	    for (i = 0; i < ngroups; i++){
	        if (aux_groups[i] != 0){
	            aux_groups[dst++] = aux_groups[i];
	        	}
	    	}
	        ngroups = dst;
	}

	if (ngroups > 0){
	    if (setgroups(ngroups, aux_groups) != 0)
	    	fprintf(stderr, "setgroups failed. run this process in privileged mode\n");
	    	return;
	}

	fprintf(stdout, "number of groups excluding group %d  is = %d \n", TEST_GETGID, ngroups);
}

