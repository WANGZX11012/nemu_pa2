#ifndef NPC_DEVICE_H
#define NPC_DEVICE_H

#ifdef __cplusplus
#include <cstdint>
extern "C" {
#else
#include <stdint.h>
#endif

bool npc_device_in_range(uint32_t addr);
bool npc_skip_consume(void);
void npc_skip_set(void);
void npc_device_write(uint32_t addr, int len, uint32_t data);
uint32_t npc_device_read(uint32_t addr, int len);

#ifdef __cplusplus
}
#endif

// 供仿真循环查询：SDL 窗口是否被关闭
bool npc_vga_quit(void);

#endif