#include "sim_bridge.h"

#include "Vtop.h"
#include "dpic.h"
#include "verilated.h"

#include <cstdio>

//这里可以访问电路vtop

static Vtop *top = nullptr;
static bool sim_halted = false;
static int sim_halt_ret = 0;

static bool sim_abort = false;

static uint32_t last_inst = 0;

extern "C" void npc_sim_mark_halt(int code) 
{
  sim_halted = true;
  sim_halt_ret = code;
}

extern "C" void npc_sim_mark_abort(void)
{
  uint32_t abort_pc = (top != nullptr) ? top->pc : 0;
  std::fprintf(stderr, "NPC abort at 0x%08x INST:0x%08x\n", abort_pc, last_inst);
  sim_abort = true;
}

extern "C" bool npc_sim_is_aborted(void)
{
  return sim_abort;
}



extern "C" void npc_sim_init(void) 
{
  if (top != nullptr) return;

  top = new Vtop;

  top->clk = 0;
  top->reset = 1;
  top->inst = 0;
  top->eval(); //先重置 相当于main.cpp

  top->clk = 1;
  top->eval();

  top->clk = 0;
  top->reset = 0;
  top->eval();
}

extern "C" void npc_sim_step_once(void) //npc 电路时钟脉冲一次
{
  if (top == nullptr || sim_halted || sim_abort) return;

  uint32_t pc = top->pc;
  last_inst = pc_read(pc);
  top->inst = last_inst;

  top->clk = 1;
  top->eval();

  top->clk = 0;
  top->eval();
}

extern "C" uint32_t npc_sim_get_pc(void) 
{
  return (top != nullptr) ? top->pc : 0;
}

extern "C" uint32_t npc_sim_get_inst(void) 
{
  return last_inst;
}

extern "C" bool npc_sim_is_halted(void) 
{
  return sim_halted;
}

extern "C" int npc_sim_halt_code(void) 
{
  return sim_halt_ret;
}
