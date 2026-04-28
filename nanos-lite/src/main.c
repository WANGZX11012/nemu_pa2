#include <common.h>

void init_mm(void);
void init_device(void);
void init_ramdisk(void);
void init_irq(void);
void init_fs(void);
void init_proc(void);

int main() {
  extern const char logo[];
  /* Call chain:
   *  nanos-lite/src/main.c: printf("%s", logo)
   *    -> abstract-machine/klib/src/stdio.c: printf -> vsprintf
   *       -> outputs formatted bytes one-by-one via putch(ch)
   *    -> platform AM implementation (e.g. abstract-machine/am/src/platform/nemu/trm.c): putch(ch)
   *       -> on NEMU this maps to outb(SERIAL_PORT, ch) which writes to MMIO serial
   */
  printf("%s", logo);
  Log("'Hello World!' from Nanos-lite");
  Log("Build time: %s, %s", __TIME__, __DATE__);

  init_mm();

  init_device();

  init_ramdisk();

#ifdef HAS_CTE
  init_irq();//初始化上下文/异常处理
#endif

  init_fs();

  init_proc();

  Log("Finish initialization");

#ifdef HAS_CTE
  yield();
#endif

  panic("Should not reach here");
}
