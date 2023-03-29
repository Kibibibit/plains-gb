#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>
#include "game_object.h"

#define PLAYER_JUMP (int16_t)0xFD40

#define PLAYER_STATE_JUMP       0x0
#define PLAYER_STATE_GROUNDED   0x1
#define PLAYER_STATE_WALK       0x2
#define PLAYER_STATE_DROP       0x3

typedef struct {
    game_object_t * object;
    uint8_t state;
    uint8_t health;
} player_t;

player_t * player_create();
void player_update(player_t * player);

void player_set_frame(const frame_t * frame);
void player_draw();
uint8_t player_on_floor(game_object_t * object);
uint8_t player_hori_will_collide(game_object_t * object, uint8_t offset);
uint8_t player_vert_will_collide(game_object_t * object, uint8_t offset);
uint8_t player_top_will_collide(game_object_t * object);
uint8_t player_bottom_will_collide(game_object_t * object);
uint8_t player_left_will_collide(game_object_t * object);
uint8_t player_right_will_collide(game_object_t * object);



void player_update_jump(player_t * player, uint8_t input);
void player_update_grounded(player_t * player, uint8_t input);
void player_update_walk(player_t * player, uint8_t input);
void player_move(player_t * player, uint8_t input, int8_t speed, uint8_t friction);
void player_update_drop(player_t * player, uint8_t input);
uint8_t player_bounce_tile(player_t * player);

#endif