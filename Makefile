CPPFLAGS = -Wall -O3

all: test

#main: $(OBJECTS)
#	g++ $(OBJECTS) $(CFLAGS) -o main

test-main: test-main.o test-bitstring.o
	g++ test-*.o -o test-main

test: test-main
	./test-main

clean:
	rm -rf *.o main test-main
