#include <gb/gb.h>
#include <gb/cgb.h>
#include "bkg_funcs.h"
#include "sprites.h"
#include "tiles.h"


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


int main() {

    init_bkg(0);
    init_win(0);

    SHOW_SPRITES;
    SHOW_BKG;    

    set_sprite_data(0x1,8,sprites);
    set_bkg_data(0x0,4,tiles);
    set_sprite_palette(0x0,4,palettes);
    set_sprite_tile(0,0x2);

    set_bkg_palette(0,4,bkg_palettes);

    set_bkg_tile_xy(0,0,3);
    set_bkg_tile_xy(0,1,2);
    set_bkg_tile_xy(0,2,1);
    set_bkg_prop_xy(0,0,1);
    set_bkg_prop_xy(0,1,1);
    set_bkg_prop_xy(0,2,1);

    set_sprite_prop(0,1);

    move_sprite(0,40,40);

    


    while(1) {

    

        wait_vbl_done();
        
    }

}