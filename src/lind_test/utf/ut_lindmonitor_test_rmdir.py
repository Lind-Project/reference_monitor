import os

os.mkdir('/tmp/foodir')
os.rmdir('/tmp/foodir')

try:
  os.rmdir('/tmp/foodir')
  print "this si sa  a est"
except OSError:
  # expected error for dir does not exist
  pass
else:
  print 'error, directory did not exist, but error was not raised...'
  
