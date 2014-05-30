/*
 * monitor.c
 *
 *  Created on: April 17, 2014
 *      Author:  Ali Gholami
 */

#include "lind_monitor.h"
#include "platform/lind_platform.h"

int main(int argc, char** argv) {

	/* check the command line arguments to see if a process is defined for trace */
	if (argc <= 0) {
		fprintf(stderr, "Usage %s <program> <options>\n", argv[0]);
		exit(-1);
	}

	init_ptrace(argc, argv);
	LindPythonInit();
	intercept_calls();
	return 0;
}

/* initialize a process to be traced */
void init_ptrace(int argc, char** argv) {

	char *args[argc + 1];
	memcpy(args, argv, argc * sizeof(char *));

	load_config();
	tracee = fork();

	/* check if fork was successful */
	if (tracee < 0) {
		perror("No process could be initialized.");
		exit(-1);
	}

	/* trace the child */
	if (tracee == 0) {
		/* to let the parent process to trace the child*/
		ptrace(PTRACE_TRACEME, 0, 0, 0);

		/* stop the current process*/
		kill(getpid(), SIGSTOP);

		/* the binary or command to be executed */
		int ret_stat = execlp(argv[1], argv[2], NULL);
		if(ret_stat<0) {
			perror("execlp: ");
		}
	}
}

