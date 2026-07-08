#include "timer.h"
#include <ctime>

static uint64_t boot_time_us = 0;

static uint64_t get_time_us() {
  timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (uint64_t)ts.tv_sec * 1000000ull + ts.tv_nsec / 1000;
}

static uint64_t uptime_us() {
  if (boot_time_us == 0) boot_time_us = get_time_us();
  return get_time_us() - boot_time_us;
}

bool timer_in_range(uint32_t addr) {
  return addr == RTC_ADDR || addr == RTC_ADDR + 4;
}

uint32_t timer_read(uint32_t addr, int len) {
  uint64_t us = uptime_us();
  return (addr == RTC_ADDR) ? (uint32_t)us : (uint32_t)(us >> 32);
}
