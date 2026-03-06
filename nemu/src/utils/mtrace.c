#include "mtrace.h"
#include <memory/paddr.h>

void p_mread(paddr_t addr, int len, word_t val) //物理内存读
{
    printf("mem read at " FMT_PADDR " len = %d val = 0x%08x\n", addr, len, val);
    log_write("MTR R " FMT_PADDR " len=%d val = %d\n", addr, len, val);
}


void p_mwrite(paddr_t addr, int len, word_t data)
{
    printf("mem write at " FMT_PADDR " len = %d data = %08x\n",addr, len, data);
    log_write("MTR W " FMT_PADDR " len=%d val=" FMT_WORD "\n", addr, len, data);
}