/* intercept the system calls issued by the tracee process */
void intercept_calls() {

	int entering = 1;
	int status, syscall_num, ret_val;
	char * path;
	char * path1;
	char *var;
	struct syscall_args regs, regs_orig;

	/* wait for the child to stop */
	wait(&status);

	ptrace(PTRACE_SETOPTIONS, tracee, 0, PTRACE_O_TRACESYSGOOD|PTRACE_O_TRACEEXIT);


	while (1) {

		/* to get every sysacall and be notified in the tracee stops */
		ptrace(PTRACE_SYSCALL, tracee, 0, 0);

		/* wait for other syscalls */
		wait(&status);

		/* if tracee is terminated */
		if (WIFEXITED(status))
			break;

		if (!WIFSTOPPED(status)) {
			perror("wait(&status)");
			exit(0);
		}

		if((WSTOPSIG(status) == SIGTRAP) && (status&(PTRACE_EVENT_EXIT<<8))) {
			get_args(&regs);
			if (regs.syscall != __NR_execve)
			printf("%s(%d)\n", syscall_names[regs.syscall], regs.arg1);

		}

		if (WSTOPSIG(status) == (SIGTRAP | 0x80 )) {

			if (entering == 1) {
				entering = 0;
				get_args(&regs);
				syscall_num = regs.syscall;

				regs_orig = regs;

				switch(syscall_num){
				case __NR_close:
					regs.arg1 = get_mapping(regs.arg1);
					set_args(&regs);
					break;
				default:
					break;
				}

			} else {

				/* get the tracee registers */
				get_args(&regs);
				entering = 1;

				if (monitor_actions[syscall_num] == DENY_LIND) {
					fprintf(stderr, "Deny call by Lind: %s %ld\n",
							syscall_names[syscall_num], regs.retval);
					regs.retval = EINVAL;
					set_args(&regs);

				} else if (monitor_actions[syscall_num] == ALLOW_OS) {

					/* */
					switch (syscall_num) {

					/* if pwritev is allowed by OS */
					case __NR_pwritev:
						regs.arg1 = get_mapping(regs.arg1);
						set_args(&regs);
						break;

					case __NR_mmap:
						fprintf(stderr, "mmap()= 0x%jx \n", regs.retval);
						break;

					case __NR_brk:
						fprintf(stderr, "brk()= 0x%jx \n", regs.retval);
						break;


					default:
						fprintf(stderr, "%s()= %ld \n",
								syscall_names[syscall_num], regs.retval);
						break;
					} /* switch*/

				} else if (monitor_actions[syscall_num] == ALLOW_LIND) {

					int lind_fd;

					struct lind_statfs stfs;

					switch (syscall_num) {

					case __NR_getuid:
						regs.retval = lind_getuid();
						fprintf(stderr, "getuid()=%ld \n", regs.retval);
						break;

					case __NR_read:
						var = malloc(regs.arg3);
						regs.retval = lind_read(regs.arg1, var, regs.arg3);
						set_mem(regs.arg2, var, regs.arg3);
						fprintf(stderr, "read(%ld, 0x%lx[\"%s\"], %ld) = %ld",
								regs.arg1, regs.arg2, var, regs.arg3,
								regs.retval);
						break;

					case __NR_open:
						if (regs_orig.retval>=0) {
						path = get_path(regs.arg1);
						lind_fd = lind_open(path, regs.arg2, regs.arg3);
						add_mapping(regs.retval, lind_fd);
						regs.retval = lind_fd;
						}
						fprintf(stderr, "open(%s)=%ld\n", path, regs.retval);
						break;

					case __NR_access:
						path = get_path(regs.arg1);
						fprintf(stderr, "access(%s)=%ld\n", path, regs.retval);
						regs.retval = lind_access(path, regs.arg2);
						break;

					case __NR_close:
						//lind_fd = get_mapping(regs.arg1);
						regs.retval = lind_close(regs.arg1);
						fprintf(stderr, "close(%d)=%d \n", regs.arg1,
								regs.retval);
						break;

					case __NR_rmdir:
						path = get_path(regs.arg1);
						regs.retval = lind_rmdir(path);
						fprintf(stderr, "rmdir(%s)=%d \n", path, regs.retval);

						break;

					case __NR_statfs:
						path = get_path(regs.arg1);
						regs.retval = lind_statfs(path, &stfs);
						set_mem(regs.arg2, &stfs, sizeof(stfs));

						fprintf(stderr, "statfs(%s)=%d \n", path, regs.retval);

						break;

					case __NR_stat:
						path = get_path(regs.arg1);
						struct lind_stat st;
						regs.retval = lind_stat(path, &st);
						set_mem(regs.arg2, &st, sizeof(st));

						fprintf(stderr, "stat(%s)=%d \n", path, regs.retval);

						break;
					case __NR_fstat:
						regs.retval = lind_fstat(regs.arg1, &st);
						set_mem(regs.arg2, &st, sizeof(st));
						fprintf(stderr, "fstat(%d)=%d \n", regs.arg1,
								regs.retval);
						break;

					case __NR_fstatfs:
						regs.retval = lind_fstatfs(regs.arg1, &stfs);
						set_mem(regs.arg2, &stfs, sizeof(stfs));
						fprintf(stderr, "fstatfs(%d)=%d \n", regs.arg1,
								regs.retval);
						break;

					case __NR_write:
						regs.retval = lind_write(regs.arg1,
								get_mem(regs.arg2, regs.arg3), regs.arg3);
						fprintf(stderr, "write(%ld, 0x%lx[\"%s\"], %ld) = %ld \n",
								regs.arg1, regs.arg2,
								get_mem(regs.arg2, regs.arg3), regs.arg3,
								regs.retval);
						break;

					case __NR_mkdir:
						path = get_path(regs.arg1);
						regs.retval = lind_mkdir(path, regs.arg2);
						fprintf(stderr, "mkdir(%s)=%ld \n", path, regs.retval);
						break;

					case __NR_chdir:
						path = get_path(regs.arg1);
						regs.retval = lind_chdir(path);
						fprintf(stderr, "chdir(%s)=%ld \n", path, regs.retval);
						break;

					case __NR_getcwd:
						path = get_mem(regs.arg1, regs.arg2);
						regs.retval = lind_getcwd(path, regs.arg2);
						fprintf(stderr, "getcwd(%s)=%ld \n", path, regs.retval);
						break;

					case __NR_dup:
						regs.retval = lind_dup(regs.arg1);
						fprintf(stderr, "dup(%ld)=%ld \n", regs.arg1,
								regs.retval);
						break;

					case __NR_dup2:
						regs.retval = lind_dup2(regs.arg1, regs.arg2);
						fprintf(stderr, "dup2(%ld)=%ld \n", regs.arg1,
								regs.retval);
						break;

					case __NR_getpid:
						regs.retval = lind_getpid();
						fprintf(stderr, "getpid()=%ld \n", regs.retval);
						break;

					case __NR_geteuid:
						regs.retval = lind_geteuid();
						fprintf(stderr, "geteuid()=%ld \n", regs.retval);
						break;

					case __NR_getgid:
						regs.retval = lind_getgid();
						fprintf(stderr, "getgid()=%ld \n", regs.retval);
						break;

					case __NR_getegid:
						regs.retval = lind_getegid();
						fprintf(stderr, "getegid()=%ld \n", regs.retval);
						break;

					case __NR_unlink:
						path = get_path(regs.arg1);
						regs.retval = lind_unlink(path);
						fprintf(stderr, "unlink(%s)=%ld \n", path, regs.retval);
						break;

					case __NR_link:
						path = get_path(regs.arg1);
						path1 = get_path(regs.arg2);
						regs.retval = lind_link(path, path1);
						fprintf(stderr, "link(%s, %s)=%ld \n", path, path1,
								regs.retval);
						break;

					case __NR_fcntl:
						regs.retval = lind_fcntl(regs.arg1, regs.arg2);
						fprintf(stderr, "fcntl(%ld, %ld)=%ld \n", regs.arg1,
								regs.arg2, regs.retval);
						break;

					case __NR_listen:
						regs.retval = lind_listen(regs.arg1, regs.arg2);
						fprintf(stderr, "listen(%ld, %ld)=%ld \n", regs.arg1,
								regs.arg2, regs.retval);
						break;

					case __NR_shutdown:
						regs.retval = lind_shutdown(regs.arg1, regs.arg2);
						fprintf(stderr, "shutdown(%ld, %ld)=%ld \n", regs.arg1,
								regs.arg2, regs.retval);
						break;

					case __NR_flock:
						regs.retval = lind_flock(regs.arg1, regs.arg2);
						fprintf(stderr, "flock(%ld, %ld)=%ld \n", regs.arg1,
								regs.arg2, regs.retval);
						break;

					case __NR_epoll_create:
						regs.retval = lind_epoll_create(regs.arg1);
						fprintf(stderr, "epoll_create(%ld)=%ld \n", regs.arg1,
								regs.retval);
						break;

					case __NR_getdents:
						regs.retval = lind_getdents(regs.arg1,
								get_mem(regs.arg2, regs.arg3), regs.arg3);
						fprintf(stderr, "getdents(%s)=%ld \n",
								get_mem(regs.arg2, regs.arg3), regs.retval);
						break;

					case __NR_lseek:
						regs.retval = lind_lseek(regs.arg1, regs.arg2,
								regs.arg3);
						fprintf(stderr, "lseek(%ld, %ld, %ld)=%ld \n",
								regs.arg1, regs.arg2, regs.arg3, regs.retval);
						break;

					case __NR_pread64:
						regs.retval = lind_pread(regs.arg1,
								get_mem(regs.arg2, regs.arg3), regs.arg3,
								regs.arg4);
						fprintf(stderr,
								"pread64(%ld, 0x%lx[\"%s\"], %ld, %ld) = %ld \n",
								regs.arg1, regs.arg2, var, regs.arg3, regs.arg4,
								regs.retval);
						break;

					case __NR_pwritev:
						regs.arg1 = get_mapping(regs.arg1);
						regs.retval = lind_pwrite(regs.arg1,
								get_mem(regs.arg2, regs.arg3), regs.arg3,
								regs.arg4);
						fprintf(stderr,
								"pwritev(%ld, 0x%lx[\"%s\"], %ld, %ld) = %ld",
								regs.arg1, regs.arg2,
								get_mem(regs.arg2, regs.arg3), regs.arg3,
								regs.arg4, regs.retval);

						break;

					case __NR_socket:
						if (regs.arg1 == AF_INET) {
							regs.retval = lind_socket(regs.arg1, regs.arg2,
									regs.arg3);
							fprintf(stderr, "socket(%ld, %ld, %ld)=%ld \n",
									regs.arg1, regs.arg2, regs.arg3,
									regs.retval);
						}
						break;

					case __NR_bind:
						regs.retval = lind_bind(regs.arg1,
								get_mem(regs.arg2,
										sizeof(struct lind_sockaddr)),
								regs.arg3);
						fprintf(stderr, "bind(%ld,)=%ld \n", regs.arg1,
								regs.retval);
						break;

					case __NR_connect:
						regs.retval = lind_connect(regs.arg1,
								get_mem(regs.arg2,
										sizeof(struct lind_sockaddr)),
								regs.arg3);

						break;

					case __NR_accept:
						regs.retval = lind_accept(regs.arg1,
								get_mem(regs.arg2,
										sizeof(struct lind_sockaddr)),
								regs.arg3);
						fprintf(stderr, "accept(%ld)=%ld \n", regs.arg1,
								regs.retval);
						break;

					case __NR_sendto:
						regs.retval = lind_sendto(regs.arg1,
								get_mem(regs.arg2, regs.arg3), regs.arg3,
								regs.arg4,
								get_mem(regs.arg2,
										sizeof(struct lind_sockaddr)),
								regs.arg5);
						fprintf(stderr, "sendto(%ld)=%ld \n", regs.arg1,
								regs.retval);
						break;

					case __NR_recvfrom:
						regs.retval = lind_recvfrom(regs.arg1,
								get_mem(regs.arg2, regs.arg3), regs.arg3,
								regs.arg4,
								get_mem(regs.arg2,
										sizeof(struct lind_sockaddr)),
								regs.arg5);
						fprintf(stderr, "recvfrom(%ld)=%ld \n", regs.arg1,
								regs.retval);
						break;

					case __NR_recvmsg:
						regs.retval = lind_recvmsg(regs.arg1,
								get_mem(regs.arg2, sizeof(struct lind_msghdr)),
								regs.arg3);
						fprintf(stderr, "recvmsg(%ld)=%ld \n", regs.arg1,
								regs.retval);

						break;

					case __NR_sendmsg:
						regs.retval = lind_sendmsg(regs.arg1,
								get_mem(regs.arg2, sizeof(struct lind_msghdr)),
								regs.arg3);
						fprintf(stderr, "sendmsg(%ld)=%ld \n", regs.arg1,
								regs.retval);
						break;

					case __NR_getsockname:
						regs.retval = lind_getsockname(regs.arg1,
								get_mem(regs.arg2,
										sizeof(struct lind_sockaddr)),
								regs.arg3);
						fprintf(stderr, "getsockname(%ld)=%ld \n", regs.arg1,
								regs.retval);
						break;

					case __NR_getsockopt:
						regs.retval = lind_getsockopt(regs.arg1, regs.arg2,
								regs.arg3,
								get_mem(regs.arg4,
										sizeof(struct lind_sockaddr)),
								regs.arg5);

						fprintf(stderr, "getsockopt(%ld)=%ld \n", regs.arg1,
								regs.retval);
						break;

					case __NR_setsockopt:
						regs.retval = lind_setsockopt(regs.arg1, regs.arg2,
								regs.arg3,
								get_mem(regs.arg4,
										sizeof(struct lind_sockaddr)),
								regs.arg5);
						fprintf(stderr, "setsockopt(%ld)=%ld \n", regs.arg1,
								regs.retval);
						break;

					case __NR_socketpair:
						regs.retval = lind_socketpair(regs.arg1, regs.arg2,
								get_mem(regs.arg3, 2 * sizeof(int)), regs.arg3);
						fprintf(stderr, "socketpair(%ld, %ld)=%ld \n",
								regs.arg1, regs.arg2, regs.retval);

						break;

					case __NR_select:
						regs.retval = lind_select(regs.arg1,
								get_mem(regs.arg2, sizeof(fd_set)),
								get_mem(regs.arg3, sizeof(fd_set)),
								get_mem(regs.arg4, sizeof(fd_set)),
								get_mem(regs.arg5, sizeof(struct timeval)));

						fprintf(stderr, "select(%ld)=%ld \n", regs.arg1,
								regs.retval);
						break;

					case __NR_getpeername:
						regs.retval = lind_getpeername(regs.arg1,
								get_mem(regs.arg2,
										sizeof(struct lind_sockaddr)),
								regs.arg3);

						fprintf(stderr, "getpeername(%ld)=%ld \n", regs.arg1,
								regs.retval);

						break;

					case __NR_poll:
						regs.retval = lind_poll(
								get_mem(regs.arg1, sizeof(struct lind_pollfd)),
								get_mem(regs.arg1, sizeof(lind_nfds_t)),
								regs.arg2);
						fprintf(stderr, "poll(%ld)=%ld \n", regs.arg1,
								regs.retval);

						break;

					case __NR_epoll_ctl:
						regs.retval = lind_epoll_ctl(regs.arg1, regs.arg2,
								regs.arg3,
								get_mem(regs.arg4,
										sizeof(struct lind_epoll_event)));
						fprintf(stderr, "epoll_ctl(%ld)=%ld \n", regs.arg1,
								regs.retval);

						break;

					case __NR_epoll_wait:
						regs.retval = lind_epoll_wait(regs.arg1,
								get_mem(regs.arg2,
										sizeof(struct lind_epoll_event)),
								regs.arg3, regs.arg4);
						fprintf(stderr, "epoll_wait(%ld)=%ld \n", regs.arg1,
								regs.retval);

						break;

					default:
						regs.retval = EINVAL;
						break;
					} /* switch*/
					set_args(&regs);
				}
			} /* entering */
		} /* WSTOPSIG*/
	}/* while */
}

