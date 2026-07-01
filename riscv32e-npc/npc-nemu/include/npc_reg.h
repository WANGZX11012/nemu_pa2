#ifndef __NPC_REG_H__
#define __NPC_REG_H__

#include <npc_common.h>

// 显示所有寄存器（info r）
void npc_isa_reg_display(void);

// 将寄存器名字符串转为数值（供表达式求值使用）
// 支持格式：x0..x31, a0..a7, ra, sp, gp, tp, t0..t6,
//           s0..s11, zero, pc
//           $$0..$$31（双 $ 加编号）
word_t npc_isa_reg_str2val(const char *s, bool *success);

#endif
