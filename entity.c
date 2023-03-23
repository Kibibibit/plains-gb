
#include <stdint.h>
#include <stdlib.h>
#include <gb/gb.h>
#include <gbdk/emu_debug.h>
#include "bkg_funcs.h"
#include "entity.h"
#include "fixed.h"
#include "maths.h"

entity_t *create_entity(uint8_t tile_index, uint8_t sprite_index)
{
    entity_t *out = (entity_t *)malloc(sizeof(entity_t));

    out->x.w = 0;
    out->y.w = 0;
    out->dx.w = 0;
    out->dy.w = 0;
    out->height = 8;
    out->width = 8;
    out->sprite_index = sprite_index;
    out->tile_index = tile_index;
    out->props = 0;
    out->timer = 0;
    set_sprite_tile(sprite_index, tile_index);
    move_sprite(sprite_index, 0, 0);
    return out;
}
void entity_update(entity_t *entity)
{

    entity->timer += 1;

    entity->dy.w += GRAVITY;

    ufixed_t new_x, new_y, collide_new_x, collide_new_y;
    new_x.w = entity->x.w + entity->dx.w;
    new_y.w = entity->y.w + entity->dy.w;
    if (entity->dx.h >= 0) {
        collide_new_x = ufixed_ceil(new_x);
    } else {
        collide_new_x = ufixed_floor(new_x);
    }
    if (entity->dy.h >= 0) {
        collide_new_y = ufixed_ceil(new_y);
    } else {
        collide_new_y = ufixed_floor(new_y);
    }
    
    if (entity_collides_at(entity, collide_new_x.h, entity->y.h))
    {
        int8_t diff = collide_new_x.h - entity->x.h;
        int8_t s = sign(diff);
        entity->dx.w = 0;
        while (entity_collides_at(entity, collide_new_x.h, entity->y.h))
        {
            collide_new_x.h -= s;
        }
        new_x.w = collide_new_x.w;
    }

    entity->x.w = new_x.w;

    if (entity_collides_at(entity, entity->x.h, collide_new_y.h))
    {
        int8_t diff = collide_new_y.h - entity->y.h;
        if (diff == 0) {
            diff = collide_new_y.l - entity->y.l;
        }
        
        int8_t s = sign(diff);
        entity->dy.w = 0;
        while (entity_collides_at(entity, entity->x.h, collide_new_y.h))
        {
            collide_new_y.h -= s;
        }
        new_y.w = collide_new_y.w;
    }
    
    entity->y.w = new_y.w;
}

uint8_t entity_collides_at(entity_t *entity, uint8_t x, uint8_t y)
{
    uint8_t x0, y0, x1, y1, x0y0, x1y0, x0y1, x1y1, w;
    x0 = x;
    y0 = y;
    w = entity->width-1;

    if (entity_face_left(entity)) {
        x0 = x + (7-w);
    }

    x1 = x0 + entity->width-1;
    y1 = y0 + entity->height-1;
    x0y0 = solid_at(x0, y0);
    x1y0 = solid_at(x1, y1);
    x0y1 = solid_at(x0, y1);
    x1y1 = solid_at(x1, y1);
    return x0y0 | x1y0 | x0y1 | x1y1;
}

uint8_t on_floor(entity_t *entity)
{
    uint8_t x1, y1;
    x1 = entity->x.h + entity->width - 1;
    y1 = entity->y.h + entity->height;
    return solid_at(entity->x.h, y1) | solid_at(x1, y1);
}

void entity_set_prop(entity_t *entity, uint8_t prop)
{
    entity->props = prop;
    set_sprite_prop(entity->sprite_index, prop);
}
void entity_move(entity_t *entity, uint8_t x, uint8_t y)
{
    entity->x.h = x;
    entity->y.h = y;
    entity->x.l = 0;
    entity->y.l = 0;
}
void entity_draw(entity_t *entity)
{
    uint8_t sx = (entity->x.h);
    uint8_t sy = (entity->y.h);
    move_sprite(entity->sprite_index, sx, sy);
}
uint8_t entity_face_left(entity_t *entity)
{
    return (entity->props & PROP_HORI_FLIP) > 0;
}

void destroy_entity(entity_t *entity)
{
    set_sprite_tile(entity->sprite_index, 0);
    move_sprite(entity->sprite_index, 0, 0);
    set_sprite_prop(entity->sprite_index, 0);
    free(entity);
}

void entity_set_tile(entity_t *entity, uint8_t tile_index)
{
    entity->tile_index = tile_index;
    set_sprite_tile(entity->sprite_index, tile_index);
}