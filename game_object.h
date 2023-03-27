#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

#include <stdint.h>
#include "fixed.h"
#include "frame.h"

#define GRAVITY 0x20

typedef struct {
    ufixed_t x;
    ufixed_t y;
    fixed_t dx;
    fixed_t dy;
    const frame_t * frame;
    uint8_t prop;
    uint8_t gravity;
    uint8_t draw_order;
    uint8_t facing_left;
    uint8_t left;
    uint8_t right;
    uint8_t top;
    uint8_t bottom;
    uint8_t oam;
} game_object_t;

game_object_t * create_game_object();
void destroy_game_object(game_object_t * object);
void game_object_update(game_object_t * object);
void game_object_set_prop(game_object_t * object, uint8_t prop);
void game_object_draw(game_object_t * object);
void game_object_clear_oam(game_object_t * object);
void game_object_set_frame(game_object_t * object, const frame_t * frame);
uint8_t game_object_collides_at(game_object_t * object, uint8_t x, uint8_t y);


#endif