#include "npc_device.h"
#include "generated/autoconf.h"

#ifdef CONFIG_HAS_SERIAL
#include "device/serial.h"
#endif
#ifdef CONFIG_HAS_TIMER
#include "device/timer.h"
#endif
#ifdef CONFIG_HAS_VGA
#include "device/vga.h"
#endif
#ifdef CONFIG_HAS_KEYBOARD
#include "device/keyboard.h"
#endif

#include <cstdio>

// ===== Skip 机制 =====
static bool is_skip = false;
void npc_skip_set(void) { is_skip = true; }
bool npc_skip_consume(void) { bool val = is_skip; is_skip = false; return val; }

// ===== 设备范围判断 =====
#ifdef CONFIG_DEVICE
bool npc_device_in_range(uint32_t addr) 
{
#ifdef CONFIG_HAS_SERIAL
  if (serial_in_range(addr)) return true;
#endif
#ifdef CONFIG_HAS_TIMER
  if (timer_in_range(addr))  return true;
#endif
#ifdef CONFIG_HAS_VGA
  if (vga_in_range(addr))      return true;
#endif
#ifdef CONFIG_HAS_KEYBOARD
  if (keyboard_in_range(addr)) return true;
#endif
  return false;
}

// ===== 设备读 =====
uint32_t npc_device_read(uint32_t addr, int len) 
{
#ifdef CONFIG_HAS_SERIAL
  if (serial_in_range(addr))  return serial_read(addr, len);
#endif
#ifdef CONFIG_HAS_TIMER
  if (timer_in_range(addr))   return timer_read(addr, len);
#endif
#ifdef CONFIG_HAS_VGA
  if (vga_in_range(addr))      return vga_read(addr, len);
#endif
#ifdef CONFIG_HAS_KEYBOARD
  if (keyboard_in_range(addr)) return keyboard_read(addr);
#endif
  return 0;
}

// ===== 设备写 =====
void npc_device_write(uint32_t addr, int len, uint32_t data) 
{
#ifdef CONFIG_HAS_SERIAL
  if (serial_in_range(addr))  { serial_write(addr, len, data); return; }
#endif
#ifdef CONFIG_HAS_TIMER
  if (timer_in_range(addr))   { /* timer is read-only */ return; }
#endif
#ifdef CONFIG_HAS_VGA
  if (vga_in_range(addr))      { vga_write(addr, len, data); return; }
#endif
#ifdef CONFIG_HAS_KEYBOARD
  if (keyboard_in_range(addr)) { /* keyboard is read-only */ return; }
#endif
  std::printf("npc_device_write: bad addr = 0x%08x, len = %d, data = 0x%08x\n",
              addr, len, data);
}

#else // !CONFIG_DEVICE
bool     npc_device_in_range(uint32_t) { return false; }
uint32_t npc_device_read(uint32_t, int) { return 0; }
void     npc_device_write(uint32_t, int, uint32_t) {}
#endif // CONFIG_DEVICE

// npc_vga_quit
#ifdef CONFIG_HAS_VGA
bool npc_vga_quit(void) { return vga_quit(); }
#else
bool npc_vga_quit(void) { return false; }
#endif
