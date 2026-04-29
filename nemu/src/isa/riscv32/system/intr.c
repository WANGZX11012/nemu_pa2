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




word_t isa_raise_intr(word_t NO, vaddr_t epc) 
{
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * Then return the address of the interrupt/exception vector.
   */
  cpu.csr.mcause = NO; //异常值赋值 M mode是11
  cpu.csr.mepc = epc;

  word_t mstatus = cpu.csr.mstatus;

  mstatus = set_bit(mstatus, 7, get_bit(mstatus, 3));// MPIE <- MIE ，MPIE是第七位 MIE是第三位
  mstatus = set_bit(mstatus, 3, 0);//MIE 设置为0
  //MPP 11～12位设置为3
  mstatus = set_bit(mstatus, 11, 1);
  mstatus = set_bit(mstatus, 12, 1);

  cpu.csr.mstatus = mstatus; //写回



  return cpu.csr.mtvec;//要return 下一个pc
}

word_t isa_query_intr() {
  return INTR_EMPTY;
}
