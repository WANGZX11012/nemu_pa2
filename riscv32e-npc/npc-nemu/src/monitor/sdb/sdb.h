
#ifndef __NPC_SDB_H__
#define __NPC_SDB_H__

#include <npc_common.h>

// 表达式求值（来自 expr.c）
word_t expr(char *e, bool *success);

void npc_sdb_set_batch_mode(void);
void npc_sdb_mainloop(void);
void npc_init_sdb(void);

#endif
