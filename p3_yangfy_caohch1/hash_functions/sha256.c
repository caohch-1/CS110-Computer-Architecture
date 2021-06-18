/*********************************************************************
Homework 5
CS 110: Computer Architecture, Spring 2021
ShanghaiTech University

* Last Modified: 03/28/2021
* Copyright Notice: This file is dervived from work of Brad Conte at
  https://github.com/B-Con/crypto-algorithms/
*********************************************************************/

/*********************************************************************
* Filename:   sha256.c
* Author:     Brad Conte (brad AT bradconte.com)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Implementation of the SHA-256 hashing algorithm.
              SHA-256 is one of the three algorithms in the SHA2
              specification. The others, SHA-384 and SHA-512, are not
              offered in this implementation.
              Algorithm specification can be found here:
               * http://csrc.nist.gov/publications/fips/fips180-2/fips180-2withchangenotice.pdf
              This implementation uses little endian byte order.
*********************************************************************/

/*************************** HEADER FILES ***************************/
#include "sha256.h"
#include <memory.h>
#include <stdlib.h>
#include <xmmintrin.h>
#include <omp.h>
#include <stdio.h>

/****************************** MACROS ******************************/
#define ROTLEFT(a, b) (((a) << (b)) | ((a) >> (32 - (b))))

/**************************** VARIABLES *****************************/
static const WORD k[64] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
        0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
        0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
        0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
        0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
        0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
        0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
        0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
        0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

/*********************** FUNCTION DEFINITIONS ***********************/
void p128_hex_u32(__m128i in) {
    WORD v[4]__attribute__((aligned(16)));
    _mm_store_si128((__m128i *) v, in);
    printf("%x %x %x %x\n", v[0], v[1], v[2], v[3]);
}

#define ROTRIGHT(a, b) (((a) >> (b)) | ((a) << (32 - (b))))

#define CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))

__m128i ch(const __m128i *x, const __m128i *y, const __m128i *z) {
    return ((*x) & (*y)) ^ ((~(*x)) & (*z));
}

#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))

__m128i maj(const __m128i *x, const __m128i *y, const __m128i *z) {
    return ((*x) & (*y)) ^ ((*x) & (*z)) ^ ((*y) & (*z));
}

#define EP0(x) (ROTRIGHT(x, 2) ^ ROTRIGHT(x, 13) ^ ROTRIGHT(x, 22))

__m128i ep0(const __m128i *x) {
    __m128i rot2, rot13, rot22, temp, res;

    rot2 = _mm_srli_epi32(*x, 2);
    temp = _mm_slli_epi32(*x, 32 - 2);
    rot2 = _mm_or_si128(rot2, temp);

    rot13 = _mm_srli_epi32(*x, 13);
    temp = _mm_slli_epi32(*x, 32 - 13);
    rot13 = _mm_or_si128(rot13, temp);

    rot22 = _mm_srli_epi32(*x, 22);
    temp = _mm_slli_epi32(*x, 32 - 22);
    rot22 = _mm_or_si128(rot22, temp);

    res = _mm_xor_si128(rot2, rot13);
    res = _mm_xor_si128(res, rot22);

    return res;
}

#define EP1(x) (ROTRIGHT(x, 6) ^ ROTRIGHT(x, 11) ^ ROTRIGHT(x, 25))

__m128i ep1(const __m128i *x) {
    __m128i rot6, rot11, rot25, temp, res;

    rot6 = _mm_srli_epi32(*x, 6);
    temp = _mm_slli_epi32(*x, 32 - 6);
    rot6 = _mm_or_si128(rot6, temp);

    rot11 = _mm_srli_epi32(*x, 11);
    temp = _mm_slli_epi32(*x, 32 - 11);
    rot11 = _mm_or_si128(rot11, temp);

    rot25 = _mm_srli_epi32(*x, 25);
    temp = _mm_slli_epi32(*x, 32 - 25);
    rot25 = _mm_or_si128(rot25, temp);

    res = _mm_xor_si128(rot6, rot11);
    res = _mm_xor_si128(res, rot25);

    return res;
}

#define SIG0(x) (ROTRIGHT(x, 7) ^ ROTRIGHT(x, 18) ^ ((x) >> 3))

