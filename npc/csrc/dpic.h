#ifndef NPC_DPIC_H
#define NPC_DPIC_H

#include <cstdint>
#include <cstddef>

// Initialize program memory from hex file
bool load_hex_program(const char *path);

// Fallback init memory explicitly when no hex file is provided
void init_pmem(size_t bytes);

// Read instruction word from simulated program memory
uint32_t pc_read(uint32_t addr);

// DPI-C exposed ebreak handler (implemented in C++)
extern "C" void npc_ebreak(int code);

#endif // NPC_DPIC_H
