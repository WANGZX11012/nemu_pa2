#ifndef __FTRACE_H__
#define __FTRACE_H__

#include <stdint.h>

void init_ftrace(const char *elf_path);
void ftrace_call(uint32_t call_pc, uint32_t target);
void ftrace_ret(uint32_t ret_pc, uint32_t target);

#endif