__m128i sig0(const __m128i x) {
    __m128i rot7, rot18, shift3, temp, res;
    rot7 = _mm_srli_epi32(x, 7);
    temp = _mm_slli_epi32(x, 32 - 7);
    rot7 = _mm_or_si128(rot7, temp);

    rot18 = _mm_srli_epi32(x, 18);
    temp = _mm_slli_epi32(x, 32 - 18);
    rot18 = _mm_or_si128(rot18, temp);

    shift3 = _mm_srli_epi32(x, 3);

    res = _mm_xor_si128(rot7, rot18);
    res = _mm_xor_si128(res, shift3);

    return res;
}

#define SIG1(x) (ROTRIGHT(x, 17) ^ ROTRIGHT(x, 19) ^ ((x) >> 10))

__m128i sig1(const __m128i *x) {
    __m128i rot17, rot19, shift10, temp, res;
    rot17 = _mm_srli_epi32(*x, 17);
    temp = _mm_slli_epi32(*x, 32 - 17);
    rot17 = _mm_or_si128(rot17, temp);

    rot19 = _mm_srli_epi32(*x, 19);
    temp = _mm_slli_epi32(*x, 32 - 19);
    rot19 = _mm_or_si128(rot19, temp);

    shift10 = _mm_srli_epi32(*x, 10);

    res = _mm_xor_si128(rot17, rot19);
    res = _mm_xor_si128(res, shift10);

    return res;
}

void process64(__m128i *a, __m128i *b, __m128i *c, __m128i *d, __m128i *e, __m128i *f, __m128i *g, __m128i *h,
               const __m128i k_add_m) {
//    t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
//    t2 = EP0(a) + MAJ(a, b, c);
    __m128i t1, t2;
    t1 = _mm_add_epi32(*h, ep1(e));
    t1 = _mm_add_epi32(ch(e, f, g), t1);
    t1 = _mm_add_epi32(k_add_m, t1);

    t2 = _mm_add_epi32(ep0(a), maj(a, b, c));

    *d = _mm_add_epi32(*d, t1);
    *h = _mm_add_epi32(t1, t2);

}

void sha256_transform(SHA256_CTX *ctx, const BYTE data[]) {
    WORD m[64];
    WORD state0, state1, state2, state3, state4, state5, state6, state7;
    WORD t1, t2;

    int j;
#pragma omp simd
    for (int i = 0; i < 16; i++) {
        j = i * 4;
        m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]);
        m[i + 1] = (data[j + 4] << 24) | (data[j + 5] << 16) | (data[j + 6] << 8) | (data[j + 7]);
    }

    __m128i vec[3], vec_temp;
//    #pragma omp simd
    for (int i = 16; i < 64; i += 4) {
        // m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];
        vec[0] = _mm_load_si128((__m128i * )(m + i - 16));
        vec[1] = sig0(_mm_load_si128((__m128i * )(m + i - 15)));
        vec[2] = _mm_load_si128((__m128i * )(m + i - 7));

        vec_temp = _mm_add_epi32(_mm_add_epi32(vec[0], vec[1]), vec[2]);
        _mm_store_si128((__m128i * )(m + i), vec_temp);

        m[i] += SIG1(m[i - 2]);
        m[i + 1] += SIG1(m[i - 1]);
        m[i + 2] += SIG1(m[i]);
        m[i + 3] += SIG1(m[i + 1]);
    }

    state0 = ctx->state[0];
    state1 = ctx->state[1];
    state2 = ctx->state[2];
    state3 = ctx->state[3];
    state4 = ctx->state[4];
    state5 = ctx->state[5];
    state6 = ctx->state[6];
    state7 = ctx->state[7];

    __m128i k_m_h, h;
    WORD state_res[64];

