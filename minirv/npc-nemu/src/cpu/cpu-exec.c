#include <cpu/npc_cpu.h>
#include "../../../csrc/sim_bridge.h"
#include <stdio.h>
#include <npc_debug.h>

#define MAX_INST_TO_PRINT 10

NPC_CPU_state npc_cpu = {};
uint64_t g_nr_guest_inst = 0;     // 已执行的客户指令计数
static uint64_t g_timer = 0;      // unit: us 耗时
static bool g_print_step = false; // 是否打印每条指令的trace

void npc_set_state(int state, vaddr_t pc, int halt_ret) 
{
  npc_state.state = state;
  npc_state.halt_pc = pc;
  npc_state.halt_ret = halt_ret;
}

static void npc_sync_cpu_state(void) 
{
  npc_cpu.pc = npc_sim_get_pc();//同步到nemu侧的cpu状态
}

void npc_exec_once(void) 
{
  if(npc_sim_is_aborted())//首先检查abort
  {
    npc_set_state(NPC_ABORT, npc_cpu.pc, -1);
  }

  if (npc_state.state == NPC_END || npc_state.state == NPC_ABORT || npc_state.state == NPC_QUIT) 
  {
    return;
  }

  

  npc_sim_init();       //reset
  npc_state.state = NPC_RUNNING;

  uint32_t this_pc = npc_sim_get_pc();
  npc_sim_step_once();   //时钟脉冲一次
  npc_sync_cpu_state();  //更新nemu侧
  g_nr_guest_inst++;

  if (npc_sim_is_aborted())
  {
    npc_set_state(NPC_ABORT, npc_cpu.pc, -1);
    return;
  }

   npc_Log(ANSI_FMT("PC:0x%08x inst:0x%08x", ANSI_FG_GREEN), this_pc, npc_sim_get_inst());

  if (g_print_step) 
  {
    printf("pc=0x%08x inst=0x%08x next_pc=0x%08x\n",
        this_pc, npc_sim_get_inst(), npc_cpu.pc);
  }

  if (npc_sim_is_halted()) 
  {
    int halt_ret = npc_sim_halt_code();
    npc_set_state(NPC_END, this_pc, halt_ret);
    printf("npc: program ended at pc = 0x%08x, code = %d\n", this_pc, halt_ret);
    return;
  }

  npc_state.state = NPC_STOP;
}

void npc_cpu_exec(uint64_t n) 
{
  if (npc_state.state == NPC_ABORT || /*npc_state.state == NPC_END ||*/ npc_state.state == NPC_QUIT)
  {
    return;//直接退出
  }

  if (npc_state.state == NPC_END) 
  {
    printf(ANSI_FMT("Press Q to quit\n", ANSI_FG_RED));
    return;
  }


  


  if (n == (uint64_t)-1) 
  {
        while (npc_state.state != NPC_END &&
          npc_state.state != NPC_ABORT &&
          npc_state.state != NPC_QUIT) 
        {
            npc_exec_once();
        }
    return;
  }

  for (uint64_t i = 0; i < n; i++) 
  {
    if (npc_state.state == NPC_END || npc_state.state == NPC_ABORT || npc_state.state == NPC_QUIT) 
    {
        printf(ANSI_FMT("Press Q to quit\n", ANSI_FG_RED));
        break;
    }
    npc_exec_once();
  }
}

void assert_fail_msg() {
  fprintf(stderr, "npc-nemu assertion failed\n");
}
