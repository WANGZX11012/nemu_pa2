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

static inline bool check_reg_or_fail(const char *name, vaddr_t pc, word_t ref_val, word_t dut_val) 
{
  if (!difftest_check_reg(name, pc, ref_val, dut_val)) 
  {
    #ifdef CONFIG_ITRACE
      itbuf_d(10);
    #endif
    return false;
  }
  return true;
}

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) 
{
  if (!check_reg_or_fail("pc", pc, ref_r->pc, cpu.pc)) return false; //单独比较pc

  for (int i = 0; i < MUXDEF(CONFIG_RVE, 16, 32); i++) 
  {
    if (!check_reg_or_fail(reg_name(i), pc, ref_r->gpr[i], cpu.gpr[i])) return false;
  }

  //新增的CSR对比
  if (!check_reg_or_fail("mcause", pc, ref_r->csr.mcause, cpu.csr.mcause)) return false;
  if (!check_reg_or_fail("mepc", pc, ref_r->csr.mepc, cpu.csr.mepc)) return false;
  if (!check_reg_or_fail("mstatus", pc, ref_r->csr.mstatus, cpu.csr.mstatus)) return false;
  if (!check_reg_or_fail("mtvec", pc, ref_r->csr.mtvec, cpu.csr.mtvec)) return false;


  return true;
}

void isa_difftest_attach() 
{
  ref_difftest_regcpy(&cpu, DIFFTEST_TO_REF);
}
