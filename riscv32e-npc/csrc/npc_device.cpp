#include "npc_device.h"
#include "generated/autoconf.h"

#include <cstdint>
#include <cstdio>
#include <ctime>

// ===== Skip 机制（用于 difftest 跳过 MMIO 指令比对）=====
// 即使 CONFIG_DEVICE 关闭，difftest 仍可能调用 npc_skip_consume()
static bool is_skip = false;

void npc_skip_set(void) //跳过对比标志设置
{
  is_skip = true;
}

bool npc_skip_consume(void) //跳过对比标志消费
{
  bool val = is_skip; //读旧值
  is_skip = false;    //清除标志
  return val;         //返回旧值 也就如果读到true 返回true
}

// ===== Device Implementation (only compiled when CONFIG_DEVICE) =====
#ifdef CONFIG_DEVICE

static constexpr uint32_t SERIAL_ADDR = 0xa00003f8u;  //串口地址
static constexpr uint32_t RTC_ADDR = 0xa0000048u;  //RTC地址

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

#else // !CONFIG_DEVICE

// Stubs: no device support, all addresses map to normal memory
bool npc_device_in_range(uint32_t) { return false; }
uint32_t npc_device_read(uint32_t, int) { return 0; }
void npc_device_write(uint32_t, int, uint32_t) {}

#endif // CONFIG_DEVICE


