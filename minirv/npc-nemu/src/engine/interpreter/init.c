#include <cpu/npc_cpu.h>

void npc_sdb_mainloop();

void npc_engine_start() 
{
//   #ifdef CONFIG_TARGET_AM
//     cpu_exec(-1);
//   #else
  /* Receive commands from user. */
    npc_sdb_mainloop();
//   #endif
}
