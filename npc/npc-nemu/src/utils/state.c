#include <npc_utils.h>

NpcState npc_state = { .state = NPC_STOP }; //初始化为stop

int npc_is_exit_status_bad() 
{
  int good = (npc_state.state == NPC_END && npc_state.halt_ret == 0) ||
    (npc_state.state == NPC_QUIT);
  return !good;
}

