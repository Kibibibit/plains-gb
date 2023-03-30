#include "inputs.h"
#include <stdint.h>
#include <gb/gb.h>


uint8_t _input = 0x0;
uint8_t _pressed = 0x0;
uint8_t _released = 0x0;

uint8_t * const input = &_input;
uint8_t * const pressed = &_pressed;
uint8_t * const released = &_released;


uint8_t next_input = 0x0;
uint8_t changed = 0x0;


void update_input()
{
    next_input = joypad();
    changed = next_input ^ *input;
    *pressed = (changed & next_input);
    *released = (changed & *input);
    *input = next_input;
}