/* get the path of files required by a syscall through the defined address */
char *get_path(long addr) {
	static char path[PATH_MAX];
	long ret;
	char ch;
	int i;
	int j = 0;
	size_t len = 0;
	do {
		ret = ptrace(PTRACE_PEEKDATA, tracee, (char *) (addr + j), 0);
		for (i = 0; i < sizeof(long); i++) {
			ch = ret - (ret >> 8) * 256;
			if (ch == 0)
				break;
			path[len++] = ch;
			ret = ret >> 8;
		}
		j = j + sizeof(long);
	} while (ch != 0);
	path[len++] = '\0';
	return path;
}

/* set the memory from an address to a specific buffer */
void set_mem(long addr, void * buff, size_t count) {
	long ret;
	int i;

	int fullblocks = count / sizeof(long);
	int remainder = count % sizeof(long);

	for (i = 0; i < fullblocks; i++) {
		ret = ptrace(PTRACE_POKEDATA, tracee,
				(char *) (addr + sizeof(long) * i),
				*(long*) ((char*) buff + sizeof(long) * i));
	}

	if (remainder) {
		unsigned long value = ptrace(PTRACE_PEEKDATA, tracee,
				(char *) (addr + sizeof(long) * fullblocks), 0);
		value = (ret & (ULONG_MAX << (remainder * 8)))
				| (*(long*) ((char*) buff + sizeof(long) * fullblocks)
						& (~(ULONG_MAX << (remainder * 8))));
		ret = ptrace(PTRACE_POKEDATA, tracee,
				(char *) (addr + sizeof(long) * fullblocks), value);
	}

}

