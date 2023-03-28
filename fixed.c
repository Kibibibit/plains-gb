#include <stdint.h>
#include "macros.h"
#include "fixed.h"

fixed_t *new_fixed(int8_t h, uint8_t l)
{
    fixed_t *out = MALLOC(fixed_t);
    out->h = h;
    out->l = l;
    return out;
}

ufixed_t *new_ufixed(uint8_t h, uint8_t l)
{
    ufixed_t *out = MALLOC(ufixed_t);
    out->h = h;
    out->l = l;
    return out;
}

fixed_t *fixed_copy(fixed_t *x)
{
    return new_fixed(x->h, x->l);
}
ufixed_t *ufixed_copy(ufixed_t *x)
{
    return new_ufixed(x->h, x->l);
}

void fixed_floor(fixed_t *x)
{

    if (x->h < 0)
    {
        x->h -= 1;
    }
    x->l = 0;
}

void fixed_ceil(fixed_t * x)
{
    if (x->l != 0)
    {
        x->h += 1;
    }
    x->l = 0;
}

void ufixed_round(ufixed_t *x)
{
    if (x->l >= 128)
    {
        x->h += 1;
    }
    x->l = 0;
}

void ufixed_ceil(ufixed_t *x)
{
    if (x->l != 0)
    {
        x->h += 1;
    }
    x->l = 0;
}

void ufixed_floor(ufixed_t *x)
{
    x->l = 0;
}

void fixed_abs(fixed_t *x)
{
    fixed_t *mask = new_fixed(0xFF, 0xFF);
    x->w = x->w ^ mask->w;
    x->w -= mask->w;
}