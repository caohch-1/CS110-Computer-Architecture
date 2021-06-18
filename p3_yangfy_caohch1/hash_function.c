/*********************************************************************
Homework 5
CS 110: Computer Architecture, Spring 2021
ShanghaiTech University

* Last Modified: 03/28/2021
*********************************************************************/

#include "hash_function.h"
#include "hash_functions/sha256.h"

/*
   The custom hash function based on SHA256 used for your testing.

   In the auto-grader, we will use another hash function (called PVG256) derived
   from SHA256 for testing.
*/
void blockchain_do_hash(unsigned char *data, size_t data_size,
                        unsigned char buf[HASH_BLOCK_SIZE]) {
  SHA256_CTX ctx;

  sha256_init(&ctx);
  sha256_update(&ctx, data, data_size);
  sha256_final(&ctx, buf);
}
