#ifndef _FRAME_H
#define _FRAME_H

#include <stdint.h>
#include "helpers.h"

#define FRAME_X_TILES(frame) nibble_high(frame->size)
#define FRAME_Y_TILES(frame) nibble_low(frame->size)
typedef struct {
    uint8_t size;
    uint8_t tile_count;
    uint8_t * tiles;
} frame_t;

void frame_set_prop(const frame_t * frame, uint8_t oam, uint8_t prop);
void frame_draw(const frame_t * frame, uint8_t oam, uint8_t x, uint8_t y, uint8_t facing_left);
void frame_clear_oam(const frame_t * frame, uint8_t oam);
uint8_t frame_get_prop(const uint8_t oam);
void frame_hide(const frame_t * frame, uint8_t oam);

#endif