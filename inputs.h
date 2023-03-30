#ifndef _INPUTS_H
#define _INPUTS_H

#include <stdint.h>

extern uint8_t * const input;
extern uint8_t * const pressed;
extern uint8_t * const released;

void update_input();

#endif