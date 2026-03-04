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

#include <isa.h>
#include <cpu/difftest.h>
#include "../local-include/reg.h"

#include "itbuf.h" //new

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) //pc是是nemu的pc，ref_r结构体中是spike的各种量 
//cpu.pc 是执行后的 PC（post‑exec）
//传入的这个pc是刚刚执行完成的指令的地址
{
  /* Compare PC first (ref_r contains spike's state). */
  /* Note: compare REF's next-PC with DUT's current pc (post-execution). */
  if (ref_r->pc != cpu.pc) {
    printf("Difftest failed at pc = " FMT_WORD "\n", pc);
    printf("  pc : ref = " FMT_WORD ", dut = " FMT_WORD "\n", ref_r->pc, cpu.pc);
  #ifdef CONFIG_ITRACE
      itbuf_d(10); /* 打印最近 10 条 */
  #endif
    return false;
  }

  for (int i = 0; i <  MUXDEF(CONFIG_RVE, 16, 32); i++) 
  {
    if (ref_r->gpr[i] != cpu.gpr[i]) 
    {
      printf("Difftest failed at pc = " FMT_WORD "\n", pc);
      printf("  reg[%2d] (%-4s): ref = " FMT_WORD ", dut = " FMT_WORD "\n",
             i, reg_name(i), ref_r->gpr[i], cpu.gpr[i]);
  #ifdef CONFIG_ITRACE //不匹配时itrace打印
      itbuf_d(10);
  #endif
      return false;
    }
  }

  return true; // all matched
}

void isa_difftest_attach() {
  ref_difftest_regcpy(&cpu, DIFFTEST_TO_REF);
}
