CC = gcc
CFLAGS = -g -std=c89 -Wpedantic -Wall -Wextra -Werror
ASSEMBLER_FILES = src/tables.c src/utils.c src/translate_utils.c src/translate.c

all: assembler

.PHONY: assembler clean check grade

assembler: clean
	$(CC) $(CFLAGS) -o assembler assembler.c $(ASSEMBLER_FILES)

clean:
	-$(MAKE) -C test clean
	-rm -f *.o assembler
	-rm -rf out
	-rm -rf __pycache__

check: assembler
	$(MAKE) -C test

grade: assembler
	$(MAKE) -C test grade
