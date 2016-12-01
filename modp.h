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
void modp_mul(uint32_t *Z, const uint32_t *X, const uint32_t *Y);
void modp_inv(uint32_t *Z, const uint32_t *X);

// How to represent an integer A that is already taken modulo M?
//
// method 1 --- using 32 uint8_t objects (x[0] ... x[31])
//
//      x[0]*(2^8)^31 + ... + x[30]*(2^8)^2 + x[30]*(2^8) + x[31]
//
// method 2 --- using 8 uint32_t objects (y[0] ... y[7])
//
//      (y[0] + y[1]*(2^32) + y[1]*(2^32)^2 + ... + y[7]*(2^32)^7) * R^-1 mod M
//
// encode: from method 1 to method 2
// decode: from method 2 to method 1

void modp_encode(uint32_t *out, const uint8_t *inp);
void modp_decode(uint8_t *out, const uint32_t *inp);

#endif
