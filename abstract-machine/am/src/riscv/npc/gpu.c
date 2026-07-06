#include <am.h>
#include <klib-macros.h>

#define VGACTL_ADDR 0xa0000100u
#define SYNC_ADDR   (VGACTL_ADDR + 4)
#define BLIT_BASE   (VGACTL_ADDR + 8)
// BLIT 寄存器: +0=src, +4=x, +8=y, +12=w, +16=h, +20=exec

static uint32_t screen_w = 0;
static uint32_t screen_h = 0;

void __am_gpu_init() 
{
  uint32_t cfg = *(volatile uint32_t *)VGACTL_ADDR;
  screen_w = cfg >> 16;
  screen_h = cfg & 0xffff;
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = (int)screen_w, .height = (int)screen_h,
    .vmemsz = (int)(screen_w * screen_h * sizeof(uint32_t)),
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  if (screen_w == 0 || screen_h == 0) {
    if (ctl->sync) *(volatile uint32_t *)SYNC_ADDR = 1;
    return;
  }
  if (ctl->x >= (int)screen_w || ctl->y >= (int)screen_h) {
    if (ctl->sync) *(volatile uint32_t *)SYNC_ADDR = 1;
    return;
  }

  // 使用 GPU BLIT: 6 次 MMIO 写完成整个矩形块拷贝
  // C++ 侧（npc_device）收到 EXEC 后批量从 pmem 读像素写入帧缓冲
  *(volatile uint32_t *)(BLIT_BASE + 0)  = (uint32_t)(uintptr_t)ctl->pixels;
  *(volatile uint32_t *)(BLIT_BASE + 4)  = (uint32_t)ctl->x;
  *(volatile uint32_t *)(BLIT_BASE + 8)  = (uint32_t)ctl->y;
  *(volatile uint32_t *)(BLIT_BASE + 12) = (uint32_t)ctl->w;
  *(volatile uint32_t *)(BLIT_BASE + 16) = (uint32_t)ctl->h;
  *(volatile uint32_t *)(BLIT_BASE + 20) = 1;  // EXEC: 触发批量拷贝

  if (ctl->sync)
    *(volatile uint32_t *)SYNC_ADDR = 1;
}

void __am_gpu_status(AM_GPU_STATUS_T *status) { status->ready = true; }
