#include <stdbool.h>
#include "addsub.h"
#include "modp.h"

// p
static const uint32_t M[8] = {
    0xffffffff, 0xffffffff, 0xffffffff, 0x00000000,
    0x00000000, 0x00000000, 0x00000001, 0xffffffff };

// -p^-1 mod 2^32
#define L 0x00000001

void modp_add(uint32_t *Z, const uint32_t *X, const uint32_t *Y)
{
    uint32_t T[8];
    bool f = add256_o(Z, X, Y);
    bool g = sub256_o(T, Z, M);
    uint32_t mask = -(f == g);
    for (int i = 0; i < 8; ++i) {
        Z[i] ^= (Z[i] ^ T[i]) & mask;
    }
}

void modp_sub(uint32_t *Z, const uint32_t *X, const uint32_t *Y)
{
    uint32_t T[8];
    uint32_t mask = -sub256_o(Z, X, Y);
    add256(T, Z, M);
    for (int i = 0; i < 8; ++i) {
        Z[i] ^= (Z[i] ^ T[i]) & mask;
    }
}



static void spmp_mul(uint32_t *z, uint32_t x, const uint32_t *y)
{
    uint32_t crr = 0;
    for (int i = 0; i < 8; ++i) {
        uint64_t product = (uint64_t)x * (uint64_t)y[i] + (uint64_t)crr;
        z[i] = (uint32_t)product;
        crr = (uint32_t)(product >> 32);
    }
    z[8] = crr;
}

void modp_mmul(uint32_t *Z, const uint32_t *X, const uint32_t *Y)
{
/*
    V is a 257-bit register (9-byte)
    S is a 288-bit register (9-byte)
    T is a 289-bit register (10-byte)

    V <- 0
    FOR i = 0 ~ 7 DO
        S <- Y * X[i]                       (288 bits) <- (256 bits) * (32 bits)
        T <- V + S                          (289 bits) <- (257 bits) + (288 bits)
        S <- M * ((uint32_t)(T[0] * L))     (288 bits) <- (256 bits) * (32 bits)
        V <- (T + S) / 2**32                (289 bits) <- (289 bits) + (288 bits)

    W <- V - M
    IF W < 0
        Z <- V
    ELSE
        Z <- W
*/

    // TODO
    (void)Z;
    (void)X;
    (void)Y;

    (void)L;
}
