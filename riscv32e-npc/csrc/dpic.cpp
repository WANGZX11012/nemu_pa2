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
  0x00000297,  // auipc t0,0
  0x00028823,  // sb  zero,16(t0)     //把0x8000_0010高位字节写为0了 小端序
  0x0102c503,  // lbu a0,16(t0)       //读到字节为0 写入a0寄存器
  0x00100073,  // ebreak (used as nemu_trap)
  0xdeadbeef,  // some data
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
  pmem_words_size = PMEM_MAX_WORDS;
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
        cur = strtoul(tok, NULL, 16); // v3 words addressed
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
  printf("[DEBUG] First instruction: dynamic_pc_inst[0]=0x%08x\n", dynamic_pc_inst_size > 0 ? dynamic_pc_inst[0] : 0);
  return true;
}

extern "C" uint32_t pc_read(uint32_t addr)
{
  uint32_t index;
  uint32_t inst = 0;
  // 1) 把取指地址映射成“按 4 字节对齐的指令下标 index”
  if (addr >= PC_BASE) {
    index = (addr - PC_BASE) >> 2;
  } else {
    index = addr >> 2;
  }

  // 2) 优先从动态加载区取指
  if (index < dynamic_pc_inst_size) {
    inst = dynamic_pc_inst[index];
  } else if (index < sizeof(default_pc_inst) / sizeof(default_pc_inst[0])) {
    // 3) fallback to builtin default instructions
    inst = default_pc_inst[index];
  } else {
    // 4) out of available instruction range -> return 0
    inst = 0;
  }

  // pc_read 不再打印，打印由 sim_bridge 统一负责以避免重复输出
  return inst;
}

extern "C" void pmem_copy_out(void *dst, size_t bytes)
{
  if (dst == nullptr || bytes == 0) return;

  size_t words = bytes / sizeof(uint32_t);
  if (bytes % sizeof(uint32_t) != 0) 
  {
    words += 1;
  }
  if (words > pmem_words_size) {
    words = pmem_words_size;
  }

  std::memcpy(dst, pmem_words, words * sizeof(uint32_t));
}

