#ifndef MODP_H_INCLUDED
#define MODP_H_INCLUDED

#include <stdint.h>

//  P = 0xffffffff00000001000000000000000000000000ffffffffffffffffffffffff
//
//  Three 256-bit unsigned integer operations are implemented here:
//
//      Z <- X + Y mod P
//      Z <- X - Y mod P
//      Z <- X * Y * 2^-256 mod P
//
//  All three operations assume that X and Y are in the range [0, P-1].  In
//  this implementation, an integer in the range [0, P-1] is represented as 8
//  uint32_t objects of which the more significant objects have higher memory
//  addresses.

void modp_add(uint32_t *Z, const uint32_t *X, const uint32_t *Y);
void modp_sub(uint32_t *Z, const uint32_t *X, const uint32_t *Y);
void modp_mmul(uint32_t *Z, const uint32_t *X, const uint32_t *Y);

#endif
