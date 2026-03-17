#include <cstdint>
#include <cstdio>
#include "Vtop.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

// 最小程序内存：按32位字存放指令
static uint32_t pmem[256] = {
  0x01400513, // 0x00: addi a0, zero, 20
  0x010000e7, // 0x04: jalr ra, 16(zero) -> 跳到 0x10, ra=0x08
  0x00c000e7, // 0x08: jalr ra, 12(zero) -> 跳到 0x0c, ra=0x0c
  0x00c00067, // 0x0c: jalr zero,12(zero) -> 自旋 halt
  0x00a50513, // 0x10: addi a0, a0, 10
  0x00008067, // 0x14: jalr zero,0(ra) -> 返回到 ra(0x08)
};

static inline uint32_t pmem_read(uint32_t addr) {
  // 指令按4字节对齐读取
  return pmem[(addr & ~0x3u) >> 2];
}

int main(int argc, char** argv) {
  Verilated::commandArgs(argc, argv);


  Verilated::traceEverOn(true);
  VerilatedVcdC* tfp = new VerilatedVcdC;

  

  Vtop *top = new Vtop;

  top->trace(tfp, 0);
  tfp->open("obj_dir/wave.vcd");
  vluint64_t sim_time = 0;

  // reset
  top->clk = 0;
  top->reset = 1;
  top->inst = 0;
  top->eval();
  tfp->dump(sim_time++);

  top->clk = 1;
  top->eval();
  tfp->dump(sim_time++);

  top->clk = 0;
  top->reset = 0;
  top->eval();
  tfp->dump(sim_time++);

  // 暂时用固定步数（后续可换成 ebreak 退出）
  for (int i = 0; i < 50; i++) {
    // 1) 取指：用当前pc去读内存
    top->inst = pmem_read(top->pc);
    printf("pc=0x%08x inst=0x%08x\n", top->pc, top->inst);
    // 2) 一个时钟周期
    top->clk = 1; top->eval(); tfp->dump(sim_time++);
    top->clk = 0; top->eval(); tfp->dump(sim_time++);

    // 可选调试
    
  }

  tfp->close();
  delete tfp;

  delete top;
  return 0;
}