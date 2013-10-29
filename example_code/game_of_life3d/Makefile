#!/usr/bin/make
CC = mpicc
CFLAGS=-g -Wall
LDFLAGS=

all: life3d mkinit3d
mkinit3d: mkinit3d.c
	gcc $(CFLAGS) $? $(LDFLAGS) -o $@
clean:
	rm -rf life3d mkinit3d *.o *.dSYM
