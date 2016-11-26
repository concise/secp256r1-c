#include <stdbool.h>
#include "addsub.h"
#include "modn.h"

// index bits starting from the most significant bit
#define FROM_M_MINUS_TWO_EXTRACT_BIT(i) (((const unsigned char *) \
    "\xff\xff\xff\xff\x00\x00\x00\x00\xff\xff\xff\xff\xff\xff\xff\xff" \
    "\xbc\xe6\xfa\xad\xa7\x17\x9e\x84\xf3\xb9\xca\xc2\xfc\x63\x25\x4f" \
    )[i >> 3] >> (~i & 7) & 1)

#define L 0xee00bc4f

// 0xffffffff00000000ffffffffffffffffbce6faada7179e84f3b9cac2fc632551
static const uint32_t M[8] = {
    0xfc632551, 0xf3b9cac2, 0xa7179e84, 0xbce6faad,
    0xffffffff, 0xffffffff, 0x00000000, 0xffffffff };

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

void modn_inv(uint32_t *Z, const uint32_t *X)
{
    uint32_t T[8];
    for (int i = 0; i < 8; i++) {
        T[i] = X[i];
    }
    for (int i = 1; i < 256; i++) {
        modn_mmul(T, T, T);
        if (FROM_M_MINUS_TWO_EXTRACT_BIT(i)) {
            modn_mmul(T, T, X);
        }
    }
    for (int i = 0; i < 8; i++) {
        Z[i] = T[i];
    }
}