/* get count number of memory defined through an address */
void *get_mem(long addr, size_t count) {

	long ret;
	int i;
	long * mem = malloc((count / sizeof(long) + 1) * sizeof(long));

	for (i = 0; i < count / sizeof(long) + 1; i++) {
		ret = ptrace(PTRACE_PEEKDATA, tracee,
				(char *) (addr + sizeof(long) * i), 0);
		mem[i] = ret;
	}
	return (void*) mem;
}

/* return syscall number by name */
int get_syscall_num(char *name) {
	int i;

	for (i = 0; i < TOTAL_SYSCALLS; i++)
		if (syscall_names[i] && !strcmp(syscall_names[i], name))
			return i;
	return -1;
}

/* return the arguments of a syscall by ptrace */
void get_args(struct syscall_args *args) {

	if (ptrace(PTRACE_GETREGS, tracee, 0, &args->user) < 0)
		perror("ptrace did not get the register arguments.");

	args->syscall = args->user.regs.orig_rax;
	args->retval = args->user.regs.rax;

	args->arg1 = args->user.regs.rdi;
	args->arg2 = args->user.regs.rsi;
	args->arg3 = args->user.regs.rdx;
	args->arg4 = args->user.regs.r10;
	args->arg5 = args->user.regs.r8;
	args->arg6 = args->user.regs.r9;

}

