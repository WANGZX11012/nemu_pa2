#ifndef NPC_DEVICE_H
#define NPC_DEVICE_H

#include <cstdint>

bool npc_device_in_range(uint32_t addr);
void npc_device_write(uint32_t addr, int len, uint32_t data);
uint32_t npc_device_read(uint32_t addr, int len);

#endif