//    #pragma omp simd

    // a0, b1, c2, d3, e4, f5, g6, h7
    // i = 0
    k_m_h = _mm_add_epi32(_mm_load_si128((__m128i * )(m + 0)), _mm_load_si128((__m128i * )(k + 0)));
    h = _mm_set_epi32(state4, state5, state6, state7);
    k_m_h = _mm_add_epi32(k_m_h, h);
    _mm_store_si128((__m128i * )(state_res + 0), k_m_h);

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[0];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[0 + 1];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[0 + 2];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[0 + 3];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // i= 4
    k_m_h = _mm_add_epi32(_mm_load_si128((__m128i * )(m + 4)), _mm_load_si128((__m128i * )(k + 4)));
    h = _mm_set_epi32(state4, state5, state6, state7);
    k_m_h = _mm_add_epi32(k_m_h, h);
    _mm_store_si128((__m128i * )(state_res + 4), k_m_h);

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[4];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[4 + 1];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[4 + 2];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[4 + 3];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    //i = 8
    k_m_h = _mm_add_epi32(_mm_load_si128((__m128i * )(m + 8)), _mm_load_si128((__m128i * )(k + 8)));
    h = _mm_set_epi32(state4, state5, state6, state7);
    k_m_h = _mm_add_epi32(k_m_h, h);
    _mm_store_si128((__m128i * )(state_res + 8), k_m_h);

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[8];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[8 + 1];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[8 + 2];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[8 + 3];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // i = 12
    k_m_h = _mm_add_epi32(_mm_load_si128((__m128i * )(m + 12)), _mm_load_si128((__m128i * )(k + 12)));
    h = _mm_set_epi32(state4, state5, state6, state7);
    k_m_h = _mm_add_epi32(k_m_h, h);
    _mm_store_si128((__m128i * )(state_res + 12), k_m_h);

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[12];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[13];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[14];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[15];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    //i = 16
    k_m_h = _mm_add_epi32(_mm_load_si128((__m128i * )(m + 16)), _mm_load_si128((__m128i * )(k + 16)));
    h = _mm_set_epi32(state4, state5, state6, state7);
    k_m_h = _mm_add_epi32(k_m_h, h);
    _mm_store_si128((__m128i * )(state_res + 16), k_m_h);

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[16];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[17];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[18];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[19];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // i = 20
    k_m_h = _mm_add_epi32(_mm_load_si128((__m128i * )(m + 20)), _mm_load_si128((__m128i * )(k + 20)));
    h = _mm_set_epi32(state4, state5, state6, state7);
    k_m_h = _mm_add_epi32(k_m_h, h);
    _mm_store_si128((__m128i * )(state_res + 20), k_m_h);

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[20];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[21];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[22];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[23];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // i = 24
    k_m_h = _mm_add_epi32(_mm_load_si128((__m128i * )(m + 24)), _mm_load_si128((__m128i * )(k + 24)));
    h = _mm_set_epi32(state4, state5, state6, state7);
    k_m_h = _mm_add_epi32(k_m_h, h);
    _mm_store_si128((__m128i * )(state_res + 24), k_m_h);

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[24];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[25];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[26];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[27];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    //i = 28
    k_m_h = _mm_add_epi32(_mm_load_si128((__m128i * )(m + 28)), _mm_load_si128((__m128i * )(k + 28)));
    h = _mm_set_epi32(state4, state5, state6, state7);
    k_m_h = _mm_add_epi32(k_m_h, h);
    _mm_store_si128((__m128i * )(state_res + 28), k_m_h);

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[28];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[29];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[30];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[31];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    //i = 32
    k_m_h = _mm_add_epi32(_mm_load_si128((__m128i * )(m + 32)), _mm_load_si128((__m128i * )(k + 32)));
    h = _mm_set_epi32(state4, state5, state6, state7);
    k_m_h = _mm_add_epi32(k_m_h, h);
    _mm_store_si128((__m128i * )(state_res + 32), k_m_h);

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[32];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[33];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[34];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[35];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // i = 36
    k_m_h = _mm_add_epi32(_mm_load_si128((__m128i * )(m + 36)), _mm_load_si128((__m128i * )(k + 36)));
    h = _mm_set_epi32(state4, state5, state6, state7);
    k_m_h = _mm_add_epi32(k_m_h, h);
    _mm_store_si128((__m128i * )(state_res + 36), k_m_h);

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[36];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[37];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[38];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[39];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // i = 40
    k_m_h = _mm_add_epi32(_mm_load_si128((__m128i * )(m + 40)), _mm_load_si128((__m128i * )(k + 40)));
    h = _mm_set_epi32(state4, state5, state6, state7);
    k_m_h = _mm_add_epi32(k_m_h, h);
    _mm_store_si128((__m128i * )(state_res + 40), k_m_h);

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[40];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[41];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[42];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[43];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    //i =44
    k_m_h = _mm_add_epi32(_mm_load_si128((__m128i * )(m + 44)), _mm_load_si128((__m128i * )(k + 44)));
    h = _mm_set_epi32(state4, state5, state6, state7);
    k_m_h = _mm_add_epi32(k_m_h, h);
    _mm_store_si128((__m128i * )(state_res + 44), k_m_h);

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[44];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[45];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[46];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[47];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    //i = 48
    k_m_h = _mm_add_epi32(_mm_load_si128((__m128i * )(m + 48)), _mm_load_si128((__m128i * )(k + 48)));
    h = _mm_set_epi32(state4, state5, state6, state7);
    k_m_h = _mm_add_epi32(k_m_h, h);
    _mm_store_si128((__m128i * )(state_res + 48), k_m_h);

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[48];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[49];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[50];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[51];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    //i = 52
    k_m_h = _mm_add_epi32(_mm_load_si128((__m128i * )(m + 52)), _mm_load_si128((__m128i * )(k + 52)));
    h = _mm_set_epi32(state4, state5, state6, state7);
    k_m_h = _mm_add_epi32(k_m_h, h);
    _mm_store_si128((__m128i * )(state_res + 52), k_m_h);

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[52];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[53];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[54];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[55];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // i = 56
    k_m_h = _mm_add_epi32(_mm_load_si128((__m128i * )(m + 56)), _mm_load_si128((__m128i * )(k + 56)));
    h = _mm_set_epi32(state4, state5, state6, state7);
    k_m_h = _mm_add_epi32(k_m_h, h);
    _mm_store_si128((__m128i * )(state_res + 56), k_m_h);

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[56];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[57];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[58];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[59];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // i = 60
    k_m_h = _mm_add_epi32(_mm_load_si128((__m128i * )(m + 60)), _mm_load_si128((__m128i * )(k + 60)));
    h = _mm_set_epi32(state4, state5, state6, state7);
    k_m_h = _mm_add_epi32(k_m_h, h);
    _mm_store_si128((__m128i * )(state_res + 60), k_m_h);

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[60];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[61];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[62];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;

    // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
    t1 = EP1(state4) + CH(state4, state5, state6) + state_res[63];
    // t2 = EP0(a) + MAJ(a, b, c);
    t2 = EP0(state0) + MAJ(state0, state1, state2);
    // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    state7 = state6;
    state6 = state5;
    state5 = state4;
    state4 = state3 + t1;
    state3 = state2;
    state2 = state1;
    state1 = state0;
    state0 = t1 + t2;
