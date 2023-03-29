#include "player_globals.h"
#include <stdint.h>
#include <stdlib.h>
#include "frame.h"
#include "macros.h"
#include "frame_constants.h"

const ufixed_t *player_x;
const ufixed_t *player_y;
const fixed_t *player_dx;
const fixed_t *player_dy;
const frame_t *player_frame;
uint8_t player_prop;
uint8_t player_timer;
uint8_t player_facing_left;
uint8_t player_do_draw;
uint8_t player_on_floor;
uint8_t player_state;
uint8_t player_on_screen;

void player_init()
{
    player_x = new_ufixed(0x0, 0x0);
    player_y = new_ufixed(0x0, 0x0);
    player_dx = new_fixed(0x0, 0x0);
    player_dy = new_fixed(0x0, 0x0);

    player_frame = shovel_frame_jump;
    player_prop = 0x0;
    player_timer = 0x0;
    player_facing_left = 0x0;
    player_do_draw = 0x1;
    player_on_floor = 0x0;
    player_state = PLAYER_STATE_JUMP;
    player_on_screen = 0x1;
}
void player_destroy()
{
    free(player_x);
    free(player_y);
    free(player_dx);
    free(player_dy);
}