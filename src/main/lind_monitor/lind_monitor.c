/*
 * lind_monitor.c
 *
 *  Created on: April 17, 2014
 *      Author: Ali Gholami, Shengqian Ji
 */

#include "lind_monitor.h"
#include "../platform/lind_platform.h"

void monitor_ns()
{

	if (entering) {
		entering = 0;
	} else {
		regs.retval = EINVAL;
		fprintf(stdout, "%s () not supported by lind\n",
				syscall_names[regs.syscall], regs.retval);

		set_args(&regs);
		entering = 1;

	}
}

void monitor_execve()
{
	if (entering) {
		entering = 0;

		char *execve_path;
		char **execve_args;

		execve_path = get_path(regs.arg1);
		char** p;
		int i = 0, argc;
		while (1) {
			p = (char**) get_mem(regs.arg2 + i * sizeof(char*), sizeof(char*));
			++i;
			if (!*p)
				break;
		}
		argc = i;
		execve_args = malloc(sizeof(char*) * argc);
		execve_args[argc - 1] = 0;
		i = 0;
		fprintf(stdout, "[monitor] execve(%s, [", execve_path);
		for (i = 0; i < argc; ++i) {
			p = get_mem(regs.arg2 + i * sizeof(char*), sizeof(char*));
			if (*p) {
				execve_args[i] = get_path(*p);
				fprintf(stdout, "%s ", execve_args[i]);
			}
		}
	} else {
		fprintf(stdout, "]");
		fprintf(stdout, ") = %d \n", regs.retval);
		entering = 1;
	}
}

void monitor_deny()
{
	if (entering) {
		regs.retval = EINVAL;
		set_args(&regs);
		entering = 0;
	} else {
		fprintf(stdout, "[monitor] Deny call by Lind: %s() = %d\n",
				syscall_names[regs.syscall], regs.retval);
		entering = 1;
	}
}

void monitor_close()
{
	if (entering) {
		if ((int32_t) regs.arg1 >= 0) {
			regs.arg1 = get_mapping(regs.arg1);
			entering = 0;
		}
	} else {

		regs.retval = lind_close(regs.arg1);
		fprintf(stdout, "[monitor] close(%d) = %d \n", (int) regs.arg1,
				(int) regs.retval);
		entering = 1;
	}
	set_args(&regs);
}

void monitor_getuid()
{
	if (entering) {
		entering = 0;
	} else {
		regs.retval = lind_getuid();
		set_args(&regs);
		fprintf(stdout, "[monitor] getuid() = %d \n", (int) regs.retval);
		entering = 1;
	}
}

void monitor_read()
{
	if (entering) {
		entering = 0;
	} else {
		regs.arg1 = get_mapping(regs.arg1);
		void *buff = malloc(regs.arg3);
		regs.retval = lind_read(regs.arg1, buff, regs.arg3);
		set_mem(regs.arg2, buff, regs.arg3);
		set_args(&regs);
		fprintf(stdout, "read(%ld, 0x%lx[\"%p\"], %ld) = %d \n", regs.arg1,
				regs.arg2, buff, regs.arg3, regs.retval);
		entering = 1;
	}
}

void monitor_open()
{
	if (entering) {
		entering = 0;
	} else {

		char *path = get_path(regs.arg1);
		int lind_fd = lind_open(path, regs.arg2, regs.arg3);

		if (lind_fd >= 0) {
			add_mapping(regs.retval, lind_fd);
			regs.retval = lind_fd;
		} else {
			regs.retval = -1;
		}

		set_args(&regs);
		fprintf(stdout, "[monitor] open(%s, %d, %d) = %d\n", path,
				(int) regs.arg2, (int) regs.arg3, (int) regs.retval);
		entering = 1;
	}
}

void monitor_openat()
{
	if (entering) {
		entering = 0;
	} else {
		char *path = get_path(regs.arg2);
		int lind_fd = lind_openat(regs.arg1, path, regs.arg3, regs.arg4);

		if (lind_fd >= 0) {
			add_mapping(regs.retval, lind_fd);
			regs.retval = lind_fd;
		} else {
			regs.retval = -1;
		}
		set_args(&regs);
		fprintf(stdout, "[monitor] openat(%d, %s, %d, %d) = %d\n",
				(int) regs.arg1, path, (int) regs.arg2, (int) regs.arg4,
				(int) regs.retval);
		entering = 1;
	}
}

void monitor_access()
{
	if (entering) {
		entering = 0;
	} else {

		char *path = get_path(regs.arg1);
		int lind_fd = lind_access(path, regs.arg2);
		if (lind_fd >= 0) {
			add_mapping(regs.retval, lind_fd);
			regs.retval = lind_fd;
		} else {
			regs.retval = -1;
		}
		set_args(&regs);
		fprintf(stdout, "[monitor] access(%s, %d, %d) = %d\n", path,
				(int) regs.arg2, (int) regs.arg3, (int) regs.retval);
		entering = 1;
	}
}

