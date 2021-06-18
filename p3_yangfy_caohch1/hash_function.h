/*********************************************************************
Homework 5
CS 110: Computer Architecture, Spring 2021
ShanghaiTech University

* Last Modified: 03/28/2021
*********************************************************************/

#ifndef _HASH_FUNCTION_H
#define _HASH_FUNCTION_H

#include <stddef.h>
#include <stdint.h>

#define HASH_BLOCK_SIZE 32

typedef void hash_func(unsigned char *data, size_t data_size,
                       unsigned char buf[HASH_BLOCK_SIZE]);

void blockchain_do_hash(unsigned char *data, size_t data_size,
                        unsigned char buf[HASH_BLOCK_SIZE]);

#endif /* hash_function.h */
