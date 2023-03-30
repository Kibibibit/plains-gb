#include "frame.h"
#include <stdint.h>
#include <gb/gb.h>
#include "iters.h"
#include "sprite_constants.h"

void frame_set_prop(const frame_t *frame, uint8_t oam, uint8_t prop)
{
    uint8_t tile_count = frame->tile_count;
    for (*iter0 = 0; *iter0 < tile_count; *iter0 = *iter0 + 1)
    {
        set_sprite_prop(oam + *iter0, prop);
    }
}
void frame_draw(const frame_t *frame, uint8_t oam, uint8_t x, uint8_t y, uint8_t facing_left)
{
    *iter0 = 0;
    uint8_t xx = 0x0;
    uint8_t yy = 0x0;
    uint8_t y_tiles = FRAME_Y_TILES(frame);
    uint8_t x_tiles = FRAME_X_TILES(frame);

    for (*iter1 = 0; *iter1 < y_tiles; *iter1 = *iter1 + 1)
    {
        for (*iter2 = 0; *iter2 < x_tiles; *iter2 = *iter2 + 1)
        {
            if (facing_left)
            {
                xx = x + ((x_tiles - 1 - *iter2) << 3);
            }
            else
            {
                xx = x + (*iter2 << 3);
            }

            yy = y + (*iter1 << 3);
            move_sprite(oam + *iter0, xx, yy);
            *iter0 = *iter0 + 1;
        }
    }
}
void frame_clear_oam(const frame_t *frame, uint8_t oam)
{
    uint8_t tile_count = frame->tile_count;
    for (*iter0 = 0; *iter0 < tile_count; *iter0 = *iter0 + 1)
    {
        set_sprite_tile(oam + *iter0, SPR_BLANK);
        move_sprite(oam + *iter0, 0, 0);
        set_sprite_prop(oam + *iter0, 0);
    }
}

uint8_t frame_get_prop(uint8_t oam) {
    return get_sprite_prop(oam);
}

void frame_hide(const frame_t * frame, uint8_t oam) 
{
    uint8_t tile_count = frame->tile_count;
    for (*iter0 = 0; *iter0 < tile_count; *iter0 = *iter0 +1)
    {
        move_sprite(oam + *iter0, 0, 0);
    }
}