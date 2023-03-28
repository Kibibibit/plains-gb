#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

#include <stdint.h>
#include "fixed.h"
#include "frame.h"
#include "helpers.h"

#define GRAVITY 0x20

#define OBJECT_LEFT(object) nibble_high(object->lr)
#define OBJECT_RIGHT(object) nibble_low(object->lr)
#define OBJECT_TOP(object) nibble_high(object->tb)
#define OBJECT_BOTTOM(object) nibble_low(object->tb)

typedef struct {
    ufixed_t * x;
    ufixed_t * y;
    fixed_t * dx;
    fixed_t * dy;
    const frame_t * frame;
    uint8_t prop;
    uint8_t timer;
    uint8_t gravity;
    uint8_t draw_order;
    uint8_t facing_left;
    uint8_t lr;
    uint8_t tb;
    uint8_t oam;
    uint8_t do_draw;
    uint8_t on_floor;
} game_object_t;

game_object_t * create_game_object();
void destroy_game_object(game_object_t * object);
void game_object_update(game_object_t * object);
void game_object_set_prop(game_object_t * object, uint8_t prop);
void game_object_draw(game_object_t * object);
void game_object_clear_oam(game_object_t * object);
void game_object_set_frame(game_object_t * object, const frame_t * frame);
uint8_t game_object_on_floor(game_object_t * object);
uint8_t game_object_hori_will_collide(game_object_t * object);
uint8_t game_object_vert_will_collide(game_object_t * object);
uint8_t abs_ceil_fixed(fixed_t * x);
#endif