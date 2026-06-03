#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;

Context* __am_irq_handle(Context *c) 
{
  if (user_handler) 
  {
    Event ev = {0};
    switch (c->mcause) 
    {
      case 11:
        if ((uintptr_t)c->GPR1 == (uintptr_t)-1) 
        {
          ev.event = EVENT_YIELD;
        } else {
          ev.event = EVENT_SYSCALL;
        }
        c->mepc += 4;   // ecall 保存的是自身 PC，mret 需 +4 返回下一条
        break;
      default: ev.event = EVENT_ERROR; break;
    }

    c = user_handler(ev, c);
    assert(c != NULL);
  }

  return c;
}

extern void __am_asm_trap(void);

bool cte_init(Context*(*handler)(Event, Context*)) {
  // initialize exception entry
  asm volatile("csrw mtvec, %0" : : "r"(__am_asm_trap));

  // register event handler
  user_handler = handler;

  return true;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) 
{
  Context *c = (Context*)kstack.end - 1; //在这块内核栈的最高地址往下放一个 Context。
  c->mepc = (uintptr_t)entry ; //将来 mret 时，PC 会变成 entry，线程就从 f() 开始跑
  c->pdir = NULL;
  c->mcause = 0x1800;
  c->gpr[10] = (uintptr_t)arg;  // a0 第一个参数放在 a0，也就是 x10。所以 f(void *arg) 以后能拿到参数。
  return c;
}

void yield() {
#ifdef __riscv_e
  asm volatile("li a5, -1; ecall");
#else
  asm volatile("li a7, -1; ecall");
#endif
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}
