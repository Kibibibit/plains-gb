#include <stdint.h>
#include "maths.h"

int8_t sign(int8_t x)
{
    if (x < 0)
    {
        return -1;
    }
    if (x > 0)
    {
        return 1;
    }
    return 0;
}