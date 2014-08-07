/*
 * lind_platform.c
 *
 *  Created on: Jul 23, 2013
 *      Author: sji, Ali Gholami
 */
#include <Python.h>
#include <errno.h>
#include <stdarg.h>

#include "lind_platform.h"

PyObject* repylib = NULL;
PyObject* code = NULL;
PyObject* context = NULL;

static int initialized = 0;

#define UNREFERENCED_PARAMETER(x) do {(void)(x);} while(0)

#define GOTO_ERROR_IF_NULL(x) if(!(x)) {goto error;}

PyObject* CallPythonFunc(PyObject* context, const char* func, PyObject* args) {
	PyObject* func_obj = NULL;
	PyObject* result = NULL;
	func_obj = PyDict_GetItemString(context, func);
	GOTO_ERROR_IF_NULL(func_obj);
	GOTO_ERROR_IF_NULL(args);
	result = PyObject_CallObject(func_obj, args);
	GOTO_ERROR_IF_NULL(result);
	return result;
	error: PyErr_Print();
	Py_XDECREF(func_obj);
	return 0;
}

static PyObject* CallPythonFunc0(PyObject* context, const char* func) {
	PyObject* args = Py_BuildValue("()");
	return CallPythonFunc(context, func, args);
}

static PyObject* CallPythonFunc1(PyObject* context, const char* func,
		PyObject* arg) {
	PyObject* args = Py_BuildValue("(O)", arg);
	return CallPythonFunc(context, func, args);
}

int LindPythonInit(void) {
	PyObject* path = NULL;
	PyObject* repylib_name = NULL;
	PyObject* result = NULL;
	PyObject* repy_main_func = NULL;
	PyObject* repy_main_args = NULL;
	char* argv[] = { "dummy" };

	if (initialized++) {
		return 1;
	}
	Py_SetProgramName("dummy");
	PyEval_InitThreads();
	Py_InitializeEx(0);
	PySys_SetArgvEx(1, argv, 0);

	const char* repy_path = get_repy_path();
	const char *rest_lind = "restrictions.lind";
	char *repy_rest_path = malloc ((strlen (repy_path) + strlen(rest_lind) +1) * sizeof(char));
	strcpy(repy_rest_path, repy_path);
	strcat(repy_rest_path, rest_lind);


	path = PySys_GetObject("path");
	GOTO_ERROR_IF_NULL(path);
	PyList_Append(path, PyString_FromString(repy_path));

	repylib_name = PyString_FromString("repylib");
	repylib = PyImport_Import(repylib_name);
	GOTO_ERROR_IF_NULL(repylib);
	repy_main_func = PyObject_GetAttrString(repylib, "repy_main");
	GOTO_ERROR_IF_NULL(repy_main_func);

	const char *lind_server = "lind_server.py";
	char *lind_serv_path = malloc((strlen(repy_path) + strlen(lind_server)+1) * sizeof(char));
	strcpy(lind_serv_path, repy_path);
	strcat(lind_serv_path, lind_server);
	repy_main_args = Py_BuildValue("([sssss])", "lind", "--safebinary", repy_rest_path, lind_serv_path, "./dummy.nexe");


	result = PyObject_CallObject(repy_main_func, repy_main_args);

	free(repy_rest_path);
	free(lind_serv_path);
	GOTO_ERROR_IF_NULL(result);
	PyOS_AfterFork();
	PyArg_ParseTuple(result, "OO", &code, &context);
	GOTO_ERROR_IF_NULL(code && context);
	result = PyEval_EvalCode((PyCodeObject*) code, context, context);
	GOTO_ERROR_IF_NULL(result);
	PyEval_ReleaseLock();
	return 1;
	error: initialized = 0;
	PyErr_Print();
	PyEval_ReleaseLock();

	return 0;
}

