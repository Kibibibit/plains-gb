
#include <stdint.h>
#include <stdlib.h>
#include <gb/gb.h>
#include "entity.h"
#include "fixed.h"

entity_t *create_entity(uint8_t tile_index, uint8_t sprite_index)
{
    entity_t *out = (entity_t *)malloc(sizeof(entity_t));

    out->x.w = 0;
    out->y.w = 0;
    out->dx.w = 0;
    out->dy.w = 0;
    out->height = 0;
    out->width = 0;
    out->sprite_index = sprite_index;
    out->tile_index = tile_index;
    set_sprite_tile(sprite_index, tile_index);
    move_sprite(sprite_index, 0, 0);
    return out;
}
void entity_update(entity_t *entity)
{
    entity->dy.w += GRAVITY;

    entity->x.w += entity->dx.w;
    entity->y.w += entity->dy.w;
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