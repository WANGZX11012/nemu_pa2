#ifndef __NPC_ISA_H__
#define __NPC_ISA_H__

#include <npc_common.h>

// 目前最小版 NPC-NEMU 只保留寄存器显示接口，供 `info r` 使用。
void npc_isa_reg_display(void);

#endif
