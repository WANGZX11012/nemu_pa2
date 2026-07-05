// NPC 表达式求值器 —— 迁移自 NEMU 的 expr.c
// 主要改动：
//   1. vaddr_read → pmem_read_u32（通过 DPI-C 读 NPC 的 pmem）
//   2. isa_reg_str2val → npc_isa_reg_str2val
//   3. ANSI_FMT → npc_ANSI_FMT, ARRLEN → npc_ARRLEN
//   4. panic/Assert → printf + assert
//   5. 其他宏名改为 npc_ 前缀以适配 NPC 环境

#include <npc_common.h>
#include <npc_utils.h>
#include <npc_reg.h>
#include <regex.h>         // POSIX 正则表达式库
#include <assert.h>
#include "expr.h"

// DPI-C 内存读函数（来自 dpic.cpp，C 链接）
extern uint32_t pmem_read_u32(uint32_t raddr);

// ───── 标记类型枚举 ─────
enum {
  TK_NOTYPE = 256,   // 空格（忽略）
  TK_EQ,             // ==
  TK_NEQ,            // !=
  TK_DEC,            // 十进制数
  TK_NEG,            // 一元负号
  TK_UPLUS,          // 一元正号
  TK_DREF,           // 解引用 *
  TK_REG,            // 寄存器 $name 或 $$index
  TK_HEX,            // 十六进制 0x...
  TK_L_OR,           // ||
  TK_L_AND,          // &&
  TK_LE,             // <=
};

// ───── 规则表：正则 → 标记类型 ─────
static struct rule {
  const char *regex;
  int token_type;
} rules[] = {
  {" +",                TK_NOTYPE},   // 空格（忽略）
  {"\\+",               '+'},          // +
  {"==",                TK_EQ},        // ==
  {"\\-",               '-'},          // -
  {"\\*",               '*'},          // *
  {"\\/",               '/'},          // /
  {"\\(",               '('},          // (
  {"\\)",               ')'},          // )
  {"!=",                TK_NEQ},       // !=
  {"<=",                TK_LE},        // <=
  {"&&",                TK_L_AND},     // &&
  {"\\|\\|",            TK_L_OR},      // ||
  {"0[xX][0-9a-fA-F]+", TK_HEX},      // 0x...（优先级高于十进制）
  {"[0-9]+",            TK_DEC},       // 十进制
  {"\\$[a-z0-9]+|\\$\\$[0-9]+", TK_REG}, // $reg 或 $$index
};

#define NR_REGEX npc_ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

// ───── 初始化正则表达式 ─────
void init_regex(void) {
  for (int i = 0; i < NR_REGEX; i++) {
    char error_msg[128];
    int ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, sizeof(error_msg));
      printf("regex compilation failed: %s\n%s\n", error_msg, rules[i].regex);
      assert(0);
    }
  }
}

// ───── 标记结构体 ─────
typedef struct token {
  int type;
  char str[1024];
} Token;

static Token tokens[1024] __attribute__((used)) = {};
static int nr_token __attribute__((used)) = 0;

