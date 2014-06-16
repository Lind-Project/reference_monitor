/*
 * monitor.h
 *
 *  Created on: April 17, 2014
 *      Author:  Ali Gholami, Shengqian Ji
 */

#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/reg.h>
#include <signal.h>
#include <sys/user.h>
#include <errno.h>
#include <stdarg.h>
#include <linux/unistd.h>	/* __NR_XXX */
#include "lind_util.h"

#ifdef __x86_64__

#define CONFIG_FILE get_lindcwd()

#define SIZE(a) (int)(sizeof(a)/sizeof(a[0]))

struct syscall_args {
	uint64_t syscall;
	uint64_t arg1, arg2, arg3, arg4, arg5, arg6;
	uint64_t retval;
	struct user user;
};


/* initialize a process to be traced */
void init_ptrace(int argc, char** argv);
/* intercept the system calls issued by the tracee process */
void intercept_calls();
/* get the path of files required by a syscall through the defined address */
char *get_path(long addr);
/* set the memory from an address to a specific buffer */
void set_mem(long addr, void * buff, size_t count);
/* get count number of memory defined through an address */
void *get_mem(long addr, size_t count);
/* return syscall number by name */
int get_syscall_num(char *name);
/* return the arguments of a syscall by ptrace */
void get_args(struct syscall_args *args);
/* set the arguments of a syscall by ptrace */
void set_args(struct syscall_args *args);
/* load the config file containing the policies to dispatch the syscalls */
int load_config();


/* possible actions for syscalls */
enum monitor_action {
	ALLOW_LIND,
	DENY_LIND,
	ALLOW_OS
};

/* the tracee pid that should be monitored */
pid_t tracee;

const char * const syscall_names[] = {
#include "configs/syscalls.h"
		};

#define TOTAL_SYSCALLS SIZE(syscall_names)

/* actions that are required to be taken upon issuing calls */
int monitor_actions[TOTAL_SYSCALLS];

#endif
