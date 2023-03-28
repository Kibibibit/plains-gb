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
    game_object_t *object = player->object;
    int8_t dyh = object->dy->h;
    if (player->state != PLAYER_STATE_DROP)
    {
        if (dyh > 0)
        {
            player->state = PLAYER_STATE_JUMP;
        }
        else if (dyh < 0)
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
    game_object_update(object);
}

void player_update_jump(player_t *player, uint8_t input)
{
    game_object_t *object = player->object;
    game_object_set_frame(object, shovel_frame_jump);

    player_move(player, input, 2, 10);

    if ((input & J_DOWN) > 0)
    {
        player->state = PLAYER_STATE_DROP;
        return;
    }

    if (object->on_floor)
    {
        player->state = PLAYER_STATE_GROUNDED;
        return;
    }
}
void player_update_grounded(player_t *player, uint8_t input)
{
    game_object_t *object = player->object;
    game_object_set_frame(object, shovel_frame_idle);

    if (((input & J_A) > 0))
    {
        object->dy->w = PLAYER_JUMP;
    }

    player_move(player, input, 2, 2);

    if (object->dx->w != 0)
    {
        player->state = PLAYER_STATE_WALK;
        return;
    }
}
void player_update_walk(player_t *player, uint8_t input)
{
    game_object_t *object = player->object;
    if (((input & J_A) > 0))
    {
        object->dy->w = PLAYER_JUMP;
        return;
    }

    uint8_t frame_i = object->timer >> 3;
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

    game_object_set_frame(object, frame);

    player_move(player, input, 2, 2);

    if (object->dx->w == 0)
    {
        player->state = PLAYER_STATE_GROUNDED;
        return;
    }
}

void player_move(player_t *player, uint8_t input, int8_t speed, uint8_t friction)
{
    game_object_t *object = player->object;
    uint8_t face_left = object->facing_left;
    uint8_t p = object->prop;
    fixed_t *odx = object->dx;
    if ((input & J_RIGHT))
    {
        odx->h = speed;

        // Clear fifth bit
        p = p & (~PROP_HORI_FLIP);
        object->facing_left = 0x0;
        game_object_set_prop(object, p);
    }
    else if ((input & J_LEFT))
    {
        odx->h = -speed;
        p = p | (PROP_HORI_FLIP);
        object->facing_left = 0x1;
        game_object_set_prop(object, p);
    }
    else
    {
        if (odx->w != 0)
        {
            int8_t odxh = odx->h;
            uint8_t odxl = odx->l;
            odx->w = odx->w / friction;
            if (odxh == -1 && odxl > 128)
            {
                odx->w = 0;
            } else if (odxh == 0 && odxl < 128)
            {
                odx->w = 0;
            }
        }
    }
}

uint8_t player_bounce_tile(player_t *player)
{
    game_object_t *object = player->object;
    uint8_t y;
    y = object->y->h + 2;
    uint8_t y0 = y + OBJECT_BOTTOM(object);
    uint8_t x0 = object->x->h;
    uint8_t r = OBJECT_RIGHT(object);
    for (*iter_x = OBJECT_LEFT(object); *iter_x < r; *iter_x = *iter_x + 7)
    {
        if (break_at(x0 + *iter_x, y0))
        {
            return 0x1;
        }
    }
    return break_at(x0 + r, y0);
}

void player_update_drop(player_t *player, uint8_t input)
{
    game_object_t *object = player->object;
    game_object_set_frame(object, shovel_frame_drop);

    player_move(player, input, 2, 10);

    if (player_bounce_tile(player))
    {
        object->dy->w = PLAYER_JUMP;
        uint8_t y;
        y = object->y->h + 2;
        uint8_t y0 = y + OBJECT_BOTTOM(object);
        uint8_t x0 = object->x->h;
        uint8_t r = OBJECT_RIGHT(object);
        for (*iter_x = OBJECT_LEFT(object); *iter_x < r; *iter_x = *iter_x + 7)
        {
            if (break_at(x0 + *iter_x, y0))
            {
                break_tile(x0 + *iter_x, y0);
            }
        }
        if (break_at(x0 + r, y0))
        {
            break_tile(x0 + r, y0);
        }
    }
    else if (object->on_floor)
    {
        player->state = PLAYER_STATE_GROUNDED;
        return;
    }
}