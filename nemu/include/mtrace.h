#ifndef __MTRACE_H__
#define __MTRACE_H__

#include <common.h>

void p_mread(paddr_t addr, int len, word_t val);
void p_mwrite(paddr_t addr, int len, word_t data);

#endif