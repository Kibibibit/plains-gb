#ifndef _PLAYER_GLOBALS_H
#define _PLAYER_GLOBALS_H

#include <stdint.h>
#include "fixed.h"
#include "frame.h"

#define PLAYER_LEFT 0x4
#define PLAYER_RIGHT 0xC
#define PLAYER_TOP 0x0
#define PLAYER_BOTTOM 0xF
#define PLAYER_OAM 0x0

#define GRAVITY 0x20

#define PLAYER_JUMP (int16_t)0xFD40

#define PLAYER_STATE_JUMP 0x0
#define PLAYER_STATE_GROUNDED 0x1
#define PLAYER_STATE_WALK 0x2
#define PLAYER_STATE_DROP 0x3

extern ufixed_t * player_x;
extern ufixed_t * player_y;
extern fixed_t * player_dx;
extern fixed_t * player_dy;
extern const frame_t *player_frame;
extern uint8_t player_prop;
extern uint8_t player_timer;
extern uint8_t player_facing_left;
extern uint8_t player_do_draw;
extern uint8_t player_on_floor;
extern uint8_t player_state;
extern uint8_t player_on_screen;

#endif