extern "C" size_t pmem_get_word_count(void)
{
  return pmem_words_size;
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

extern "C" void init_pmem(size_t bytes) //pmem初始化
{
  size_t words = bytes; 
  if (words == 0) words = 1;
  if (words > PMEM_MAX_WORDS) words = PMEM_MAX_WORDS;

  pmem_words_size = words;
  for (size_t i = 0; i < pmem_words_size; i++) //初始化为0
  {
    pmem_words[i] = 0;
  }
}

extern "C" uint32_t pmem_read_u32(uint32_t raddr) 
{
  uint32_t index;
  uint32_t orig = raddr;
  if (raddr < PC_BASE)
  {
    raddr = (raddr & 0x07ffffffu) | PC_BASE; // map low addresses into 0x80000000..0x87ffffff
    // printf("pmem_read_u32 mapped 0x%08x -> 0x%08x\n", orig, raddr);
  }

  index = (raddr >= PC_BASE) ? ((raddr - PC_BASE) >> 2) : (raddr >> 2);

  if (index >= pmem_words_size)
  {
    printf("pmem read out of range!\n");
    return 0;
  }
  return pmem_words[index];
}

extern "C" uint8_t pmem_read_u8(uint32_t raddr) 
{
  uint32_t index;
  uint32_t byte_off;
  uint32_t orig = raddr;
  if (raddr < PC_BASE)
  {
    raddr = (raddr & 0x07ffffffu) | PC_BASE; // map low addresses into 0x80000000..0x87ffffff
    // printf("pmem_read_u8 mapped 0x%08x -> 0x%08x\n", orig, raddr);
  }

  // 统一计算相对于 pmem 的字节偏移，再派生出 word 下标和字节偏移。
  // 这样不依赖隐含不变式，代码更清晰且避免未初始化风险。
  uint32_t off = (raddr >= PC_BASE) ? (raddr - PC_BASE) : raddr;
  index = off >> 2;
  byte_off = off & 3u;

  if (index >= pmem_words_size)
  {
    printf("pmem read out of range!\n");
    return 0;
  }
  uint32_t word = pmem_words[index];
  return (word >> (byte_off * 8)) & 0xff;
}

extern "C" uint16_t pmem_read_u16(uint32_t raddr) //半字的读
{
  uint32_t orig = raddr;
  if (raddr < PC_BASE)
  {
    raddr = (raddr & 0x07ffffffu) | PC_BASE; // map low addresses into 0x80000000..0x87ffffff
    // printf("pmem_read_u16 mapped 0x%08x -> 0x%08x\n", orig, raddr);
  }

  uint32_t off = (raddr >= PC_BASE) ? (raddr - PC_BASE) : raddr;
  uint32_t index = off >> 2;
  uint32_t byte_off = off & 3u;

  if (index >= pmem_words_size)
  {
    printf("pmem read out of range!\n");
    return 0;
  }

  uint16_t val;

  if (byte_off <= 2)
  {
    val = (uint16_t)((pmem_words[index] >> (byte_off * 8)) & 0xffff);
    return val;
  }
  else /* byte_off == 3: halfword crosses to next word */
  {
    if (index + 1 >= pmem_words_size) {
      return 0;
    }
    uint32_t low = (pmem_words[index] >> 24) & 0xffu;
    uint32_t high = pmem_words[index + 1] & 0xffu;
    val = (uint16_t)((high << 8) | low);
    return val;
  }
}





extern "C" void pmem_write_u32(uint32_t waddr, uint32_t wdata) 
{
  uint32_t index;
  uint32_t orig = waddr;
  if (waddr < PC_BASE)
  {
    waddr = (waddr & 0x07ffffffu) | PC_BASE; // map low addresses into 0x80000000..0x87ffffff
    // printf("pmem_write_u32 mapped 0x%08x -> 0x%08x\n", orig, waddr);
  }
  index = (waddr >= PC_BASE) ? ((waddr - PC_BASE) >> 2) : (waddr >> 2);
  if (index >= pmem_words_size)
  {
    printf("pmem write out of range!\n");
    return;
  }
  // printf("pmem_write_u32 addr=0x%08x data=0x%08x\n", waddr, wdata);
  fflush(stdout);
  pmem_words[index] = wdata;
}

extern "C" void pmem_write_u8(uint32_t addr, uint8_t data) 
{
  uint32_t index;
  uint32_t byte_off;
  uint32_t orig = addr;
  if (addr < PC_BASE)
  {
    addr = (addr & 0x07ffffffu) | PC_BASE; // map low addresses into 0x80000000..0x87ffffff
    // printf("pmem_write_u8 mapped 0x%08x -> 0x%08x\n", orig, addr);
  }

  uint32_t off = (addr >= PC_BASE) ? (addr - PC_BASE) : addr;
  index = off >> 2;
  byte_off = off & 3u;
  if (index >= pmem_words_size)
  {
    printf("pmem write out of range!\n");
    return;
  }
  uint32_t word = pmem_words[index];
  uint32_t mask = ~(0xffu << (byte_off * 8));
  pmem_words[index] = (word & mask) | ((uint32_t)data << (byte_off * 8));
  // printf("pmem_write_u8 addr=0x%08x data=0x%02x\n", addr, data);
  fflush(stdout);
}

extern "C" void pmem_write_u16(uint32_t addr, uint16_t data) 
{
  uint32_t index;
  uint32_t orig = addr;
  if (addr < PC_BASE)
  {
    addr = (addr & 0x07ffffffu) | PC_BASE; // map low addresses into 0x80000000..0x87ffffff
    // printf("pmem_write_u16 mapped 0x%08x -> 0x%08x\n", orig, addr);
  }

  uint32_t off = (addr >= PC_BASE) ? (addr - PC_BASE) : addr;
  index = off >> 2;
  uint32_t byte_off = off & 3u;

  if (index >= pmem_words_size) 
  {
    printf("pmem write out of range!\n");
    return;
  }

  if (byte_off <= 2) 
  {
    uint32_t word = pmem_words[index];
    uint32_t mask = ~(0xffffu << (byte_off * 8));
    pmem_words[index] = (word & mask) | (((uint32_t)data) << (byte_off * 8));
    printf("pmem_write_u16 addr=0x%08x data=0x%04x\n", addr, data);
    fflush(stdout);
    return;
  } 
  else 
  {
    /* byte_off == 3: split across two words */
    if (index + 1 >= pmem_words_size) {
      return;
    }
    uint32_t low = (uint32_t)(data & 0xffu);
    uint32_t high = (uint32_t)((data >> 8) & 0xffu);

    uint32_t word = pmem_words[index];
    word = (word & ~(0xffu << 24)) | (low << 24);
    pmem_words[index] = word;

    uint32_t next = pmem_words[index + 1];
    next = (next & ~0xffu) | high;
    pmem_words[index + 1] = next;

    // printf("pmem_write_u16 addr=0x%08x data=0x%04x (cross-word)\n", addr, data);
    fflush(stdout);
    return;
  }
}
