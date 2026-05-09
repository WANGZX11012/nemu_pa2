#include <common.h>

static Context* do_event(Event e, Context* c) {
  switch (e.event) 
  {
    case EVENT_YIELD:
      printf("y\n");
    default: panic("Unhandled event ID = %d", e.event);
  }

  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  cte_init(do_event);//操作系统向硬件注册事件发生（如中断）的回调函数
  // /参数为事件和相关的程序上下文
}