void monitor_statfs()
{
	struct lind_stat st;
	struct lind_statfs stfs;

	if (entering) {
		entering = 0;
	} else {
		char *path = get_path(regs.arg1);
		regs.retval = lind_statfs(path, &stfs);
		set_mem(regs.arg2, &stfs, sizeof(stfs));
		set_args(&regs);
		fprintf(stdout, "[monitor] statfs(%s) = %d \n", path,
				(int) regs.retval);
		entering = 1;
	}
}

void monitor_stat()
{
	struct lind_stat st;

	if (entering) {
		entering = 0;
	} else {
		char *path = get_path(regs.arg1);
		regs.retval = lind_stat(path, &st);
		set_mem(regs.arg2, &st, sizeof(st));
		set_args(&regs);
		fprintf(stdout, "[monitor] stat(%s) = %d \n", path, (int) regs.retval);
		entering = 1;
	}
}

void monitor_fstat()
{
	struct lind_stat st;

	if (entering) {
		entering = 0;
	} else {
		regs.arg1 = get_mapping(regs.arg1);
		regs.retval = lind_fstat(regs.arg1, &st);
		set_mem(regs.arg2, &st, sizeof(st));
		fprintf(stdout, "[monitor] fstat(%d) = %d \n", regs.arg1,
				(int) regs.retval);
		set_args(&regs);
		entering = 1;
	}
}

void monitor_fstatfs()
{
	struct lind_stat st;
	struct lind_statfs stfs;

	if (entering) {
		entering = 0;
	} else {
		regs.arg1 = get_mapping(regs.arg1);
		regs.retval = lind_fstatfs(regs.arg1, &stfs);
		set_mem(regs.arg2, &stfs, sizeof(stfs));
		set_args(&regs);
		fprintf(stdout, "[monitor] fstatfs(%d) = %d \n", (int) regs.arg1,
				(int) regs.retval);
		entering = 1;
	}
}

void monitor_rmdir()
{
	if (entering) {
		entering = 0;
	} else {
		char* path = get_path(regs.arg1);
		regs.retval = lind_rmdir(path);
		set_args(&regs);
		fprintf(stdout, "[monitor] rmdir(%s) = %d \n", path, (int) regs.retval);
		entering = 1;
	}
}

void monitor_getpid()
{
	if (entering) {
		entering = 0;
	} else {
		regs.retval = lind_getpid();
		set_args(&regs);
		fprintf(stdout, "[monitor] getpid() = %d \n", (int) regs.retval);
		entering = 1;
	}
}

void monitor_geteuid()
{
	if (entering) {
		entering = 0;
	} else {
		regs.retval = lind_geteuid();
		set_args(&regs);
		fprintf(stdout, "[monitor] geteuid() = %d \n", (int) regs.retval);
		entering = 1;
	}
}

void monitor_getgid()
{
	if (entering) {
		entering = 0;
	} else {
		regs.retval = lind_getgid();
		fprintf(stdout, "[monitor] getgid() = %d \n", (int) regs.retval);
		set_args(&regs);
		entering = 1;
	}
}

void monitor_getegid()
{
	if (entering) {
		entering = 0;
	} else {
		regs.retval = lind_getegid();
		set_args(&regs);
		fprintf(stdout, "[monitor] getegid() = %d \n", (int) regs.retval);
		entering = 1;
	}
}

void monitor_write()
{
	if (entering) {
		entering = 0;
	} else {
		regs.arg1 = get_mapping(regs.arg1);
		regs.retval = lind_write(regs.arg1, get_mem(regs.arg2, regs.arg3),
				regs.arg3);

		set_args(&regs);
		fprintf(stdout, "write(%d, 0x%lx[\"%p\"], %d) = %d \n", (int) regs.arg1,
				(int) regs.arg2, get_mem(regs.arg2, regs.arg3), (int) regs.arg3,
				(int) regs.retval);
		entering = 1;
	}
}

void monitor_unlink()
{
	if (entering) {
		entering = 0;
	} else {
		char *path = get_path(regs.arg1);
		regs.retval = lind_unlink(path);
		set_args(&regs);

		fprintf(stdout, "[monitor] unlink(%s) = %d \n", path,
				(int) regs.retval);

		entering = 1;
	}
}

void monitor_link()
{
	if (entering) {
		entering = 0;
	} else {
		char *path = get_path(regs.arg1);
		char *path1 = get_path(regs.arg2);
		regs.retval = lind_link(path, path1);
		set_args(&regs);
		fprintf(stdout, "[monitor] link(%s, %s) = %d \n", path, path1,
				(int) regs.retval);
		entering = 1;
	}
}

void monitor_fcntl()
{
	if (entering) {
		entering = 0;
	} else {
		regs.arg1 = get_mapping(regs.arg1);
		regs.retval = lind_fcntl(regs.arg1, regs.arg2);
		set_args(&regs);
		fprintf(stdout, "[monitor] fcntl(%ld, %ld) = %d \n", (int) regs.arg1,
				(int) regs.arg2, (int) regs.retval);
		entering = 1;
	}
}

