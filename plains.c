#include <gb/gb.h>
#include <gb/cgb.h>
#include "inputs.h"
#include "bkg_funcs.h"
#include "sprites.h"
#include "tiles.h"
#include "player.h"
#include "player_globals.h"
#include "fixed.h"
#include "constants.h"
#include "frame_constants.h"

const palette_color_t palettes[] = {
    spritesCGBPal0c0,
    spritesCGBPal0c1,
    spritesCGBPal0c2,
    spritesCGBPal0c3,
    spritesCGBPal1c0,
    spritesCGBPal1c1,
    spritesCGBPal1c2,
    spritesCGBPal1c3,
    spritesCGBPal2c0,
    spritesCGBPal2c1,
    spritesCGBPal2c2,
    spritesCGBPal2c3,
    spritesCGBPal3c0,
    spritesCGBPal3c1,
    spritesCGBPal3c2,
    spritesCGBPal3c3,
    spritesCGBPal4c0,
    spritesCGBPal4c1,
    spritesCGBPal4c2,
    spritesCGBPal4c3,
};

const palette_color_t bkg_palettes[] = {
    tilesCGBPal0c0,
    tilesCGBPal0c1,
    tilesCGBPal0c2,
    tilesCGBPal0c3,
    tilesCGBPal1c0,
    tilesCGBPal1c1,
    tilesCGBPal1c2,
    tilesCGBPal1c3,
    tilesCGBPal2c0,
    tilesCGBPal2c1,
    tilesCGBPal2c2,
    tilesCGBPal2c3,
    tilesCGBPal3c0,
    tilesCGBPal3c1,
    tilesCGBPal3c2,
    tilesCGBPal3c3,
};

const uint8_t test_floor[] = {
    TILE_INDEX_GRASS_FG, TILE_INDEX_GRASS_FG, TILE_INDEX_GRASS_FG, TILE_INDEX_GRASS_FG, TILE_INDEX_GRASS_FG, TILE_INDEX_GRASS_FG, TILE_INDEX_GRASS_FG, TILE_INDEX_GRASS_FG, TILE_INDEX_GRASS_FG, TILE_INDEX_GRASS_FG, TILE_INDEX_GRASS_FG, TILE_INDEX_GRASS_FG, TILE_INDEX_GRASS_FG, TILE_INDEX_GRASS_FG, TILE_INDEX_GRASS_FG, TILE_INDEX_GRASS_FG, TILE_INDEX_GRASS_FG, TILE_INDEX_GRASS_FG, TILE_INDEX_GRASS_FG, TILE_INDEX_GRASS_FG};
const uint8_t test_floor_prop[] = {
    TILE_PAL_INDEX_FG_1, TILE_PAL_INDEX_FG_1, TILE_PAL_INDEX_FG_1, TILE_PAL_INDEX_FG_1, TILE_PAL_INDEX_FG_1, TILE_PAL_INDEX_FG_1, TILE_PAL_INDEX_FG_1, TILE_PAL_INDEX_FG_1, TILE_PAL_INDEX_FG_1, TILE_PAL_INDEX_FG_1, TILE_PAL_INDEX_FG_1, TILE_PAL_INDEX_FG_1, TILE_PAL_INDEX_FG_1, TILE_PAL_INDEX_FG_1, TILE_PAL_INDEX_FG_1, TILE_PAL_INDEX_FG_1, TILE_PAL_INDEX_FG_1, TILE_PAL_INDEX_FG_1, TILE_PAL_INDEX_FG_1, TILE_PAL_INDEX_FG_1};

int main()
{

    init_bkg(0);
    init_win(0);

    SHOW_SPRITES;
    SHOW_BKG;

    set_sprite_data(0x1, 20, sprites);
    set_bkg_data(TILE_INDEX_BLANK, 1, tilesTLE0);
    set_bkg_data(TILE_INDEX_SKY_BG, 1, tilesTLE1);
    set_bkg_data(TILE_INDEX_DIRT_BG, 1, tilesTLE2);
    set_bkg_data(TILE_INDEX_DIRT_BG_TL, 1, tilesTLE3);
    set_bkg_data(TILE_INDEX_DIRT_BG_L, 1, tilesTLE4);
    set_bkg_data(TILE_INDEX_DIRT_BG_BL, 1, tilesTLE5);
    set_bkg_data(TILE_INDEX_DIRT_FG, 1, tilesTLE6);
    set_bkg_data(TILE_INDEX_GRASS_FG, 1, tilesTLE7);
    set_bkg_data(TILE_INDEX_DIRT_BREAK, 1, tilesTLE8);
    set_bkg_data(TILE_INDEX_STONE_BREAK, 1, tilesTLE9);
    set_sprite_palette(0x0, 5, palettes);
    set_bkg_palette(0, 4, bkg_palettes);


    for (uint8_t i = 0; i < 0x20; i++) {
        set_bkg_tile_xy(i,11,TILE_INDEX_GRASS_FG);
        set_bkg_prop_xy(i, 11,TILE_PAL_INDEX_FG_1);
    }



    player_init();

    fixed_t * world_scroll_x_f = new_fixed(0x0,0x0);

    player_x->h = 40;
    player_y->h = 40;

    uint8_t last_x = 40;
    int8_t x_diff = 0;

    world_scroll_x = 0x0;
    world_scroll_y = 0x0;

    while (0x1)
    {
        update_input();
        
        
        player_update();
        x_diff = player_x->h-last_x;
        world_scroll_x += x_diff;
        scroll_bkg(x_diff,0);
        player_draw();
        wait_vbl_done();
        last_world_scroll_x = world_scroll_x;
        last_world_scroll_y = world_scroll_y;
        last_x = player_x->h;
    }
}
