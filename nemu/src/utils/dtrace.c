#include "dtrace.h"
#include <common.h>
#include <device/map.h>
#include <utils.h>
#include <stdio.h>


void dtrace_read(paddr_t addr, int len, IOMap *map)
{
    log_write("dtrace : read %10s at " FMT_PADDR " len: %d\n", map->name, addr, len);
    printf("dtrace : read %10s at " FMT_PADDR " len: %d\n", map->name, addr, len);
}

void dtrace_write(paddr_t addr, int len, word_t data, IOMap *map)
{
    log_write("dtrace: write%10s at " FMT_PADDR " len: %d data is:0x%08x \n", map->name, addr, len, data);
    printf("dtrace: write%10s at " FMT_PADDR " len: %d data is:0x%08x \n", map->name, addr, len, data);
}