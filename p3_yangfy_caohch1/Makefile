#*********************************************************************
# Homework 5
# CS 110: Computer Architecture, Spring 2021
# ShanghaiTech University
#
# * Last Modified: 03/30/2021
#*********************************************************************

# Specify the compiler and flags that will be used.
CC=gcc
CFLAGS=-Wpedantic -Wall -Wextra -Werror -O3 -fopenmp -mtune=native

# Simplify the target list below.

SHA256_HEADER=hash_functions/sha256.h
BLOCKCHAIN_HEADER=blockchain.h hash_function.h bool.h

SHA256_IMPL=hash_functions/sha256.c
BLOCKCHAIN_IMPL=blockchain.c hash_function.c

SHA256_TEST=hash_functions/sha256_test.c
BLOCKCHAIN_TEST=test.c

SHA256_TARGET=test-sha256.out
BLOCKCHAIN_TARGET=blockchain.out

SUBMISSION_IMPL=Makefile hash_functions/sha256.c blockchain.c hash_function.c
SUBMISSION_TARGET=proj3.tar

# Target for the blockchain implementation that will test your implementation.
# Trigger this target by `make` or `make blockchain.out`.
$(BLOCKCHAIN_TARGET): Makefile $(SHA256_HEADER) $(BLOCKCHAIN_HEADER) $(SHA256_IMPL) $(BLOCKCHAIN_IMPL) $(BLOCKCHAIN_TEST)
	${CC} ${CFLAGS} $(BLOCKCHAIN_IMPL) $(SHA256_IMPL) $(BLOCKCHAIN_TEST) -o $(BLOCKCHAIN_TARGET)

# Target for testing if the SHA256 implementation works on your computer.
# Trigger this target by `make test-sha256.out`.
$(SHA256_TARGET): Makefile $(SHA256_TEST) $(SHA256_IMPL) $(SHA256_HEADER)
	${CC} ${CFLAGS} $(SHA256_IMPL) $(SHA256_TEST) -o $(SHA256_TARGET)

# Target for creating the tarball for submission.
# Trigger this target by `make submission`.
.PHONY: submission
submission:
	tar -cvpf $(SUBMISSION_TARGET) $(SUBMISSION_IMPL)
	
# Target for cleanup your workspace - deleting all files created by your compiler.
# Trigger this target by `make clean`.
.PHONY: clean
clean:
	rm -rf $(BLOCKCHAIN_TARGET) $(SHA256_TARGET) $(SUBMISSION_TARGET) *.dSYM

