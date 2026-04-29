#include <am.h>
#include <nemu.h>

#define AUDIO_FREQ_ADDR      (AUDIO_ADDR + 0x00)
#define AUDIO_CHANNELS_ADDR  (AUDIO_ADDR + 0x04)
#define AUDIO_SAMPLES_ADDR   (AUDIO_ADDR + 0x08)
#define AUDIO_SBUF_SIZE_ADDR (AUDIO_ADDR + 0x0c)
#define AUDIO_INIT_ADDR      (AUDIO_ADDR + 0x10)
#define AUDIO_COUNT_ADDR     (AUDIO_ADDR + 0x14)

void __am_audio_init() 
{
  //先空着
}

void __am_audio_config(AM_AUDIO_CONFIG_T *cfg) //返回给上层 说有音频设备
{
  cfg->present = true; 
  cfg->bufsize = inl(AUDIO_SBUF_SIZE_ADDR);

}

void __am_audio_ctrl(AM_AUDIO_CTRL_T *ctrl) //已经由test填好了值 要写到ADDR里面那些地方
//长层传到下层
{
  outl(AUDIO_FREQ_ADDR, ctrl->freq); 
  outl(AUDIO_CHANNELS_ADDR, ctrl->channels);
  outl(AUDIO_SAMPLES_ADDR, ctrl->samples);//一次取多少采样点

  outl(AUDIO_INIT_ADDR, 1);

}

void __am_audio_status(AM_AUDIO_STATUS_T *stat) //缓冲区还有多少没播放 但是占用了
{
  stat->count = inl(AUDIO_COUNT_ADDR);
}

void __am_audio_play(AM_AUDIO_PLAY_T *ctl) 
{
  int len = ctl->buf.end - ctl->buf.start; //这次写数据的长度
  uint32_t buf_size = inl(AUDIO_SBUF_SIZE_ADDR);
  uint32_t count = inl(AUDIO_COUNT_ADDR);

  if(buf_size > count)
  {
    if(len <= buf_size - count)
    {
      uint8_t *dst = (uint8_t *)(uintptr_t)(AUDIO_SBUF_ADDR + count);
      uint8_t *src = (uint8_t *)ctl->buf.start;
      for (int i = 0; i < len; i++) 
      {
        dst[i] = src[i];
      }
      outl(AUDIO_COUNT_ADDR, count + len);
    }
  }

}
