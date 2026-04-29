/***************************************************************************************
* Copyright (c) 2014-2024 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#ifndef __ISA_RISCV_H__
#define __ISA_RISCV_H__

#include <common.h>

//新增的位操作函数
static inline word_t set_bit(word_t x, int n, int v) 
{
  return v ? (x | (1u << n)) : (x & ~(1u << n));
}

static inline int get_bit(word_t x, int n) 
{
  return (x >> n) & 1u;
}


typedef struct 
{
  word_t mcause; // 异常/中断原因 中断(1) / 异常(0)低位：具体编号（如 ecall、非法指令、timer irq 等）
  vaddr_t mepc; //异常返回地址(pc) 异常处理结束后 mret 通常从这里返回
  word_t mstatus;//机器状态寄存器
  word_t mtvec;//异常入口地址 如asm trap
} riscv32_CSRs;




typedef struct {
  word_t gpr[MUXDEF(CONFIG_RVE, 16, 32)];
  vaddr_t pc;
  riscv32_CSRs csr; //adding csr

} MUXDEF(CONFIG_RV64, riscv64_CPU_state, riscv32_CPU_state); //用来difftest

// decode
typedef struct {
  uint32_t inst;
} MUXDEF(CONFIG_RV64, riscv64_ISADecodeInfo, riscv32_ISADecodeInfo);

#define isa_mmu_check(vaddr, len, type) (MMU_DIRECT)

#endif
