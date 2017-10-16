CC=$(CXX)
CPPFLAGS = -Wall #-O3

all: test

test-main: test-main.o \
           bit-string.o bit-string-test.o \
           solution-buffer.o solution-buffer-test.o \
					 types-test.o

test: test-main
	./test-main

clean:
	rm -rf *.o test-main
