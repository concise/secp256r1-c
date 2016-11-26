#ifndef MODN_H_INCLUDED
#define MODN_H_INCLUDED

#include <stdint.h>

/// N = 0xffffffff00000000ffffffffffffffffbce6faada7179e84f3b9cac2fc632551
//
//  Three 256-bit unsigned integer operations are implemented here:
//
//      Z <- X + Y mod N
//      Z <- X - Y mod N
//      Z <- X * Y * 2^-256 mod N
//
//  All three operations assume that X and Y are in the range [0, N-1].  In
//  this implementation, an integer in the range [0, N-1] is represented as 8
//  uint32_t objects of which the more significant objects have higher memory
//  addresses.

void modn_add(uint32_t *Z, const uint32_t *X, const uint32_t *Y);
void modn_sub(uint32_t *Z, const uint32_t *X, const uint32_t *Y);
void modn_mmul(uint32_t *Z, const uint32_t *X, const uint32_t *Y);

#endif
