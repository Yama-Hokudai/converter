#
#INCLUDE_DIR = /usr/src/linux/include
CC = gcc
CFLAGS = -Wall

OBJS = ailong.o

all:$(OBJS)
	$(CC) $^ -o ailong -lcaio

clean:
	rm -f *.o
