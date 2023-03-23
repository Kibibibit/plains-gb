
#include <stdint.h>
#include <gb/gb.h>
#include "bkg_funcs.h"
#include "constants.h"

uint8_t * set_bkg_prop_xy(uint8_t x, uint8_t y, uint8_t t)
{
    VBK_REG = 1;
    uint8_t * out = set_bkg_tile_xy(x,y,t);
    VBK_REG = 0;
    return out;
}

void * set_bkg_props(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t * t)
{
    VBK_REG = 1;
    set_bkg_tiles(x,y,w,h,t);
    VBK_REG = 0;
}

uint8_t tile_x(uint8_t x) {
    uint8_t out = x;
    out -= 8;
    out = out >> 3;
    return out;
}

uint8_t tile_y(uint8_t y) {
    uint8_t out = y;
    out -= 16;
    out = out >> 3;
    return out;
}

uint8_t tile_at(uint8_t x, uint8_t y) {
    uint8_t tx = tile_x(x);
    uint8_t ty = tile_y(y);
    
    return get_bkg_tile_xy(tx,ty);
}
uint8_t solid_at(uint8_t x, uint8_t y) {
    uint8_t out = tile_at(x,y);
    out = out & SOLID_FLAG;
    return out == SOLID_FLAG;
}

uint8_t break_at(uint8_t x, uint8_t y) {
    uint8_t out = tile_at(x,y);
    out = out & BREAK_FLAG;
    return out == BREAK_FLAG;
}

void break_tile(uint8_t x, uint8_t y) {
    uint8_t x0 = tile_x(x);
    uint8_t y0 = tile_y(y);
    set_bkg_tile_xy(x0,y0, TILE_INDEX_SKY_BG);
    set_bkg_prop_xy(x0,y0, TILE_PAL_INDEX_BLANK);
}