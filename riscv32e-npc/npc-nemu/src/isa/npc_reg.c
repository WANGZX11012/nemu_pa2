#include <npc_reg.h>
#include <cpu/npc_cpu.h>
#include <stdio.h>
#include <string.h>

// ───── 寄存器名称 ↔ 编号映射表 ─────
// ABI 名字、通用 x 名字、以及特殊名 pc / zero
static const struct {
  const char *name;
  int idx;          // -1 表示特殊（pc），-2 表示 x0 读固定返回 0
} reg_alias[] = {
  // x 编号名
  {"x0", 0},  {"x1", 1},  {"x2", 2},  {"x3", 3},  {"x4", 4},
  {"x5", 5},  {"x6", 6},  {"x7", 7},  {"x8", 8},  {"x9", 9},
  {"x10",10}, {"x11",11}, {"x12",12}, {"x13",13}, {"x14",14},
  {"x15",15}, {"x16",16}, {"x17",17}, {"x18",18}, {"x19",19},
  {"x20",20}, {"x21",21}, {"x22",22}, {"x23",23}, {"x24",24},
  {"x25",25}, {"x26",26}, {"x27",27}, {"x28",28}, {"x29",29},
  {"x30",30}, {"x31",31},
  // ABI 名
  {"zero", 0},  {"ra", 1},  {"sp", 2},  {"gp", 3},  {"tp", 4},
  {"t0", 5},   {"t1", 6},   {"t2", 7},
  {"s0", 8},   {"fp", 8},   {"s1", 9},
  {"a0", 10},  {"a1", 11},  {"a2", 12},  {"a3", 13},
  {"a4", 14},  {"a5", 15},  {"a6", 16},  {"a7", 17},
  {"s2", 18},  {"s3", 19},  {"s4", 20},  {"s5", 21},
  {"s6", 22},  {"s7", 23},  {"s8", 24},  {"s9", 25},
  {"s10",26},  {"s11",27},
  {"t3", 28},  {"t4", 29},  {"t5", 30},  {"t6", 31},
  // 特殊
  {"pc", -1},
};

void npc_isa_reg_display(void)
{
  printf("pc  = 0x%08x\n", npc_cpu.pc);
  for (int i = 0; i < NPC_GPR_NR; i++)
  {
    printf("x%-2d = 0x%08x\n", i, npc_cpu.gpr[i]);
  }
}

// ───── 寄存器字符串 → 值 ─────
// 支持的输入格式（token 中已去掉前导 $）：
//   x0..x31          → 通用寄存器编号
//   a0..a7, ra, sp,  → ABI 名字 → 对应编号
//   pc               → 当前 PC
//   $0..$31          → 双 $ 编号（去掉第一个 $ 后剩 $0 形式）
word_t npc_isa_reg_str2val(const char *s, bool *success) 
{
  // 处理双 $ 形式：$0..$31（去掉一个 $ 后还剩一个 $ 开头）
  if (s[0] == '$') 
  {
    // $$N 形式 → 直接取编号
    int idx = atoi(s + 1); //传进去$0 也就是x0
    if (idx >= 0 && idx < NPC_GPR_NR) 
    {
      *success = true;
      return npc_cpu.gpr[idx];
    }
    *success = false;
    return 0;
  }

  // 查找 ABI 名称表
  int n = sizeof(reg_alias) / sizeof(reg_alias[0]);
  for (int i = 0; i < n; i++) 
  {
    if (strcmp(s, reg_alias[i].name) == 0) 
    {
      int idx = reg_alias[i].idx;
      if (idx == -1) 
      {          // pc
        *success = true;
        return npc_cpu.pc;
      }
      if (idx >= 0 && idx < NPC_GPR_NR) 
      {
        *success = true;
        return npc_cpu.gpr[idx];
      }
      *success = false; //没匹配到就false
      return 0;
    }
  }

  printf("Unknown register: $%s\n", s);
  *success = false;
  return 0;
}