/* set the arguments of a syscall by ptrace */
void set_args(struct syscall_args *args) {
	args->user.regs.orig_rax = args->syscall;
	args->user.regs.rax = args->retval;
	args->user.regs.rdi = args->arg1;
	args->user.regs.rsi = args->arg2;
	args->user.regs.rdx = args->arg3;
	args->user.regs.r10 = args->arg4;
	args->user.regs.r8 = args->arg5;
	args->user.regs.r9 = args->arg6;

	if (ptrace(PTRACE_SETREGS, tracee, 0, &args->user) < 0) {
		perror("ptrace was not set.");
	}
}

/* load the config file containing the policies to dispatch the syscalls */
int load_config() {

	char *str, buff[100];
	char *key, *value;
	enum monitor_action mact = ALLOW_LIND;
	int i = 0;

	FILE *fp = fopen(CONFIG_FILE, "r");

	if (fp == NULL) {
		fprintf(stderr, "Config file %s could not be opened. \n ", CONFIG_FILE);
		return -1;
	}

	while ((str = fgets(buff, sizeof buff, fp)) != NULL) {

		if (buff[0] == '\n' || buff[0] == '#')
			continue;

		char* sep;
		if (sep = strchr(str, '=')) {
			*sep++ = 0;
			key = strdup(str);
			value = strdup(sep);

			if (strcmp(value, "ALLOW_LIND\n") == 0) {
				mact = ALLOW_LIND;
			} else if (strcmp(value, "DENY_LIND\n") == 0) {
				mact = DENY_LIND;
			} else if (strcmp(value, "ALLOW_OS\n") == 0) {
				mact = ALLOW_OS;
			}
			int sys = get_syscall_num(key);
			monitor_actions[sys] = mact;
		}
	}
	fflush(fp);
	fclose(fp);
	return 0;
}

char * trim(char * str) {
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
