CC=g++
CPPFLAGS = -Wall -O3

all: test

bit-string-test: bit-string.o

two-bit-string-test: two-bit-string.o

test-main: test-main.o \
           bit-string.o bit-string-test.o \
           two-bit-string.o two-bit-string-test.o

test: test-main
	./test-main

clean:
	rm -rf *.o main test-main
