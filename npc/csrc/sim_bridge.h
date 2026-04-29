#ifndef NPC_SIM_BRIDGE_H
#define NPC_SIM_BRIDGE_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void npc_sim_init(void);
void npc_sim_step_once(void);
uint32_t npc_sim_get_pc(void);
uint32_t npc_sim_get_inst(void);
bool npc_sim_is_halted(void);
bool npc_sim_is_aborted(void);
int npc_sim_halt_code(void);
void npc_sim_mark_halt(int code);
void npc_sim_mark_abort(void);

#ifdef __cplusplus
}
#endif

#endif
