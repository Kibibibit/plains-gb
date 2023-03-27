#include <stdint.h>
#include <stdlib.h>
#include <gb/gb.h>
#include "player.h"
#include "game_object.h"
#include "fixed.h"
#include "constants.h"
#include "bkg_funcs.h"
#include "macros.h"
#include "frame_constants.h"
#include "iters.h"

player_t *player_create()
{
    player_t *out = (player_t *)malloc(sizeof(player_t));
    out->object = create_game_object();
    out->object->lr = 0x4C;
    out->object->tb = 0x0F;
    out->health = 0;
    out->state = PLAYER_STATE_JUMP;
    return out;
}

void player_update(player_t *player)
{
    if (player->state != PLAYER_STATE_DROP)
    {
        if (player->object->dy.h > 0)
        {
            player->state = PLAYER_STATE_JUMP;
        }
        else if (player->object->dy.h < 0)
        {
            player->state = PLAYER_STATE_JUMP;
        }
    }

    uint8_t input = joypad();

    switch (player->state)
    {
    case PLAYER_STATE_GROUNDED:
        player_update_grounded(player, input);
        break;
    case PLAYER_STATE_JUMP:
        player_update_jump(player, input);
        break;
    case PLAYER_STATE_WALK:
        player_update_walk(player, input);
        break;
    case PLAYER_STATE_DROP:
        player_update_drop(player, input);
        break;
    default:
        player->state = PLAYER_STATE_JUMP;
        break;
    };

    game_object_update(player->object);
}

void player_update_jump(player_t *player, uint8_t input)
{
    game_object_set_frame(player->object, shovel_frame_jump);

    player_move(player, input, 2, 10);

    if ((input & J_DOWN) > 0)
    {
        player->state = PLAYER_STATE_DROP;
        return;
    }

    if (player->object->on_floor)
    {
        player->state = PLAYER_STATE_GROUNDED;
        return;
    }

}
void player_update_grounded(player_t *player, uint8_t input)
{
    game_object_set_frame(player->object, shovel_frame_idle);

    if (((input & J_A) > 0))
    {
        player->object->dy.h = PLAYER_JUMP_DYH;
        player->object->dy.l = PLAYER_JUMP_DYL;
    }

    player_move(player, input, 2, 2);

    if (player->object->dx.w != 0)
    {
        player->state = PLAYER_STATE_WALK;
        return;
    }
}
void player_update_walk(player_t *player, uint8_t input)
{
    if (((input & J_A) > 0))
    {
        player->object->dy.h = PLAYER_JUMP_DYH;
        player->object->dy.l = PLAYER_JUMP_DYL;
        return;
    }

    uint8_t frame_i = player->object->timer >> 3;
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

    game_object_set_frame(player->object, frame);

    player_move(player, input, 2, 2);

    if (player->object->dx.w == 0)
    {
        player->state = PLAYER_STATE_GROUNDED;
        return;
    }
}

void player_move(player_t *player, uint8_t input, int8_t speed, uint8_t friction)
{
    uint8_t face_left = player->object->facing_left;
    uint8_t p = player->object->prop;
    if ((input & J_RIGHT))
    {
        player->object->dx.h = speed;

        // Clear fifth bit
        p = p & (~PROP_HORI_FLIP);
        player->object->facing_left = 0x0;
        game_object_set_prop(player->object, p);
    }
    else if ((input & J_LEFT))
    {
        player->object->dx.h = -speed;
        p = p | (PROP_HORI_FLIP);
        player->object->facing_left = 0x1;
        game_object_set_prop(player->object, p);
    }
    else
    {
        player->object->dx.w = player->object->dx.w / friction;
        if (player->object->dx.h == -1 && player->object->dx.l > 128)
        {
            player->object->dx.w = 0;
        }
        if (player->object->dx.h == 0 && player->object->dx.l < 128)
        {
            player->object->dx.w = 0;
        }
    }
}

uint8_t player_bounce_tile(player_t *player)
{
    uint8_t y;
    y = player->object->y.h + 2;
    uint8_t y0 = y + OBJECT_BOTTOM(player->object);
    uint8_t x0 = player->object->x.h;
    for (*iter_x = OBJECT_LEFT(player->object); *iter_x < OBJECT_RIGHT(player->object); *iter_x = *iter_x + 7)
    {
        if (break_at(x0 + *iter_x, y0))
        {
            return 0x1;
        }
    }
    return break_at(x0 + OBJECT_RIGHT(player->object), y0);
}

void player_update_drop(player_t *player, uint8_t input)
{
    game_object_set_frame(player->object, shovel_frame_drop);

    player_move(player, input, 2, 10);

    if (player_bounce_tile(player))
    {
        player->object->dy.h = PLAYER_JUMP_DYH;
        player->object->dy.l = PLAYER_JUMP_DYL;
        uint8_t y;
        y = player->object->y.h + 2;
        uint8_t y0 = y + OBJECT_BOTTOM(player->object);
        uint8_t x0 = player->object->x.h;
        for (*iter_x = OBJECT_LEFT(player->object); *iter_x < OBJECT_RIGHT(player->object); *iter_x = *iter_x + 7)
        {
            if (break_at(x0 + *iter_x, y0))
            {
                break_tile(x0 + *iter_x, y0);
            }
        }
        if (break_at(x0 + OBJECT_RIGHT(player->object), y0))
        {
            break_tile(x0 + OBJECT_RIGHT(player->object), y0);
        }
    }
    else if (player->object->on_floor)
    {
        player->state = PLAYER_STATE_GROUNDED;
        return;
    }
}