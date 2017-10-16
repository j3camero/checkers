CPPFLAGS = -Wall -O3

all: test

test-main: test-main.o test-bit-string.o
	g++ test-*.o -o test-main

test: test-main
	./test-main

clean:
	rm -rf *.o main test-main
