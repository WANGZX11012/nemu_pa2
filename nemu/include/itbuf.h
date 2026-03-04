#ifndef __ITBUF_H__
#define __ITBUF_H__

#include <isa.h>

void itbuf_p(vaddr_t pc, const char* logbuf); //字符串
void itbuf_d(int n);

#endif