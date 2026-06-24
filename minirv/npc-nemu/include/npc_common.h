#ifndef __NPC_COMMON_H__
#define __NPC_COMMON_H__

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <npc_macro.h>

typedef uint32_t word_t;
typedef int32_t  sword_t;
typedef uint32_t vaddr_t;
typedef uint32_t paddr_t;
typedef uint16_t ioaddr_t;

#define FMT_WORD  "0x%08" PRIx32
#define FMT_PADDR "0x%08" PRIx32

#ifndef CONFIG_TRACE
#define CONFIG_TRACE 0
#endif

#ifndef CONFIG_TRACE_START
#define CONFIG_TRACE_START 0
#endif

#ifndef CONFIG_TRACE_END
#define CONFIG_TRACE_END 0
#endif

#ifndef CONFIG_TARGET_NATIVE_ELF
#define CONFIG_TARGET_NATIVE_ELF 1
#endif

#ifndef CONFIG_TIMER_GETTIMEOFDAY
#define CONFIG_TIMER_GETTIMEOFDAY 1
#endif

#ifndef __GUEST_ISA__
#define __GUEST_ISA__ riscv32
#endif

#include <npc_debug.h>

#endif
