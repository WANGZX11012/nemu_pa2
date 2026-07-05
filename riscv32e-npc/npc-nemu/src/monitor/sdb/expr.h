#ifndef __NPC_EXPR_H__
#define __NPC_EXPR_H__

#include <npc_common.h>

word_t expr(char *e, bool *success);

// 初始化正则表达式（在 init_sdb 中调用）
void init_regex(void);

#endif
