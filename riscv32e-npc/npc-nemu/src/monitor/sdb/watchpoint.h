#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include <npc_common.h>

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  char *expr;
  word_t old_val;
} WP;

// 函数声明
void init_wp_pool(void);
WP  *new_wp(const char *expr, word_t old_val);
void free_wp(int no);
void print_watchpoints(void);
int  update_wp(void);      // 遍历所有监视点，返回变更次数

#endif
