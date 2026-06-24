#include "dpic.h"
#include "sim_bridge.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

static const uint32_t PC_BASE = 0x80000000u;

static const size_t MAX_WORDS = 1024 * 1024 / 4; // dynamic_pc_inst max (1MiB / 4 bytes)
static const size_t PMEM_MAX_WORDS = (128 * 1024 * 1024) / 4; // pmem: 128MiB / 4 bytes (cover 0x8000_0000..0x87FF_FFFF)
static uint32_t pmem_words[PMEM_MAX_WORDS];
static size_t pmem_words_size = 0;

static uint32_t dynamic_pc_inst[MAX_WORDS];
static size_t dynamic_pc_inst_size = 0;

static const uint32_t default_pc_inst[] = 
{
  0x000010b7, // PC=0x00000000: lui  x1,0x1      ; x1 = 0x1 << 12 = 0x1000
  0x02008093, // PC=0x00000004: addi x1,x1,32   ; x1 = x1 + 32 = 0x1020
  0x00500113, // PC=0x00000008: addi x2,x0,5    ; x2 = 5
  0x002081b3, // PC=0x0000000c: add  x3,x1,x2   ; x3 = x1 + x2
  0x00302023, // PC=0x00000010: sw   x3,0(x0)   ; store x3 -> mem[0]
  0x00002203, // PC=0x00000014: lw   x4,0(x0)   ; load mem[0] -> x4
  0x00200223, // PC=0x00000018: sb   x2,4(x0)   ; store byte (x2&0xff) -> mem[4]
  0x00404283, // PC=0x0000001c: lw   x5,4(x0)   ; load from mem[4] -> x5 (low byte=5)
  0x024003e7, // PC=0x00000020: jalr x7,x0,0x24 ; x7 = pc+4, jump to 0x24

  // 0x12345678, //invalid test

  0x00100073,   //EBREAK
  0x00c00067, // PC=0x00000024: jalr x0,x0,0xc  ; jump to 0x0c (form loop)  EBREAK
};

extern "C" bool load_hex_program(const char *path) 
{
  FILE *fp = fopen(path, "r");
  if (!fp) 
  {
    perror("load_hex fopen");
    return false;
  }

  char line[512];
  size_t cur = 0;
  size_t max_idx = 0;

  // pass 1: compute max word index
  while (fgets(line, sizeof(line), fp)) 
  {
    char *tok = strtok(line, " \t\r\n");  //用\t \r \n切割line里面的字符串
    
    while (tok) //内层循环
    {
      size_t len = strlen(tok);
      if (len > 0 && tok[len - 1] == ':') //说明是label token 例如0x224:  
      {
        tok[len - 1] = '\0';//冒号变成终止符
        cur = strtoul(tok, NULL, 16); // v3 words addressed
      } 
      else 
      {
        char *end = NULL;
        strtoul(tok, &end, 16);
        if (end != tok && *end == '\0') 
        {
          if (cur + 1 > max_idx) max_idx = cur + 1; //max idx保存最大索引值
          cur++;
        }
      }
      tok = strtok(NULL, " \t\r\n");//继续上一次解析 处理同一行（strtok 内部保存位置）
    }
  }

  if (max_idx == 0) 
  {
    fclose(fp);
    printf("load_hex: no valid instruction in %s\n", path);
    return false;
  }

  if (max_idx > MAX_WORDS) 
  {
    fclose(fp);
    printf("load_hex: program too large (%zu words), max %zu\n", max_idx, MAX_WORDS);
    return false;
  }

  dynamic_pc_inst_size = max_idx;
  for (size_t i = 0; i < dynamic_pc_inst_size; i++) //把dynamic pc mem初始化为0
  {
    dynamic_pc_inst[i] = 0;
  }

  pmem_words_size = dynamic_pc_inst_size;
  for (size_t i = 0; i < pmem_words_size; i++) //pmem 初始化
  {
    pmem_words[i] = 0;
  }

  // pass 2: 把指令读进pmem 和pc mem 重置计数的
  rewind(fp);
  cur = 0;
  size_t loaded = 0;

  while (fgets(line, sizeof(line), fp)) 
  {
    char *tok = strtok(line, " \t\r\n");
    while (tok) 
    {
      if (tok[0] == '#') break;

      size_t len = strlen(tok);
      if (len > 0 && tok[len - 1] == ':') 
      {
        tok[len - 1] = '\0';
        unsigned long v = strtoul(tok, NULL, 16);
        if (v >= PC_BASE) {
          cur = (v - PC_BASE) >> 2;
        } else if ((v & 3u) == 0 && v > MAX_WORDS) {
          cur = v >> 2; // label was a byte address, normalize to word index
        } else {
          cur = v; // assume already a word index
        }
      } 
      else 
      {
        char *end = NULL;
        unsigned long v = strtoul(tok, &end, 16);
        if (end != tok && *end == '\0') 
        {
          if (cur < dynamic_pc_inst_size) 
          {
            dynamic_pc_inst[cur] = (uint32_t)v;
            if (cur < pmem_words_size) 
            {
              pmem_words[cur] = (uint32_t)v;
            }
            loaded++;
          }
          cur++;
        }
      }

      tok = strtok(NULL, " \t\r\n");
    }
  }

  fclose(fp);
  printf("load_hex: allocated %zu words, loaded %zu instructions from %s\n", dynamic_pc_inst_size, loaded, path);
  return true;
}

