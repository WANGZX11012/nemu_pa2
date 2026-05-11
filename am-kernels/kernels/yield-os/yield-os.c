#include <am.h>
#include <klib.h>
#include <klib-macros.h>

#define STACK_SIZE (4096 * 8)
typedef union //union 的所有成员 从同一个起始地址开始放，占用空间取最大成员的大小。 
{
  uint8_t stack[STACK_SIZE];
  struct { Context *cp; };
} PCB;
static PCB pcb[2], pcb_boot, *current = &pcb_boot;//定义了三个PCB pcb[0],[1]是两个线程
//pcb boot是启动线程的pcb main在跑的执行流 current指向boot 一开始

// 低地址
// +------------------+  <--- &pcb[i] 也是 pcb[i].stack 的起点
// | Context *cp      |  (固定在栈底)
// +------------------+
// |                  |
// |    栈空间 stack  |
// |                  |
// +------------------+  <--- 栈顶（高地址），kcontext 常从这里往下放 Context
// 高地址



static void f(void *arg) 
{
  int i = 0;
  while (i < 100) 
  {
    putch("?AB"[(uintptr_t)arg > 2 ? 0 : (uintptr_t)arg]);
    for (int volatile i = 0; i < 100; i++) ; //“故意浪费一点时间，然后主动让出 CPU”，用来让你更容易观察到 A/B 交替输出、并触发上下文切换。
    yield();
    i++;
  }
  printf("\n");
  halt(0); 
}

static Context *schedule(Event ev, Context *prev) 
{
  current->cp = prev;
  current = (current == &pcb[0] ? &pcb[1] : &pcb[0]);
  return current->cp;
}

int main() 
{
  cte_init(schedule);  
  //人工构造上下文
  pcb[0].cp = kcontext((Area) { pcb[0].stack, &pcb[0] + 1 }, f, (void *)1L); 
  //指向线程0的初始地址context &pcb[0] + 1高地址栈顶
  //等这个上下文第一次被恢复（切换到这个线程）时，就从函数 f 开始执行。
  //(void *)1L用指针类型携带一个小整数。 在f() 里又把它转回整数：(uintptr_t)arg

  pcb[1].cp = kcontext((Area) { pcb[1].stack, &pcb[1] + 1 }, f, (void *)2L);
  
  
  yield(); //第一次yield 当前执行流既不是pcb0 也不是pcb1 而是pcb boot
  panic("Should not reach here!");
}

// cte_init(schedule) 只做一次：把 mtvec 指到 trap.S，并记住回调 schedule 的地址。
// 之后每次 yield() 都会 ecall → 再次进入 trap.S（所以能无限次发生）。
// trap.S 固定会调用 __am_irq_handle(Context *prev)：这是 CTE 的“中间层”，不是你注册的函数。
// cte_init(schedule) 注册的是 schedule 到 user_handler：__am_irq_handle 每次 trap 都会 user_handler(ev, prev)，让 OS 决定“下一次恢复哪个 Context*”。
// schedule 只负责“选人”：保存 prev 到当前 PCB，再返回另一个 PCB 里的 cp（Context* next）。
// f 不是回调，它是“线程入口函数”：kcontext(kstack, f, arg) 会伪造一份初始 Context，把里面的 mepc 设为 f（并把参数寄存器设为 arg）。
// 所以当 schedule 第一次返回某个线程的 Context* 时，trap 返回 mret 恢复到该 Context 的 mepc，CPU 就开始执行 f(arg)。