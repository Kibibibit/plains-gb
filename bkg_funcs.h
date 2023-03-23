#ifndef BKG_FUNCS_H
#define BKG_FUNCS_H

#include <stdint.h>
#include <gb/gb.h>

uint8_t * set_bkg_prop_xy(uint8_t x, uint8_t y, uint8_t t);
void * set_bkg_props(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t * t);

uint8_t tile_x(uint8_t x);
uint8_t tile_y(uint8_t y);
uint8_t tile_at(uint8_t x, uint8_t y);
uint8_t solid_at(uint8_t x, uint8_t y);
uint8_t break_at(uint8_t x, uint8_t y);
void break_tile(uint8_t x, uint8_t y);
#endif