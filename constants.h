#ifndef CONSTANTS_H
#define CONSTANTS_H


#define SOLID_FLAG 0x8
#define SOLID_TILE(TILE_INDEX) {return TILE_INDEX & SOLID_FLAG > 0;}

#define SPR_INDEX_BLANK     0x0
#define SPR_INDEX_KNIGHT    0x1
#define SPR_INDEX_BLORB     0x2
#define SPR_INDEX_BEETO     0x3
#define SPR_INDEX_GEM_0     0x4
#define SPR_INDEX_GEM_1     0x5
#define SPR_INDEX_GEM_2     0x6
#define SPR_INDEX_GEM_4     0x7
#define SPR_INDEX_BUBBLE    0x8

#define TILE_INDEX_BLANK        0x0
#define TILE_INDEX_SKY_BG       0x1
#define TILE_INDEX_DIRT_BG      0x2
#define TILE_INDEX_DIRT_BG_TL   0x3
#define TILE_INDEX_DIRT_BG_L    0x4
#define TILE_INDEX_DIRT_BG_BL   0x5
#define TILE_INDEX_DIRT_FG      0x8
#define TILE_INDEX_GRASS_FG     0x9



#endif