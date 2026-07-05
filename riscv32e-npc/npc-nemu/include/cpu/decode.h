#ifndef __CPU_DECODE_H__
#define __CPU_DECODE_H__

#include <npc_common.h>

typedef struct Decode {
  vaddr_t pc;
  IFDEF(CONFIG_ITRACE, char logbuf[128]);
} Decode;

#endif
