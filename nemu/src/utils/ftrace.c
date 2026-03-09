#include "ftrace.h"
#include "utils.h"
#include "ftrace.h"
#include <memory/paddr.h>

static int fdepth = 0;

void init_ftrace(const char *elf_path) 
{
  //只为了传参数
  (void)elf_path;
}

void ftrace_call(uint32_t call_pc, uint32_t target) 
{
  for (int i = 0; i < fdepth; i++) log_write("  ");
  
  log_write("CALL %08x -> %08x\n", call_pc, target);
  fdepth++;
}

void ftrace_ret(uint32_t ret_pc, uint32_t target) 
{
  if (fdepth > 0) fdepth--;
  for (int i = 0; i < fdepth; i++) log_write("  ");
  log_write("RET  %08x -> %08x\n", ret_pc, target);
}