int LindPythonFinalize(void) {
	int retval = 0;
	PyObject* repy_finalize_func = NULL;
	PyObject* repy_finalize_args = NULL;
	PyObject* result = NULL;
	PyGILState_STATE gstate;
	gstate = PyGILState_Ensure();
	if (!initialized) {
		return 0;
	}
	result = CallPythonFunc0(context, "finalize");
	GOTO_ERROR_IF_NULL(result);
	repy_finalize_func = PyObject_GetAttrString(repylib, "finalize");
	GOTO_ERROR_IF_NULL(repy_finalize_func);
	repy_finalize_args = Py_BuildValue("()");
	result = PyObject_CallObject(repy_finalize_func, repy_finalize_args);
	GOTO_ERROR_IF_NULL(result);
	Py_Finalize();
	initialized = 0;
	retval = 1;
	goto cleanup;
	error: PyErr_Print();
	cleanup: Py_XDECREF(repy_finalize_func);
	Py_XDECREF(result);
	Py_XDECREF(code);
	Py_XDECREF(context);
	Py_XDECREF(repylib);
	PyGILState_Release(gstate);
	return retval;
}

int GetHostFdFromLindFd(int lindFd) {
	int retval = -1;
	PyObject* pyLindFd = NULL;
	PyObject* pyHostFd = NULL;
	PyGILState_STATE gstate;
	gstate = PyGILState_Ensure();
	if (lindFd < 0) {
		goto cleanup;
	}
	pyLindFd = PyInt_FromLong(lindFd);
	pyHostFd = CallPythonFunc1(context, "GetHostFdFromLindFd", pyLindFd);
	GOTO_ERROR_IF_NULL(pyHostFd);
	if (!PyInt_CheckExact(pyHostFd)) {
		goto error;
	}
	retval = (int) PyInt_AsLong(pyHostFd);
	goto cleanup;
	error: PyErr_Print();
	cleanup: Py_XDECREF(pyLindFd);
	Py_XDECREF(pyHostFd);
	PyGILState_Release(gstate);
	return retval;
}

void CopyData(char* dst, char* src, int maxlen, int srclen) {
	assert(maxlen >= srclen);
	memcpy(dst, src, srclen);
}

void CopyMultiDataVa(char* src, int num, va_list varg) {
	int offset;
	char* dst;
	int maxlen;
	int srclen;

	offset = sizeof(uint32_t) * num;
	for (int i = 0; i < num; ++i) {
		dst = va_arg(varg, char*);
		maxlen = va_arg(varg, int);
		srclen = ((uint32_t*) src)[i];
		CopyData(dst, src + offset, maxlen, srclen);
		offset += srclen;
	}
}

void CopyMultiData(char* src, int num, ...) {
	va_list varg;

	va_start(varg, num);
	CopyMultiDataVa(src, num, varg);
	va_end(varg);
}

int ParseResponse(PyObject* response, int num, ...) {
	int retval = 0;
	PyObject* attrIsError = NULL;
	PyObject* attrCode = NULL;
	PyObject* attrDataOrMessage = NULL;
	int isError;
	int code;
	char* dataOrMessage;
	int len;
	va_list varg;
	PyGILState_STATE gstate;
	char* dst;
	int maxlen;


	gstate = PyGILState_Ensure();

	attrIsError = PyObject_GetAttrString(response, "is_error");
	GOTO_ERROR_IF_NULL(attrIsError);

	attrCode = PyObject_GetAttrString(response, "return_code");
	GOTO_ERROR_IF_NULL(attrCode);

	dataOrMessage = NULL;
	len = 0;

	if (attrIsError == Py_True) {
		isError = 1;
		attrDataOrMessage = PyObject_GetAttrString(response, "message");
		GOTO_ERROR_IF_NULL(attrDataOrMessage);
	} else {
		isError = 0;
		attrDataOrMessage = PyObject_GetAttrString(response, "data");
	}

	code = PyInt_AsLong(attrCode);
	if (PyErr_Occurred()) {
		goto error;
	}

	if (attrDataOrMessage) {
		len = (int) PyString_Size(attrDataOrMessage);
		if (PyErr_Occurred()) {
			goto error;
		}
		dataOrMessage = PyString_AsString(attrDataOrMessage);
		if (PyErr_Occurred()) {
			goto error;
		}
	}
	if (isError) {
	}
	errno = isError ? code : 0;
	retval = isError ? -1 : code;
	if (isError) {
		goto cleanup;
	}
	va_start(varg, num);

	if (num == 1) {
		dst = va_arg(varg, char*);
		maxlen = va_arg(varg, int);
		CopyData(dst, dataOrMessage, maxlen, len);
	} else if (num > 1) {
		CopyMultiDataVa(dataOrMessage, len, varg);
	}

	va_end(varg);
	goto cleanup;
	error:
	PyErr_Print();
	cleanup: Py_XDECREF(attrIsError);
	Py_XDECREF(attrCode);
	Py_XDECREF(attrDataOrMessage);
	Py_XDECREF(response);
	PyGILState_Release(gstate);
	return retval;
}

