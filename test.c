#include <stdio.h>
#include <string.h>
#include "modn.h"
#include "modp.h"

#define DATA1 ((const uint8_t *) "\xac\x8c\x50\x5d\x2c\x48\xc4\xe9\x2b\x32\x0f\xd1\xb3\x10\x84\x32\x19\x41\x28\xec\x73\xf0\x64\x73\xc0\xdf\xf9\xe5\x09\xf3\x30\xcd")
#define DATA2 ((const uint8_t *) "\x7f\x34\x55\x6a\x12\xe6\x98\xd8\x66\x29\xfc\x2d\x35\xac\xf8\x0c\x39\xeb\xec\xe3\x74\x37\x4c\xc0\x3f\x63\x14\x63\x47\x6b\x94\xe2")

static void dumpbuf(const char *label, const uint32_t *Z)
{
    printf("%s = 0x", label);
    for (int i = 7; i >= 0; i--) {
        printf("%02x", (Z[i] >> 24) & 0xff);
        printf("%02x", (Z[i] >> 16) & 0xff);
        printf("%02x", (Z[i] >>  8) & 0xff);
        printf("%02x", (Z[i] >>  0) & 0xff);
    }
    printf("\n");
}

int main(void)
{
    uint32_t X[8] = {0};
    uint32_t Y[8] = {0};
    uint32_t Z[8] = {0};

    puts("M = 0xffffffff00000001000000000000000000000000ffffffffffffffffffffffff");
    memcpy(X, DATA1, 32);   dumpbuf("X", X);
    memcpy(Y, DATA2, 32);   dumpbuf("Y", Y);
    modp_add(Z, X, Y);      dumpbuf("Z", Z);
    puts("print((X + Y - Z) % M == 0)");

    puts("M = 0xffffffff00000001000000000000000000000000ffffffffffffffffffffffff");
    memcpy(X, DATA1, 32);   dumpbuf("X", X);
    memcpy(Y, DATA2, 32);   dumpbuf("Y", Y);
    modp_sub(Z, X, Y);      dumpbuf("Z", Z);
    puts("print((X - Y - Z) % M == 0)");

    puts("M = 0xffffffff00000001000000000000000000000000ffffffffffffffffffffffff");
    memcpy(X, DATA1, 32);   dumpbuf("X", X);
    memcpy(Y, DATA2, 32);   dumpbuf("Y", Y);
    modp_mul(Z, X, Y);     dumpbuf("Z", Z);
    puts("print((X*Y - Z*2**256) % M == 0)");

    puts("M = 0xffffffff00000001000000000000000000000000ffffffffffffffffffffffff");
    puts("R = 2**256");
    memcpy(X, DATA1, 32);   dumpbuf("X", X);
    modp_inv(Z, X);        dumpbuf("Z", Z);
    puts("print((Z*X - R**2)%M == 0)");

    puts("M = 0xffffffff00000000ffffffffffffffffbce6faada7179e84f3b9cac2fc632551");
    memcpy(X, DATA1, 32);   dumpbuf("X", X);
    memcpy(Y, DATA2, 32);   dumpbuf("Y", Y);
    modn_add(Z, X, Y);      dumpbuf("Z", Z);
    puts("print((X + Y - Z) % M == 0)");

    puts("M = 0xffffffff00000000ffffffffffffffffbce6faada7179e84f3b9cac2fc632551");
    memcpy(X, DATA1, 32);   dumpbuf("X", X);
    memcpy(Y, DATA2, 32);   dumpbuf("Y", Y);
    modn_sub(Z, X, Y);      dumpbuf("Z", Z);
    puts("print((X - Y - Z) % M == 0)");

    puts("M = 0xffffffff00000000ffffffffffffffffbce6faada7179e84f3b9cac2fc632551");
    memcpy(X, DATA1, 32);   dumpbuf("X", X);
    memcpy(Y, DATA2, 32);   dumpbuf("Y", Y);
    modn_mul(Z, X, Y);     dumpbuf("Z", Z);
    puts("print((X*Y - Z*2**256) % M == 0)");

    puts("M = 0xffffffff00000000ffffffffffffffffbce6faada7179e84f3b9cac2fc632551");
    puts("R = 2**256");
    memcpy(X, DATA1, 32);   dumpbuf("X", X);
    modn_inv(Z, X);        dumpbuf("Z", Z);
    puts("print((Z*X - R**2)%M == 0)");

    return 0;
}
