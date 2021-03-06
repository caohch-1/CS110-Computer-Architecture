CC=gcc
CFLAGS=-Wpedantic -Wall -Wextra -Werror -std=c89

test: test.o doubll2d.o
	${CC} ${CFLAGS} test.o doubll2d.o -o test

doubll2d.o: doubll2d.c doubll2d.h
	${CC} -c ${CFLAGS} doubll2d.c -o doubll2d.o

test.o: test.c
	${CC} -c ${CFLAGS} test.c -o test.o

