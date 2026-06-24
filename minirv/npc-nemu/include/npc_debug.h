#ifndef __NPC_DEBUG_H__
#define __NPC_DEBUG_H__

#include <npc_common.h>
#include <stdio.h>
#include <npc_utils.h>

#define npc_Log(format, ...) \
    npc__Log(ANSI_FMT("[%s:%d %s] " format, ANSI_FG_BLUE) "\n", \
        __FILE__, __LINE__, __func__, ## __VA_ARGS__)

#define npc_Assert(cond, format, ...) \
  do { \
    if (!(cond)) { \
      npc_MUXDEF(CONFIG_TARGET_AM, printf(ANSI_FMT(format, ANSI_FG_RED) "\n", ## __VA_ARGS__), \
        (fflush(stdout), fprintf(stderr, ANSI_FMT(format, ANSI_FG_RED) "\n", ##  __VA_ARGS__))); \
      npc_IFNDEF(CONFIG_TARGET_AM, extern FILE* log_fp; fflush(log_fp)); \
      extern void assert_fail_msg(); \
      assert_fail_msg(); \
      assert(cond); \
    } \
  } while (0)

#define npc_panic(format, ...) npc_Assert(0, format, ## __VA_ARGS__)

#define npc_TODO() npc_panic("please implement me")

// compatibility aliases
#define Log(format, ...) npc_Log(format, ## __VA_ARGS__)
#define Assert(cond, format, ...) npc_Assert(cond, format, ## __VA_ARGS__)
#define panic(format, ...) npc_panic(format, ## __VA_ARGS__)
#define TODO() npc_TODO()

#endif
