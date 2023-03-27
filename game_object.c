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
#include "maths.h"
#include "bkg_funcs.h"

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
    out->timer = 0x0;
    out->tb = 0x08;
    out->lr = 0x08;
    out->oam = 0x0;
    out->frame = blank_frame;
    out->on_floor = 0x0;
    out->do_draw = 0x1;

    return out;
}
void destroy_game_object(game_object_t *object)
{
    game_object_clear_oam(object);
    free(object);
}
void game_object_update(game_object_t *object)
{
    uint8_t x = object->x.h;
    uint8_t y = object->y.h;

    object->timer++;
    if (object->dy.w != 0)
    {
        object->dy.w += GRAVITY;
        object->on_floor = 0x0;
    }
    else if (!game_object_on_floor(object))
    {
        object->dy.w += GRAVITY;
        object->on_floor = 0x0;
    } else {
        object->on_floor = 0x1;
    }

    if (object->dy.w != 0)
    {
        if (object->dy.h > 7) {
            object->dy.h = 7;
        }
        if (game_object_vert_will_collide(object))
        {
            object->y = ufixed_floor(object->y);
            int8_t s = sign(object->dy.h);
            uint8_t r = ufixed_ceil(fixed_abs(object->dy)).h;
            object->dy.h = s;
            object->dy.l = 0;
            for (*iter_y = 0; *iter_y < r; *iter_y = *iter_y + 1)
            {
                if (!game_object_vert_will_collide(object))
                {
                    object->y.h += s;
                }
                else
                {
                    break;
                }
            }
            object->dy.w = 0;
        }
        else
        {
            object->y.w += object->dy.w;
        }
    }

    if (object->dx.w != 0)
    {
        if (game_object_hori_will_collide(object))
        {
            object->x = ufixed_floor(object->x);
            int8_t s = sign(object->dx.h);
            uint8_t r = ufixed_ceil(fixed_abs(object->dx)).h;
            object->dx.h = s;
            object->dx.l = 0;
            for (*iter_x = 0; *iter_x < r; *iter_x = *iter_x + 1)
            {
                if (!game_object_hori_will_collide(object))
                {
                    object->x.h += s;
                }
                else
                {
                    break;
                }
            }
            object->dx.w = 0;
        }
        else
        {
            object->x.w += object->dx.w;
        }
    }
    if (object->x.h != x || object->y.h != y)
    {
        object->do_draw = 0x1;
    }
}
void game_object_set_prop(game_object_t *object, uint8_t prop)
{
    for (*iter_i = 0; *iter_i < object->frame->tile_count; *iter_i = *iter_i + 1)
    {
        set_sprite_prop(object->oam + *iter_i, prop);
    }
}

void game_object_clear_oam(game_object_t *object)
{
    for (*iter_i = 0; *iter_i < object->frame->tile_count; *iter_i = *iter_i + 1)
    {
        set_sprite_tile(object->oam + *iter_i, SPR_BLANK);
        move_sprite(object->oam + *iter_i, 0, 0);
        set_sprite_prop(object->oam + *iter_i, 0);
    }
}

void game_object_set_frame(game_object_t *object, const frame_t *frame)
{
    if (object->frame == frame) {
        return;
    }
    if (object->frame->tile_count != frame->tile_count)
    {
        game_object_clear_oam(object);
    }
    object->frame = frame;
    for (*iter_i = 0; *iter_i < object->frame->tile_count; *iter_i = *iter_i + 1)
    {
        set_sprite_tile(object->oam + *iter_i, frame->tiles[*iter_i]);
    }
}

void game_object_draw(game_object_t *object)
{
    if (object->do_draw)
    {
        *iter_i = 0;

        uint8_t xx = 0x0;
        uint8_t yy = 0x0;
        uint8_t y_tiles = FRAME_Y_TILES(object->frame);
        uint8_t x_tiles = FRAME_X_TILES(object->frame);

        for (*iter_y = 0; *iter_y < y_tiles; *iter_y = *iter_y + 1)
        {
            for (*iter_x = 0; *iter_x < x_tiles; *iter_x = *iter_x + 1)
            {
                if (object->facing_left)
                {
                    xx = object->x.h + ((x_tiles - 1 - *iter_x) << 3);
                }
                else
                {
                    xx = object->x.h + (*iter_x << 3);
                }

                yy = object->y.h + (*iter_y << 3);
                move_sprite(object->oam + *iter_i, xx, yy);
                *iter_i = *iter_i + 1;
            }
        }
        object->do_draw = 0x0;
    }
}

uint8_t game_object_vert_will_collide(game_object_t *object)
{
    ufixed_t y;
    y.w = object->y.w + object->dy.w;
    uint8_t y0 = y.h + OBJECT_TOP(object);
    uint8_t y1 = y.h + OBJECT_BOTTOM(object);
    uint8_t l = OBJECT_LEFT(object);
    uint8_t r = OBJECT_RIGHT(object);
    uint8_t x0 = object->x.h;
    uint8_t x1 = x0+r;
    for (*iter_x = OBJECT_LEFT(object); *iter_x < OBJECT_RIGHT(object); *iter_x = *iter_x + 7)
    {
        if (solid_at(x0 + *iter_x, y0) || solid_at(x0 + *iter_x, y1))
        {
            return 0x1;
        }
    }
    return solid_at(x1, y0) || solid_at(x1, y1);
}

uint8_t game_object_hori_will_collide(game_object_t *object)
{
    ufixed_t x;
    x.w = object->x.w + object->dx.w;
    uint8_t x0 = x.h + OBJECT_LEFT(object);
    uint8_t x1 = x.h + OBJECT_RIGHT(object);
    uint8_t b = OBJECT_BOTTOM(object);
    uint8_t t = OBJECT_TOP(object);
    uint8_t y0 = object->y.h;
    uint8_t y1 = y0+b;
    for (*iter_y = t; *iter_y < b; *iter_y = *iter_y + 7)
    {
        if (solid_at(x0, y0 + *iter_y) || solid_at(x1, y0 + *iter_y))
        {
            return 0x1;
        }
    }
    return solid_at(x0, y1) || solid_at(x1, y1);
}

uint8_t game_object_on_floor(game_object_t *object)
{
    uint8_t y;
    y = object->y.h + 1;
    uint8_t y0 = y + OBJECT_BOTTOM(object);
    uint8_t x0 = object->x.h;
    for (*iter_x = OBJECT_LEFT(object); *iter_x < OBJECT_RIGHT(object); *iter_x = *iter_x + 7)
    {
        if (solid_at(x0 + *iter_x, y0))
        {
            return 0x1;
        }
    }
    return solid_at(x0 + OBJECT_RIGHT(object), y0);
}