void monitor_listen()
{
	if (entering) {
		entering = 0;
	} else {
		regs.arg1 = get_mapping(regs.arg1);
		regs.retval = lind_listen(regs.arg1, regs.arg2);
		set_args(&regs);
		fprintf(stdout, "[monitor] listen(%d, %d) = %d \n", (int) regs.arg1,
				(int) regs.arg2, (int) regs.retval);
		entering = 1;
	}
}

void monitor_shutdown()
{
	if (entering) {
		entering = 0;
	} else {
		regs.arg1 = get_mapping(regs.arg1);
		regs.retval = lind_shutdown(regs.arg1, regs.arg2);
		set_args(&regs);
		fprintf(stdout, "[monitor] shutdown(%d, %d) = %d \n", (int) regs.arg1,
				(int) regs.arg2, (int) regs.retval);
		entering = 1;
	}
}

void monitor_mkdir()
{
	if (entering) {
		entering = 0;
	} else {
		char *path = get_path(regs.arg1);
		regs.retval = lind_mkdir(path, regs.arg2);
		set_args(&regs);
		fprintf(stdout, "[monitor] mkdir(%s) = %d \n", path, (int) regs.retval);
		entering = 1;
	}
}

void monitor_chdir()
{
	if (entering) {
		entering = 0;
	} else {
		char *path = get_path(regs.arg1);
		regs.retval = lind_chdir(path);
		set_args(&regs);
		fprintf(stdout, "[monitor] chdir(%s) = %d \n", path, (int) regs.retval);
		entering = 1;
	}
}

void monitor_getcwd() {

	if (entering) {
		entering = 0;
	} else {
		char *path = get_mem(regs.arg1, regs.arg2);
		regs.retval = lind_getcwd(path, regs.arg2);
		set_args(&regs);
		fprintf(stdout, "[monitor] getcwd(%s) = %d \n", path,
				(int) regs.retval);
		entering = 1;
	}
}

void monitor_dup()
{
	if (entering) {
		entering = 0;
	} else {
		int lind_fd = lind_dup(regs.arg1);
		if (lind_fd >= 0) {
			add_mapping(regs.retval, lind_fd);
			regs.retval = lind_fd;
		} else {
			regs.retval = -1;
		}
		set_args(&regs);
		fprintf(stdout, "[monitor] dup(%d) = %d \n", (int) regs.arg1,
				(int) regs.retval);
		entering = 1;
	}
}

void monitor_dup2()
{

	if (entering) {
		entering = 0;
	} else {
		int lind_fd = lind_dup2(regs.arg1, regs.arg2);
		if (lind_fd >= 0) {
			add_mapping(regs.retval, lind_fd);
			regs.retval = lind_fd;
		} else {
			regs.retval = -1;
		}
		set_args(&regs);
		fprintf(stdout, "[monitor] dup2(%d, %d) = %d \n", (int) regs.arg1,
				(int) regs.arg2, (int) regs.retval);
		entering = 1;
	}
}

void monitor_dup3()
{
	if (entering) {
		entering = 0;
	} else {

		int lind_fd = lind_dup3(regs.arg1, regs.arg2, regs.arg3);
		if (lind_fd >= 0) {
			add_mapping(regs.retval, lind_fd);
			regs.retval = lind_fd;
		} else {
			regs.retval = -1;
		}
		set_args(&regs);
		fprintf(stdout, "[monitor] dup3(%d, %d, %d) = %d \n", (int) regs.arg1,
				(int) regs.arg2, (int) regs.arg3, (int) regs.retval);
		entering = 1;
	}
}

void monitor_flock()
{
	if (entering) {
		entering = 0;
	} else {
		regs.arg1 = get_mapping(regs.arg1);
		regs.retval = lind_flock(regs.arg1, regs.arg2);
		set_args(&regs);
		fprintf(stdout, "[monitor] flock(%d, %d) = %d \n", (int) regs.arg1,
				(int) regs.arg2, (int) regs.retval);
		entering = 1;
	}
}

void monitor_epoll_create()
{
	if (entering) {
		entering = 0;
	} else {
		int lind_fd = lind_epoll_create(regs.arg1);
		if (lind_fd >= 0) {
			add_mapping(regs.retval, lind_fd);
			regs.retval = lind_fd;
		} else {
			regs.retval = -1;
		}
		set_args(&regs);
		fprintf(stdout, "[monitor] epoll_create(%d) = %d \n", (int) regs.arg1,
				(int) regs.retval);
		entering = 1;
	}
}

void monitor_getdents()
{
	if (entering) {
		entering = 0;
	} else {
		regs.arg1 = get_mapping(regs.arg1);
		regs.retval = lind_getdents(regs.arg1, get_mem(regs.arg2, regs.arg3),
				regs.arg3);
		set_args(&regs);
		fprintf(stdout, "[monitor] getdents(%d, %d) = %d \n", regs.arg1,
				regs.arg3, regs.retval);
		entering = 1;
	}
}

void monitor_lseek()
{
	if (entering) {
		entering = 0;
	} else {

		regs.arg1 = get_mapping(regs.arg1);
		regs.retval = lind_lseek(regs.arg1, regs.arg2, regs.arg3);
		set_args(&regs);
		fprintf(stdout, "[monitor] lseek(%u, %lld, %u) = %d \n", regs.arg1,
				regs.arg2, regs.arg3, regs.retval);
		entering = 1;
	}
}

