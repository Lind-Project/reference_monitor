/*
 * test_access.c
 *
 *  Created on: Jun 20, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main(int argc, char **argv)
{
	char cwd [4096];
	char *path = "/testfiles/test_access.txt";
	test_access(strcat(getcwd(cwd, sizeof(cwd)), path));
	return 0;
}

void test_access(char *path)
{
	 int rval;
	 /* Check file existence. */
	 rval = access (path, F_OK);
	 if (rval == 0)
	  fprintf (stdout, "%s exists\n", path);
	 else {
	  if (errno == ENOENT)
		  fprintf (stderr, "%s does not exist\n", path);
	  else if (errno == EACCES)
		  fprintf (stderr, "%s is not accessible\n", path);
	  return ;
	 }

	 /* Check read access. */
	 rval = access (path, R_OK);
	 if (rval == 0)
		 fprintf (stdout, "%s is readable\n", path);
	 else
		 fprintf (stderr, "%s is not readable (access denied)\n", path);

	 /* Check write access. */
	 rval = access (path, W_OK);
	 if (rval == 0)
		 fprintf (stdout, "%s is writable\n", path);
	 else if (errno == EACCES)
		 fprintf (stderr, "%s is not writable (access denied)\n", path);
	 else if (errno == EROFS)
		 fprintf (stderr, "%s is not writable (read-only filesystem)\n", path);

}
