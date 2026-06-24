#include <npc_isa.h>
#include <cpu/npc_cpu.h>
#include <stdio.h>

void npc_isa_reg_display(void) 
{
  printf("pc  = 0x%08x\n", npc_cpu.pc);
  for (int i = 0; i < NPC_GPR_NR; i++) 
  {
    printf("x%-2d = 0x%08x\n", i, npc_cpu.gpr[i]);
  }
}
