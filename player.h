#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>
#include "entity.h"

typedef struct {
    entity_t entity;
    uint8_t state;
    uint8_t health;
} player_t;


#endif