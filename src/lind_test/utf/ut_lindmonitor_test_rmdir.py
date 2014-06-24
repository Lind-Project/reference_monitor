import os

os.mkdir('/tmp/foodir')
os.rmdir('/tmp/foodir')

try:
  os.rmdir('/tmp/foodir')
except OSError:
  # expected error for dir does not exist
  pass
else:
  print 'error, directory did not exist, but error was not raised...'
  
