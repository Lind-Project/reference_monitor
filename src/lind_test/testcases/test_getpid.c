
/*
 * test_getpid.c
 *
 *  Created on: Jun 10, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main(int argc, char **argv)
{
	test_getpid();
	return 0;
}

void test_getpid()
{
  char process[20];
  pid_t pid;

  readlink ("/proc/self", process, sizeof(process));
  sscanf (process, "%d", &pid);

  assert(getpid() == pid);

  fprintf(stdout, "Current process id number is: %d \n", pid);
}
