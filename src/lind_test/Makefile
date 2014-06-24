CC = gcc
CFLAGS = -g -Wall -pthread
BUILD_DIR= utf/build/testcases
HEADERS = testcases/testcases.h
LIBS= /usr/include

OBJDIR = utf/build

SRCS=$(wildcard testcases/*.c)
OBJS=$(SRCS:.c=.o )

	
.PHONY: all clean

all:$(OBJS)
%.o: %.c
	$(CC) $(CFLAGS) $(HEADERS)  $< -o $(OBJDIR)/$@
         
clean:
	@- $(RM) $(BUILD_DIR)/*

