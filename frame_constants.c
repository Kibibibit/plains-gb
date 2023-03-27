#include <stdint.h>
#include "frame_constants.h"
#include "sprite_constants.h"
#include "frame.h"

const uint8_t _blank_frame_tiles[1] = {
    SPR_BLANK};

const uint8_t _shovel_frame_idle_tiles[4] = {
    SPR_SK_IDLE_TL, SPR_SK_IDLE_TR, SPR_SK_IDLE_BL, SPR_SK_IDLE_BR};
const uint8_t _shovel_frame_walk_1_tiles[4] = {
    SPR_SK_WALK_TL, SPR_SK_WALK_TR, SPR_SK_WALK_1_BL, SPR_SK_WALK_1_BR};
const uint8_t _shovel_frame_walk_2_tiles[4] = {
    SPR_SK_WALK_TL, SPR_SK_WALK_TR, SPR_SK_WALK_2_BL, SPR_SK_WALK_2_BR};
const uint8_t _shovel_frame_jump_tiles[4] = {
    SPR_SK_WALK_TL, SPR_SK_WALK_TR, SPR_SK_JUMP_BL, SPR_SK_JUMP_BR};
const uint8_t _shovel_frame_drop_tiles[4] = {
    SPR_SK_DROP_TL, SPR_SK_DROP_TR, SPR_SK_DROP_BL, SPR_SK_DROP_BR};
const uint8_t _shovel_frame_attack_1_tiles[4] = {
    SPR_SK_WALK_TL, SPR_SK_WALK_TR, SPR_SK_ATTACK_1_BL, SPR_SK_ATTACK_1_BR};
const uint8_t _shovel_frame_attack_2_tiles[4] = {
    SPR_SK_WALK_TL, SPR_SK_WALK_TR, SPR_SK_ATTACK_2_BL, SPR_SK_ATTACK_2_BR};

const frame_t _blank_frame = {
    .tiles = _blank_frame_tiles,
    .tile_count = 0x1,
    .size = 0x11};

const frame_t _shovel_frame_idle = {
    .tiles = _shovel_frame_idle_tiles,
    .tile_count = 0x4,
    .size = 0x22};
const frame_t _shovel_frame_walk_1 = {
    .tiles = _shovel_frame_walk_1_tiles,
    .tile_count = 0x4,
    .size = 0x22};
const frame_t _shovel_frame_walk_2 = {
    .tiles = _shovel_frame_walk_2_tiles,
    .tile_count = 0x4,
    .size = 0x22};
const frame_t _shovel_frame_jump = {
    .tiles = _shovel_frame_jump_tiles,
    .tile_count = 0x4,
    .size = 0x22};
const frame_t _shovel_frame_drop = {
    .tiles = _shovel_frame_drop_tiles,
    .tile_count = 0x4,
    .size = 0x22};
const frame_t _shovel_frame_attack_1 = {
    .tiles = _shovel_frame_attack_1_tiles,
    .tile_count = 0x4,
    .size = 0x22};
const frame_t _shovel_frame_attack_2 = {
    .tiles = _shovel_frame_attack_2_tiles,
    .tile_count = 0x4,
    .size = 0x22};

const frame_t *const blank_frame = &_blank_frame;

const frame_t *const shovel_frame_idle = &_shovel_frame_idle;
const frame_t *const shovel_frame_walk_1 = &_shovel_frame_walk_1;
const frame_t *const shovel_frame_walk_2 = &_shovel_frame_walk_2;
const frame_t *const shovel_frame_jump = &_shovel_frame_jump;
const frame_t *const shovel_frame_drop = &_shovel_frame_drop;
const frame_t *const shovel_frame_attack_1 = &_shovel_frame_attack_1;
const frame_t *const shovel_frame_attack_2 = &_shovel_frame_attack_2;