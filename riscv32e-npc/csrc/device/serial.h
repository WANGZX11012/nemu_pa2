#ifndef NPC_DEVICE_SERIAL_H
#define NPC_DEVICE_SERIAL_H

#include <cstdint>

static constexpr uint32_t SERIAL_ADDR = 0xa00003f8u;

bool     serial_in_range(uint32_t addr);
uint32_t serial_read   (uint32_t addr, int len);
void     serial_write  (uint32_t addr, int len, uint32_t data);

#endif
