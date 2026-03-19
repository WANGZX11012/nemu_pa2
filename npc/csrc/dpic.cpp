#include "dpic.h"
#include <cstdio>
#include <cstdlib>

#define PMEM_BYTES  (1024)
static uint8_t pmem_buf[PMEM_BYTES];


// 最小程序内存：按32位字存放指令
static uint32_t pc_inst[256] = 
{
  0x01400513, // 0x00: addi a0, zero, 20
  0x010000e7, // 0x04: jalr ra, 16(zero) -> 跳到 0x10, ra=0x08
  0x00c000e7, // 0x08: jalr ra, 12(zero) -> 跳到 0x0c, ra=0x0c
  0x00c00067, // 0x0c: jalr zero,12(zero) -> 自旋 halt
  0x00a50513, // 0x10: addi a0, a0, 10
  0x00008067, // 0x14: jalr zero,0(ra) -> 返回到 ra(0x08)
};


uint32_t pc_read(uint32_t addr) 
{
  return pc_inst[(addr & ~0x3u) >> 2];
}

extern "C" void npc_ebreak(int code) 
{
  printf("DPI-C: ebreak at PC=0x%08x\n", code);
  fflush(stdout);
  exit(0);
}

extern "C" uint32_t pmem_read_u32(uint32_t raddr)
{
    uint32_t aligned_addr = raddr & ~0x3u;
    if(aligned_addr + 4 > PMEM_BYTES) 
    {
        printf("p read out range : 0x%08x", aligned_addr);
        return 0;
    }

    return  (uint32_t)pmem_buf[aligned_addr]
        |   (uint32_t)pmem_buf[aligned_addr + 1] << 8
        |   (uint32_t)pmem_buf[aligned_addr + 2] << 16
        |   (uint32_t)pmem_buf[aligned_addr + 3] << 24;
}

extern "C" uint8_t pmem_read_u8(uint32_t raddr)
{
    if(raddr  >= PMEM_BYTES) 
    {
        printf("mem read out of range : 0x%08x", raddr);
        return 0;
    }

    return pmem_buf[raddr];

}

extern "C" void pmem_write_u32(uint32_t waddr, uint32_t wdata)
{
    uint32_t aligned_waddr = waddr & ~0x3u;
    if(aligned_waddr + 4 > PMEM_BYTES) 
    {
        printf("mem write out of range : 0x%08x", aligned_waddr);
        return ;
    }

    pmem_buf[aligned_waddr] = wdata & 0xff;
    pmem_buf[aligned_waddr + 1] = (wdata >> 8) & 0xff;
    pmem_buf[aligned_waddr + 2] = (wdata >> 16) & 0xff;
    pmem_buf[aligned_waddr + 3] = (wdata >> 24) & 0xff;
    
    
}

extern "C" void pmem_write_u8(uint32_t addr, uint8_t data) 
{
  uint32_t a = addr;
  if (a >= PMEM_BYTES) {
    fprintf(stderr, "pmem_write_u8 OOB addr=0x%08x\n", addr);
    return;
  }
  pmem_buf[a] = data;
}