void monitor_pwritev()
{
	if (entering) {
		entering = 0;
	} else {

		int lind_fd = get_mapping(regs.arg1);
		regs.retval = lind_pwrite(lind_fd, get_mem(regs.arg2, regs.arg3),
				regs.arg3, regs.arg4);
		set_args(&regs);
		fprintf(stdout, "pwritev(%u, 0x%lx[\"%p\"], %z, %lld) = %d \n", lind_fd,
				regs.arg2, get_mem(regs.arg2, regs.arg3), regs.arg3, regs.arg4,
				regs.retval);
		entering = 1;
	}
}

void monitor_pread64()
{
	if (entering) {
		entering = 0;
	} else {

		regs.retval = lind_pread(regs.arg1, get_mem(regs.arg2, regs.arg3),
				regs.arg3, regs.arg4);
		set_args(&regs);
		fprintf(stdout, "pread64(%u, 0x%lx[\"%p\"], %z, %lld) = %d \n",
				regs.arg1, regs.arg2, get_mem(regs.arg2, regs.arg3), regs.arg3,
				regs.arg4, regs.retval);
		entering = 1;
	}

}

void monitor_socket()
{
	if (entering) {
		entering = 0;
	} else {
		if (regs.arg1 == AF_INET) {
			int lind_fd = lind_socket(regs.arg1, regs.arg2, regs.arg3);

			if (lind_fd >= 0) {
				add_mapping(regs.retval, lind_fd);
				regs.retval = lind_fd;
			} else {
				regs.retval = -1;
			}
			set_args(&regs);
			fprintf(stdout, "[monitor] socket(%d, %d, %d) = %d \n", lind_fd,
					(int) regs.arg2, (int) regs.arg3, (int) regs.retval);
		}
			entering = 1;
	}
}

void monitor_bind()
{
	if (entering) {
		entering = 0;
	} else {
		regs.arg1 = get_mapping(regs.arg1);
		regs.retval = lind_bind(regs.arg1,
				get_mem(regs.arg2, sizeof(struct lind_sockaddr)), regs.arg3);

		set_args(&regs);
		fprintf(stdout, "[monitor] bind(%d) = %d \n", regs.arg1,
				(int) regs.retval);
		entering = 1;
	}
}

void monitor_connect()
{
	if (entering) {
		entering = 0;
	} else {
		regs.arg1 = get_mapping(regs.arg1);
		regs.retval = lind_connect(regs.arg1,
				get_mem(regs.arg2, sizeof(struct lind_sockaddr)), regs.arg3);

		fprintf(stdout, "[monitor] connect(%d) = %d \n", regs.arg1,
				(int) regs.retval);
		set_args(&regs);
		entering = 1;
	}
}

void monitor_accept()
{

	if (entering) {
		entering = 0;
	} else {
		int lind_fd = lind_accept(regs.arg1,
				get_mem(regs.arg2, sizeof(struct lind_sockaddr)),
				(lind_socklen_t*) regs.arg3);

		if (lind_fd >= 0) {
			add_mapping(regs.retval, lind_fd);
			regs.retval = lind_fd;
		} else {
			regs.retval = -1;
		}

		set_args(&regs);
		fprintf(stdout, "[monitor] accept(%d) = %d \n", regs.arg1,
				(int) regs.retval);
		entering = 1;
	}
}

void monitor_sendto()
{

	if (entering) {
		entering = 0;
	} else {
		regs.arg1 = get_mapping(regs.arg1);
		regs.retval = lind_sendto(regs.arg1, get_mem(regs.arg2, regs.arg3),
				regs.arg3, regs.arg4,
				get_mem(regs.arg2, sizeof(struct lind_sockaddr)), regs.arg5);
		set_args(&regs);
		fprintf(stdout, "[monitor] sendto(%d) = %d \n", regs.arg1,
				(int) regs.retval);
		entering = 1;
	}
}

void monitor_recvfrom()
{
	if (entering) {
		entering = 0;
	} else {
		regs.arg1 = get_mapping(regs.arg1);
		char *var = malloc(regs.arg3);
		struct lind_sockaddr * buff = malloc(regs.arg6);

		regs.retval = lind_recvfrom(regs.arg1, var, regs.arg3, regs.arg4, buff,
				regs.arg6);

		set_mem(regs.arg2, var, regs.arg3);
		set_mem(regs.arg5, buff, regs.arg6);

		free(var);
		free(buff);
		set_args(&regs);
		fprintf(stdout, "[monitor] recvfrom(%d) = %d \n", regs.arg1,
				(int) regs.retval);
		entering = 1;
	}
}

