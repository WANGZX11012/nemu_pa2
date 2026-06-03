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
#include <memory/paddr.h>

// this is not consistent with uint8_t
// but it is ok since we do not access the array directly

//客户程序？
static const uint32_t img [] = {
  0x00000297,  // auipc t0,0
  0x00028823,  // sb  zero,16(t0)     //把0x8000_0010高位字节写为0了 小端序
  0x0102c503,  // lbu a0,16(t0)       //读到字节为0 写入a0寄存器
  0x00100073,  // ebreak (used as nemu_trap)
  0xdeadbeef,  // some data
};

static void restart() {
  /* Set the initial program counter. */
  cpu.pc = RESET_VECTOR;

  /* The zero register is always 0. */
  cpu.gpr[0] = 0;
  cpu.csr.mstatus = 0x1800; //初始化mstatus

  // RT-Thread 会读取这些 CSR，需要初始化为与 NPC 一致的默认值
  cpu.csr.mvendorid = 0x79737978;  // "ysyx"，与 NPC CSRFile.v 一致
  cpu.csr.marchid   = 0x018D08CF;  // 学号，与 NPC CSRFile.v 一致
  cpu.csr.mcycle_lo = 0;
  cpu.csr.mcycle_hi = 0;

}

void init_isa() {
  /* Load built-in image. */
  memcpy(guest_to_host(RESET_VECTOR), img, sizeof(img));

  /* Initialize this virtual computer system. */
  restart();
}