PyObject* MakeLindSysCall(int syscall, char* format, ...) {
	PyObject* callandarg = NULL;
	PyObject* response = NULL;
	PyObject* args = NULL;
	PyGILState_STATE gstate;
	va_list varg;

	gstate = PyGILState_Ensure();
	va_start(varg, format);
	args = Py_VaBuildValue(format, varg);
	va_end(varg);
	callandarg = Py_BuildValue("(iO)", syscall, args);
	response = CallPythonFunc(context, "LindSyscall", callandarg);
	Py_XDECREF(callandarg);
	Py_XDECREF(args);
	PyGILState_Release(gstate);
	return response;
}

#define DUMP_DATA(x) printf(#x" = 0x%"NACL_PRIX64"\n", (uint64_t)(x));

#if 0
#define DUMP_STAT(x) \
        DUMP_DATA((x)->st_dev); \
        DUMP_DATA((x)->st_ino); \
        DUMP_DATA((x)->st_nlink); \
        DUMP_DATA((x)->st_mode); \
        DUMP_DATA(S_ISREG((x)->st_mode)); \
        DUMP_DATA(S_ISDIR((x)->st_mode)); \
        DUMP_DATA(S_ISCHR((x)->st_mode)); \
        DUMP_DATA(S_ISBLK((x)->st_mode)); \
        DUMP_DATA(S_ISFIFO((x)->st_mode)); \
        DUMP_DATA(S_ISLNK((x)->st_mode)); \
        DUMP_DATA(S_ISSOCK((x)->st_mode)); \
        DUMP_DATA((x)->st_uid); \
        DUMP_DATA((x)->st_gid); \
        DUMP_DATA((x)->st_rdev); \
        DUMP_DATA((x)->st_size); \
        DUMP_DATA((x)->st_blksize); \
        DUMP_DATA((x)->st_blocks)
#else
#define DUMP_STAT(x)
#endif


int lind_access(const char *pathname, int mode) {
	UNREFERENCED_PARAMETER(mode);
	return ParseResponse(
			MakeLindSysCall(LIND_safe_fs_access, "[si]", pathname, mode), 0);
}

int lind_unlink(const char *name) {
	return ParseResponse(MakeLindSysCall(LIND_safe_fs_unlink, "[s]", name), 0);
}

int lind_link(const char *from, const char *to) {
	return ParseResponse(MakeLindSysCall(LIND_safe_fs_unlink, "[ss]", from, to),
			0);
}

int lind_chdir(const char *name) {
	return ParseResponse(MakeLindSysCall(LIND_safe_fs_chdir, "[s]", name), 0);
}

int lind_mkdir(const char *path, int mode) {
	return ParseResponse(
			MakeLindSysCall(LIND_safe_fs_mkdir, "[is]", mode, path), 0);
}

int lind_rmdir(const char *path) {
	return ParseResponse(MakeLindSysCall(LIND_safe_fs_rmdir, "[s]", path), 0);
}

int lind_stat(const char *path, struct lind_stat *buf) {
	int version = 1;
	return ParseResponse(
			MakeLindSysCall(LIND_safe_fs_xstat, "[is]", version, path), 1, buf,
			sizeof(*buf));
}

int lind_open(const char *path, int flags, int mode) {
	return ParseResponse(
			MakeLindSysCall(LIND_safe_fs_open, "[sii]", path, flags, mode), 0);
}

int lind_openat (int dir_fd, const char *path, int flags, int mode){

	return ParseResponse(
			MakeLindSysCall(LIND_safe_fs_openat, "[isii]", dir_fd, path, flags, mode), 0);
}

int lind_close(int fd) {
	return ParseResponse(MakeLindSysCall(LIND_safe_fs_close, "[i]", fd), 0);
}

ssize_t lind_read(int fd, void *buf, size_t size) {
	return ParseResponse(MakeLindSysCall(LIND_safe_fs_read, "[ii]", fd, size),
			1, buf, (int) size);
}

