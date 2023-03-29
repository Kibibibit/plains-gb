#include "frame.h"
#include <stdint.h>
#include <gb/gb.h>
#include "iters.h"
#include "sprite_constants.h"

void frame_set_prop(frame_t *frame, uint8_t oam, uint8_t prop)
{
    uint8_t tile_count = frame->tile_count;
    for (*iter_i = 0; *iter_i < tile_count; *iter_i = *iter_i + 1)
    {
        set_sprite_prop(oam + *iter_i, prop);
    }
}
void frame_draw(frame_t *frame, uint8_t oam, uint8_t x, uint8_t y, uint8_t facing_left)
{
    *iter_i = 0;
    uint8_t xx = 0x0;
    uint8_t yy = 0x0;
    uint8_t y_tiles = FRAME_Y_TILES(frame);
    uint8_t x_tiles = FRAME_X_TILES(frame);

    for (*iter_y = 0; *iter_y < y_tiles; *iter_y = *iter_y + 1)
    {
        for (*iter_x = 0; *iter_x < x_tiles; *iter_x = *iter_x + 1)
        {
            if (facing_left)
            {
                xx = x + ((x_tiles - 1 - *iter_x) << 3);
            }
            else
            {
                xx = x + (*iter_x << 3);
            }

            yy = y + (*iter_y << 3);
            move_sprite(oam + *iter_i, xx, yy);
            *iter_i = *iter_i + 1;
        }
    }
}
void frame_clear_oam(frame_t *frame, uint8_t oam)
{
    uint8_t tile_count = frame->tile_count;
    for (*iter_i = 0; *iter_i < tile_count; *iter_i = *iter_i + 1)
    {
        set_sprite_tile(oam + *iter_i, SPR_BLANK);
        move_sprite(oam + *iter_i, 0, 0);
        set_sprite_prop(oam + *iter_i, 0);
    }
}

uint8_t frame_get_prop(uint8_t oam) {
    return get_sprite_prop(oam);
}