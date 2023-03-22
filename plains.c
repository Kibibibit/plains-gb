#include <gb/gb.h>
#include <gb/cgb.h>
#include <gbdk/emu_debug.h>
#include "bkg_funcs.h"
#include "sprites.h"
#include "tiles.h"
#include "entity.h"
#include "fixed.h"


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
    set_bkg_data(0x0,10,tiles);
    set_sprite_palette(0x0,4,palettes);
    set_bkg_palette(0,4,bkg_palettes);

    

    entity_t * player = create_entity(0x1, 0x0);
    entity_move(player, 8, 16);

    fixed_t test;
    test.h = -5;
    test.l = 128;

    fixed_t test2;
    test2.h = 4;
    test2.l = 128;

    fixed_t out;
    out.w = test2.w + test.w;

    BGB_printf("%d.%d %d %x", test.h, test.l, test.w, test.w);
    BGB_printf("%d.%d %d %x", test2.h, test2.l, test2.w, test2.w);
    BGB_printf("%d.%d %d %x", out.h, out.l, out.w, out.w);
    BGB_printf("%d",test.w+test2.w);

    while(1) {

        entity_update(player);
        entity_draw(player);
        
        wait_vbl_done();
        
    }

}

//   FB.80
// + 05.80
//   