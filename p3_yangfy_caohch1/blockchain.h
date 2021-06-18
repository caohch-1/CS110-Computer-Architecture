/*********************************************************************
Homework 5
CS 110: Computer Architecture, Spring 2021
ShanghaiTech University

* Last Modified: 03/28/2021
*********************************************************************/

#ifndef _BLOCKCHAIN_H
#define _BLOCKCHAIN_H

#include "bool.h"
#include "hash_function.h"
#include <stddef.h>
#include <stdint.h>

struct blockchain_node_header {
  uint32_t index;
  uint32_t timestamp;
  unsigned char prev_hash[HASH_BLOCK_SIZE];
  unsigned char data[256];
  uint64_t nonce;
} __attribute__((__packed__));

typedef struct blockchain_node_header blkh_t;

struct blockchain_node {
  blkh_t header;
  unsigned char hash[HASH_BLOCK_SIZE];
} __attribute__((__packed__));

typedef struct blockchain_node blk_t;

/*
  Initalize your block with index, timestamp, hash from previous block and a set
  of custom data.

  You are not supposed to calcutale hash nonce (i.e., mining) in this function.
  if the data is less than 256 bytes long, padding all the rest bytes with 0. If
  it is more than 256 bytes long, truncate it to 256 bytes.
 */
void blockchain_node_init(blk_t *node, uint32_t index, uint32_t timestamp,
                          unsigned char prev_hash[HASH_BLOCK_SIZE],
                          unsigned char *data, size_t data_size);

/*
  Calculate hash from header of the node (which could be done with smart use of
  `struct blockchain_node_header`), store the hash to the `hash_buf` argument.
*/
void blockchain_node_hash(blk_t *node, unsigned char hash_buf[HASH_BLOCK_SIZE],
                          hash_func func);

/*
  Verify if `node` is legitimate (i.e., have vaild hash) and if `node`'s
  previous node is `prev_node`.
*/
BOOL blockchain_node_verify(blk_t *node, blk_t *prev_node, hash_func func);

/*
  Do mining. The first 'diff' bits of hash should be 0. Only the execution of
  the function will be counted to the mining time of your program
*/
void blockchain_node_mine(blk_t *node, unsigned char hash_buf[HASH_BLOCK_SIZE],
                          size_t diff, hash_func func);

#endif /* blockchain.h */
