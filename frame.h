#ifndef FRAME_H
#define FRAME_H

#include <stdint.h>

#define X_TILES_MASK 0xF0
#define Y_TILES_MASK 0x0F
typedef struct {
    uint8_t size;
    uint8_t tile_count;
    uint8_t * tiles;
} frame_t;

uint8_t frame_x_tiles(const frame_t * frame);
uint8_t frame_y_tiles(const frame_t * frame);

#endif