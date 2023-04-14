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
#include "palettes.h"


int main()
{

    init_bkg(0);
    init_win(0);

    SHOW_SPRITES;
    SHOW_BKG;

    set_sprite_data(0x1, 20, sprites);
    set_sprite_palette(0x0, 1, sprite_palettes);
    set_bkg_palette(0, 2, tile_palettes_plains);


    for (uint8_t i = 0; i < 0x20; i++) {
        set_bkg_tile_xy(i,11,TILE_INDEX_GRASS_FG);
        set_bkg_tile_xy(i,10,TILE_INDEX_DIRT_BREAK);
        set_bkg_prop_xy(i, 11,TILE_PAL_INDEX_FG_1);
        set_bkg_prop_xy(i,10,TILE_PAL_INDEX_FG_1);
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
