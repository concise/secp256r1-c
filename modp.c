#include <stdbool.h>
#include "addsub.h"
#include "modp.h"

#define L 0x00000001

static const uint32_t M[8] = {
    0xffffffff, 0xffffffff, 0xffffffff, 0x00000000,
    0x00000000, 0x00000000, 0x00000001, 0xffffffff };

void modp_add(uint32_t *Z, const uint32_t *X, const uint32_t *Y)
{
    uint32_t T[8];
    bool f = add256_o(Z, X, Y);
    bool g = sub256_o(T, Z, M);
    uint32_t mask = -(f == g);
    for (int i = 0; i < 8; ++i) {
        Z[i] = Z[i] ^ ((Z[i] ^ T[i]) & mask);
    }
}

void modp_sub(uint32_t *Z, const uint32_t *X, const uint32_t *Y)
{
    uint32_t T[8];
    uint32_t mask = -sub256_o(Z, X, Y);
    add256(T, Z, M);
    for (int i = 0; i < 8; ++i) {
        Z[i] = Z[i] ^ ((Z[i] ^ T[i]) & mask);
    }
}

static void spmp_mul(uint32_t *z, uint32_t x, const uint32_t *y)
{
    uint32_t crr = 0;
    for (int i = 0; i < 8; ++i) {
        uint64_t product = (uint64_t)x * y[i] + crr;
        z[i] = (uint32_t)product;
        crr = (uint32_t)(product >> 32);
    }
    z[8] = crr;
}

void modp_mmul(uint32_t *Z, const uint32_t *X, const uint32_t *Y)
{
    uint32_t S[9];
    uint32_t T[9];
    uint32_t V[8] = {0};
    bool t9;
    bool v8 = 0;
    for (int i = 0; i < 8; ++i) {
        spmp_mul(S, X[i], Y);
        uint64_t tmp_sum = (uint64_t)S[8] + v8 + add256_o(T, S, V);
        T[8] = (uint32_t)tmp_sum;
        t9 = tmp_sum >> 32;
        spmp_mul(S, T[0] * L, M);
        v8 = t9 | add256_io(V, T+1, S+1, T[0]);
    }
    uint32_t mask = -(v8 != sub256_o(Z, V, M));
    for (int i = 0; i < 8; ++i) {
        Z[i] = Z[i] ^ ((Z[i] ^ V[i]) & mask);
    }
}
