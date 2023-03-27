#include <stdint.h>
#include "fixed.h"

fixed_t new_fixed(int8_t h, uint8_t l)
{
    fixed_t out;
    out.h = h;
    out.l = l;
    return out;
}

ufixed_t new_ufixed(uint8_t h, uint8_t l)
{
    ufixed_t out;
    out.h = h;
    out.l = l;
    return out;
}

fixed_t fixed_floor(fixed_t x) {
    fixed_t out;
    out.l = 0;
    if (x.h >= 0) {
        out.h = x.h;
    } else {
        out.h = x.h-1;
    }
    return out;
}

ufixed_t ufixed_round(ufixed_t x) {
    ufixed_t out;
    out.l = 0;
    if (x.l >= 128) {
        out.h = x.h+1;
    } else {
        out.h = x.h;
    }
    return out;
}

ufixed_t ufixed_ceil(ufixed_t x) {
    ufixed_t out;
    out.l = 0;
    if (x.l == 0) {
        out.h = x.h;
    } else {
        out.h = x.h+1;
    }
    return out;
}

ufixed_t ufixed_floor(ufixed_t x) {
    ufixed_t out;
    out.l = 0;
    out.h = x.h;
    return out;
}

ufixed_t fixed_abs(fixed_t x) {
    ufixed_t out;
    ufixed_t mask;
    mask.w = x.w >> 15;
    out.w = x.w ^ mask.w;
    out.w -= mask.w;
    return out;
}