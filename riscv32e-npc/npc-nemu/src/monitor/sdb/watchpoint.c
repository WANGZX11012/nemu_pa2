#include "watchpoint.h"
#include "sdb.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head  = NULL;
static WP *free_ = NULL;

void init_wp_pool(void) {
  for (int i = 0; i < NR_WP; i++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1) ? NULL : &wp_pool[i + 1];
  }
  head  = NULL;
  free_ = wp_pool;
}

WP *new_wp(const char *expr, word_t old_val) {
  if (free_ == NULL) {
    printf("Watchpoint pool is full (max %d)\n", NR_WP);
    return NULL;
  }

  WP *p = free_;
  free_ = free_->next;

  if (expr != NULL) {
    p->expr = malloc(strlen(expr) + 1);
    strcpy(p->expr, expr);           // NOLINT: buffer size checked by malloc
    p->old_val = old_val;
  } else {
    p->expr = NULL;
    p->old_val = 0;
  }

  p->next = head;
  head = p;
  return p;
}

void free_wp(int no) {
  if (head == NULL) {
    printf("Watchpoint %d not found\n", no);
    return;
  }

  // 删除头节点
  if (head->NO == no) {
    WP *target = head;
    head = head->next;

    free(target->expr);
    target->expr = NULL;
    target->old_val = 0;

    target->next = free_;
    free_ = target;
    printf("Deleted watchpoint %d\n", no);
    return;
  }

  // 在链表中查找
  for (WP *p = head; p->next != NULL; p = p->next) {
    if (p->next->NO == no) {
      WP *target = p->next;
      p->next = target->next;

      free(target->expr);
      target->expr = NULL;
      target->old_val = 0;

      target->next = free_;
      free_ = target;
      printf("Deleted watchpoint %d\n", no);
      return;
    }
  }

  printf("Watchpoint %d not found\n", no);
}

void print_watchpoints(void) {
  if (head == NULL) {
    printf("No watchpoints.\n");
    return;
  }
  printf("No  Expr                                 OldVal\n");
  for (WP *p = head; p != NULL; p = p->next) {
    printf("%-3d %-36s 0x%08x\n",
           p->NO,
           p->expr ? p->expr : "(null)",
           p->old_val);
  }
}

int update_wp(void) {
  int change_times = 0;

  for (WP *ptr = head; ptr != NULL; ptr = ptr->next) {
    bool success;
    word_t new_val = expr(ptr->expr, &success);

    if (!success) {
      printf("Watchpoint %d eval failure: %s\n", ptr->NO,
             ptr->expr ? ptr->expr : "(null)");
      continue;  // 这里 continue 是安全的，因为用 for 循环自带 ptr = ptr->next
    }

    if (new_val != ptr->old_val) {
      printf("Watchpoint %d: %s\n", ptr->NO, ptr->expr);
      printf("  Old value = 0x%08x\n", ptr->old_val);
      printf("  New value = 0x%08x\n", new_val);
      change_times++;
      ptr->old_val = new_val;
    }
  }

  return change_times;
}
