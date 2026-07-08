#ifndef NPC_DEVICE_TIMER_H
#define NPC_DEVICE_TIMER_H

#include <cstdint>

static constexpr uint32_t RTC_ADDR = 0xa0000048u;

bool     timer_in_range(uint32_t addr);
uint32_t timer_read   (uint32_t addr, int len);

#endif
