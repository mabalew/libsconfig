CC=c++
OPT=-Wall -fPIC -lsqlite3 -lm -ggdb
OPT_TEST=-Wall -L/usr/lib:. ./libsconfig.so -lsqlite3 -lm -ggdb

all: libsconfig test

libsconfig:
	$(CC) -o libsconfig.o src/libsconfig.cpp $(OPT) -c
	$(CC) -shared -fPIC -o libsconfig.so libsconfig.o
	rm -f libsconfig.o

test:
	$(CC) src/test.cpp -o test $(OPT_TEST)

clean:
	rm -f libsconfig.so libsconfig.o test
