#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>
#include "frame.h"
#include "fixed.h"

void player_init();
void player_destroy();

void player_update();

void player_set_frame(const frame_t *frame);
void player_draw();
uint8_t player_will_collide(fixed_t *speed, ufixed_t *a_position, uint8_t b_position, uint8_t vertical, uint8_t offset, uint8_t *iter);
uint8_t player_top_will_collide();
uint8_t player_bottom_will_collide();
uint8_t player_left_will_collide();
uint8_t player_right_will_collide();

void player_state_machine();
void player_update_jump();
void player_update_grounded();
void player_update_walk();
void player_move(int8_t speed, uint8_t friction);
void player_update_drop();
void player_resolve_collision(fixed_t *speed, ufixed_t *position, int8_t s, uint8_t floor_check, uint8_t *iter ,uint8_t (*collider)());
uint8_t player_bounce_tile();
uint8_t player_is_on_floor();

#endif