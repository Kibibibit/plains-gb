#include "frame.h"
#include <stdint.h>

uint8_t frame_x_tiles(const frame_t * frame)
{
    return (frame->size & X_TILES_MASK) >> 4;
}
uint8_t frame_y_tiles(const frame_t * frame)
{
    return frame->size & Y_TILES_MASK;
}