ssize_t lind_write(int fd, const void *buf, size_t count) {
	return ParseResponse(
			MakeLindSysCall(LIND_safe_fs_write, "[is#]", fd, buf, count), 0);
}

off_t lind_lseek(int fd, off_t offset, int whence) {
	int retval;
	off_t ret_off;
	retval = ParseResponse(
			MakeLindSysCall(LIND_safe_fs_read, "[iii]", offset, fd, whence), 1,
			&ret_off, sizeof(ret_off));
	if (retval < 0) {
		return retval;
	}
	return ret_off;
}

int lind_fstat(int fd, struct lind_stat *buf) {
	int version = 1;
	return ParseResponse(
			MakeLindSysCall(LIND_safe_fs_fxstat, "[ii]", fd, version), 1, buf,
			sizeof(*buf));
}

int lind_fstatfs(int fd, struct lind_statfs *buf) {
	return ParseResponse(MakeLindSysCall(LIND_safe_fs_fstatfs, "[i]", fd), 1,
			buf, sizeof(*buf));
}

int lind_statfs(const char *path, struct lind_statfs *buf) {
	return ParseResponse(MakeLindSysCall(LIND_safe_fs_statfs, "[s]", path), 1,
			buf, sizeof(*buf));
}

int lind_noop(void) {
	return ParseResponse(MakeLindSysCall(LIND_debug_noop, "[]"), 0);
}

int lind_getpid(void) {
	return ParseResponse(MakeLindSysCall(LIND_sys_getpid, "[]"), 0);
}

int lind_dup(int oldfd) {
	return ParseResponse(MakeLindSysCall(LIND_safe_fs_dup, "[i]", oldfd), 0);
}

int lind_dup2(int oldfd, int newfd) {
	return ParseResponse(
			MakeLindSysCall(LIND_safe_fs_dup2, "[ii]", oldfd, newfd), 0);
}

int lind_dup3(int oldfd, int newfd, int flags) {
	return ParseResponse(
			MakeLindSysCall(LIND_safe_fs_dup3, "[iii]", oldfd, newfd, flags), 0);
}


int lind_getdents(unsigned int fd, char *dirp, unsigned int count) {
	return ParseResponse(
			MakeLindSysCall(LIND_safe_fs_getdents, "[ii]", fd, count), 1, dirp,
			count);
}

int _lind_fcntl_get(int fd, int cmd) {
	return ParseResponse(MakeLindSysCall(LIND_safe_fs_fcntl, "[ii]", fd, cmd),
			0);
}

int _lind_fcntl_set(int fd, int cmd, long set_op) {
	return ParseResponse(
			MakeLindSysCall(LIND_safe_fs_fcntl, "[iil]", fd, cmd, set_op), 0);
}

int lind_socket(int domain, int type, int protocol) {
	return ParseResponse(
			MakeLindSysCall(LIND_safe_net_socket, "[iii]", domain, type,
					protocol), 0);
}

int lind_bind(int sockfd, const struct lind_sockaddr *addr,
		lind_socklen_t addrlen) {
	return ParseResponse(
			MakeLindSysCall(LIND_safe_net_bind, "[iis#]", sockfd, addrlen, addr), 0);
}

ssize_t lind_send(int sockfd, const void *buf, size_t len, int flags) {
	return ParseResponse(
			MakeLindSysCall(LIND_safe_net_send, "[iiis#]", sockfd, len, flags,
					buf, len), 0);
}

ssize_t lind_recv(int sockfd, void *buf, size_t len, int flags) {
	return ParseResponse(
			MakeLindSysCall(LIND_safe_net_recv, "[iii]", sockfd, len, flags), 1,
			buf, len);
}

int lind_connect(int sockfd, const struct lind_sockaddr *src_addr,
		lind_socklen_t addrlen) {
	return ParseResponse(
			MakeLindSysCall(LIND_safe_net_recv, "[iis#]", sockfd, addrlen,
					src_addr, addrlen), 0);
}

int lind_listen(int sockfd, int backlog) {
	return ParseResponse(
			MakeLindSysCall(LIND_safe_net_listen, "[ii]", sockfd, backlog), 0);
}

