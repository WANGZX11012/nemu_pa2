/***************************************************************************************
* Copyright (c) 2014-2024 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <common.h>
#include <device/map.h>
#include <SDL2/SDL.h>

enum {
  reg_freq,
  reg_channels,
  reg_samples,
  reg_sbuf_size,
  reg_init,
  reg_count,
  nr_reg
};

static uint8_t *sbuf = NULL;
static uint32_t *audio_base = NULL;

static void audio_play(void *userdata, uint8_t *stream, int len)
{
  uint32_t n_read = audio_base[reg_count];
  if(n_read > (uint32_t)len) n_read = len;   //取len和缓冲区大小最小的那个

  if(n_read > 0) //有数据要读
  {
    memcpy(stream, sbuf, n_read);//先把sbuf中的音频数据移到stream
    memmove(sbuf, sbuf + n_read, audio_base[reg_count] - n_read);
    //n_read是已经被播放掉了 把base大小-nread大小移到前面
    audio_base[reg_count] -= n_read;//更新还没播放的数据量。
  }

  if(n_read < (uint32_t)len) //当前声卡缓冲区里的数据不够 SDL 这次要的那么多
  {
    memset(stream + n_read, 0, len - n_read);
  }

}


static void audio_io_handler(uint32_t offset, int len, bool is_write)//初始化
{
  if(!is_write) return; //
  if (offset != reg_init * sizeof(uint32_t)) return;
  if (audio_base[reg_init] == 0) return;

  SDL_AudioSpec s = {0};//初始化结构体
  s.userdata = NULL;
  s.freq = audio_base[reg_freq];
  s.format = AUDIO_S16SYS;
  s.channels = audio_base[reg_channels];
  s.samples = audio_base[reg_samples];
  s.callback = audio_play;


  int ret = SDL_InitSubSystem(SDL_INIT_AUDIO);
  assert(ret == 0);
  ret = SDL_OpenAudio(&s, NULL);
  assert(ret == 0);
  SDL_PauseAudio(0);

}

void init_audio() 
{
  uint32_t space_size = sizeof(uint32_t) * nr_reg;//audio的各个寄存器数量 freq channels 等等
  audio_base = (uint32_t *)new_space(space_size);
  memset(audio_base, 0, space_size);//清空audio base

#ifdef CONFIG_HAS_PORT_IO
  add_pio_map ("audio", CONFIG_AUDIO_CTL_PORT, audio_base, space_size, audio_io_handler);
#else
  add_mmio_map("audio", CONFIG_AUDIO_CTL_MMIO, audio_base, space_size, audio_io_handler);
#endif

  sbuf = (uint8_t *)new_space(CONFIG_SB_SIZE);
  memset(sbuf, 0, CONFIG_SB_SIZE);
  add_mmio_map("audio-sbuf", CONFIG_SB_ADDR, sbuf, CONFIG_SB_SIZE, NULL);

  audio_base[reg_sbuf_size] = CONFIG_SB_SIZE;
  audio_base[reg_count] = 0;

}
