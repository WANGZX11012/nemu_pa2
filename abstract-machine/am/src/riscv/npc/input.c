#include <am.h>

#define KEYBRD_ADDR 0xa0000060u
#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  uint32_t v = *(volatile uint32_t *)KEYBRD_ADDR;
  kbd->keydown = (v >> 15) & 1;
  kbd->keycode = v & 0x7fff;
}
