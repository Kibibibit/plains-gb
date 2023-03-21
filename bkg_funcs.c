
#include <stdint.h>
#include <gb/gb.h>
#include "bkg_funcs.h"

uint8_t * set_bkg_prop_xy(uint8_t x, uint8_t y, uint8_t t)
{
    VBK_REG = 1;
    uint8_t * out = set_bkg_tile_xy(x,y,t);
    VBK_REG = 0;
    return out;
}