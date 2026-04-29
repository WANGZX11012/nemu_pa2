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
void npc_sim_get_gprs(uint32_t *gpr);
// 记录当前拍的 PC/inst，供 abort 时打印
void npc_sim_set_trace(uint32_t pc, uint32_t inst);
bool npc_sim_is_halted(void);
bool npc_sim_is_aborted(void);
int npc_sim_halt_code(void);
void npc_sim_mark_halt(int code);
void npc_sim_mark_abort(void);

#ifdef __cplusplus
}
#endif

#endif
