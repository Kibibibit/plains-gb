#include <stdint.h>
#include <stdlib.h>
#include <gb/gb.h>
#include "player.h"
#include "fixed.h"
#include "constants.h"
#include "bkg_funcs.h"
#include "macros.h"
#include "frame_constants.h"
#include "player_globals.h"
#include "iters.h"
#include "inputs.h"

void player_init()
{
    player_x = new_ufixed(0x0, 0x0);
    player_y = new_ufixed(0x0, 0x0);
    player_dx = new_fixed(0x0, 0x0);
    player_dy = new_fixed(0x0, 0x0);

    player_frame = blank_frame;
    player_set_frame(shovel_frame_jump);
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
    frame_hide(player_frame, PLAYER_OAM);
    free(player_x);
    free(player_y);
    free(player_dx);
    free(player_dy);
}

void player_draw()
{
    if (player_do_draw)
    {
        frame_draw(player_frame, PLAYER_OAM, player_x->h, player_y->h, player_facing_left);
        player_do_draw = 0x0;
    }
}

void player_set_frame(const frame_t *frame)
{
    if (player_frame == frame)
    {
        return;
    }
    if (player_frame->tile_count != frame->tile_count)
    {
        frame_clear_oam(player_frame, PLAYER_OAM);
    }
    player_frame = frame;
    uint8_t tile_count = frame->tile_count;
    for (*iter0 = 0; *iter0 < tile_count; *iter0 = *iter0 + 1)
    {
        set_sprite_tile(PLAYER_OAM + *iter0, frame->tiles[*iter0]);
    }
}

void player_update()
{

    player_state_machine();

    uint8_t x = player_x->h;
    uint8_t y = player_y->h;

    player_timer++;
    if (player_dy->w != 0)
    {
        player_dy->w += GRAVITY;
        player_on_floor = 0x0;
    }
    else if (!player_is_on_floor())
    {
        player_dy->w += GRAVITY;
        player_on_floor = 0x0;
    }
    else
    {
        player_on_floor = 0x1;
    }
    if (player_dy->h > 0)
    {
        if (player_dy->h > 7)
        {
            player_dy->h = 7;
        }
        player_resolve_collision(player_dy, player_y, 0x1, !player_on_floor, iter0, player_bottom_will_collide);
    }
    else if (player_dy->h < 0)
    {
        player_resolve_collision(player_dy, player_y, -0x1, player_on_floor, iter0, player_top_will_collide);
    }

    if (player_dx->h > 0)
    {
        player_resolve_collision(player_dx, player_x, 0x1, 0x0, iter0, player_right_will_collide);
    }
    else if (player_dx->h < 0)
    {
        player_resolve_collision(player_dx, player_x, -0x1, 0x0, iter0, player_left_will_collide);
    }
    if (player_x->h != x || player_y->h != y || world_scroll_x != last_world_scroll_x || world_scroll_y != last_world_scroll_y)
    {
        player_do_draw = 0x1;
    }
}

void player_resolve_collision(fixed_t *speed, ufixed_t *position, int8_t s, uint8_t floor_check, uint8_t *iter, uint8_t (*collider)())
{
    if ((*collider)())
    {
        ufixed_floor(position);
        uint8_t r = abs_ceil_fixed(speed);
        speed->h = s;
        speed->l = 0;
        for (*iter = 0; *iter < r; *iter = *iter + 1)
        {
            if (!(*collider)())
            {
                position->h += s;
            }
            else
            {
                break;
            }
        }
        speed->w = 0;
        if (floor_check)
        {
            player_on_floor = player_is_on_floor();
        }
    }
    else
    {
        position->w += speed->w;
    }
}

uint8_t player_will_collide(fixed_t *speed, ufixed_t *a_position, uint8_t b_position, uint8_t vertical, uint8_t offset, uint8_t *iter)
{
    ufixed_t *a = ufixed_copy(a_position);
    a->w += speed->w;
    uint8_t a0 = a->h + offset;
    free(a);
    uint8_t b0 = b_position;
    uint8_t low, high, b1;
    uint8_t *x;
    uint8_t *y;
    if (vertical)
    {
        low = PLAYER_LEFT;
        high = PLAYER_RIGHT;
        x = &b0;
        y = &a0;
    }
    else
    {
        low = PLAYER_TOP;
        high = PLAYER_BOTTOM;
        x = &a0;
        y = &b0;
    }
    b1 = b0 + high;

    for (*iter = low; *iter < high; *iter = *iter + 7)
    {
        b0 = b_position + *iter;
        if (solid_at(*x, *y))
        {
            return 0x1;
        }
    }
    b0 = b1;
    return solid_at(*x, *y);
}

uint8_t player_top_will_collide()
{
    return player_will_collide(player_dy, player_y, player_x->h, 0x1, PLAYER_TOP, iter1);
}
uint8_t player_bottom_will_collide()
{
    return player_will_collide(player_dy, player_y, player_x->h, 0x1, PLAYER_BOTTOM, iter1);
}
uint8_t player_left_will_collide()
{
    return player_will_collide(player_dx, player_x, player_y->h, 0x0, PLAYER_LEFT, iter1);
}
uint8_t player_right_will_collide()
{
    return player_will_collide(player_dx, player_x, player_y->h, 0x0, PLAYER_RIGHT, iter1);
}
uint8_t player_is_on_floor()
{
    fixed_t *ground_speed = new_fixed(0x0, 0x0);
    uint8_t out = player_will_collide(ground_speed, player_y, player_x->h, 0x1, PLAYER_BOTTOM + 1, iter2);
    free(ground_speed);
    return out;
}

