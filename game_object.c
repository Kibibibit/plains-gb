#include <stdint.h>
#include <stdlib.h>
#include <gb/gb.h>
#include <gbdk/emu_debug.h>
#include "game_object.h"
#include "frame_constants.h"
#include "sprite_constants.h"
#include "fixed.h"
#include "macros.h"
#include "iters.h"

game_object_t *create_game_object()
{
    game_object_t *out = MALLOC(game_object_t);
    out->x.w = 0x0;
    out->y.w = 0x0;
    out->dx.w = 0x0;
    out->dy.w = 0x0;
    out->facing_left = 0x0;
    out->gravity = 0x1;
    out->draw_order = 0x1;
    out->prop = 0x0;
    out->top = 0x0;
    out->bottom = 0x8;
    out->left = 0x0;
    out->right = 0x8;
    out->oam = 0x0;
    out->frame = blank_frame;

    return out;
}
void destroy_game_object(game_object_t *object)
{
    game_object_clear_oam(object);
    free(object);
}
void game_object_update(game_object_t *object)
{
    if (object->gravity)
    {
        object->dy.w += GRAVITY;
    }
}
void game_object_set_prop(game_object_t *object, uint8_t prop)
{
    uint8_t i;
    for (i = 0; i < object->frame->tile_count; i++)
    {
        set_sprite_prop(object->oam + i, prop);
    }
}

void game_object_clear_oam(game_object_t *object)
{
    uint8_t i;
    for (i = 0; i < object->frame->tile_count; i++)
    {
        set_sprite_tile(object->oam + i, SPR_BLANK);
        move_sprite(object->oam + i, 0, 0);
        set_sprite_prop(object->oam + i, 0);
    }
}

void game_object_set_frame(game_object_t *object, const frame_t *frame)
{
    if (object->frame->tile_count != frame->tile_count)
    {
        game_object_clear_oam(object);
    }
    uint8_t i;
    object->frame = frame;
    for (i = 0; i < object->frame->tile_count; i++)
    {
        set_sprite_tile(object->oam + i, frame->tiles[i]);
    }
}

void game_object_draw(game_object_t *object)
{
    *iter_i = 0;

    uint8_t xx = 0x0;
    uint8_t yy = 0x0;

    uint8_t x_tiles = frame_x_tiles(object->frame);

    for (*iter_y = 0; *iter_y < frame_y_tiles(object->frame); *iter_y = *iter_y+1)
    {
        for (*iter_x = 0;*iter_x < x_tiles; *iter_x = *iter_x+1) {
            if (object->facing_left) {
                xx = object->x.h+((x_tiles-1-*iter_x) << 3);
            } else {
                xx = object->x.h + (*iter_x << 3);
            }
            
            yy = object->y.h + (*iter_y << 3);
            move_sprite(object->oam+*iter_i, xx, yy);
            *iter_i = *iter_i + 1;
        }
    }
}
uint8_t game_object_collides_at(game_object_t *object, uint8_t x, uint8_t y);
