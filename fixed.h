#ifndef FIXED_H
#define FIXED_H

#include <stdint.h>

typedef union _fixed_t
{
    int16_t w;
    struct
    {
        uint8_t l;
        int8_t h;
    };
} fixed_t;

typedef union _ufixed_t
{
    uint16_t w;
    struct
    {
        uint8_t l;
        uint8_t h;
    };

} ufixed_t;


fixed_t new_fixed(int8_t h, uint8_t l);
ufixed_t new_ufixed(uint8_t h, uint8_t l);
fixed_t fixed_floor(fixed_t x);
ufixed_t ufixed_round(ufixed_t x);
ufixed_t ufixed_ceil(ufixed_t x);
ufixed_t ufixed_floor(ufixed_t x);
ufixed_t fixed_abs(fixed_t x);
#endif