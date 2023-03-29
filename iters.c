#include "iters.h"
#include <stdint.h>

uint8_t _iter_i_val = 0x0;
uint8_t _iter_x_val = 0x0;
uint8_t _iter_y_val = 0x0;

uint8_t *const iter_i = &_iter_i_val;
uint8_t *const iter_x = &_iter_x_val;
uint8_t *const iter_y = &_iter_y_val;