ssize_t lind_sendto(int sockfd, const void *buf, size_t len, int flags,
		const struct lind_sockaddr *dest_addr, lind_socklen_t addrlen) {
	UNREFERENCED_PARAMETER(flags);
	return ParseResponse(
			MakeLindSysCall(LIND_safe_net_sendto, "[iiiis#s#]", sockfd, len,
					addrlen, dest_addr, addrlen, buf, len), 0);
}

int lind_accept(int sockfd, const struct lind_sockaddr *addr,
		lind_socklen_t* addrlen) {
	UNREFERENCED_PARAMETER(addr);
	return ParseResponse(
			MakeLindSysCall(LIND_safe_net_accept, "[ii]", sockfd, *addrlen), 0);
}

int lind_getpeername(int sockfd, struct lind_sockaddr *addr,
		lind_socklen_t* addrlen) {
	return ParseResponse(
			MakeLindSysCall(LIND_safe_net_getpeername, "[ii]", sockfd,
					*addrlen), 1, addr, *addrlen);
}

int lind_getsockname(int sockfd, struct lind_sockaddr *addr,
		lind_socklen_t *addrlen) {
	return ParseResponse(
			MakeLindSysCall(LIND_safe_net_getsockname, "[ii]", sockfd,
					*addrlen), 1, addr, *addrlen);
}

int lind_setsockopt(int sockfd, int level, int optname, const void *optval,
		lind_socklen_t optlen) {
	return ParseResponse(
			MakeLindSysCall(LIND_safe_net_setsockopt, "[iiis#]", sockfd, level,
					optname, optval, optlen), 0);
}

int lind_getsockopt(int sockfd, int level, int optname, void *optval,
		lind_socklen_t* optlen) {
	return ParseResponse(
			MakeLindSysCall(LIND_safe_net_getsockopt, "[iii]", sockfd, level,
					optname), 1, optval, optlen);
}

int lind_shutdown(int sockfd, int how) {
	return ParseResponse(
			MakeLindSysCall(LIND_safe_net_shutdown, "[ii]", sockfd, how), 0);
}

int lind_select(int nfds, fd_set * readfds, fd_set * writefds,
		fd_set * exceptfds, struct timeval *timeout) {
	struct select_results {
		struct timeval used_t;
		fd_set r;
		fd_set w;
		fd_set e;
	} result;
	PyObject* readFdObj = NULL;
	PyObject* writeFdObj = NULL;
	PyObject* exceptFdObj = NULL;
	PyObject* timeValObj = NULL;
	int retval;
	if (readfds) {
		readFdObj = PyString_FromStringAndSize((char*) readfds, sizeof(fd_set));
	} else {
		readFdObj = Py_None;
		Py_INCREF(readFdObj);
	}
	if (writefds) {
		writeFdObj = PyString_FromStringAndSize((char*) writefds,
				sizeof(fd_set));
	} else {
		writeFdObj = Py_None;
		Py_INCREF(writeFdObj);
	}
	if (exceptfds) {
		exceptFdObj = PyString_FromStringAndSize((char*) exceptfds,
				sizeof(fd_set));
	} else {
		exceptFdObj = Py_None;
		Py_INCREF(exceptFdObj);
	}
	if (timeout) {
		timeValObj = PyString_FromStringAndSize((char*) timeout,
				sizeof(struct timeval));
	} else {
		timeValObj = Py_None;
		Py_INCREF(timeValObj);
	}
	retval = ParseResponse(
			MakeLindSysCall(LIND_safe_net_select, "[iOOOO]", nfds, readFdObj,
					writeFdObj, exceptFdObj, timeValObj), 1, &result,
			sizeof(result));
	Py_XDECREF(readFdObj);
	Py_XDECREF(writeFdObj);
	Py_XDECREF(exceptFdObj);
	Py_XDECREF(timeValObj);
	if (retval < 0) {
		return retval;
	}
	if (readfds) {
		*readfds = result.r;
	}
	if (writefds) {
		*writefds = result.w;
	}
	if (exceptfds) {
		*exceptfds = result.e;
	}
	if (timeout) {
		*timeout = result.used_t;
	}
	return retval;
}