void monitor_recvmsg()
{
	if (entering) {
		entering = 0;
	} else {
		struct lind_msghdr msg_orig;
		struct lind_msghdr* msg = (struct lind_msghdr*) get_mem(regs.arg2,
				sizeof(struct lind_msghdr));
		msg_orig = *msg;
		struct lind_iovec* iovs = (struct lind_iovec*) get_mem(msg->msg_iov,
				sizeof(struct lind_iovec) * msg->msg_iovlen);
		struct lind_iovec* iovs_orig = (struct lind_iovec*) malloc(
				sizeof(struct lind_iovec) * msg->msg_iovlen);
		memcpy(iovs_orig, iovs, sizeof(struct lind_iovec) * msg->msg_iovlen);

		for (int i = 0; i < msg->msg_iovlen; ++i) {
			//iovs[i].iov_base = get_mem(iovs[i].iov_base, iovs[i].iov_len);
			iovs[i].iov_base = malloc(iovs[i].iov_len);
		}
		msg->msg_iov = iovs;
		//msg->msg_name = get_mem(msg->msg_name, msg->msg_namelen);
		msg->msg_name = malloc(msg->msg_namelen);
		//msg->msg_control = get_mem(msg->msg_control, msg->msg_controllen);
		msg->msg_control = malloc(msg->msg_controllen);

		regs.arg1 = get_mapping(regs.arg1);

		regs.retval = lind_recvmsg(regs.arg1, msg, regs.arg3);

		set_mem(msg_orig.msg_name, msg->msg_name, msg->msg_namelen);
		set_mem(msg_orig.msg_control, msg->msg_control, msg->msg_controllen);
		for (int i = 0; i < msg->msg_iovlen; ++i) {
			set_mem(iovs_orig[i].iov_base, iovs[i].iov_base,
					iovs_orig[i].iov_len);
		}
		free(iovs);
		free(iovs_orig);
		free(msg);

		set_args(&regs);
		fprintf(stdout, "[monitor] recvmsg(%d, %d) = %d \n", regs.arg1,
				(int) regs.arg3, regs.retval);
		entering = 1;
	}
}

void monitor_sendmsg()
{
	if (entering) {
		entering = 0;
	} else {
		struct lind_msghdr* msg = (struct lind_msghdr*) get_mem(regs.arg2,
				sizeof(struct lind_msghdr));

		struct lind_iovec* iovs = (struct lind_iovec*) get_mem(msg->msg_iov,
				sizeof(struct lind_iovec) * msg->msg_iovlen);
		struct lind_iovec* iovs_orig = (struct lind_iovec*) malloc(
				sizeof(struct lind_iovec) * msg->msg_iovlen);
		memcpy(iovs_orig, iovs, sizeof(struct lind_iovec) * msg->msg_iovlen);
		for (int i = 0; i < msg->msg_iovlen; ++i) {
			iovs[i].iov_base = get_mem(iovs[i].iov_base, iovs[i].iov_len);
		}
		msg->msg_iov = iovs;
		//msg->msg_name = get_mem(msg->msg_name, msg->msg_namelen);
		msg->msg_name = malloc(msg->msg_namelen);
		//msg->msg_control = get_mem(msg->msg_control, msg->msg_controllen);
		msg->msg_control = malloc(msg->msg_controllen);

		regs.arg1 = get_mapping(regs.arg1);

		regs.retval = lind_sendmsg(regs.arg1, msg, regs.arg3);
		free(msg);
		free(iovs);
		free(iovs_orig);
		set_args(&regs);
		fprintf(stdout, "[monitor] sendmsg(%ld) = %d \n", regs.arg1,
				regs.retval);
		entering = 1;
	}
}

void monitor_getsockname()
{
	if (entering) {
		entering = 0;
	} else {
		struct lind_sockaddr *buff = malloc(regs.arg3);

		regs.arg1 = get_mapping(regs.arg1);

		regs.retval = lind_getsockname(regs.arg1, buff,
				(lind_socklen_t*) regs.arg3);
		set_mem(regs.arg2, buff, sizeof(struct lind_sockaddr));

		set_args(&regs);
		fprintf(stdout, "[monitor] getsockname(%ld) = %d \n", regs.arg1,
				regs.retval);
		entering = 1;

	}
}

void monitor_getsockopt()
{
	if (entering) {
		entering = 0;
	} else {
		struct lind_sockaddr *buff = malloc(regs.arg2);
		regs.arg1 = get_mapping(regs.arg1);
		regs.retval = lind_getsockopt(regs.arg1, regs.arg2, regs.arg3, buff,
				(lind_socklen_t*) regs.arg5);
		set_mem(regs.arg4, buff, sizeof(struct lind_sockaddr));
		set_args(&regs);
		fprintf(stdout, "[monitor] getsockopt(%d) = %d \n", (int) regs.arg1,
				(int) regs.retval);
		entering = 1;
	}
}

void monitor_setsockopt()
{
	if (entering) {
		entering = 0;
	} else {
		regs.arg1 = get_mapping(regs.arg1);
		regs.retval = lind_setsockopt(regs.arg1, regs.arg2, regs.arg3,
				get_mem(regs.arg4, sizeof(struct lind_sockaddr)), regs.arg5);

		set_args(&regs);
		fprintf(stdout, "[monitor] setsockopt(%d) = %d \n", (int) regs.arg1,
				(int) regs.retval);
		entering = 1;

	}
}

