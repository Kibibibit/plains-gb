#include "iters.h"
#include <stdint.h>

uint8_t _iter_i_val = 0x0;
uint8_t _iter_x_val = 0x0;
uint8_t _iter_y_val = 0x0;

uint8_t *const iter_i = &_iter_i_val;
uint8_t *const iter_x = &_iter_x_val;
uint8_t *const iter_y = &_iter_y_val;

void set_iter(uint8_t *const iter, uint8_t val) {
    *iter = val;
} 
void reset_iter(uint8_t *const iter)
{
    set_iter(iter, 0x0);
}
void inc_iter(uint8_t *const iter)
{
    *iter = (*iter) + 0x1;
}