ssize_t lind_pread(int fd, void* buf, int count, off_t offset)
{
	off_t cur_pos = 0;
	int ret = 0;
	cur_pos = lind_lseek(fd, 0, SEEK_CUR);
	lind_lseek(fd, offset, SEEK_SET);
	ret = lind_read(fd, buf, count);
	lind_lseek(fd, cur_pos, SEEK_SET);
	return ret;
}

ssize_t lind_pwrite(int fd, const void *buf, int count, off_t offset) {
	off_t cur_pos = 0;
	int ret = 0;
	cur_pos = lind_lseek(fd, 0, SEEK_CUR);
	lind_lseek(fd, offset, SEEK_SET);
	ret = lind_write(fd, buf, count);
	lind_lseek(fd, cur_pos, SEEK_SET);
	return ret;
}

int lind_getifaddrs(int ifaddrs_buf_siz, void *ifaddrs) {
	return ParseResponse(
			MakeLindSysCall(LIND_safe_net_getifaddrs, "[i]", ifaddrs_buf_siz),
			1, ifaddrs, ifaddrs_buf_siz);
}

ssize_t lind_recvfrom(int sockfd, void *buf, size_t len, int flags,
		struct lind_sockaddr *src_addr, lind_socklen_t * addrlen) {
	return ParseResponse(
			MakeLindSysCall(LIND_safe_net_recvfrom, "[iiii]", sockfd, len,
					flags, addrlen), 3, addrlen, sizeof(*addrlen), buf, len,
			src_addr, sizeof(*src_addr));
}

int lind_poll(struct lind_pollfd *fds, lind_nfds_t nfds, int timeout) {
	return ParseResponse(
			MakeLindSysCall(LIND_safe_net_poll, "[iis#]", nfds, timeout, fds,
					sizeof(struct pollfd) * nfds), 1, fds,
			sizeof(struct pollfd) * nfds);
}

int lind_socketpair(int domain, int type, int protocol, int *fds) {
	return ParseResponse(
			MakeLindSysCall(LIND_safe_net_socketpair, "[iii]", domain, type,
					protocol), 1, fds, sizeof(int) * 2);
}

int lind_getuid(void) {
	return ParseResponse(MakeLindSysCall(LIND_safe_sys_getuid, "[]"), 0);
}

int lind_geteuid(void) {
	return ParseResponse(MakeLindSysCall(LIND_safe_sys_getegid, "[]"),0);
}

int lind_getgid(void){
	return ParseResponse(MakeLindSysCall(LIND_safe_sys_getgid, "[]"), 0);
}

int lind_getegid(void) {
	return ParseResponse(MakeLindSysCall(LIND_safe_sys_getegid, "[]"), 0);
}

int lind_flock(int fd, int operation) {
	return ParseResponse(
			MakeLindSysCall(LIND_safe_fs_flock, "[ii]", fd, operation), 0);
}

char* lind_getcwd(char* buf, size_t size) {
	UNREFERENCED_PARAMETER(buf);
	UNREFERENCED_PARAMETER(size);
	return NULL;
}

/**
 * This function packs the msg_iovec to a string and passes it to be handled by Repy.
 */
ssize_t lind_sendmsg(int sockfd, const struct lind_msghdr *msg, int flags) {

	/* Strip the msg->iov vector and extract each individual vector to be joined in a string. */
	unsigned int i = 0;
	int total = 0; /* Total length of all iovec elements. */
	int * lengths; /* Length of individual iovec elements. */
	char * concatenated;
	char * final_message;

	lengths = malloc(msg->msg_iov->iov_len * sizeof(int)); /* Memory allocation to store the lengths of the individual elements. */

	concatenated = malloc(total + 1);

	for (i = 0; i < msg->msg_iov->iov_len; i++) {
		lengths[i] = msg->msg_iov[i].iov_len;
		total += lengths[i];
	}

	concatenated = malloc(total); /* Memory allocation for the concatenated strings. */

	final_message = concatenated;

	for (i = 0; i < msg->msg_iov->iov_len; i++) {
		int j;
		for (j = 0; j < lengths[i]; j++) {
			memcpy(final_message + j, msg->msg_iov[i].iov_base,
					msg->msg_iov[i].iov_len);

		}

		final_message += lengths[i];

	}

	free(lengths);
	free(concatenated);

	return ParseResponse(
			MakeLindSysCall(LIND_safe_net_sendmsg, "[is#s#is#ii]", sockfd,
					msg->msg_name, msg->msg_namelen, final_message,
					msg->msg_iovlen, msg->msg_control, msg->msg_controllen,
					msg->msg_flags, flags), 0);
}

