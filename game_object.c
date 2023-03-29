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
    out->x = new_ufixed(0x0, 0x0);
    out->y = new_ufixed(0x0, 0x0);
    out->dx = new_fixed(0x0, 0x0);
    out->dy = new_fixed(0x0, 0x0);
    out->facing_left = 0x0;
    out->gravity = 0x1;
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



void game_object_update(game_object_t *object)
{
    // pointer to object->x
    ufixed_t *ox = object->x;
    // poitner to object->y
    ufixed_t *oy = object->y;
    // Pointer to object->dx
    fixed_t *odx = object->dx;
    // Pointer to object->dy;
    fixed_t *ody = object->dy;

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
    }
    else
    {
        object->on_floor = 0x1;
    }

    if (ody->h > 0)
    {
        if (ody->h > 7)
        {
            ody->h = 7;
        }
        if (game_object_bottom_will_collide(object))
        {
            ufixed_floor(oy);
            uint8_t r = abs_ceil_fixed(ody);
            ody->h = 0x1;
            ody->l = 0;
            for (*iter_y = 0; *iter_y < r; *iter_y = *iter_y + 1)
            {
                if (!game_object_bottom_will_collide(object))
                {
                    oy->h += 0x1;
                }
                else
                {
                    break;
                }
            }
            ody->w = 0;
            if (object->on_floor)
            {
                object->on_floor = game_object_on_floor(object);
            }
        }
        else
        {
            oy->w += ody->w;
        }
    }
    else if (ody->h < 0)
    {
        if (game_object_top_will_collide(object))
        {
            ufixed_floor(oy);
            uint8_t r = abs_ceil_fixed(ody);
            ody->h = -0x1;
            ody->l = 0;
            for (*iter_y = 0; *iter_y < r; *iter_y = *iter_y + 1)
            {
                if (!game_object_top_will_collide(object))
                {
                    oy->h -= 0x1;
                }
                else
                {
                    break;
                }
            }
            ody->w = 0;
            if (!object->on_floor)
            {
                object->on_floor = game_object_on_floor(object);
            }
        }
        else
        {
            oy->w += ody->w;
        }
    }

    if (odx->h > 0)
    {
        if (game_object_right_will_collide(object))
        {
            ufixed_floor(ox);
            uint8_t r = abs_ceil_fixed(odx);
            odx->h = 0x1;
            odx->l = 0;
            for (*iter_x = 0; *iter_x < r; *iter_x = *iter_x + 1)
            {
                if (!game_object_right_will_collide(object))
                {
                    ox->h += 0x1;
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
    else if (odx->h < 0)
    {
        if (game_object_left_will_collide(object))
        {
            ufixed_floor(ox);
            uint8_t r = abs_ceil_fixed(odx);
            odx->h = -0x1;
            odx->l = 0;
            for (*iter_x = 0; *iter_x < r; *iter_x = *iter_x + 1)
            {
                if (!game_object_left_will_collide(object))
                {
                    ox->h -= 0x1;
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
    frame_set_prop(object->frame, object->oam, prop);
}

void game_object_clear_oam(game_object_t *object)
{
    frame_clear_oam(object->frame, object->oam);
}

void game_object_set_frame(game_object_t *object, const frame_t *frame)
{

    if (object->frame == frame)
    {
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
        frame_draw(object->frame, object->oam, object->x->h, object->y->h, object->facing_left);
        object->do_draw = 0x0;
    }
}

uint8_t game_object_vert_will_collide(game_object_t *object, uint8_t offset)
{
    ufixed_t *y = ufixed_copy(object->y);
    y->w += object->dy->w;
    uint8_t y0 = y->h + offset;
    free(y);
    uint8_t l = OBJECT_LEFT(object);
    uint8_t r = OBJECT_RIGHT(object);
    uint8_t x0 = object->x->h;
    uint8_t x1 = x0 + r;
    for (*iter_x = l; *iter_x < r; *iter_x = *iter_x + 7)
    {
        if (solid_at(x0 + *iter_x, y0))
        {
            return 0x1;
        }
    }
    return solid_at(x1, y0);
}

uint8_t game_object_hori_will_collide(game_object_t *object, uint8_t offset)
{
    ufixed_t *x = ufixed_copy(object->x);
    x->w += object->dx->w;
    uint8_t x0 = x->h + offset;
    free(x);
    uint8_t b = OBJECT_BOTTOM(object);
    uint8_t t = OBJECT_TOP(object);
    uint8_t y0 = object->y->h;
    uint8_t y1 = y0 + b;
    for (*iter_y = t; *iter_y < b; *iter_y = *iter_y + 7)
    {
        if (solid_at(x0, y0 + *iter_y))
        {
            return 0x1;
        }
    }
    return solid_at(x0, y1);
}

uint8_t game_object_top_will_collide(game_object_t *object)
{
    return game_object_vert_will_collide(object, OBJECT_TOP(object));
}
uint8_t game_object_bottom_will_collide(game_object_t *object)
{
    return game_object_vert_will_collide(object, OBJECT_BOTTOM(object));
}
uint8_t game_object_left_will_collide(game_object_t *object)
{
    return game_object_hori_will_collide(object, OBJECT_LEFT(object));
}
uint8_t game_object_right_will_collide(game_object_t *object)
{
    return game_object_hori_will_collide(object, OBJECT_RIGHT(object));
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