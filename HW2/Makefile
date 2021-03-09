CC=gcc
CFLAGS=-Wpedantic -Wall -Wextra -Werror -std=c89

test: main.o doubll2d.o
	${CC} ${CFLAGS} main.o doubll2d.o -o test

doubll2d.o: doubll2d.c doubll2d.h
	${CC} -c ${CFLAGS} doubll2d.c -o doubll2d.o

test.o: main.c
	${CC} -c ${CFLAGS} main.c -o test.o

