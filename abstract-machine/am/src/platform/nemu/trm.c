#include <am.h>
#include <nemu.h>

extern char _heap_start;
int main(const char *args);

Area heap = RANGE(&_heap_start, PMEM_END);
static const char mainargs[MAINARGS_MAX_LEN] = TOSTRING(MAINARGS_PLACEHOLDER); // defined in CFLAGS

void putch(char ch) {
  /* Platform (NEMU) putch: write one byte to MMIO serial port.
   * This is where the AM output maps to the simulated hardware.
   * Upstream call chain example:
   *   nanos-lite/src/main.c: printf("%s", logo)
   *     -> klib printf -> vsprintf -> putch(ch)
   *     -> AM putch (this function) -> outb(SERIAL_PORT, ch)
   *     -> NEMU sees MMIO write at SERIAL_PORT and prints to monitor terminal
   */
  outb(SERIAL_PORT, ch);
}

void halt(int code) 
{
  nemu_trap(code);

  // should not reach here
  while (1);
}

void _trm_init() {
  int ret = main(mainargs);
  halt(ret);
}
