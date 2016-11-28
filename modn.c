#include <stdbool.h>
#include "addsub.h"
#include "modn.h"

// M = 0xffffffff00000000ffffffffffffffffbce6faada7179e84f3b9cac2fc632551
static const uint32_t M[8] = {
    0xfc632551, 0xf3b9cac2, 0xa7179e84, 0xbce6faad,
    0xffffffff, 0xffffffff, 0x00000000, 0xffffffff,
    };

// L = -M^-1 % 2^32
static const uint32_t L = 0xee00bc4f;

// K = M - 2
static const _Bool K[256] = {
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,0,1,1,1,1,0,0,1,1,1,0,0,1,1,0,1,1,1,1,1,0,1,0,1,0,1,0,1,1,0,1,
    1,0,1,0,0,1,1,1,0,0,0,1,0,1,1,1,1,0,0,1,1,1,1,0,1,0,0,0,0,1,0,0,
    1,1,1,1,0,0,1,1,1,0,1,1,1,0,0,1,1,1,0,0,1,0,1,0,1,1,0,0,0,0,1,0,
    1,1,1,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,1,0,0,1,0,1,0,1,0,0,1,1,1,1,
    };

// J = R^2 % M
//   = 0x66e12d94f3d956202845b2392b6bec594699799c49bd6fa683244c95be79eea2
static const uint32_t J[8] = {
    0xbe79eea2, 0x83244c95, 0x49bd6fa6, 0x4699799c,
    0x2b6bec59, 0x2845b239, 0xf3d95620, 0x66e12d94,
    };

// I = 1
static const uint32_t I[8] = { 1, 0, 0, 0, 0, 0, 0, 0, };

void modn_add(uint32_t *Z, const uint32_t *X, const uint32_t *Y)
{
    uint32_t T[8];
    bool f = add256_o(Z, X, Y);
    bool g = sub256_o(T, Z, M);
    uint32_t mask = -(f == g);
    for (int i = 0; i < 8; i++) {
        Z[i] ^= (Z[i] ^ T[i]) & mask;
    }
}

void modn_sub(uint32_t *Z, const uint32_t *X, const uint32_t *Y)
{
    uint32_t T[8];
    uint32_t mask = -sub256_o(Z, X, Y);
    add256(T, Z, M);
    for (int i = 0; i < 8; i++) {
        Z[i] ^= (Z[i] ^ T[i]) & mask;
    }
}

void modn_mmul(uint32_t *Z, const uint32_t *X, const uint32_t *Y)
{
    uint32_t S[9];
    uint32_t T[9];
    uint32_t V[8] = {0};
    bool t9;
    bool v8 = 0;
    for (int i = 0; i < 8; i++) {
        mul_to_288_from_32_256(S, X[i], Y);
        uint64_t tmp_sum = (uint64_t)S[8] + v8 + add256_o(T, S, V);
        T[8] = tmp_sum & 0xffffffff;
        t9 = tmp_sum >> 32;
        mul_to_288_from_32_256(S, T[0]*L, M);
        v8 = t9 | add256_io(V, T+1, S+1, T[0]);
    }
    bool bf = sub256_o(Z, V, M);
    uint32_t mask = -(v8 != bf);
    for (int i = 0; i < 8; i++) {
        Z[i] ^= (Z[i] ^ V[i]) & mask;
    }
}

void modn_minv(uint32_t *Z, const uint32_t *X)
{
    uint32_t T[8];
    for (int i = 0; i < 8; i++) {
        T[i] = X[i];
    }
    for (int i = 1; i < 256; i++) {
        modn_mmul(T, T, T);
        if (K[i]) {
            modn_mmul(T, T, X);
        }
    }
    for (int i = 0; i < 8; i++) {
        Z[i] = T[i];
    }
}
