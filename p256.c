/// The definition of the elliptic curve group secp256r1 can be found here
///
///     https://www.secg.org/sec1-v2.pdf
///     https://www.secg.org/sec2-v2.pdf
///
/// Three ways to represent a field element in a large prime field GF(p) where
///
///     p == 2**256 - 2**224 + 2**192 + 2**96 - 1
///       == 0xffffffff00000001000000000000000000000000ffffffffffffffffffffffff
///
///     #1: an integer modulo p
///     #2: an integer modulo p in the Montgomery domain
///     #3: an octet string (most significant octet first)
///
/// We implement these four field operations: x+y x+(-y) x*y x^-1
///
/// We also have to deal with the prime field GF(n) where
///
///     n == 0xffffffff00000000ffffffffffffffffbce6faada7179e84f3b9cac2fc632551

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct GFp {
    uint32_t blocks[8];
};

void GFp_to_octets(const struct GFp *target, uint8_t *source);
void GFp_to_octets(const struct GFp *target, uint8_t *source)
{
    // TODO
    (void)target;
    (void)source;
}

void octets_to_GFp(const uint8_t *source, struct GFp *target);
void octets_to_GFp(const uint8_t *source, struct GFp *target)
{
    // TODO
    (void)target;
    (void)source;
}





// C <- A + B mod M
void modm_add(uint32_t *C, const uint32_t *A, const uint32_t *B);

// C <- A - B mod M
void modm_sub(uint32_t *C, const uint32_t *A, const uint32_t *B);

// Z <- X * Y * 2^-256 mod M
void modm_montmul(uint32_t *Z, const uint32_t *X, const uint32_t *Y);





static const uint32_t M[8] = {
    0xffffffff, 0xffffffff, 0xffffffff, 0x00000000,
    0x00000000, 0x00000000, 0x00000001, 0xffffffff };

//
// crri
// a[0] a[1] ...... a[n-2] a[n-1]
// b[0] b[1] ...... b[n-2] b[n-1]      (+
// --------------------------------------
// c[0] c[1] ...... c[n-2] c[n-1] crro
//
// "add-with-carry" style implementation
//
static bool mpmp_add(uint32_t *c, const uint32_t *a, const uint32_t *b, int n,
                     bool cflag)
{
    for (int i = 0; i < n; ++i) {
        uint64_t tmp = (uint64_t)a[i] + (uint64_t)b[i] + (uint64_t)cflag;
        c[i] = (uint32_t)tmp;
        cflag = (bool)(tmp >> 32);
    }
    return cflag;
}




// u256_u256____addto_u256_co
// u256_u256_ci_addto_u256_co
// u288_u288____addto_u288_co
// u256_sub





//
// a[0] a[1] ...... a[6] a[7]
// b[0] b[1] ...... b[6] b[7]      (-
// ----------------------------------
// c[0] c[1] ...... c[6] c[7] crro
//
// "subtract-with-carry" style implementation
//
static bool mpmp_sub(uint32_t *c, const uint32_t *a, const uint32_t *b)
{
    bool cflag = 0;
    for (int i = 0; i < 8; ++i) {
        uint64_t tmp = (uint64_t)a[i] - (uint64_t)b[i] - (uint64_t)cflag;
        c[i] = (uint32_t)tmp;
        cflag = (bool)(tmp >> 32);
    }
    return cflag;
}

//
// b[0] b[1] ...... b[6] b[7]
// a                               (x
// ----------------------------------
// c[0] c[1] ...... c[6] c[7] c[8]
//
static void spmp_mul(uint32_t *c, uint32_t a, const uint32_t *b)
{
    uint32_t crr = 0;
    for (int i = 0; i < 8; ++i) {
        uint64_t product = (uint64_t)a * (uint64_t)b[i] + (uint64_t)crr;
        c[i] = (uint32_t)product;
        crr = (uint32_t)(product >> 32);
    }
    c[8] = crr;
}

void modm_add(uint32_t *C, const uint32_t *A, const uint32_t *B)
{
    // D <- A + B
    uint32_t D[9];
    D[8] = mpmp_add(D, A, B, 8, 0);

    // E <- A + B - M
    uint32_t E[9];
    E[8] = D[8] - mpmp_sub(E, D, M);

    // C <- (A + B - M < 0) ? D : E;
    uint32_t mask_D = -(!!E[8]);
    uint32_t mask_E = ~mask_D;
    int i;
    for (i = 0; i < 8; ++i)
        C[i] = (D[i] & mask_D) ^ (E[i] & mask_E);


    uint32_t swap;
    uint32_t T[8];
    swap = -mpmp_add(C, A, B, 8, 0);
}

void modm_sub(uint32_t *C, const uint32_t *A, const uint32_t *B)
{
    uint32_t T[8];
    uint32_t swap = -mpmp_sub(C, A, B);
    mpmp_add(T, C, M, 8, 0);
    for (int i = 0; i < 8; ++i) {
        C[i] ^= (C[i] ^ T[i]) & swap;
    }
}

void modm_montmul(uint32_t *Z, const uint32_t *X, const uint32_t *Y)
{
    uint32_t S[9];
    uint32_t T[10];
    uint32_t V[9] = {0};
    uint32_t W[9];

    for (int i = 0; i < 8; ++i) {
        spmp_mul(S, X[i], Y);
        T[9] = mpmp_add(T, V, S, 9, 0);
        spmp_mul(S, T[0] * 1, M);
        V[8] = mpmp_add(V, T + 1, S + 1, 8, !!T[0]) + T[9];
    }
    W[8] = V[8] - mpmp_sub(W, V, M);

    uint32_t maskv = -(!!W[8]);
    uint32_t maskw = ~maskv;
    for (int i = 0; i < 8; ++i)
        Z[i] = (maskv & V[i]) ^ (maskw & W[i]);
}

static void hexstr_to_u256(const char *str, uint32_t *x)
{
    int i;
    for (i = 0; i < 8; ++i)
        x[i] = 0;
    for (i = 0; i < 64; ++i) {
        char c = str[i];
        uint32_t val = (uint32_t) (c < 'a' ? c - '0' : c - 'a' + 10);
        x[7 - (i >> 3)] |= val << (4 * (7 - i & 0x7));
    }
}

static void print_u256(const char *name, const uint32_t *x)
{
    int i;
    printf("%s = 0x", name);
    for (i = 7; i >= 0; --i)
        printf("%08x", x[i]);
    printf("\n");
}

int main(void)
{
    uint32_t X[8];
    uint32_t Y[8];

    hexstr_to_u256("3b6859c358bb6fa30b3f11ff6c29164d"
                   "c21b2abaf4c2027ea8e6701e99dd5b7c", X);
    hexstr_to_u256("d7dab791a8647ac88695e20e29960a6f"
                   "d41707258cc88cc0480ea7e5bb3f132f", Y);

    uint32_t Z[8];

    modm_montmul(Z, X, Y);

    print_u256("M", M);
    print_u256("X", X);
    print_u256("Y", Y);
    print_u256("Z", Z);
    printf("(X * Y - Z * 2**256) %% M == 0\n");

    return 0;
}
