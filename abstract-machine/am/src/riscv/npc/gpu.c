#include <am.h>
#include <klib-macros.h>

#define VGACTL_ADDR 0xa0000100u
#define FB_ADDR     0xa1000000u

static uint32_t screen_w = 0;
static uint32_t screen_h = 0;

void __am_gpu_init() {
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
    if (ctl->sync) *(volatile uint32_t *)(VGACTL_ADDR + 4) = 1;
    return;
  }
  int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
  if (x >= (int)screen_w || y >= (int)screen_h) {
    if (ctl->sync) *(volatile uint32_t *)(VGACTL_ADDR + 4) = 1;
    return;
  }
  if (x + w > (int)screen_w) w = (int)screen_w - x;
  if (y + h > (int)screen_h) h = (int)screen_h - y;
  uint32_t *pixels = (uint32_t *)ctl->pixels;
  for (int j = 0; j < h; j++)
    for (int i = 0; i < w; i++)
      *(volatile uint32_t *)(FB_ADDR + ((y + j) * screen_w + (x + i)) * 4) = pixels[j * ctl->w + i];
  if (ctl->sync) *(volatile uint32_t *)(VGACTL_ADDR + 4) = 1;
}

void __am_gpu_status(AM_GPU_STATUS_T *status) { status->ready = true; }
