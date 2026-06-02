#include <am.h>
#include <nemu.h> //包括了include riscv32.h

#define SYNC_ADDR (VGACTL_ADDR + 4)


static uint32_t val; //通过io地址读寄存器
static int w; //长宽
static int h;


void __am_gpu_init() 
{
  val = inl(VGACTL_ADDR);
  int i = 0;
  w = val >> 16;
  h = val & 0xffff;

  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR; //FRAME BUFFER
  for ( i = 0; i < w * h; i++)
  {
    fb[i] = i;
  }
  outl(SYNC_ADDR,1);   //写1 刷新请求
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) 
{
  *cfg = (AM_GPU_CONFIG_T) 
  {
    .present = true, .has_accel = false,
    .width = w, .height = h,
    .vmemsz = w * h * 4
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) 
{
  
 if(w == 0 || h == 0) //空绘制 长宽为0
 {
  if(ctl->sync)
    outl(SYNC_ADDR, 1);
  return;
 }

 if(ctl->x >= w || ctl->y >= h)//越界
  {
    if(ctl->sync)
      outl(SYNC_ADDR, 1);
    return;
  }

  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  uint32_t *src = (uint32_t*)ctl->pixels;

  for(int j = 0; j < ctl->h; j ++)
  {
    for(int i = 0; i < ctl->w; i++)
    {
      int dx = ctl->x + i;
      int dy = ctl->y + j;
      fb[dy * w + dx] = src[j * ctl->w + i];
    }
  }
  
  if (ctl->sync) outl(SYNC_ADDR, 1);

}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
