#include "addsub.h"

bool add256_io(uint32_t *z, const uint32_t *x, const uint32_t *y, bool cflag)
{
    for (int i = 0; i < 8; i++) {
        uint64_t tmp = (uint64_t)x[i] + y[i] + cflag;
        z[i] = tmp & 0xffffffff;
        cflag = tmp >> 32;
    }
    return cflag;
}

bool add256_o(uint32_t *z, const uint32_t *x, const uint32_t *y)
{
    return add256_io(z, x, y, 0);
}

void add256_i(uint32_t *z, const uint32_t *x, const uint32_t *y, bool cflag)
{
    add256_io(z, x, y, cflag);
}

void add256(uint32_t *z, const uint32_t *x, const uint32_t *y)
{
    add256_io(z, x, y, 0);
}

bool sub256_io(uint32_t *z, const uint32_t *x, const uint32_t *y, bool bflag)
{
    for (int i = 0; i < 8; i++) {
        uint64_t tmp = (uint64_t)x[i] - y[i] - bflag;
        z[i] = tmp & 0xffffffff;
        bflag = tmp >> 32;
    }
    return bflag;
}

bool sub256_o(uint32_t *z, const uint32_t *x, const uint32_t *y)
{
    return sub256_io(z, x, y, 0);
}

void sub256_i(uint32_t *z, const uint32_t *x, const uint32_t *y, bool bflag)
{
    sub256_io(z, x, y, bflag);
}

void sub256(uint32_t *z, const uint32_t *x, const uint32_t *y)
{
    sub256_io(z, x, y, 0);
}

void mul_to_288_from_32_256(uint32_t *z, uint32_t x, const uint32_t *y)
{
    uint32_t crr = 0;
    for (int i = 0; i < 8; i++) {
        uint64_t product = (uint64_t)x * y[i] + crr;
        z[i] = product & 0xffffffff;
        crr = product >> 32;
    }
    z[8] = crr;
}
