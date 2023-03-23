#include <stdint.h>
#include <stdlib.h>
#include <gb/gb.h>
#include "player.h"
#include "entity.h"
#include "fixed.h"
#include "constants.h"

player_t *player_create()
{
    player_t *out = (player_t *)malloc(sizeof(player_t));
    out = (player_t *)create_entity(SPR_INDEX_KNIGHT_0, 0);
    out->entity.width = 6;
    out->health = 0;
    out->state = PLAYER_STATE_FALL;
    return out;
}

void player_update(player_t *player)
{

    if (player->entity.dy.h > 0)
    {
        player->state = PLAYER_STATE_FALL;
    }
    else if (player->entity.dy.w < 0)
    {
        player->state = PLAYER_STATE_JUMP;
    }

    uint8_t input = joypad();

    switch (player->state)
    {
    case PLAYER_STATE_FALL:
        player_update_fall(player, input);
        break;
    case PLAYER_STATE_GROUNDED:
        player_update_grounded(player, input);
        break;
    case PLAYER_STATE_JUMP:
        player_update_jump(player, input);
        break;
    case PLAYER_STATE_WALK:
        player_update_walk(player, input);
        break;
    default:
        player->state = PLAYER_STATE_FALL;
        break;
    };

    entity_update((entity_t *)player);
}

void player_update_fall(player_t *player, uint8_t input)
{
    entity_set_tile((entity_t *)player, SPR_INDEX_KNIGHT_2);

    player_move(player, input, 2, 10);

    if (on_floor((entity_t *)player)) {
        player->state = PLAYER_STATE_GROUNDED;
    }
}
void player_update_jump(player_t *player, uint8_t input)
{
    entity_set_tile((entity_t *)player, SPR_INDEX_KNIGHT_1);

    player_move(player, input, 2, 10);

    if (on_floor((entity_t *)player)) {
        player->state = PLAYER_STATE_GROUNDED;
    }

    if (player->entity.dy.h > 0)
    {
        player->state = PLAYER_STATE_FALL;
    }
}
void player_update_grounded(player_t *player, uint8_t input)
{
    entity_set_tile((entity_t *)player, SPR_INDEX_KNIGHT_0);

    if (((input & J_A) > 0))
    {
        player->entity.dy.h = PLAYER_JUMP_DYH;
        player->entity.dy.l = PLAYER_JUMP_DYL;
    }

    player_move(player, input, 2, 2);

    if (player->entity.dx.w != 0)
    {
        player->state = PLAYER_STATE_WALK;
    }
}
void player_update_walk(player_t *player, uint8_t input)
{
    if (((input & J_A) > 0))
    {
        player->entity.dy.h = PLAYER_JUMP_DYH;
        player->entity.dy.l = PLAYER_JUMP_DYL;
        return;
    }

    uint8_t frame = player->entity.timer >> 2;
    frame = frame % 4;
    uint8_t index = SPR_INDEX_KNIGHT_0;
    switch (frame) {
        case 0x1:
            index = SPR_INDEX_KNIGHT_1;
            break;
        case 0x2:
            index = SPR_INDEX_KNIGHT_0;
            break;
        case 0x3:
            index = SPR_INDEX_KNIGHT_2;
            break;
    }

    entity_set_tile((entity_t *)player, index);

    player_move(player, input, 2, 2);

    if (player->entity.dx.w == 0) {
        player->state = PLAYER_STATE_GROUNDED;
    }
}

void player_move(player_t *player, uint8_t input, int8_t speed, uint8_t friction)
{
    uint8_t p = player->entity.props;
    if ((input & J_RIGHT))
    {
        player->entity.dx.h = speed;

        // Clear fifth bit
        p = p & (~PROP_HORI_FLIP);
        entity_set_prop((entity_t *)player, p);
    }
    else if ((input & J_LEFT))
    {
        player->entity.dx.h = -speed;
        p = p | (PROP_HORI_FLIP);
        entity_set_prop((entity_t *)player, p);
    }
    else
    {
        player->entity.dx.w = player->entity.dx.w / friction;
        if (player->entity.dx.h == -1 && player->entity.dx.l > 128) {
            player->entity.dx.w = 0;
        }
        if (player->entity.dx.h == 0 && player->entity.dx.l < 128) {
            player->entity.dx.w = 0;
        }
    }
}