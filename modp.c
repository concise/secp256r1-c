#include <stdbool.h>
#include "addsub.h"
#include "modp.h"

// M = 0xffffffff00000001000000000000000000000000ffffffffffffffffffffffff
static const uint32_t M[8] = {
    0xffffffff, 0xffffffff, 0xffffffff, 0x00000000,
    0x00000000, 0x00000000, 0x00000001, 0xffffffff,
    };

// L = -M^-1 % 2^32
static const uint32_t L = 0x00000001;

// K = M - 2
static const _Bool K[256] = {
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,
    };

// J = R^2 % M
//   = 0x00000004fffffffdfffffffffffffffefffffffbffffffff0000000000000003
static const uint32_t J[8] = {
    0x00000003, 0x00000000, 0xffffffff, 0xfffffffb,
    0xfffffffe, 0xffffffff, 0xfffffffd, 0x00000004,
    };

// I = 1
static const uint32_t I[8] = { 1, 0, 0, 0, 0, 0, 0, 0, };

void modp_add(uint32_t *Z, const uint32_t *X, const uint32_t *Y)
{
    uint32_t T[8];
    bool f = add256_o(Z, X, Y);
    bool g = sub256_o(T, Z, M);
    uint32_t mask = -(f == g);
    for (int i = 0; i < 8; i++) {
        Z[i] ^= (Z[i] ^ T[i]) & mask;
    }
}

void modp_sub(uint32_t *Z, const uint32_t *X, const uint32_t *Y)
{
    uint32_t T[8];
    uint32_t mask = -sub256_o(Z, X, Y);
    add256(T, Z, M);
    for (int i = 0; i < 8; i++) {
        Z[i] ^= (Z[i] ^ T[i]) & mask;
    }
}

void modp_mmul(uint32_t *Z, const uint32_t *X, const uint32_t *Y)
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

void modp_minv(uint32_t *Z, const uint32_t *X)
{
    uint32_t T[8];
    for (int i = 0; i < 8; i++) {
        T[i] = X[i];
    }
    for (int i = 1; i < 256; i++) {
        modp_mmul(T, T, T);
        if (K[i]) {
            modp_mmul(T, T, X);
        }
    }
    for (int i = 0; i < 8; i++) {
        Z[i] = T[i];
    }
}

void convert_32_u8_objects_to_8_u32_objects(uint32_t *out, const uint8_t *inp)
{
#define i (uint32_t)inp
    out[0] = i[28] << 24 | i[29] << 16 | i[30] << 8 | i[31] ;
    out[1] = i[24] << 24 | i[25] << 16 | i[26] << 8 | i[27] ;
    out[2] = i[20] << 24 | i[21] << 16 | i[22] << 8 | i[23] ;
    out[3] = i[16] << 24 | i[17] << 16 | i[18] << 8 | i[19] ;
    out[4] = i[12] << 24 | i[13] << 16 | i[14] << 8 | i[15] ;
    out[5] = i[8]  << 24 | i[9]  << 16 | i[10] << 8 | i[11] ;
    out[6] = i[4]  << 24 | i[5]  << 16 | i[6]  << 8 | i[7]  ;
    out[7] = i[0]  << 24 | i[1]  << 16 | i[2]  << 8 | i[3]  ;
#undef i
}
