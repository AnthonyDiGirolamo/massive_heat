#!/usr/bin/make
CC = mpicc
CFLAGS=-g -Wall
LDFLAGS=

all: main mkinit
mkinit: mkinit.c
		gcc $(CFLAGS) $? $(LDFLAGS) -o $@
clean:
		rm -f main mkinit *.o
profile: main.c
		mpecc -mpilog $(CFLAGS) $? $(LDFLAGS) -o main
view_profile:
		clog2TOslog2 main.clog2 && jumpshot main.slog2