//    for (int i = 0; i < 64; i += 4) {
//        // a0, b1, c2, d3, e4, f5, g6, h7
//        k_m_h = _mm_add_epi32(_mm_load_si128((__m128i * )(m + i)), _mm_load_si128((__m128i * )(k + i)));
//        h = _mm_set_epi32(state4, state5, state6, state7);
//        k_m_h = _mm_add_epi32(k_m_h, h);
//        _mm_store_si128((__m128i * )(state_res + i), k_m_h);
//
//        // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
//        t1 = EP1(state4) + CH(state4, state5, state6) + state_res[i];
//        // t2 = EP0(a) + MAJ(a, b, c);
//        t2 = EP0(state0) + MAJ(state0, state1, state2);
//        // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
//        state7 = state6;
//        state6 = state5;
//        state5 = state4;
//        state4 = state3 + t1;
//        state3 = state2;
//        state2 = state1;
//        state1 = state0;
//        state0 = t1 + t2;
//
//        // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
//        t1 = EP1(state4) + CH(state4, state5, state6) + state_res[i + 1];
//        // t2 = EP0(a) + MAJ(a, b, c);
//        t2 = EP0(state0) + MAJ(state0, state1, state2);
//        // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
//        state7 = state6;
//        state6 = state5;
//        state5 = state4;
//        state4 = state3 + t1;
//        state3 = state2;
//        state2 = state1;
//        state1 = state0;
//        state0 = t1 + t2;
//
//        // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
//        t1 = EP1(state4) + CH(state4, state5, state6) + state_res[i + 2];
//        // t2 = EP0(a) + MAJ(a, b, c);
//        t2 = EP0(state0) + MAJ(state0, state1, state2);
//        // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
//        state7 = state6;
//        state6 = state5;
//        state5 = state4;
//        state4 = state3 + t1;
//        state3 = state2;
//        state2 = state1;
//        state1 = state0;
//        state0 = t1 + t2;
//
//        // t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
//        t1 = EP1(state4) + CH(state4, state5, state6) + state_res[i + 3];
//        // t2 = EP0(a) + MAJ(a, b, c);
//        t2 = EP0(state0) + MAJ(state0, state1, state2);
//        // h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
//        state7 = state6;
//        state6 = state5;
//        state5 = state4;
//        state4 = state3 + t1;
//        state3 = state2;
//        state2 = state1;
//        state1 = state0;
//        state0 = t1 + t2;
//    }


    ctx->state[0] += state0;
    ctx->state[1] += state1;
    ctx->state[2] += state2;
    ctx->state[3] += state3;
    ctx->state[4] += state4;
    ctx->state[5] += state5;
    ctx->state[6] += state6;
    ctx->state[7] += state7;
}

