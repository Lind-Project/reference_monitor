#!/bin/bash

MAKE_FILE=./build/
GEN_SYSCALLS=./configs/syscall_generator

if [ -z "$GEN_SYSCALLS" ]; then
   echo "No syscall generator found"
   exit 1
fi


if [ -z "$MAKE_FILE" ]; then
   echo "No make file found"
   exit 1
fi

$GEN_SYSCALLS > configs/syscalls.h
cd $MAKE_FILE
make clean
make