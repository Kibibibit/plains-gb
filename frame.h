#ifndef FRAME_H
#define FRAME_H

#include <stdint.h>
typedef struct {
    uint8_t x_tiles: 4;
    uint8_t y_tiles: 4;
    uint8_t tile_count;
    uint8_t * tiles;
} frame_t;


#endif