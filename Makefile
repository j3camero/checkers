CC=$(CXX)
CPPFLAGS = -Wall #-O3

all: test

clean:
	rm -rf *.o test-main

enumerator-benchmark: enumerator-benchmark.o \
                      board.o \
                      combinator.o \
                      enumerator.o \

test: test-main
	./test-main

test-main: test-main.o \
           bit-string.o bit-string-test.o \
           board.o board-test.o \
           combinator.o combinator-test.o \
           enumerator.o enumerator-test.o \
           solution-buffer.o solution-buffer-test.o \
					 types-test.o
