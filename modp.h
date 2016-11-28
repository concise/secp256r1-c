#ifndef MODP_H_INCLUDED
#define MODP_H_INCLUDED

#include <stdint.h>

//  R = 2^256
//  M = 0xffffffff00000001000000000000000000000000ffffffffffffffffffffffff
//
//  Three 256-bit unsigned integer operations are implemented here:
//
//      Z <- X + Y mod M
//      Z <- X - Y mod M
//      Z <- X * Y * R^-1 mod M
//      Z <- X^-1 * R^2 mod M
//
//  These operations assume that X and Y are already in the range [0, M-1].
//  In this implementation, an integer in the range [0, M-1] is represented as
//  eight uint32_t objects of which the more significant objects have higher
//  memory addresses.

void modp_add(uint32_t *Z, const uint32_t *X, const uint32_t *Y);
void modp_sub(uint32_t *Z, const uint32_t *X, const uint32_t *Y);
void modp_mmul(uint32_t *Z, const uint32_t *X, const uint32_t *Y);
void modp_minv(uint32_t *Z, const uint32_t *X);

#endif
