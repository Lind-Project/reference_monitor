INCLUD= /usr/include

CC = gcc
CFLAGS = -Wall -g -O2 $(IFLAGS) -std=C99
LFLAGS = -lpthread -lcurl
IFLAGS = -I$(INCLUDE) 

BUILD_DIR = utf/build/testcases
HEADERS = testcases/testcases.h
OBJDIR = utf/build

SRCS = $(wildcard testcases/*.c)
OBJS = $(SRCS:.c=.o )

.PHONY: all clean

all: $(OBJS) $(HEADERS)
%.o: %.c
	$(CC) $(CFLAGS) $< -o $(OBJDIR)/$@ $(LFLAGS)

clean:
	@- $(RM) $(BUILD_DIR)/*