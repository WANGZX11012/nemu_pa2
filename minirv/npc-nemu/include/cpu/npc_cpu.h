#ifndef __NPC_CPU_H__
#define __NPC_CPU_H__

#include <npc_common.h>
#include <npc_utils.h>

#define NPC_GPR_NR 32

typedef struct {
  uint32_t pc;
  uint32_t gpr[NPC_GPR_NR];
} NPC_CPU_state;

extern NPC_CPU_state npc_cpu;

void npc_cpu_exec(uint64_t n);
void npc_exec_once(void);
void npc_set_state(int state, vaddr_t pc, int halt_ret);
void npc_invalid_inst(vaddr_t thispc);

#endif
