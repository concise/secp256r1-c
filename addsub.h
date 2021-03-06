#ifndef ADDSUB_H_INCLUDED
#define ADDSUB_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>

// ZZ + (2**256) Co <- XX + YY + Ci
bool add256_io(uint32_t *z, const uint32_t *x, const uint32_t *y, bool cflag);
bool add256_o(uint32_t *z, const uint32_t *x, const uint32_t *y);
void add256_i(uint32_t *z, const uint32_t *x, const uint32_t *y, bool i);
void add256(uint32_t *z, const uint32_t *x, const uint32_t *y);

// ZZ - (2**256) Bo <- XX - YY - Bi
bool sub256_io(uint32_t *z, const uint32_t *x, const uint32_t *y, bool bflag);
bool sub256_o(uint32_t *z, const uint32_t *x, const uint32_t *y);
void sub256_i(uint32_t *z, const uint32_t *x, const uint32_t *y, bool bflag);
void sub256(uint32_t *z, const uint32_t *x, const uint32_t *y);

// ZZ <- x YY
void mul_to_288_from_32_256(uint32_t *z, uint32_t x, const uint32_t *y);

#endif