// ───── 词法分析：将字符串拆成 token 序列 ─────
static bool make_token(char *e) {
  int position = 0;
  regmatch_t pmatch;
  nr_token = 0;

  while (e[position] != '\0') {
    int i;
    for (i = 0; i < NR_REGEX; i++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        if (substr_len == 0) continue;  // 跳过空匹配

        position += substr_len;

        switch (rules[i].token_type) {
          case TK_NOTYPE: break;

          case '+': tokens[nr_token++].type = '+'; break;
          case '-': tokens[nr_token++].type = '-'; break;
          case '*': tokens[nr_token++].type = '*'; break;
          case '/': tokens[nr_token++].type = '/'; break;
          case '(': tokens[nr_token++].type = '('; break;
          case ')': tokens[nr_token++].type = ')'; break;
          case TK_EQ:   tokens[nr_token++].type = TK_EQ;   break;
          case TK_NEQ:  tokens[nr_token++].type = TK_NEQ;  break;
          case TK_L_AND: tokens[nr_token++].type = TK_L_AND; break;
          case TK_L_OR:  tokens[nr_token++].type = TK_L_OR;  break;
          case TK_LE:    tokens[nr_token++].type = TK_LE;    break;

          case TK_HEX:
            tokens[nr_token].type = TK_HEX;
            assert(substr_len < 1024);
            strncpy(tokens[nr_token].str, substr_start, substr_len);
            tokens[nr_token++].str[substr_len] = '\0';
            break;

          case TK_REG:
            tokens[nr_token].type = TK_REG;
            assert(substr_len < 1024);
            // 跳过首字符 $，复制剩余部分
            strncpy(tokens[nr_token].str, substr_start + 1, substr_len - 1);
            tokens[nr_token++].str[substr_len - 1] = '\0';
            break;

          case TK_DEC:
            tokens[nr_token].type = TK_DEC;
            assert(substr_len < 1024);
            strncpy(tokens[nr_token].str, substr_start, substr_len);
            tokens[nr_token++].str[substr_len] = '\0';
            break;

          default: break;
        }
        break;  // 匹配成功，跳出 for 循环继续下一个位置
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*s^\n", position, e, position, "");
      return false;
    }
  }
  return true;
}

// ───── 检查括号是否匹配（外层） ─────
static bool check_bracket(int p, int q) {
  int diff = 0;
  for (int i = p; i < q; i++) {
    diff = (tokens[i].type == '(') ? diff + 1 :
           (tokens[i].type == ')') ? diff - 1 : diff;
    if (diff == 0) return false;   // 中途归零 → 不是一对
  }
  return (tokens[q].type == ')' && diff == 1);
}

// ───── 检查运算符位置是否被括号保护 ─────
static bool surrounded_by_bracket(int x, int p, int q) {
  int l_bracket = 0, r_bracket = 0;
  for (int i = x; i >= p; i--) {
    l_bracket += (tokens[i].type == '(') ? 1 : (tokens[i].type == ')') ? -1 : 0;
  }
  for (int i = x; i <= q; i++) {
    r_bracket += (tokens[i].type == '(') ? 1 : (tokens[i].type == ')') ? -1 : 0;
  }
  return (l_bracket == 0 && r_bracket == 0);
}

// ───── 运算符优先级 ─────
static int get_op_priority(int op) {
  switch (op) {
    case TK_NEG: case TK_DREF: case TK_UPLUS: return 2;
    case '*': case '/':                        return 3;
    case '+': case '-':                        return 4;
    case TK_LE:                                return 6;
    case TK_EQ: case TK_NEQ:                   return 7;
    case TK_L_AND: case TK_L_OR:               return 8;
    default:                                   return 0;
  }
}

// ───── 查找主运算符 ─────
static int find_main_op(int p, int q) {
  int op = -1, last_priority = 0;
  for (int i = p; i <= q; i++) {
    int prio = get_op_priority(tokens[i].type);
    if (!surrounded_by_bracket(i, p, q) || prio == 0) continue;
    if (last_priority <= prio) {   // 同优先级取最右（左结合）
      last_priority = prio;
      op = i;
    }
  }
  if (last_priority == 0) {
    printf(npc_ANSI_FMT("Can't find main op", ANSI_BG_RED) "\n");
    return -1;
  }
  return op;
}

