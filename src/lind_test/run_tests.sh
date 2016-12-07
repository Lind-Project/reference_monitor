#!/bin/bash

LIND_REPY_METADATA_DIR=$(pwd)

# compile C codes
function compile_tests() {
  testdir=$1
  cd $LIND_MONITOR/src/lind_test/$testdir
  rm *.nexe

  count=1
  for cFile in *.c;
  do
     basename=${cFile%.c}
     outputname="${basename}.nexe"

     echo "Compiling[$count] $cFile"
     $NACL_SDK_ROOT/toolchain/linux_x86_glibc/bin/x86_64-nacl-gcc -o $outputname $cFile
     #$NACL_SDK_ROOT/toolchain/linux_x86_glibc/bin/x86_64-nacl-gcc -Wall -g -I/usr/include/ -lpthread -o $outputname $cFile
     count=$((count+1))
  done
}


# runing python lind_fs_utils.py for every *.nexe file
function cp_nexe_files() {
  testdir=$1
  cd $REPY_PATH/repy
  if [ -f 'lind.metadata' ]; then
    rm lind.metadata
  fi
  cp ${LIND_REPY_METADATA_DIR}/lind.metadata $REPY_PATH/repy/lind.metadata

  for nexeFile in $LIND_MONITOR/src/lind_test/$testdir/*.nexe;
  do
     python lind_fs_utils.py cp $REPY_PATH/repy $nexeFile
  done
}


# run executable files
function run_nexe() {
  testdir=$1
  cd $REPY_PATH/repy
  for nexeFile in $LIND_MONITOR/src/lind_test/$testdir/*.nexe;
  do
     echo "RUNNING $nexeFile"
     ../bin/sel_ldr -a -s -- /glibc/runnable-ld.so --library-path /glibc $nexeFile
  done
}


# main function
function MAIN {
  # TODO later we have to add here 'for cicle' in order to run all test dyrectories.
  testdir="graphene"
  echo "Start Compiling"
  compile_tests $testdir
  echo "Start Copying"
  cp_nexe_files $testdir
  echo "Run Executables"
  run_nexe $testdir
}

MAIN

