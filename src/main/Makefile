INCLUDE = /usr/include
CC = gcc
CFLAGS = -g -Wall -O3 $(IFLAGS)  `python-config --includes` --std=gnu99
LFLAGS = -lpthread  `python-config --libs`
IFLAGS = -I$(INCLUDE) -I/usr/repy/repy 

BUILD_DIR = monitor
OBJDIR = monitor
HEADERS = $(wildcard lind_monitor/*.h) $(wildcard platform/*.h)
SRCS = $(wildcard lind_monitor/*.c) $(wildcard platform/*.c)
OBJECTS = $(SRCS:.c=.o )


.PHONY: default all clean

TARGET = reference_monitor

default: $(TARGET)

all: default

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(IFLAGS) $(LFLAGS) -o $@
	
clean:
	@- $(RM) *.o $(TARGET) lind_monitor/*.o

