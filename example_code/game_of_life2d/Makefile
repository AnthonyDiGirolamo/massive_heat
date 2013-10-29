#!/usr/bin/make
CC = mpicc
CFLAGS=-g -Wall
LDFLAGS=

all: life mkinit
mkinit: mkinit.c
	gcc $(CFLAGS) $? $(LDFLAGS) -o $@
clean:
	rm -f life mkinit *.o
profile: life.c
	mpecc -mpilog $(CFLAGS) $? $(LDFLAGS) -o life
view_profile:
	clog2TOslog2 life.clog2 && jumpshot life.slog2
