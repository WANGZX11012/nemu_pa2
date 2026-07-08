#include "keyboard.h"
#include <cstdio>

#define KEYDOWN_MASK 0x8000
#define QUEUE_SIZE   64

static uint32_t g_key_queue[QUEUE_SIZE];
static int      g_key_head = 0;  // 读指针
static int      g_key_tail = 0;  // 写指针
static uint8_t  g_keymap[256] = {};
static bool     g_kbd_quit = false;

// AM keycodes（与 amdev.h 中 AM_KEYS 宏展开顺序严格一致）
enum {
  AMK_NONE = 0,
  AMK_ESCAPE, AMK_F1, AMK_F2, AMK_F3, AMK_F4, AMK_F5, AMK_F6, AMK_F7, AMK_F8, AMK_F9, AMK_F10, AMK_F11, AMK_F12,
  AMK_GRAVE, AMK_1, AMK_2, AMK_3, AMK_4, AMK_5, AMK_6, AMK_7, AMK_8, AMK_9, AMK_0, AMK_MINUS, AMK_EQUALS, AMK_BACKSPACE,
  AMK_TAB, AMK_Q, AMK_W, AMK_E, AMK_R, AMK_T, AMK_Y, AMK_U, AMK_I, AMK_O, AMK_P, AMK_LEFTBRACKET, AMK_RIGHTBRACKET, AMK_BACKSLASH,
  AMK_CAPSLOCK, AMK_A, AMK_S, AMK_D, AMK_F, AMK_G, AMK_H, AMK_J, AMK_K, AMK_L, AMK_SEMICOLON, AMK_APOSTROPHE, AMK_RETURN,
  AMK_LSHIFT, AMK_Z, AMK_X, AMK_C, AMK_V, AMK_B, AMK_N, AMK_M, AMK_COMMA, AMK_PERIOD, AMK_SLASH, AMK_RSHIFT,
  AMK_LCTRL, AMK_APPLICATION, AMK_LALT, AMK_SPACE, AMK_RALT, AMK_RCTRL,
  AMK_UP, AMK_DOWN, AMK_LEFT, AMK_RIGHT, AMK_INSERT, AMK_DELETE, AMK_HOME, AMK_END, AMK_PAGEUP, AMK_PAGEDOWN
};

static void init_keymap(void) {
  if (g_keymap[SDL_SCANCODE_A] != 0) return;
#define K(sdl, am) g_keymap[SDL_SCANCODE_##sdl] = AMK_##am
  K(A, A); K(B, B); K(C, C); K(D, D); K(E, E); K(F, F); K(G, G); K(H, H);
  K(I, I); K(J, J); K(K, K); K(L, L); K(M, M); K(N, N); K(O, O); K(P, P);
  K(Q, Q); K(R, R); K(S, S); K(T, T); K(U, U); K(V, V); K(W, W); K(X, X);
  K(Y, Y); K(Z, Z);
  K(1, 1); K(2, 2); K(3, 3); K(4, 4); K(5, 5); K(6, 6); K(7, 7); K(8, 8); K(9, 9); K(0, 0);
  K(RETURN, RETURN); K(SPACE, SPACE); K(BACKSPACE, BACKSPACE); K(TAB, TAB);
  K(ESCAPE, ESCAPE); K(UP, UP); K(DOWN, DOWN); K(LEFT, LEFT); K(RIGHT, RIGHT);
  K(LCTRL, LCTRL); K(RCTRL, RCTRL); K(LALT, LALT); K(RALT, RALT);
  K(LSHIFT, LSHIFT); K(RSHIFT, RSHIFT); K(CAPSLOCK, CAPSLOCK);
  K(GRAVE, GRAVE); K(MINUS, MINUS); K(EQUALS, EQUALS);
  K(LEFTBRACKET, LEFTBRACKET); K(RIGHTBRACKET, RIGHTBRACKET);
  K(BACKSLASH, BACKSLASH); K(SEMICOLON, SEMICOLON); K(APOSTROPHE, APOSTROPHE);
  K(COMMA, COMMA); K(PERIOD, PERIOD); K(SLASH, SLASH);
  K(F1, F1); K(F2, F2); K(F3, F3); K(F4, F4); K(F5, F5); K(F6, F6);
  K(F7, F7); K(F8, F8); K(F9, F9); K(F10, F10); K(F11, F11); K(F12, F12);
  K(INSERT, INSERT); K(DELETE, DELETE); K(HOME, HOME); K(END, END);
  K(PAGEUP, PAGEUP); K(PAGEDOWN, PAGEDOWN);
#undef K
}

// ===== 入队 / 出队 =====
static void kbd_enqueue(uint32_t v) {
  int next = (g_key_tail + 1) % QUEUE_SIZE;
  if (next != g_key_head) {  // 队列未满
    g_key_queue[g_key_tail] = v;
    g_key_tail = next;
  }
}
static uint32_t kbd_dequeue(void) {
  if (g_key_head == g_key_tail) return 0;  // 队列为空 → keycode=0=AM_KEY_NONE
  uint32_t v = g_key_queue[g_key_head];
  g_key_head = (g_key_head + 1) % QUEUE_SIZE;
  return v;
}

// ===== 对外接口 =====

bool keyboard_in_range(uint32_t addr) {
  return addr == KEYBRD_ADDR;
}

uint32_t keyboard_read(uint32_t addr) {
  if (addr != KEYBRD_ADDR) return 0;
  SDL_PumpEvents();
  SDL_Event ev;
  int cnt = 32;
  while (--cnt > 0 && SDL_PollEvent(&ev)) {
    std::printf("[KBD] event type=0x%x\n", ev.type);
    if (ev.type == SDL_KEYDOWN || ev.type == SDL_KEYUP || ev.type == SDL_QUIT) {
      keyboard_handle_event(&ev);
    }
  }
  uint32_t v = kbd_dequeue();
  if (v != 0) std::printf("[KBD] read -> keycode=%d keydown=%d\n", v & 0x7fff, (v >> 15) & 1);
  return v;
}

void keyboard_handle_event(const SDL_Event *ev) {
  init_keymap();
  if (ev->type == SDL_QUIT) {
    g_kbd_quit = true;
    return;
  }
  if (ev->type == SDL_KEYDOWN || ev->type == SDL_KEYUP) {
    uint8_t amk = g_keymap[ev->key.keysym.scancode];
    if (amk != AMK_NONE) {
      kbd_enqueue(amk | ((ev->type == SDL_KEYDOWN) ? KEYDOWN_MASK : 0));
    }
  }
}

bool keyboard_quit(void) { return g_kbd_quit; }
