#!/usr/bin/make
CC = gcc
CFLAGS=-g -Wall
LDFLAGS=

all: massive_heat_tests.exe

massive_heat_tests.exe: tests.c libeuler.a
	$(CC) $(CFLAGS) -I. -L. -o massive_heat_tests.exe tests.c -leuler
	./massive_heat_tests.exe

libeuler.a: euler.c euler.h
	$(CC) $(CFLAGS) -c euler.c
	ar rs libeuler.a euler.o

clean:
	rm -f *.exe
	rm -f *.o
	rm -f *.a
