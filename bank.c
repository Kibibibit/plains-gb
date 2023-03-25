#include <stdint.h>
#include "bank.h"

const unsigned char bank_table[0x80] = {
    0x1, 0x2, 0x1, 0x3, 0x1, 0x2, 0x1, 0x4, 0x1, 0x2, 0x1, 0x3, 0x1, 0x2, 0x1, 0x5, 0x1, 0x2, 0x1, 0x3, 0x1, 0x2, 0x1, 0x4, 0x1, 0x2, 0x1, 0x3, 0x1, 0x2, 0x1, 0x6, 0x1, 0x2, 0x1, 0x3, 0x1, 0x2, 0x1, 0x4, 0x1, 0x2, 0x1, 0x3, 0x1, 0x2, 0x1, 0x5, 0x1, 0x2, 0x1, 0x3, 0x1, 0x2, 0x1, 0x4, 0x1, 0x2, 0x1, 0x3, 0x1, 0x2, 0x1, 0x7, 0x1, 0x2, 0x1, 0x3, 0x1, 0x2, 0x1, 0x4, 0x1, 0x2, 0x1, 0x3, 0x1, 0x2, 0x1, 0x5, 0x1, 0x2, 0x1, 0x3, 0x1, 0x2, 0x1, 0x4, 0x1, 0x2, 0x1, 0x3, 0x1, 0x2, 0x1, 0x6, 0x1, 0x2, 0x1, 0x3, 0x1, 0x2, 0x1, 0x4, 0x1, 0x2, 0x1, 0x3, 0x1, 0x2, 0x1, 0x5, 0x1, 0x2, 0x1, 0x3, 0x1, 0x2, 0x1, 0x4, 0x1, 0x2, 0x1, 0x3, 0x1, 0x2, 0x1, BANK_FULL};

static unsigned char oam_banks[0x5] = {
    0x0, 0x0, 0x0, 0x0, 0x0};

uint8_t get_free_bank_entry(uint8_t bank)
{
    if ((bank % 0x2) == 0x0)
    {
        return 0x0;
    }
    const uint8_t val = (bank - 0x1) >> 0x1;
    return bank_table[val];
}

uint8_t clear_bank_entry(uint8_t bank, uint8_t i)
{
    return bank & (~(0x1 << i));
}

uint8_t set_bank_entry(uint8_t bank, uint8_t i)
{
    return bank | (0x1 << i);
}

uint8_t get_bank_entry(uint8_t bank, uint8_t i) {
    return (bank & (0x1 << i)) > 0x0;
}

uint8_t get_free_bank() {
    uint8_t i;
    for (i = 0; i < BANK_COUNT; i++) {
        const uint8_t val = get_free_bank_entry(i);
        const uint8_t bank_i = i << 0x4;
        if (val != BANK_FULL) {
            return val | bank_i;
        }
    }
    return OAM_FULL;
}
void set_bank(uint8_t bank_entry) {
    const uint8_t bank_i = (bank_entry & 0xF0) >> 0x4;
    const uint8_t entry_i = (bank_entry & 0x0F);
    const uint8_t set = get_bank_entry(oam_banks[bank_i], entry_i);
    
    oam_banks[bank_i] = set_bank_entry(oam_banks[bank_i], entry_i);

}