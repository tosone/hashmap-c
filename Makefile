CC     = gcc
CFLAGS = -O2 -std=c99

TARGET = libhashmap

all: libhashmap.a

libhashmap.a:
	${CC} ${CFLAGS} -c hashmap.c -o ${TARGET}.o
	ar rcs ${TARGET}.a ${TARGET}.o

test: libhashmap.a
	gcc ${CFLAGS} test.c -L. -lhashmap -I. -o hashmap.test

clean:
	-rm *.a *.o *.test

.PHONY: libhashmap.a test