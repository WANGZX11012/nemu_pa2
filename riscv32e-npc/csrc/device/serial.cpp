#include "serial.h"
#include <cstdio>

bool serial_in_range(uint32_t addr) 
{
  return addr == SERIAL_ADDR;
}

uint32_t serial_read(uint32_t addr, int len) 
{
  return 0; // serial is write-only
}

void serial_write(uint32_t addr, int len, uint32_t data) 
{
  if (addr != SERIAL_ADDR) return;
  if (len != 1) std::printf("serial write len = %d, data = 0x%08x\n", len, data);
  std::putchar(data & 0xff);
  std::fflush(stdout);
}
