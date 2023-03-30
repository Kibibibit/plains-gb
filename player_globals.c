#include "player_globals.h"
#include <stdint.h>
#include <stdlib.h>
#include "frame.h"
#include "macros.h"
#include "frame_constants.h"

ufixed_t *player_x;
ufixed_t *player_y;
fixed_t *player_dx;
fixed_t *player_dy;
const frame_t *player_frame;
uint8_t player_prop;
uint8_t player_timer;
uint8_t player_facing_left;
uint8_t player_do_draw;
uint8_t player_on_floor;
uint8_t player_state;
uint8_t player_on_screen;
int8_t world_scroll_x;
int8_t world_scroll_y;
int8_t last_world_scroll_x;
int8_t last_world_scroll_y;