void sha256_init(SHA256_CTX *ctx) {
    ctx->datalen = 0;
    ctx->bitlen = 0;
    ctx->state[0] = 0x6a09e667;
    ctx->state[1] = 0xbb67ae85;
    ctx->state[2] = 0x3c6ef372;
    ctx->state[3] = 0xa54ff53a;
    ctx->state[4] = 0x510e527f;
    ctx->state[5] = 0x9b05688c;
    ctx->state[6] = 0x1f83d9ab;
    ctx->state[7] = 0x5be0cd19;


}

void sha256_update(SHA256_CTX *ctx, const BYTE data[], size_t len) {
    WORD i;

//    #pragma omp simd
    for (i = 0; i < len; ++i) {
        ctx->data[ctx->datalen] = data[i];
        ctx->datalen++;
        if (ctx->datalen == 64) {
            sha256_transform(ctx, ctx->data);
            ctx->bitlen += 512;
            ctx->datalen = 0;
        }
    }
}

void sha256_final(SHA256_CTX *ctx, BYTE hash[]) {
    WORD i;

    i = ctx->datalen;

    /* Pad whatever data is left in the buffer. */
    if (ctx->datalen < 56) {
        ctx->data[i++] = 0x80;
        while (i < 56)
            ctx->data[i++] = 0x00;
    } else {
        ctx->data[i++] = 0x80;
        while (i < 64)
            ctx->data[i++] = 0x00;
        sha256_transform(ctx, ctx->data);
        memset(ctx->data, 0, 56);
    }

    /* Append to the padding the total message's length in bits and transform. */
    ctx->bitlen += ctx->datalen * 8;
    ctx->data[63] = ctx->bitlen;
    ctx->data[62] = ctx->bitlen >> 8;
    ctx->data[61] = ctx->bitlen >> 16;
    ctx->data[60] = ctx->bitlen >> 24;
    ctx->data[59] = ctx->bitlen >> 32;
    ctx->data[58] = ctx->bitlen >> 40;
    ctx->data[57] = ctx->bitlen >> 48;
    ctx->data[56] = ctx->bitlen >> 56;
    sha256_transform(ctx, ctx->data);

    /* Since this implementation uses little endian byte ordering and SHA uses big
       endian, reverse all the bytes when copying the final state to the output
       hash. */
#pragma omp simd
    for (i = 0; i < 4; ++i) {
        hash[i] = (ctx->state[0] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 4] = (ctx->state[1] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 8] = (ctx->state[2] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 20] = (ctx->state[5] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 24] = (ctx->state[6] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 28] = (ctx->state[7] >> (24 - i * 8)) & 0x000000ff;
    }
}