/**
 * This function delivers the received message from a socket to Repy.
 */
ssize_t lind_recvmsg(int sockfd, struct lind_msghdr *msg, int flags) {

	/* Strip the msg->iov vector and extract each individual vector to be joined in a string. */
	unsigned int i = 0;
	int total = 0; /* Total length of all iovec elements. */
	int * lengths; /* Length of individual iovec elements. */
	char * concatenated;
	char * final_message;

	lengths = malloc(msg->msg_iov->iov_len * sizeof(int)); /* Memory allocation to store the lengths of individual elements. */

	concatenated = malloc(total + 1);

	for (i = 0; i < msg->msg_iov->iov_len; i++) {
		lengths[i] = msg->msg_iov[i].iov_len;
		total += lengths[i];
	}

	concatenated = malloc(total); /* Memory allocation for the concatenated strings. */

	final_message = concatenated;

	for (i = 0; i < msg->msg_iov->iov_len; i++) {
		int j;
		for (j = 0; j < lengths[i]; j++) {
			memcpy(final_message + j, msg->msg_iov[i].iov_base,
					msg->msg_iov[i].iov_len);
		}

		final_message += lengths[i];
	}

	free(lengths);
	free(concatenated);

	return ParseResponse(
			MakeLindSysCall(LIND_safe_net_recvmsg, "[is#s#is#ii]", sockfd,
					msg->msg_name, msg->msg_namelen, final_message,
					msg->msg_iovlen, msg->msg_control, msg->msg_controllen,
					msg->msg_flags, flags), 0);

}

int lind_epoll_create(int size) {
	UNREFERENCED_PARAMETER(size);
	return 0;
}

int lind_epoll_ctl(int epfd, int op, int fd, struct lind_epoll_event *event) {
	UNREFERENCED_PARAMETER(epfd);
	UNREFERENCED_PARAMETER(op);
	UNREFERENCED_PARAMETER(fd);
	UNREFERENCED_PARAMETER(event);
	return 0;
}

int lind_epoll_wait(int epfd, struct lind_epoll_event *events, int maxevents,
		int timeout) {
	UNREFERENCED_PARAMETER(epfd);
	UNREFERENCED_PARAMETER(events);
	UNREFERENCED_PARAMETER(maxevents);
	UNREFERENCED_PARAMETER(timeout);
	return 0;
}


int lind_fcntl(int fd, int cmd, ...) {
	UNREFERENCED_PARAMETER(fd);
	UNREFERENCED_PARAMETER(cmd);
	return 0;
}


void add_mapping(int src, int dest){
	PyObject* args = NULL;
	PyObject* result = NULL;

	if (src < 0 || dest < 0) {
		return;
	}

	args = Py_BuildValue("(ii)", src, dest);
	result = CallPythonFunc(context, "map_native_to_lind", args);

	if (result == NULL){
		fprintf(stderr, "Error while building the arguments src: %d  dest: %d\n", src, dest);
	}
	Py_XDECREF(args);
	Py_XDECREF(result);
}

int get_mapping(int fd){



	PyObject* args = NULL;
	PyObject* result = NULL;
	int retval = -1;
	args = Py_BuildValue("(i)", fd);

	result = CallPythonFunc(context, "get_lind_map_to_native", args);

	if (result == NULL){
		fprintf(stderr, "No mapping found in Lind for fd %d \n", fd);
	}

	if (!PyInt_Check(result)){
		fprintf(stderr, "Results is not integer. \n");
	} else {
	    retval= PyInt_AS_LONG(result);
	}

	Py_XDECREF(args);
	Py_XDECREF(result);

	return retval;
}

const char *get_repy_path()
{
	const char *name = "REPY_PATH";
	const char *rel_path = "/repy/";

	char *path = getenv(name);

	if (path == NULL) {
		fprintf(stderr, "Could not read %s. \n", path);
		exit(-1);
	}

	char *monitor_home = (char *) malloc((strlen(path) + strlen(rel_path) +1) * sizeof(char));

	strcpy(monitor_home, path);
	strcat(monitor_home, rel_path);

	return monitor_home;
}
