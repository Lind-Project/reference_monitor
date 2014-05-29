#!/bin/bash

MAKE_FILE=./build/
GEN_SYSCALLS=./configs/syscall_generator

if [ -z "$GEN_SYSCALLS" ]; then
   echo "No syscall generator not found"
   exit 1
fi


if [ -z "$MAKE_FILE" ]; then
   echo "No make file not found"
   exit 1
fi

$GEN_SYSCALLS > configs/syscalls.h
cd $MAKE_FILE
make clean
make