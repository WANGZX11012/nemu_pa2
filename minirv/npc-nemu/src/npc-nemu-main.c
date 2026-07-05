#include <npc_common.h>

void npc_init_monitor(int, char *[]);
// void am_init_monitor();
void npc_engine_start();
int npc_is_exit_status_bad();

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
// #ifdef CONFIG_TARGET_AM
//   am_init_monitor();
// #else
  npc_init_monitor(argc, argv);
// #endif

  /* Start engine. */
  npc_engine_start();//就进入sdb调试

  return npc_is_exit_status_bad();
}
