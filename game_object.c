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

    return out;
}
void destroy_game_object(game_object_t *object)
{
    game_object_clear_oam(object);
    free(object);
}
void game_object_update(game_object_t *object)
{
    object->timer++;
    object->dy.w += GRAVITY;

    ufixed_t new_x, new_y, collide_new_x, collide_new_y;
    new_x.w = object->x.w + object->dx.w;
    new_y.w = object->y.w + object->dy.w;
    if (object->dx.h >= 0)
    {
        collide_new_x = ufixed_ceil(new_x);
    }
    else
    {
        collide_new_x = ufixed_floor(new_x);
    }
    if (object->dy.h >= 0)
    {
        collide_new_y = ufixed_ceil(new_y);
    }
    else
    {
        collide_new_y = ufixed_floor(new_y);
    }

    if (game_object_collides_at(object, collide_new_x.h, object->y.h))
    {
        int8_t diff = collide_new_x.h - object->x.h;
        int8_t s = sign(diff);
        object->dx.w = 0;
        while (game_object_collides_at(object, collide_new_x.h, object->y.h))
        {
            collide_new_x.h -= s;
        }
        new_x.w = collide_new_x.w;
    }

    object->x.w = new_x.w;

    if (game_object_collides_at(object, object->x.h, collide_new_y.h))
    {
        int8_t diff = collide_new_y.h - object->y.h;
        if (diff == 0)
        {
            diff = collide_new_y.l - object->y.l;
        }

        int8_t s = sign(diff);
        object->dy.w = 0;
        while (game_object_collides_at(object, object->x.h, collide_new_y.h))
        {
            collide_new_y.h -= s;
        }
        new_y.w = collide_new_y.w;
    }

    object->y.w = new_y.w;
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

    uint8_t x_tiles = FRAME_X_TILES(object->frame);

    for (*iter_y = 0; *iter_y < FRAME_Y_TILES(object->frame); *iter_y = *iter_y + 1)
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
}
uint8_t game_object_collides_at(game_object_t *object, uint8_t x, uint8_t y)
{
    uint8_t out, l, r, t, b, xc, yc;
    xc = (FRAME_X_TILES(object->frame) << 3)-1;
    yc = (FRAME_Y_TILES(object->frame) << 3)-1;
    out = 0x0;
    if (object->facing_left) {
        l = OBJECT_RIGHT(object);
        r = xc-OBJECT_LEFT(object);
    } else {
        l = OBJECT_LEFT(object);
        r = xc-OBJECT_RIGHT(object);
    }
    t = OBJECT_TOP(object);
    b = yc-OBJECT_BOTTOM(object);

    *iter_x = l;
    while (*iter_x < r)
    {
        out = out | solid_at(*iter_x + x, t + y);
        out = out | solid_at(*iter_x + x, b + y);
        *iter_x = *iter_x + 0x8;
    }
    out = out | solid_at(r + x, t + y);
    out = out | solid_at(r + x, b + y);
    if (out)
    {
        return out;
    }
    *iter_y = t;
    while (*iter_y < b)
    {
        out = out | solid_at(r + x, *iter_y + y);
        out = out | solid_at(l + x, *iter_y + y);
        *iter_y = *iter_y + 0x8;
    }
    out = out | solid_at(r + x, b + y);
    out = out | solid_at(l + x, b + y);
    return out;
}


uint8_t game_object_on_floor(game_object_t * object)
{
    uint8_t x1, y1;
    x1 = object->x.h + 0xF - OBJECT_RIGHT(object);
    y1 = object->y.h + 0xF - OBJECT_BOTTOM(object)+2;
    return solid_at(object->x.h, y1) | solid_at(x1, y1);
}