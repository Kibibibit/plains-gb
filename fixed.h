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


fixed_t * new_fixed(int8_t h, uint8_t l);
ufixed_t * new_ufixed(uint8_t h, uint8_t l);
fixed_t * fixed_copy(fixed_t * x);
ufixed_t * ufixed_copy(ufixed_t * x);
void fixed_floor(fixed_t * x);
void fixed_ceil(fixed_t * x);
void ufixed_round(ufixed_t * x);
void ufixed_ceil(ufixed_t * x);
void ufixed_floor(ufixed_t * x);
void fixed_abs(fixed_t * x);
uint8_t abs_ceil_fixed(fixed_t * x);
#endif