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
    out->x = new_ufixed(0x0,0x0);
    out->y = new_ufixed(0x0,0x0);
    out->dx = new_fixed(0x0,0x0);
    out->dy = new_fixed(0x0,0x0);
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
    free(object->x);
    free(object->y);
    free(object->dx);
    free(object->dy);
    free(object);
}

uint8_t abs_ceil_fixed(fixed_t * x) {
    fixed_t * copy = fixed_copy(x);
    if (copy->h < 0) {
        fixed_t *mask = new_fixed(0xFF, 0xFF);
        copy->w = copy->w ^ mask->w;
        copy->w -= mask->w;
    }
    uint8_t l = copy->l;
    uint8_t h = (uint8_t)copy->h;
    free(copy);
    if (l != 0)
    {
        return h + 1;
    }
    return h;
    
}

void game_object_update(game_object_t *object)
{
    //pointer to object->x
    ufixed_t * ox = object->x;
    // poitner to object->y
    ufixed_t * oy = object->y;
    // Pointer to object->dx
    fixed_t * odx = object->dx;
    // Pointer to object->dy;
    fixed_t * ody = object->dy;

    uint8_t x = ox->h;
    uint8_t y = oy->h;

    object->timer++;
    if (ody->w != 0)
    {
        ody->w += GRAVITY;
        object->on_floor = 0x0;
    }
    else if (!game_object_on_floor(object))
    {
        ody->w += GRAVITY;
        object->on_floor = 0x0;
    } else {
        object->on_floor = 0x1;
    }

    if (ody->w != 0)
    {
        if (ody->h > 7) {
            ody->h = 7;
        }
        if (game_object_vert_will_collide(object))
        {
            ufixed_floor(oy);
            int8_t s = sign(ody->h);
            uint8_t r = abs_ceil_fixed(ody);
            ody->h = s;
            ody->l = 0;
            for (*iter_y = 0; *iter_y < r; *iter_y = *iter_y + 1)
            {
                if (!game_object_vert_will_collide(object))
                {
                    oy->h += s;
                }
                else
                {
                    break;
                }
            }
            ody->w = 0;
            if (!object->on_floor) {
                object->on_floor = game_object_on_floor(object);
            }
        }
        else
        {
            oy->w += ody->w;
            if (object->on_floor) {
                object->on_floor = game_object_on_floor(object);
            }
        }
    }
    
    if (odx->w != 0)
    {
        if (game_object_hori_will_collide(object))
        {
            ufixed_floor(ox);
            int8_t s = sign(odx->h);
            uint8_t r = abs_ceil_fixed(odx);
            odx->h = s;
            odx->l = 0;
            for (*iter_x = 0; *iter_x < r; *iter_x = *iter_x + 1)
            {
                if (!game_object_hori_will_collide(object))
                {
                    ox->h += s;
                }
                else
                {
                    break;
                }
            }
            odx->w = 0;
        }
        else
        {
            ox->w += odx->w;
        }
    }
    if (ox->h != x || oy->h != y)
    {
        object->do_draw = 0x1;
    }
}
void game_object_set_prop(game_object_t *object, uint8_t prop)
{
    uint8_t tile_count = object->frame->tile_count;
    uint8_t oam = object->oam;
    for (*iter_i = 0; *iter_i < tile_count; *iter_i = *iter_i + 1)
    {
        set_sprite_prop(oam + *iter_i, prop);
    }
}

void game_object_clear_oam(game_object_t *object)
{
    uint8_t oam = object->oam;
    uint8_t tile_count = object->frame->tile_count;
    for (*iter_i = 0; *iter_i < tile_count; *iter_i = *iter_i + 1)
    {
        set_sprite_tile(oam + *iter_i, SPR_BLANK);
        move_sprite(oam + *iter_i, 0, 0);
        set_sprite_prop(oam + *iter_i, 0);
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
    uint8_t oam = object->oam;
    uint8_t tile_count = frame->tile_count;
    for (*iter_i = 0; *iter_i < tile_count; *iter_i = *iter_i + 1)
    {
        set_sprite_tile(oam + *iter_i, frame->tiles[*iter_i]);
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
        uint8_t xh = object->x->h;
        uint8_t yh = object->y->h;
        uint8_t oam = object->oam;
        uint8_t facing_left = object->facing_left;

        for (*iter_y = 0; *iter_y < y_tiles; *iter_y = *iter_y + 1)
        {
            for (*iter_x = 0; *iter_x < x_tiles; *iter_x = *iter_x + 1)
            {
                if (facing_left)
                {
                    xx = xh + ((x_tiles - 1 - *iter_x) << 3);
                }
                else
                {
                    xx = xh + (*iter_x << 3);
                }

                yy = yh + (*iter_y << 3);
                move_sprite(oam + *iter_i, xx, yy);
                *iter_i = *iter_i + 1;
            }
        }
        object->do_draw = 0x0;
    }
}

uint8_t game_object_vert_will_collide(game_object_t *object)
{
    ufixed_t * y = ufixed_copy(object->y);
    y->w += object->dy->w;
    uint8_t y0 = y->h + OBJECT_TOP(object);
    uint8_t y1 = y->h + OBJECT_BOTTOM(object);
    free(y);
    uint8_t l = OBJECT_LEFT(object);
    uint8_t r = OBJECT_RIGHT(object);
    uint8_t x0 = object->x->h;
    uint8_t x1 = x0+r;
    for (*iter_x = l; *iter_x < r; *iter_x = *iter_x + 7)
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
    ufixed_t * x = ufixed_copy(object->x);
    x->w += object->dx->w;
    uint8_t x0 = x->h + OBJECT_LEFT(object);
    uint8_t x1 = x->h + OBJECT_RIGHT(object);
    free(x);
    uint8_t b = OBJECT_BOTTOM(object);
    uint8_t t = OBJECT_TOP(object);
    uint8_t y0 = object->y->h;
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
    y = object->y->h + 1;
    uint8_t y0 = y + OBJECT_BOTTOM(object);
    uint8_t r = OBJECT_RIGHT(object);
    uint8_t x0 = object->x->h;
    for (*iter_x = OBJECT_LEFT(object); *iter_x < r; *iter_x = *iter_x + 7)
    {
        if (solid_at(x0 + *iter_x, y0))
        {
            return 0x1;
        }
    }
    return solid_at(x0 + r, y0);
}