void player_state_machine()
{
    int8_t dyh = player_dy->h;
    if (player_state != PLAYER_STATE_DROP)
    {
        if (dyh > 0)
        {
            player_state = PLAYER_STATE_JUMP;
        }
        else if (dyh < 0)
        {
            player_state = PLAYER_STATE_JUMP;
        }
    }

    switch (player_state)
    {
    case PLAYER_STATE_GROUNDED:
        player_update_grounded();
        break;
    case PLAYER_STATE_JUMP:
        player_update_jump();
        break;
    case PLAYER_STATE_WALK:
        player_update_walk();
        break;
    case PLAYER_STATE_DROP:
        player_update_drop();
        break;
    default:
        player_state = PLAYER_STATE_JUMP;
        break;
    };
}

void player_update_jump()
{

    player_move(2, 10);

    if ((*input & J_DOWN) > 0)
    {
        player_set_frame(shovel_frame_drop);
        player_state = PLAYER_STATE_DROP;
        return;
    }

    if (player_on_floor)
    {
        player_state = PLAYER_STATE_GROUNDED;
        player_set_frame(shovel_frame_idle);
        return;
    }
}
void player_update_grounded()
{
    if (((*pressed & J_A) > 0))
    {
        player_dy->w = PLAYER_JUMP;
        player_set_frame(shovel_frame_jump);
    }

    player_move(2, 2);

    if (player_dx->w != 0)
    {
        player_state = PLAYER_STATE_WALK;
        return;
    }
}
void player_update_walk()
{
    if (((*pressed & J_A) > 0))
    {
        player_dy->w = PLAYER_JUMP;
        player_set_frame(shovel_frame_jump);
        return;
    }

    uint8_t frame_i = player_timer >> 3;
    frame_i = frame_i % 4;
    const frame_t *frame = shovel_frame_idle;
    switch (frame_i)
    {
    case 0x1:
        frame = shovel_frame_walk_1;
        break;
    case 0x2:
        frame = shovel_frame_idle;
        break;
    case 0x3:
        frame = shovel_frame_walk_2;
        break;
    }

    player_set_frame(frame);

    player_move(2, 2);

    if (player_dx->w == 0)
    {
        player_set_frame(shovel_frame_idle);
        player_state = PLAYER_STATE_GROUNDED;
        return;
    }
}

void player_move(int8_t speed, uint8_t friction)
{
    uint8_t p = frame_get_prop(PLAYER_OAM);
    if ((*input & J_RIGHT))
    {
        player_dx->h = speed;
        player_dx->l = 0;

        // Clear fifth bit
        p = p & (~PROP_HORI_FLIP);
        player_facing_left = 0x0;
        frame_set_prop(player_frame, PLAYER_OAM, p);
    }
    else if ((*input & J_LEFT))
    {
        player_dx->h = -speed;
        player_dx->l = 0;
        p = p | (PROP_HORI_FLIP);
        player_facing_left = 0x1;
        frame_set_prop(player_frame, PLAYER_OAM, p);
    }
    else
    {
        if (player_dx->w != 0)
        {
            int8_t odxh = player_dx->h;
            uint8_t odxl = player_dx->l;
            player_dx->w = player_dx->w / friction;
            if (odxh == -1 && odxl > 128)
            {
                player_dx->w = 0;
            }
            else if (odxh == 0 && odxl < 128)
            {
                player_dx->w = 0;
            }
        }
    }
}

uint8_t player_bounce_tile()
{
    uint8_t y;
    y = player_y->h + 2;
    uint8_t y0 = y + PLAYER_BOTTOM;
    uint8_t x0 = player_x->h;
    for (*iter0 = PLAYER_LEFT; *iter0 < PLAYER_RIGHT; *iter0 = *iter0 + 7)
    {
        if (break_at(x0 + *iter0, y0))
        {
            return 0x1;
        }
    }
    return break_at(x0 + PLAYER_RIGHT, y0);
}

void player_update_drop()
{

    player_move(2, 10);

    if (player_bounce_tile())
    {
        player_dy->w = PLAYER_JUMP;
        uint8_t y;
        y = player_y->h + 2;
        uint8_t y0 = y + PLAYER_BOTTOM;
        uint8_t x0 = player_x->h;
        for (*iter0 = PLAYER_LEFT; *iter0 < PLAYER_RIGHT; *iter0 = *iter0 + 7)
        {
            if (break_at(x0 + *iter0, y0))
            {
                break_tile(x0 + *iter0, y0);
            }
        }
        if (break_at(x0 + PLAYER_RIGHT, y0))
        {
            break_tile(x0 + PLAYER_RIGHT, y0);
        }
    }
    else if (player_on_floor)
    {   
        player_set_frame(shovel_frame_idle);
        player_state = PLAYER_STATE_GROUNDED;
        return;
    }
}