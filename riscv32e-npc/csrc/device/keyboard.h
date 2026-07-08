#ifndef NPC_DEVICE_KEYBOARD_H
#define NPC_DEVICE_KEYBOARD_H

#include <cstdint>
#include <SDL2/SDL.h>

static constexpr uint32_t KEYBRD_ADDR = 0xa0000060u;

bool     keyboard_in_range      (uint32_t addr);
uint32_t keyboard_read          (uint32_t addr);
void     keyboard_handle_event  (const SDL_Event *ev);
bool     keyboard_quit          (void);

#endif
