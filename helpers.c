#include "helpers.h"
#include <stdint.h>

uint8_t nibble_high(uint8_t byte)
{
    return (byte >> 4) & 0xF;
}
uint8_t nibble_low(uint8_t byte)
{
    return byte & 0xF;
}