#include <npc_common.h>

extern uint64_t g_nr_guest_inst;

#ifndef CONFIG_TARGET_AM
FILE *log_fp = NULL;
// cppcheck-suppress unusedFunction: CONFIG_TARGET_AM is not set, but function may be called
// __attribute__((used)): 通过 monitor.c 初始化调用（CONFIG_TARGET_AM）
__attribute__((used))void npc_init_log(const char *log_file) 
{
  log_fp = stdout;
  if (log_file != NULL) 
  {
    FILE *fp = fopen(log_file, "w");
    npc_Assert(fp, "Can not open '%s'", log_file);
    log_fp = fp;
  }
  npc_Log("Log is written to %s", log_file ? log_file : "stdout");
}

//MUXDEF预处理宏 如果定义了TRACE 就展开成前一个 
bool npc_log_enable() 
{
  return npc_MUXDEF(CONFIG_TRACE,
    (g_nr_guest_inst >= CONFIG_TRACE_START) &&
    (g_nr_guest_inst <= CONFIG_TRACE_END),
    false);
}
#endif
