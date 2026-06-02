#include <am.h>
#include <stdint.h>

#define RTC_ADDR 0xa0000048u

void __am_timer_init() {
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  uint32_t lo = *(volatile uint32_t *)(RTC_ADDR + 0);
  uint32_t hi = *(volatile uint32_t *)(RTC_ADDR + 4);
  uptime->us = ((uint64_t)hi << 32) | lo;
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) 
{
  AM_TIMER_UPTIME_T uptime;
  __am_timer_uptime(&uptime);
  uint64_t sec = uptime.us / 1000000;

  rtc->second = sec % 60;
  rtc->minute = (sec / 60) % 60;
  rtc->hour   = (sec / 3600) % 24;
  rtc->day    = 1;
  rtc->month  = 6;
  rtc->year   = 2026;
}
