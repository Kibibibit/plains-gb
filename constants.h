#ifndef CONSTANTS_H
#define CONSTANTS_H


#define SOLID_FLAG 0x8
#define SOLID_TILE(TILE_INDEX) {return TILE_INDEX & SOLID_FLAG > 0;}

#define SPR_INDEX_BLANK     0x0
#define SPR_INDEX_KNIGHT_0  0x1
#define SPR_INDEX_KNIGHT_1  0x2
#define SPR_INDEX_KNIGHT_2  0x3
#define SPR_INDEX_BLORB_0   0x4
#define SPR_INDEX_BLORB_1   0x5
#define SPR_INDEX_BEETO_0   0x6
#define SPR_INDEX_BEETO_1   0x7
#define SPR_INDEX_GEM_0     0x8
#define SPR_INDEX_GEM_1     0x9
#define SPR_INDEX_GEM_2     0xA
#define SPR_INDEX_GEM_4     0xB
#define SPR_INDEX_BUBBLE    0xC

#define TILE_INDEX_BLANK        0x0
#define TILE_INDEX_SKY_BG       0x1
#define TILE_INDEX_DIRT_BG      0x2
#define TILE_INDEX_DIRT_BG_TL   0x3
#define TILE_INDEX_DIRT_BG_L    0x4
#define TILE_INDEX_DIRT_BG_BL   0x5
#define TILE_INDEX_DIRT_FG      0x8
#define TILE_INDEX_GRASS_FG     0x9

#define SPR_PAL_INDEX_KNIGHT    0x0
#define SPR_PAL_INDEX_BLORB     0x1
#define SPR_PAL_INDEX_BEETO     0x2
#define SPR_PAL_INDEX_BOSS      0x3

#define TILE_PAL_INDEX_BLANK    0x0
#define TILE_PAL_INDEX_BG_0     0x1
#define TILE_PAL_INDEX_FG_1     0x2



#endif