// ───── 递归求值 ─────
static uint32_t eval(int p, int q, bool *success) {
  if (!*success) return 0;
  if (p > q) {
    printf(npc_ANSI_FMT("Bad expr", ANSI_BG_RED) "\n");
    *success = false;
    return 0;
  }

  // 单 token
  if (p == q) {
    if (tokens[p].type == TK_DEC)
      return strtoul(tokens[p].str, NULL, 10);
    else if (tokens[p].type == TK_HEX)
      return strtoul(tokens[p].str, NULL, 16);
    else if (tokens[p].type == TK_REG)
      return npc_isa_reg_str2val(tokens[p].str, success);
    else {
      printf(npc_ANSI_FMT("Bad expr", ANSI_BG_RED) "\n");
      *success = false;
      return 0;
    }
  }

  // 去掉外层括号
  if (check_bracket(p, q))
    return eval(p + 1, q - 1, success);

  // 找主运算符
  int op = find_main_op(p, q);
  if (op == -1) {
    *success = false;
    return 0;
  }

  uint32_t val1, val2;

  // 一元运算符
  if (tokens[op].type == TK_NEG || tokens[op].type == TK_UPLUS) {
    val2 = eval(op + 1, q, success);
    return (tokens[op].type == TK_NEG) ? -val2 : val2;
  }
  if (tokens[op].type == TK_DREF) {
    uint32_t addr = eval(op + 1, q, success);
    if (!*success) return 0;
    return pmem_read_u32(addr);  // 从 NPC pmem 读 32 位
  }

  // 短路求值 &&
  if (tokens[op].type == TK_L_AND) {
    val1 = eval(p, op - 1, success);
    if (!*success) return 0;
    if (val1 == 0) return 0;
    val2 = eval(op + 1, q, success);
    return (val2 != 0);
  }

  // 短路求值 ||
  if (tokens[op].type == TK_L_OR) {
    val1 = eval(p, op - 1, success);
    if (!*success) return 0;
    if (val1 != 0) return 1;
    val2 = eval(op + 1, q, success);
    return (val2 != 0);
  }

  // 二元运算符
  val1 = eval(p, op - 1, success);
  val2 = eval(op + 1, q, success);

  switch (tokens[op].type) {
    case TK_LE:  return ((int32_t)val1 <= (int32_t)val2);
    case '+':    return val1 + val2;
    case '-':    return val1 - val2;
    case '*':    return val1 * val2;
    case '/':
      if (val2 == 0) {
        printf(npc_ANSI_FMT("Can't divide 0", ANSI_BG_RED) "\n");
        *success = false;
        return 0;
      }
      return (int32_t)val1 / (int32_t)val2;
    case TK_EQ:  return (val1 == val2) ? 1 : 0;
    case TK_NEQ: return (val1 != val2) ? 1 : 0;
    default:
      printf(npc_ANSI_FMT("sth wrong", ANSI_BG_RED) "\n");
      *success = false;
      return 0;
  }
}

// ───── 修正一元运算符（把上下文中的 + - * 分类为一元/二元） ─────
#define PREV_ALLOW_UNARY(i) \
  ((i) == 0 || \
   tokens[(i)-1].type == '+' || tokens[(i)-1].type == '-' || \
   tokens[(i)-1].type == '*' || tokens[(i)-1].type == '/' || \
   tokens[(i)-1].type == '(' || \
   tokens[(i)-1].type == TK_NEG || tokens[(i)-1].type == TK_UPLUS || \
   tokens[(i)-1].type == TK_DREF || \
   tokens[(i)-1].type == TK_EQ || tokens[(i)-1].type == TK_NEQ || \
   tokens[(i)-1].type == TK_LE || tokens[(i)-1].type == TK_L_AND || \
   tokens[(i)-1].type == TK_L_OR)

static void fix_ops(Token *tokens, int nr_token) {
  for (int i = 0; i < nr_token; i++) {
    if (PREV_ALLOW_UNARY(i)) {
      if (tokens[i].type == '+') tokens[i].type = TK_UPLUS;
      else if (tokens[i].type == '-') tokens[i].type = TK_NEG;
      else if (tokens[i].type == '*') tokens[i].type = TK_DREF;
    }
  }
}
#undef PREV_ALLOW_UNARY

// ───── 表达式求值主入口 ─────
word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  fix_ops(tokens, nr_token);

  bool eval_success = true;
  uint32_t result = eval(0, nr_token - 1, &eval_success);

  if (eval_success) {
    *success = true;
    return result;
  } else {
    *success = false;
    return 0;
  }
}