void monitor_socketpair()
{
	if (entering) {
		entering = 0;
	} else {
		int lind_fd = lind_socketpair(regs.arg1, regs.arg2,
				get_mem(regs.arg3, 2 * sizeof(int)), regs.arg3);

		if (lind_fd >= 0) {
			add_mapping(regs.retval, lind_fd);
			regs.retval = lind_fd;
		} else {
			regs.retval = -1;
		}
		set_args(&regs);
		fprintf(stdout, "[monitor] socketpair(%d, %d) = %d \n", (int) regs.arg1,
				(int) regs.arg2, (int) regs.retval);
		entering = 1;
	}
}

void monitor_getpeername()
{
	if (entering) {
		entering = 0;
	} else {

		regs.arg1 = get_mapping(regs.arg1);
		struct lind_sockaddr *buff = malloc(regs.arg3);

		regs.retval = lind_getpeername(regs.arg1, buff, regs.arg3);
		set_mem(regs.arg2, buff, sizeof(struct lind_sockaddr));

		set_args(&regs);
		fprintf(stdout, "[monitor] getpeername(%d) = %d \n", regs.arg1,
				(int) regs.retval);
		entering = 1;
	}
}

void monitor_select()
{
	if (entering) {
		entering = 0;
	} else {

		void* set1 = get_mem(regs.arg2, sizeof(fd_set));
		void* set2 = get_mem(regs.arg3, sizeof(fd_set));
		void* set3 = get_mem(regs.arg4, sizeof(fd_set));
		void* tv = get_mem(regs.arg5, sizeof(struct timeval));

		regs.arg1 = get_mapping(regs.arg1);
		regs.retval = lind_select(regs.arg1, set1, set2, set3, tv);
		set_mem(regs.arg2, set1, sizeof(fd_set));
		set_mem(regs.arg3, set2, sizeof(fd_set));
		set_mem(regs.arg4, set3, sizeof(fd_set));
		set_mem(regs.arg5, tv, sizeof(struct timeval));
		set_args(&regs);
		fprintf(stdout, "[monitor] select(%d) = %d \n", (int) regs.arg1,
				(int) regs.retval);
		entering = 1;
	}
}

void monitor_poll()
{
	if (entering) {
		entering = 0;
	} else {
		struct lind_pollfd * lpfd = malloc(sizeof(struct lind_pollfd));
		regs.retval = lind_poll(lpfd, regs.arg2, regs.arg3);
		set_mem(regs.arg1, lpfd, sizeof(struct lind_pollfd));
		set_args(&regs);
		fprintf(stdout, "[monitor] poll(%d) = %d \n", (int) regs.arg1,
				(int) regs.retval);
		entering = 1;
	}
}

void monitor_epoll_ctl()
{
	if (entering) {
		entering = 0;
	} else {

		struct lind_epoll_event *event = malloc(
				sizeof(struct lind_epoll_event));
		regs.retval = lind_epoll_ctl(regs.arg1, regs.arg2, regs.arg3,
				get_mem(regs.arg4, sizeof(struct lind_epoll_event)));

		set_mem(regs.arg4, event, sizeof(struct lind_epoll_event));
		set_args(&regs);
		fprintf(stdout, "[monitor] epoll_ctl(%d) = %d \n", (int) regs.arg1,
				(int) regs.retval);
		entering = 1;
	}
}

void monitor_epoll_wait()
{
	if (entering) {
		entering = 0;
	} else {
		struct lind_epoll_event *event = malloc(
				sizeof(struct lind_epoll_event));
		regs.retval = lind_epoll_wait(regs.arg1, event, regs.arg3, regs.arg4);
		set_mem(regs.arg2, event, sizeof(struct lind_epoll_event));
		set_args(&regs);
		fprintf(stdout, "[monitor] epoll_wait(%d) = %d \n", (int) regs.arg1,
				(int) regs.retval);
		entering = 1;
	}
}

void monitor_os()
{
	if (entering) {
		entering = 0;
	} else {
		fprintf(stdout, "[monitor] %s()=%d \n", syscall_names[regs.syscall],
				(int) regs.retval);
		entering = 1;
	}
}

void monitor_arch_prctl()
{
	if (entering) {
		entering = 0;
		if (((int32_t) regs.arg5) >= 0) {
			regs.arg5 = get_mapping(regs.arg5);
			set_args(&regs);
			fprintf(stdout, "[monitor] arch_prctl() =  %d  \n",
					(int) regs.retval);
		}
	} else {
		entering = 1;
	}
}

void monitor_mmap()
{
	if (entering) {
		entering = 0;
		if (((int32_t) regs.arg5) >= 0) {
			regs.arg5 = get_mapping(regs.arg5);
			set_args(&regs);
			fprintf(stdout, "[monitor] mmap() = 0x%jx  \n", regs.retval);
		}
	} else {
		entering = 1;
	}
}

void monitor_munmap()
{
	if (entering) {
		entering = 0;
		fprintf(stdout, "[monitor] munmap() = 0x%jx  \n", regs.retval);
	} else {
		entering = 1;
	}
}

