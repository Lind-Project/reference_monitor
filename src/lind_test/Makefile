CC = gcc
CFLAGS = -g -Wall -pthread
BUILD_DIR= utf/build
HEADERS = testcases/testcases.h

LIBS=/usr/include
        
all: test_getuid \
	 test_getgid \
	 test_geteuid \
	 test_getegid \
	 test_getpid \
	 test_mkdir \
	 test_rmdir \
	 test_sleep \
	 test_read \
	 test_write \
	 test_rename \
	 test_open \
	 test_close \
	 test_socket \
	 test_stat \
	 test_fstat \
	 test_lstat \
	 test_statfs \
	 test_fstatfs \
	 test_access \
	 test_connect \
	 test_bind \
	 test_accept \
	 test_select \
	 test_lseek \
	 test_link \
	 test_unlink \
	 test_exit \
	 test_shutdown \
	 test_date \
	 test_chown \
	 test_listen \
	 test_getsockopt \
	 test_getsockname \
	 test_dup \
	 test_dup2

#test_curl

test_getuid:	
	${CC} ${CFLAGS} $(HEADERS) testcases/test_getuid.c -o $(BUILD_DIR)/test_getuid

test_getgid:
	${CC} ${CFLAGS} $(HEADERS) testcases/test_getgid.c -o $(BUILD_DIR)/test_getgid

test_geteuid:	
	${CC} ${CFLAGS} $(HEADERS) testcases/test_geteuid.c -o $(BUILD_DIR)/test_geteuid

test_getegid:
	${CC} ${CFLAGS} $(HEADERS) testcases/test_getegid.c -o $(BUILD_DIR)/test_getegid

test_getpid:
	${CC} ${CFLAGS} $(HEADERS) testcases/test_getpid.c -o $(BUILD_DIR)/test_getpid

test_date:
	${CC} ${CFLAGS} $(HEADERS) testcases/test_date.c -o $(BUILD_DIR)/test_date

test_chown:
	${CC} ${CFLAGS} $(HEADERS) testcases/test_chown.c -o $(BUILD_DIR)/test_chown

test_mkdir:
	${CC} ${CFLAGS} $(HEADERS) testcases/test_mkdir.c -o $(BUILD_DIR)/test_mkdir

test_rmdir:
	${CC} ${CFLAGS} $(HEADERS) testcases/test_rmdir.c -o $(BUILD_DIR)/test_rmdir

test_sleep:
	${CC} ${CFLAGS} $(HEADERS) testcases/test_sleep.c -o $(BUILD_DIR)/test_sleep

test_read:
	${CC} ${CFLAGS} $(HEADERS) testcases/test_read.c -o $(BUILD_DIR)/test_read

test_write:
	${CC} ${CFLAGS} $(HEADERS) testcases/test_write.c -o $(BUILD_DIR)/test_write

test_open:
	${CC} ${CFLAGS} $(HEADERS) testcases/test_open.c -o $(BUILD_DIR)/test_open

test_close:
	${CC} ${CFLAGS} $(HEADERS) testcases/test_close.c -o $(BUILD_DIR)/test_close

test_curl:
	${CC} ${CFLAGS} $(HEADERS) testcases/test_curl.c -o $(BUILD_DIR)/test_curl -lcurl

test_socket:
	${CC} ${CFLAGS} $(HEADERS) testcases/test_socket.c -o $(BUILD_DIR)/test_socket

test_stat:
	${CC} ${CFLAGS} $(HEADERS) testcases/test_stat.c -o $(BUILD_DIR)/test_stat

test_fstat:
	${CC} ${CFLAGS} $(HEADERS) testcases/test_fstat.c -o $(BUILD_DIR)/test_fstat

test_lstat:
	${CC} ${CFLAGS} $(HEADERS) testcases/test_lstat.c -o $(BUILD_DIR)/test_lstat

test_statfs:
	${CC} ${CFLAGS} $(HEADERS) testcases/test_statfs.c -o $(BUILD_DIR)/test_statfs
 
test_fstatfs:
	${CC} ${CFLAGS} $(HEADERS) testcases/test_fstatfs.c -o $(BUILD_DIR)/test_fstatfs

test_access:
	${CC} ${CFLAGS} $(HEADERS) testcases/test_access.c -o $(BUILD_DIR)/test_access
   
test_connect:
	${CC} ${CFLAGS} $(HEADERS) testcases/test_connect.c -o $(BUILD_DIR)/test_connect
  
test_bind:
	${CC} ${CFLAGS} $(HEADERS) testcases/test_bind.c -o $(BUILD_DIR)/test_bind
  
test_accept:
	${CC} ${CFLAGS} $(HEADERS) testcases/test_accept.c -o $(BUILD_DIR)/test_accept
  
test_select:
	${CC} ${CFLAGS} $(HEADERS) testcases/test_select.c -o $(BUILD_DIR)/test_select

test_lseek:
	${CC} ${CFLAGS} $(HEADERS) testcases/test_lseek.c -o $(BUILD_DIR)/test_lseek

test_link:
	${CC} ${CFLAGS} $(HEADERS) testcases/test_link.c -o $(BUILD_DIR)/test_link

test_unlink:
	${CC} ${CFLAGS} $(HEADERS) testcases/test_unlink.c -o $(BUILD_DIR)/test_unlink
    
test_exit:
	${CC} ${CFLAGS} $(HEADERS) testcases/test_exit.c -o $(BUILD_DIR)/test_exit
    
test_shutdown:
	${CC} ${CFLAGS} $(HEADERS) testcases/test_shutdown.c -o $(BUILD_DIR)/test_shutdown
           
test_getsockname:
	${CC} ${CFLAGS} $(HEADERS) testcases/test_getsockname.c -o $(BUILD_DIR)/test_getsockname

test_getsockopt:
	${CC} ${CFLAGS} $(HEADERS) testcases/test_getsockopt.c -o $(BUILD_DIR)/test_getsockopt

test_listen:
	${CC} ${CFLAGS} $(HEADERS) testcases/test_listen.c  -o $(BUILD_DIR)/test_listen

test_rename:
	${CC} ${CFLAGS} $(HEADERS) testcases/test_rename.c -o $(BUILD_DIR)/test_rename

test_dup:
	${CC} ${CFLAGS} $(HEADERS) testcases/test_dup.c -o $(BUILD_DIR)/test_dup


test_dup2:
	${CC} ${CFLAGS} $(HEADERS) testcases/test_dup2.c -o $(BUILD_DIR)/test_dup2


.phony: all

clean:
	@- $(RM) $(BUILD_DIR)/*
