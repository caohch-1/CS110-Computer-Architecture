/*********************************************************************
Homework 5
CS 110: Computer Architecture, Spring 2021
ShanghaiTech University

* Last Modified: 03/28/2021
*********************************************************************/

#include "blockchain.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>

void blockchain_node_init(blk_t *node, uint32_t index, uint32_t timestamp,
                          unsigned char prev_hash[32], unsigned char *data,
                          size_t data_size) {
    if (!node || !data || !prev_hash)
        return;

    node->header.index = index;
    node->header.timestamp = timestamp;
    node->header.nonce = -1;

    memset(node->header.data, 0, sizeof(unsigned char) * 256);
    memcpy(node->header.prev_hash, prev_hash, HASH_BLOCK_SIZE);
    memcpy(node->header.data, data,
           sizeof(unsigned char) * ((data_size < 256) ? data_size : 256));
}

void blockchain_node_hash(blk_t *node, unsigned char hash_buf[HASH_BLOCK_SIZE],
                          hash_func func) {
    if (node)
        func((unsigned char *) node, sizeof(blkh_t), (unsigned char *) hash_buf);
}

BOOL blockchain_node_verify(blk_t *node, blk_t *prev_node, hash_func func) {
    unsigned char hash_buf[HASH_BLOCK_SIZE];

    if (!node || !prev_node)
        return False;

    blockchain_node_hash(node, hash_buf, func);
    if (memcmp(node->hash, hash_buf, sizeof(unsigned char) * HASH_BLOCK_SIZE))
        return False;

    blockchain_node_hash(prev_node, hash_buf, func);
    if (memcmp(node->header.prev_hash, hash_buf,
               sizeof(unsigned char) * HASH_BLOCK_SIZE))
        return False;

    return True;
}


/* New Parts */
struct HASH_THREAD {
    blk_t *node;
    unsigned char hash_buf[HASH_BLOCK_SIZE];
    unsigned char one_diff[HASH_BLOCK_SIZE];
    size_t diff_q;
    BOOL found; // Used to locate which thread find ETH
    pthread_t which_thread;
    hash_func *hash_fun;
};
typedef struct HASH_THREAD hash_t;
BOOL kill_thread = False; // If one thread find ETH, kill other threads
hash_t **threads;

void *my_mine(void *arg) {
    while (True) {
        if (kill_thread == True) {
            pthread_exit(NULL);
        }

        blockchain_node_hash(threads[*((int *) arg)]->node, threads[*((int *) arg)]->hash_buf,
                             threads[*((int *) arg)]->hash_fun);

        if ((!memcmp(threads[*((int *) arg)]->hash_buf, threads[*((int *) arg)]->one_diff,
                     sizeof(unsigned char) * threads[*((int *) arg)]->diff_q)) &&
            memcmp(&threads[*((int *) arg)]->hash_buf[threads[*((int *) arg)]->diff_q],
                   &threads[*((int *) arg)]->one_diff[threads[*((int *) arg)]->diff_q],
                   sizeof(unsigned char) * (HASH_BLOCK_SIZE - threads[*((int *) arg)]->diff_q)) <= 0) {

            memcpy(threads[*((int *) arg)]->node->hash, threads[*((int *) arg)]->hash_buf,
                   sizeof(unsigned char) * HASH_BLOCK_SIZE);
            threads[*((int *) arg)]->found = True;
            kill_thread = True;
            pthread_exit(NULL);
        }
        threads[*((int *) arg)]->node->header.nonce += 20;
    }
}


/* The sequiental implementation of mining implemented for you. */
void blockchain_node_mine(blk_t *node, unsigned char hash_buf[HASH_BLOCK_SIZE],
                          size_t diff, hash_func func) {
    unsigned char one_diff[HASH_BLOCK_SIZE];
    size_t diff_q, diff_m;
    diff_q = diff / 8;
    diff_m = diff % 8;

    memset(one_diff, 0xFF, sizeof(unsigned char) * HASH_BLOCK_SIZE);
    memset(one_diff, 0, sizeof(unsigned char) * diff_q);
    one_diff[diff_q] = ((uint8_t) 0xFF) >> diff_m;

    // Init 20 threads
    threads = (hash_t **) malloc(sizeof(hash_t *) * 20);

    int thread_index = 0;
    int enum_index[20] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
    for (thread_index = 0; thread_index < 20; thread_index++) {
        // Init all threads (especially nodes) with given info
        threads[thread_index] = (hash_t *) malloc(sizeof(hash_t));
        threads[thread_index]->node = (blk_t *) malloc(sizeof(blk_t));

        blockchain_node_init(threads[thread_index]->node, node->header.index, node->header.timestamp,
                             node->header.prev_hash, node->header.data, sizeof(node->header.data));

        threads[thread_index]->node->header.nonce = thread_index;
        threads[thread_index]->diff_q = diff_q;
        memcpy(threads[thread_index]->hash_buf, hash_buf, HASH_BLOCK_SIZE);
        memcpy(threads[thread_index]->one_diff, one_diff, HASH_BLOCK_SIZE);
        threads[thread_index]->found = False;
        threads[thread_index]->hash_fun = func;
        // Start thread
        pthread_create(&threads[thread_index]->which_thread, NULL, my_mine, &enum_index[thread_index]);
    }


    for (thread_index = 0; thread_index < 20; thread_index++) {
        pthread_join(threads[thread_index]->which_thread, NULL);
    }

    for (thread_index = 0; thread_index < 20; thread_index++) {
        if (threads[thread_index]->found == True) {
            memcpy(node, threads[thread_index]->node, sizeof(blk_t));
            memcpy(hash_buf, threads[thread_index]->hash_buf, sizeof(unsigned char) * HASH_BLOCK_SIZE);
        }
   }

    for (thread_index = 0; thread_index < 20; thread_index++) {
        free(threads[thread_index]->node);
        free(threads[thread_index]);
    }
    free(threads);
    kill_thread = False;

//  while (True) {
//    blockchain_node_hash(node, hash_buf, func);
//    if ((!memcmp(hash_buf, one_diff, sizeof(unsigned char) * diff_q)) &&
//        memcmp(&hash_buf[diff_q], &one_diff[diff_q],
//               sizeof(unsigned char) * (HASH_BLOCK_SIZE - diff_q)) <= 0) {
//
//      memcpy(node->hash, hash_buf, sizeof(unsigned char) * HASH_BLOCK_SIZE);
//      break;
//    }
//    node->header.nonce++;
//  }
}
