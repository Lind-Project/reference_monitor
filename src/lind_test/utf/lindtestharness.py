"""
   Author: Justin Cappos
           Ali Gholami

   Purpose:
   
     Run tests for Lind.  This will create a clean directory structure for
     every test by copying files from ...    The test will fail if it produces
     any output on stderr or returns a non-zero status.   If this does not
     occur, the test passes.

     It will run using either the tests passed in on the command line or by
     running all test files starting with lindtest_
   
     Note that this program does not recompile the code from source.   This
     must be done via a Makefile or similar.
   
   Usage:
     ...
"""

import glob
import shutil
import sys
import os
import subprocess

CUR_DIR = os.getcwd()
TESTDIRECTORY =  CUR_DIR + '/testingdir/'
REFERENCEFSDIRECTORY = '/usr/ali/repy/repy/'
EXEC_DIR = CUR_DIR + '/build/testcases/'

def remove_existing_lindfs():
  # this may fail if either doesn't exist.   Always try to do both though.
  # raise errors up to the caller.   Sometimes they will want to know if the
  # files do not exist...
  try:
    shutil.rmtree(TESTDIRECTORY)
    os.mkdir(TESTDIRECTORY)
  except OSError, err:
    print err 
      
def setup_lind_fs(): 
  if not os.path.exists(TESTDIRECTORY): 
    os.makedirs(TESTDIRECTORY)
    
  lindata_files = glob.glob(REFERENCEFSDIRECTORY + '/linddata.*')
     
  for testfilename in lindata_files:
    shutil.copy(testfilename , TESTDIRECTORY)
         
  shutil.copy(REFERENCEFSDIRECTORY + '/lind.metadata', TESTDIRECTORY)


def main():
  # If there are command line arguments, use them as the testcases to run
  if sys.argv[1:]:
    testcases = sys.argv[1:]
  else:
    testcases = glob.glob(EXEC_DIR +'test_*')

  try:
    remove_existing_lindfs()
  except OSError:
    # This is expected if the fs is clean...
    pass   

  for testfilename in testcases:
    ###### Need to set up the file system...
    # this should load a clean reference file system for each test run
    setup_lind_fs()
        
    print 'Running %40s [' % (testfilename),
    # run the test
    process = subprocess.Popen(testfilename, stdout = subprocess.PIPE, stderr = subprocess.PIPE)

    # capture the stderr and stdout
    (stdoutdata, stderrdata) = process.communicate()

    if stderrdata != '':
      print 'FAIL ]'
  
    else:
      print 'PASS ]'

    # Write out the out/errput so that the programmer can figure out what went
    # wrong if there is a problem...
    if stdoutdata: print 'stdout:', stdoutdata
    if stderrdata: print 'stderr:', stderrdata


    # last thing for this test, clean up the file system...
    remove_existing_lindfs()

if __name__ == '__main__':
  main()