extern "C" uint32_t pc_read(uint32_t addr)
{
  uint32_t index;
  // 1) 把取指地址映射成“按 4 字节对齐的指令下标 index”
  //    - 当 addr 在 PC_BASE 以上，说明它是映射到 pmem 高地址区的 PC，先减基址再 /4
  //    - 否则按低地址直接 /4（兼容你当前调试阶段的低地址访问）
  if (addr >= PC_BASE) 
  {
    index = (addr - PC_BASE) >> 2;
  } 
  else 
  {
    index = addr >> 2;
  }
  
  // 2) 优先从“动态加载程序”取指（load_hex_program 成功后填充）
  //    dynamic_pc_inst_size 表示当前已分配/可访问的动态指令区大小
  if (index < dynamic_pc_inst_size) //是否落在范围内 0的话就加载default程序
  {
    return dynamic_pc_inst[index];
  }

  // 3) 若没有动态程序（或超出动态区），回退到内置 default_pc_inst
  //    这就是你不传镜像时仍能跑起来的原因
  if (index < sizeof(default_pc_inst) / sizeof(default_pc_inst[0]))
  {
    return default_pc_inst[index];
  }

  // 4) 两个区域都越界则返回 0（等价于取到空指令/非法指令，由上层处理）
  return 0;
}

extern "C" void npc_ebreak(int code) //连接sim bridge
{
  printf("DPI-C: ebreak, a0 = %d\n", code);//code 就是a0 中的值
  npc_sim_mark_halt(code);//调用触发ebreak
}

extern "C" void npc_invalid_inst()
{
  npc_sim_mark_abort();
}

extern "C" void init_pmem(size_t bytes) 
{
  size_t words = bytes; 
  if (words == 0) words = 1;
  if (words > PMEM_MAX_WORDS) words = PMEM_MAX_WORDS;

  pmem_words_size = words;
  for (size_t i = 0; i < pmem_words_size; i++) 
  {
    pmem_words[i] = 0;
  }
}

extern "C" uint32_t pmem_read_u32(uint32_t raddr) 
{
  uint32_t index;
  uint32_t orig = raddr;
  if (raddr < PC_BASE) {
    raddr |= PC_BASE; // temporary map low addresses into pmem high region for debug
    printf("pmem_read_u32 mapped 0x%08x -> 0x%08x\n", orig, raddr);
  }
  if (raddr >= PC_BASE) 
  {
    index = (raddr - PC_BASE) >> 2;
  } 
  else 
  {
    index = raddr >> 2;
  }
  if (index >= pmem_words_size)
  {
    printf("pmem_read_u32 out of range : 0x%08x\n", raddr);
    return 0;
  }
  return pmem_words[index];
}

extern "C" uint8_t pmem_read_u8(uint32_t raddr) 
{
  uint32_t index;
  uint32_t byte_off;
  uint32_t orig = raddr;
  if (raddr < PC_BASE) {
    raddr |= PC_BASE; // temporary mapping
    printf("pmem_read_u8 mapped 0x%08x -> 0x%08x\n", orig, raddr);
  }
  if (raddr >= PC_BASE) 
  {
    uint32_t off = raddr - PC_BASE;
    index = off >> 2;
    byte_off = off & 3u;
  } 
  else 
  {
    index = raddr >> 2;
    byte_off = raddr & 3u;
  }
  if (index >= pmem_words_size)
  {
    printf("pmem read out of range : 0x%08x\n", raddr);
    return 0;
  }
  uint32_t word = pmem_words[index];
  return (word >> (byte_off * 8)) & 0xff;
}

extern "C" void pmem_write_u32(uint32_t waddr, uint32_t wdata) 
{
  uint32_t index;
  uint32_t orig = waddr;
  if (waddr < PC_BASE) {
    waddr |= PC_BASE; // temporary map low addresses into pmem high region for debug
    printf("pmem_write_u32 mapped 0x%08x -> 0x%08x\n", orig, waddr);
  }
  if (waddr >= PC_BASE) 
  {
    index = (waddr - PC_BASE) >> 2;
  } 
  else 
  {
    index = waddr >> 2;
  }
  if (index >= pmem_words_size)
  {
    printf("mem write out of range : 0x%08x\n", waddr);
    return;
  }
  printf("pmem_write_u32 addr=0x%08x data=0x%08x\n", waddr, wdata);
  fflush(stdout);
  pmem_words[index] = wdata;
}

extern "C" void pmem_write_u8(uint32_t addr, uint8_t data) 
{
  uint32_t index;
  uint32_t byte_off;
  uint32_t orig = addr;
  if (addr < PC_BASE) {
    addr |= PC_BASE; // temporary mapping
    printf("pmem_write_u8 mapped 0x%08x -> 0x%08x\n", orig, addr);
  }
  if (addr >= PC_BASE) 
  {
    uint32_t off = addr - PC_BASE;
    index = off >> 2;
    byte_off = off & 3u;
  } 
  else 
  {
    index = addr >> 2;
    byte_off = addr & 3u;
  }
  if (index >= pmem_words_size)
  {
    fprintf(stderr, "pmem_write_u8 out of range addr=0x%08x\n", addr);
    return;
  }
  uint32_t word = pmem_words[index];
  uint32_t mask = ~(0xffu << (byte_off * 8));
  pmem_words[index] = (word & mask) | ((uint32_t)data << (byte_off * 8));
  printf("pmem_write_u8 addr=0x%08x data=0x%02x\n", addr, data);
  fflush(stdout);
}



