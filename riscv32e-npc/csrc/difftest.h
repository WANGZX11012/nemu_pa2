#ifndef NPC_DIFFTEST_H
#define NPC_DIFFTEST_H

#include <cstdint>

// Initialize difftest: load REF .so, sync memory (via memcpy) and registers
void difftest_init(uint32_t dut_pc);

// Execute one difftest step: REF exec 1 inst, then compare registers
// Returns true if match, false if mismatch
bool difftest_step(uint32_t dut_pc, const uint32_t *dut_gpr);

#endif
