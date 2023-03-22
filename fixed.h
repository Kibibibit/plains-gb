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

#endif