#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>
#include "entity.h"

#define PLAYER_JUMP_DYH -3
#define PLAYER_JUMP_DYL 128

#define PLAYER_STATE_FALL       0x0
#define PLAYER_STATE_JUMP       0x1
#define PLAYER_STATE_GROUNDED   0x2
#define PLAYER_STATE_WALK       0x3
#define PLAYER_STATE_DROP       0x4

typedef struct {
    entity_t entity;
    uint8_t state;
    uint8_t health;
} player_t;

player_t * player_create();
void player_update(player_t * player);

void player_update_fall(player_t * player, uint8_t input);
void player_update_jump(player_t * player, uint8_t input);
void player_update_grounded(player_t * player, uint8_t input);
void player_update_walk(player_t * player, uint8_t input);
void player_move(player_t * player, uint8_t input, int8_t speed, uint8_t friction);
void player_update_drop(player_t * player, uint8_t input);

#endif