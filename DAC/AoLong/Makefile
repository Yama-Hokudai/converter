#
#INCLUDE_DIR = /usr/src/linux/include
CC = gcc
CFLAGS = -Wall

OBJS = aolong.o

all:$(OBJS)
	$(CC) $^ -o aolong -lcaio -lm

clean:
	rm -f *.o
