#ifndef NPC_DPIC_H
#define NPC_DPIC_H

#include <cstdint>
#include <cstddef>

#ifdef __cplusplus
extern "C" {
#endif

// Initialize program memory from hex file
bool load_hex_program(const char *path);

// Fallback init memory explicitly when no hex file is provided
void init_pmem(size_t bytes);

// Read instruction word from simulated program memory
uint32_t pc_read(uint32_t addr);

// DPI-C exposed ebreak handler (implemented in C++)
void npc_ebreak(int code);
// DPI-C exposed invalid inst handler
void npc_invalid_inst(void); // Ensure this matches the current implementation

#ifdef __cplusplus
}
#endif

#endif // NPC_DPIC_H
