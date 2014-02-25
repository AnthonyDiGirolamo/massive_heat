#!/usr/bin/make
CC = gcc
CFLAGS=-g -Wall
LDFLAGS=

all: bin/massive_heat.exe

bin/massive_heat.exe: main.c lib/libeuler.a bin/
	$(CC) $(CFLAGS) -I. -L. -o massive_heat.exe main.c -leuler
	./massive_heat.exe

bin/massive_heat_tests.exe: tests.c lib/libeuler.a bin/
	$(CC) $(CFLAGS) -I. -Llib/ -o bin/massive_heat_tests.exe tests.c -leuler
	./bin/massive_heat_tests.exe

lib/libeuler.a: euler.c euler.h lib/
	$(CC) $(CFLAGS) -c euler.c -o lib/euler.o
	ar rs lib/libeuler.a lib/euler.o

bin/:
	mkdir -p bin/

lib/:
	mkdir -p lib/

clean:
	rm -rf bin/
	rm -rf lib/
