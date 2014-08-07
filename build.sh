#!/bin/bash

clear

MAKEFILE_DIR=./src/main/
TESTCASES_DIR=./src/lind_test/
GEN_SYSCALLS=./src/main/lind_monitor/configs/syscall_generator


if [ -z "$GEN_SYSCALLS" ]; then
   echo "No syscall generator found"
   exit 1
fi


if [ -z "$MAKEFILE_DIR" ]; then
   echo "No make file found"
   exit 1
fi


$GEN_SYSCALLS > ./src/main/lind_monitor/configs/syscalls.h

cd $TESTCASES_DIR
make clean
make

cd ../../

cd $MAKEFILE_DIR

make clean
make
