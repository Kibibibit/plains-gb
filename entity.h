#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>
#include "fixed.h"

#define PROP_HORI_FLIP 0b00100000

#define GRAVITY 0x64

typedef struct {
    ufixed_t x;
    ufixed_t y;
    fixed_t dx;
    fixed_t dy;
    uint8_t height;
    uint8_t props;
    uint8_t tile_index;
    uint8_t sprite_index;
} entity_t;


entity_t * create_entity(uint8_t tile_index, uint8_t sprite_index);
void entity_update(entity_t * entity);
void entity_set_prop(entity_t * entity, uint8_t prop);
void entity_move(entity_t * entity, uint8_t x, uint8_t y);
void entity_draw(entity_t * entity);
uint8_t entity_face_left(entity_t * entity);
void destroy_entity(entity_t * entity);



#endif