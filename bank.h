#ifndef BANK_H
#define BANK_H

#include <stdint.h>

#define BANK_COUNT 5
#define BANK_FULL 8
#define OAM_FULL 0xFF

uint8_t get_free_bank_entry(uint8_t bank);
uint8_t clear_bank_entry(uint8_t bank, uint8_t i);
uint8_t set_bank_entry(uint8_t bank, uint8_t i);
uint8_t get_free_bank();
void set_bank(uint8_t bank_entry);
extern const unsigned char bank_table[];
static unsigned char oam_banks[BANK_COUNT];

#endif