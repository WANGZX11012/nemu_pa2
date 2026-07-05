/*
 * npc-nemu 本地版 difftest 定义。
 * 这里只保留当前需要的最小集合：GPR + pc。
 */

#ifndef __NPC_DIFFTEST_DEF_H__
#define __NPC_DIFFTEST_DEF_H__

#include <stdint.h>
#include <npc_macro.h>

#define __EXPORT __attribute__((visibility("default")))

enum { DIFFTEST_TO_DUT, DIFFTEST_TO_REF };

#define NPC_DIFFTEST_GPR_NR 32

#define DIFFTEST_REG_SIZE (sizeof(uint32_t) * (NPC_DIFFTEST_GPR_NR + 1))

#endif