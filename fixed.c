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

