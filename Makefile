CC=$(CXX)
CPPFLAGS = -Wall #-O3 -funroll-loops

all: enumerator-benchmark \
     test

clean:
	rm -rf *.o enumerator-benchmark test-main

enumerator-benchmark: enumerator-benchmark.o \
                      board.o \
                      combinator.o \
                      enumerator.o \
											four-tuple.o \
											pawn-cache.o \
											seven-tuple.o \
											six-tuple.o \
											string-util.o

test: test-main
	./test-main

test-main: test-main.o \
           bit-string.o bit-string-test.o \
           board.o board-test.o \
           combinator.o combinator-test.o \
           enumerator.o enumerator-test.o \
					 four-tuple.o four-tuple-test.o \
					 huffman.o huffman-test.o \
           large-solution-buffer.o large-solution-buffer-test.o \
           medium-solution-buffer.o medium-solution-buffer-test.o \
           pawn-cache.o pawn-cache-test.o \
           seven-tuple.o seven-tuple-test.o \
           six-tuple.o six-tuple-test.o \
           small-solution-buffer.o small-solution-buffer-test.o \
					 std-test.o \
           string-util.o string-util-test.o