void monitor_mprotect()
{
	if (entering) {
		entering = 0;
		fprintf(stdout, "[monitor] mprotect() = %lu  \n", regs.retval);
	} else {
		entering = 1;
	}
}

void monitor_brk()
{
	if (entering) {
		entering = 0;
	} else {
		fprintf(stdout, "[monitor] brk() = 0x%jx  \n", (int) regs.retval);
		entering = 1;
	}
}

void monitor_exit_group()
{
	if (entering) {
		entering = 0;
		fprintf(stdout, "[monitor] exit_group() = %d  \n", (int) regs.retval);
	} else {
		entering = 1;
	}
}

void monitor_exit()
{
	if (entering) {
		entering = 0;
		fprintf(stdout, "[monitor] exit() = %d  \n", (int) regs.retval);
	} else {

		entering = 1;
	}
}
void monitor_tgkill()
{
	if (entering) {
		entering = 0;
		fprintf(stdout, "[monitor] tgkill() = %d  \n", (int) regs.retval);
	} else {

		entering = 1;
	}
}


int main(int argc, char** argv)
{

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
void init_ptrace(int argc, char** argv)
{

	char ** argv1 = malloc(sizeof(char*) * argc);
	memcpy(argv1, argv + 1, sizeof(char*) * (argc - 1));
	argv1[argc - 1] = NULL;

	load_config();
	tracee = fork();

	/* check if fork was successful */
	if (tracee < 0) {
		fprintf(stderr, "No process could be monitored. \n");
		exit(-1);
	}

	/* trace the child */
	if (tracee == 0) {
		/* to let the parent process to trace the child*/
		ptrace(PTRACE_TRACEME, tracee, 0, 0);

		/* stop the current process*/
		kill(getpid(), SIGSTOP);

		extern char **environ;

		execve(argv[1], argv1, environ);

		fprintf(stderr, "Unknown command %s\n", argv[1]);
		exit(1);
	}

}

/* intercept the system calls issued by the tracee process */
void intercept_calls()
{
	int status = -1, syscall_num = -1;

	/* wait for the child to stop */
	waitpid(tracee, &status, 0);

	ptrace(PTRACE_SETOPTIONS, tracee, 0,
			PTRACE_O_TRACESYSGOOD | PTRACE_O_TRACEEXIT);

	while (1) {

		/* to get every sysacall and be notified in the tracee stops */
		ptrace(PTRACE_SYSCALL, tracee, 0, 0);

		/* wait for other syscalls */
		waitpid(tracee, &status, 0);

		/* if tracee is terminated */
		if (WIFEXITED(status))
			break;

		if (!WIFSTOPPED(status)) {
			fprintf(stderr, "wait(&status)=%d\n", status);
			exit(status);
		}

		get_args(&regs);
		syscall_num = regs.syscall;

		if (WSTOPSIG(status) == (SIGTRAP | 0x80)) {
			if (monitor_actions[syscall_num] == DENY_LIND) {
				monitor_deny();
				break;
			} else if (monitor_actions[syscall_num] == ALLOW_OS) {

				switch (syscall_num) {

				case __NR_execve:
					monitor_execve();
					break;

				case __NR_tgkill:
					monitor_tgkill();
					break;

				case __NR_exit_group:
					monitor_exit_group();
					break;

				case __NR_exit:
					monitor_exit();
					break;

				case __NR_arch_prctl:
					monitor_arch_prctl();
					break;
				case __NR_mmap:
					monitor_mmap();
					break;

				case __NR_munmap:
					monitor_munmap();
					break;
				case __NR_mprotect:
					monitor_mprotect();
					break;

				case __NR_brk:
					monitor_brk();
					break;

				default:
					monitor_os();
					break;
				} /* switch*/

			} else if (monitor_actions[syscall_num] == ALLOW_LIND) {

				switch (syscall_num) {

				case __NR_getuid:
					monitor_getuid();
					break;

				case __NR_read:
					monitor_read();
					break;

				case __NR_open:
					monitor_open();
					break;

				case __NR_openat:
					monitor_openat();
					break;

				case __NR_access:
					monitor_access();
					break;

				case __NR_close:
					monitor_close();
					break;

				case __NR_rmdir:
					monitor_rmdir();
					break;

				case __NR_stat:
					monitor_stat();
					break;

				case __NR_statfs:
					monitor_statfs();
					break;

				case __NR_fstat:
					monitor_fstat();
					break;

				case __NR_fstatfs:
					monitor_fstatfs();
					break;

				case __NR_write:
					monitor_write();
					break;

				case __NR_mkdir:
					monitor_mkdir();
					break;

				case __NR_chdir:
					monitor_chdir();
					break;

				case __NR_getcwd:
					monitor_getcwd();
					break;

				case __NR_dup:
					monitor_dup();
					break;

				case __NR_dup2:
					monitor_dup2();
					break;

				case __NR_dup3:
					monitor_dup3();
					break;

				case __NR_getpid:
					monitor_getpid();
					break;

				case __NR_geteuid:
					monitor_geteuid();
					break;

				case __NR_getgid:
					monitor_getgid();
					break;

				case __NR_getegid:
					monitor_getegid();
					break;

				case __NR_unlink:
					monitor_unlink();
					break;

				case __NR_link:
					monitor_link();
					break;

				case __NR_fcntl:
					monitor_fcntl();
					break;

				case __NR_listen:
					monitor_listen();
					break;

				case __NR_shutdown:
					monitor_shutdown();
					break;

				case __NR_flock:
					monitor_flock();
					break;

				case __NR_getdents:
					monitor_getdents();
					break;

				case __NR_lseek:
					monitor_lseek();
					break;

				case __NR_pread64:
					monitor_pread64();
					break;

				case __NR_pwritev:
					monitor_pwritev();
					break;

				case __NR_socket:
					monitor_socket();
					break;

				case __NR_bind:
					monitor_bind();
					break;

				case __NR_connect:
					monitor_connect();
					break;

				case __NR_accept:
					monitor_accept();
					break;

				case __NR_sendto:
					monitor_sendto();
					break;

				case __NR_recvfrom:
					monitor_recvfrom();
					break;

				case __NR_recvmsg:
					monitor_recvmsg();
					break;

				case __NR_sendmsg:
					monitor_sendmsg();
					break;

				case __NR_getsockname:
					monitor_getsockname();
					break;

				case __NR_getsockopt:
					monitor_getsockopt();
					break;

				case __NR_setsockopt:
					monitor_setsockopt();
					break;

				case __NR_socketpair:
					monitor_socketpair();
					break;

				case __NR_getpeername:
					monitor_getpeername();
					break;

				case __NR_select:
					monitor_select();
					break;

				case __NR_poll:
					monitor_poll();
					break;

				case __NR_epoll_create:
					monitor_epoll_create();
					break;

				case __NR_epoll_ctl:
					monitor_epoll_ctl();
					break;

				case __NR_epoll_wait:
					monitor_epoll_wait();
					break;

				default:
					monitor_ns();
					break;
				} /* switch*/
			}
		} /* WSTOPSIG*/
	}/* while */
}

/* get the path of files required by a syscall through the defined address */
char *get_path(uintptr_t addr)
{
	size_t len = PATH_MAX;
	char *buffer = buffer = malloc(len);

	uint32_t tmp;
	int i = 0;

	while (1) {
		if (i >= len) {
			len *= 2;
			buffer = realloc(buffer, len);
		}

		tmp = ptrace(PTRACE_PEEKDATA, tracee, addr + i, NULL);
		memcpy(buffer + i, (void *) &tmp, sizeof(tmp));

		if (memchr(&tmp, 0, sizeof(tmp)) != NULL) {
			break;
		}

		i += 4;
	}

	return buffer;
}

/* set the memory from an address to a specific buffer */
void set_mem(uintptr_t addr, void * buff, size_t count)
{
	long ret = -1;
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
void *get_mem(uintptr_t addr, size_t count) {

	long ret;
	int i;
	long *mem = malloc((count / sizeof(long) + 1) * sizeof(long));

	for (i = 0; i < count / sizeof(long) + 1; i++) {
		ret = ptrace(PTRACE_PEEKDATA, tracee,
				(char *) (addr + sizeof(long) * i), 0);
		mem[i] = ret;
	}

	return (void*) mem;
}

/* return syscall number by name */
int get_syscall_num(char *name)
{
	int i;

	for (i = 0; i < TOTAL_SYSCALLS; i++)
		if (syscall_names[i] && !strcmp(syscall_names[i], name))
			return i;
	return -1;
}

/* return the arguments of a syscall by ptrace */
void get_args(struct syscall_args *args)
{

	if (ptrace(PTRACE_GETREGS, tracee, 0, &args->user) < 0) {
		fprintf(stderr, "ptrace could not get the register arguments. \n");
		return;
	}
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
void set_args(struct syscall_args *args)
{
	args->user.regs.orig_rax = args->syscall;
	args->user.regs.rax = args->retval;
	args->user.regs.rdi = args->arg1;
	args->user.regs.rsi = args->arg2;
	args->user.regs.rdx = args->arg3;
	args->user.regs.r10 = args->arg4;
	args->user.regs.r8 = args->arg5;
	args->user.regs.r9 = args->arg6;

	if (ptrace(PTRACE_SETREGS, tracee, 0, &args->user) < 0) {
		fprintf(stderr, "ptrace could not set registers . \n");
		return;
	}
}

/* load the config file containing the policies to dispatch the syscalls */
int load_config()
{
	char *str, buff[100];
	char *key, *value;
	enum monitor_action mact = ALLOW_LIND;
	const char * config_file = get_lind_config();

	FILE *fp = fopen(config_file, "r");
	if (fp == NULL) {
		fprintf(stdout, "[monitor] Config file %s could not be opened. \n ",
				config_file);
		exit(-1);
	}

	while ((str = fgets(buff, sizeof buff, fp)) != NULL) {

		if (buff[0] == '\n' || buff[0] == '#')
			continue;

		char* sep;
		if ((sep = strchr(str, '='))) {
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
