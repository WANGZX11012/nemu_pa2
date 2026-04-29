#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include "verilated.h"
#include "dpic.h" //dpic相关
#include "difftest.h"
#include "sim_bridge.h"

// DPI-C and pc read functions are implemented in dpic.cpp

int main(int argc, char** argv) {
  Verilated::commandArgs(argc, argv);

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

  npc_sim_init();

  difftest_init(npc_sim_get_pc());

  // 暂时用固定步数（后续可换成 ebreak 退出）
  for (int i = 0; i < 10000000; i++) 
  {
    // 1) 取指：先保存当前 PC 和当前指令，供 abort 日志使用
    uint32_t cur_pc = npc_sim_get_pc();
    uint32_t cur_inst = pc_read(cur_pc);
    // 把当前拍的 PC/指令记录到仿真桥里，便于非法指令时打印真实现场
    npc_sim_set_trace(cur_pc, cur_inst);
    // 2) 一个时钟周期
    npc_sim_step_once();

    uint32_t dut_gpr[32];
    npc_sim_get_gprs(dut_gpr);
    if (!difftest_step(npc_sim_get_pc(), dut_gpr)) 
    {
      printf("ABORT after difftest mismatch\n");
      return 1;
    }

    // 3) 在 ebreak / abort 后及时停止仿真
    if (npc_sim_is_halted()) {
      int code = npc_sim_halt_code();
      if (code == 0) {
        printf("HIT GOOD TRAP\n");
        return 0;
      } else {
        printf("HIT BAD TRAP(code=%d)\n", code);
        return 1;
      }
    }

    if (npc_sim_is_aborted()) {
      printf("ABORT after invalid instruction\n");
      return 1;
    }

  }
  return 0;
}
