

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "testcases.h"

/**
 * Taken from MMAP(2)     Linux Programmer's Manual        MMAP(2)
 */

#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(int argc, char *argv[])
{
	char *path = strcat(get_testfiles_dir(), "/test_open.txt");
	test_mmap(path);
	return 0;
}

void test_mmap(char *path)
{
	char *addr;
	int fd;
	struct stat sb;
	off_t offset, pa_offset;
	size_t length;
	ssize_t s;
	fd = open(path, O_RDONLY);

	if (fd == -1)
		handle_error("open");

	if (fstat(fd, &sb) == -1) /* To obtain file size */
		handle_error("fstat");

	offset=100;
	pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);
	/* offset for mmap() must be page aligned */

	if (offset >= sb.st_size) {
		fprintf(stderr, "offset is past end of file\n");
		return;
	}

	length = sb.st_size - offset;

	addr = mmap(NULL, length + offset - pa_offset, PROT_READ,
	MAP_PRIVATE, fd, pa_offset);
	if (addr == MAP_FAILED) {
		handle_error("mmap");
		return;
	}
	s = write(STDOUT_FILENO, addr + offset - pa_offset, length);

	if (s != length) {
		if (s == -1)
			handle_error("write");

		fprintf(stderr, "partial write");
		return;
	}

	return;
}
