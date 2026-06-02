#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;

Context* __am_irq_handle(Context *c) //trap.s弄的东西
//事件和上下文一并通过回调函数hander传给操作系统
// 它根据 c->mcause 判断这是啥事件（yield/syscall/timer...），
// 构造 Event，然后调用你在 cte_init 注册的 handler(Event, Context*)。
{
  if (user_handler) 
  {
    printf(" [irq] mcause=0x%x a7=0x%x\n", (uint32_t)c->mcause, (uint32_t)c->GPR1);
    Event ev = {0};
    switch (c->mcause) 
    {
      case 8:
      case 11:
        if ((uintptr_t)c->GPR1 == (uintptr_t)-1 )
        {
          ev.event = EVENT_YIELD;
        }
        else
        {
          ev.event = EVENT_SYSCALL;
        }
        c->mepc += 4;//mret的 pc + 4 
      break;
      case 0x80000007:
        ev.event = EVENT_IRQ_TIMER;
      break;
      default: ev.event = EVENT_ERROR; break;
    }

    c = user_handler(ev, c);
    assert(c != NULL);
  }

  return c;
}

extern void __am_asm_trap(void);
// 保存 GPR（通用寄存器）
// 保存 CSR（如 mcause/mepc/mstatus）
// 在栈上打包成一个 Context
// 调用 C 函数 __am_irq_handle(Context *c)






//CTE初始化 设置异常入口 ，注册回调
bool cte_init(Context*(*handler)(Event, Context*)) 
{
  // initialize exception entry
  asm volatile("csrw mtvec, %0" : : "r"(__am_asm_trap));
  //内联汇编把 __am_asm_trap 的地址写进 CSR mtvec（处理器异常入口
  //告诉 CPU，以后出异常就跳到 __am_asm_trap

  // register event handler

  user_handler = handler;
  //把用户传入的函数指针 handler 保存到文件静态变量 user_handler，以后发生异常时会调用它。

  return true;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
  return NULL;
}

void yield() {
#ifdef __riscv_e
  asm volatile("li a5, -1; ecall");
#else
  asm volatile("li a7, -1; ecall");//按响服务铃”。
// 而 a7 = -1 就像举牌写着：“这次铃声的类别是 yield”。
//ecall会使得程序流程转到之前注册的异常处理入口函数中去执行，即__am_asm_trap

// __am_asm_trap简单来说是提供了统一的异常入口地址，主要作用是将csr和gpr的内容作为参数调用__am_irq_handle，
// 最终将事件和上下文一并通过回调函数hander传给操作系统

#endif
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}
