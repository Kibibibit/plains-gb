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


#endif