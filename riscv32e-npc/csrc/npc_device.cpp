#include "npc_device.h"

#include <cstdint>
#include <cstdio>
#include <ctime>

static constexpr uint32_t SERIAL_ADDR = 0xa00003f8u;
static constexpr uint32_t RTC_ADDR = 0xa0000048u;

static uint64_t boot_time_us = 0;


static uint64_t get_time_us() 
{
  timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (uint64_t)ts.tv_sec * 1000000ull + ts.tv_nsec / 1000;
}

static uint64_t uptime_us() 
{
  if (boot_time_us == 0) 
  {
    boot_time_us = get_time_us();
  }
  return get_time_us() - boot_time_us; //当前时间-基准启动时间
}


bool npc_device_in_range(uint32_t addr)
{
    return addr == SERIAL_ADDR ||
           addr == RTC_ADDR ||
           addr == RTC_ADDR + 4;
}

uint32_t npc_device_read(uint32_t addr, int len) 
{
  if (addr == RTC_ADDR || addr == RTC_ADDR + 4) 
  {
    uint64_t us = uptime_us();
    return (addr == RTC_ADDR) ? (uint32_t)us : (uint32_t)(us >> 32);
  }

  return 0;
}


void npc_device_write(uint32_t addr, int len, uint32_t data)
{
    if(addr == SERIAL_ADDR)
    {
        if(len != 1)
        {
            std::printf("serial write len = %d, data = 0x%08x\n", len, data);
        }
 
        std::putchar(data & 0xff);
        std::fflush(stdout);
        return;
    }
    std::printf("npc_device_write: bad addr = 0x%08x, len = %d, data = 0x%08x\n",
              addr, len, data);
}


