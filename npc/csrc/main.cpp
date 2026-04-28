#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include "Vtop.h"
#include "verilated.h"
#include "verilated_vcd_c.h" //引入 Verilator 的 VCD 波形接口声明。
// 没它就不能用 VerilatedVcdC，也不能写 wave.vcd 给 GTKWave 看。
#include "dpic.h" //dpic相关
#include "sim_bridge.h"

// DPI-C and pc read functions are implemented in dpic.cpp

int main(int argc, char** argv) {
  Verilated::commandArgs(argc, argv);

  Verilated::traceEverOn(true);
  VerilatedVcdC* tfp = new VerilatedVcdC;

  Vtop *top = new Vtop;

  top->trace(tfp, 0); //把你的 DUT（top）和波形对象 tfp 绑定。
  tfp->open("obj_dir/wave.vcd"); //创建vcd
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

  if (argc >= 2) 
  {
    if (!load_hex_program(argv[1])) 
    {
      printf("main: failed to load hex %s, exiting\n", argv[1]);
      return 1;
    }
  } 
  else 
  {
    printf("main: no hex path provided, init pmem only\n");

    init_pmem(1024 * 1024);

    printf("main: using default embedded instruction set\n");
  }

  // 暂时用固定步数（后续可换成 ebreak 退出）
  for (int i = 0; i < 10000000; i++) 
  {
    // 1) 取指：用当前pc去读内存
    top->inst = pc_read(top->pc);
    // printf("pc=0x%08x inst=0x%08x\n", top->pc, top->inst);
    // 2) 一个时钟周期
    top->clk = 1; top->eval(); tfp->dump(sim_time++);
    top->clk = 0; top->eval(); tfp->dump(sim_time++);

    // 3) 在 ebreak / abort 后及时停止仿真
    if (npc_sim_is_halted()) {
      int code = npc_sim_halt_code();
      if (code == 0) 
      {
        printf("HIT GOOD TRAP\n");
      } 
      else 
      {
        printf("HIT BAD TRAP(code=%d)\n", code);
      }
      break;
    }

    if (npc_sim_is_aborted()) {
      printf("ABORT after invalid instruction\n");
      break;
    }

  }

  tfp->close();
  delete tfp;

  delete top;
  return 0;
}
