#ifndef __NPC_UTILS_H__
#define __NPC_UTILS_H__

#include <npc_common.h>

// ----------- state -----------

enum { NPC_RUNNING, NPC_STOP, NPC_END, NPC_ABORT, NPC_QUIT };
// NPC_RUNNING：模拟器正在执行指令（运行状态）。
// NPC_STOP：执行暂停（初始状态或单步后停止）。
// NPC_END：程序正常结束（客户程序完成）。
// NPC_ABORT：异常终止（错误或非法操作）。
// NPC_QUIT：用户主动退出（输入 "q"）。


typedef struct 
{
  int state;
  vaddr_t halt_pc;
  uint32_t halt_ret;
} NpcState;

extern NpcState npc_state;

// ----------- timer -----------

uint64_t npc_get_time();

// ----------- log -----------

#define ANSI_FG_BLACK   "\33[1;30m"
#define ANSI_FG_RED     "\33[1;31m"
#define ANSI_FG_GREEN   "\33[1;32m"
#define ANSI_FG_YELLOW  "\33[1;33m"
#define ANSI_FG_BLUE    "\33[1;34m"
#define ANSI_FG_MAGENTA "\33[1;35m"
#define ANSI_FG_CYAN    "\33[1;36m"
#define ANSI_FG_WHITE   "\33[1;37m"
#define ANSI_BG_BLACK   "\33[1;40m"
#define ANSI_BG_RED     "\33[1;41m"
#define ANSI_BG_GREEN   "\33[1;42m"
#define ANSI_BG_YELLOW  "\33[1;43m"
#define ANSI_BG_BLUE    "\33[1;44m"
#define ANSI_BG_MAGENTA "\33[1;45m"
#define ANSI_BG_CYAN    "\33[1;46m"
#define ANSI_BG_WHITE   "\33[1;47m"
#define ANSI_NONE       "\33[0m"

#define npc_ANSI_FMT(str, fmt) fmt str ANSI_NONE    //先设置背景 后恢复默认值

#define npc_log_write(...) npc_IFDEF(CONFIG_TARGET_NATIVE_ELF, \
  do { \
    extern FILE* log_fp; \
    extern bool npc_log_enable(); \
    if (npc_log_enable() && log_fp != NULL && log_fp != stdout) { \
      fprintf(log_fp, __VA_ARGS__); \
      fflush(log_fp); \
    } \
  } while (0) \
)

#define npc__Log(...) \
  do { \
    printf(__VA_ARGS__); \
    npc_log_write(__VA_ARGS__); \
  } while (0)

// compatibility aliases
#define ANSI_FMT(str, fmt) npc_ANSI_FMT(str, fmt)
#define log_write(...) npc_log_write(__VA_ARGS__)
#define _Log(...) npc__Log(__VA_ARGS__)

#endif

