#include "npc_device.h"
#include "generated/autoconf.h"

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <ctime>

// ===== Skip 机制（用于 difftest 跳过 MMIO 指令比对）=====
static bool is_skip = false;

void npc_skip_set(void) { is_skip = true; }
bool npc_skip_consume(void) { bool val = is_skip; is_skip = false; return val; }

// ===== Device Implementation (only compiled when CONFIG_DEVICE) =====
#ifdef CONFIG_DEVICE

static constexpr uint32_t SERIAL_ADDR = 0xa00003f8u;
static constexpr uint32_t RTC_ADDR    = 0xa0000048u;

// VGA 地址（与 NEMU 兼容）
static constexpr uint32_t VGACTL_ADDR = 0xa0000100u;
static constexpr uint32_t SYNC_ADDR   = 0xa0000104u;
static constexpr uint32_t FB_ADDR     = 0xa1000000u;
static constexpr uint32_t FB_SIZE     = 0x200000u;

static uint32_t g_screen_w = 400;
static uint32_t g_screen_h = 300;

static uint64_t boot_time_us = 0;

// ===== VGA / SDL2 frame buffer =====
#define CONFIG_VGA 1

#if defined(CONFIG_VGA)
#include <SDL2/SDL.h>

static SDL_Window   *g_sdl_window   = nullptr;
static SDL_Renderer *g_sdl_renderer = nullptr;
static SDL_Texture  *g_sdl_texture  = nullptr;
static uint32_t     *g_fb           = nullptr;
static bool          g_sdl_ready    = false;

static void sdl_init(void) {
  if (g_sdl_ready) return;
  if (SDL_Init(SDL_INIT_VIDEO) != 0) return;
  g_sdl_window = SDL_CreateWindow("NPC VGA", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, (int)g_screen_w, (int)g_screen_h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  if (!g_sdl_window) return;
  g_sdl_renderer = SDL_CreateRenderer(g_sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (!g_sdl_renderer) return;
  g_sdl_texture = SDL_CreateTexture(g_sdl_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, (int)g_screen_w, (int)g_screen_h);
  if (!g_sdl_texture) return;
  g_fb = new uint32_t[g_screen_w * g_screen_h];
  std::memset(g_fb, 0, g_screen_w * g_screen_h * sizeof(uint32_t));
  g_sdl_ready = true;
  std::printf("[VGA] SDL initialized, screen %ux%u\n", g_screen_w, g_screen_h);
}

static void sdl_render(void) {
  if (!g_sdl_ready) return;
  SDL_UpdateTexture(g_sdl_texture, nullptr, g_fb, (int)g_screen_w * (int)sizeof(uint32_t));
  SDL_RenderClear(g_sdl_renderer);
  SDL_RenderCopy(g_sdl_renderer, g_sdl_texture, nullptr, nullptr);
  SDL_RenderPresent(g_sdl_renderer);
  SDL_Event ev;
  while (SDL_PollEvent(&ev)) { if (ev.type == SDL_QUIT) break; }
}

static inline bool in_fb_range(uint32_t addr) { return addr >= FB_ADDR && addr < FB_ADDR + FB_SIZE; }
#endif

static uint64_t get_time_us() {
  timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (uint64_t)ts.tv_sec * 1000000ull + ts.tv_nsec / 1000;
}

static uint64_t uptime_us() {
  if (boot_time_us == 0) boot_time_us = get_time_us();
  return get_time_us() - boot_time_us;
}

bool npc_device_in_range(uint32_t addr) {
  if (addr == SERIAL_ADDR || addr == RTC_ADDR || addr == RTC_ADDR + 4) return true;
#if defined(CONFIG_VGA)
  if (addr == VGACTL_ADDR || addr == SYNC_ADDR) return true;
  if (in_fb_range(addr)) return true;
#endif
  return false;
}

uint32_t npc_device_read(uint32_t addr, int len) {
  if (addr == RTC_ADDR || addr == RTC_ADDR + 4) {
    uint64_t us = uptime_us();
    return (addr == RTC_ADDR) ? (uint32_t)us : (uint32_t)(us >> 32);
  }
#if defined(CONFIG_VGA)
  if (addr == VGACTL_ADDR) { sdl_init(); return (g_screen_w << 16) | g_screen_h; }
  if (in_fb_range(addr)) { sdl_init(); uint32_t off = (addr - FB_ADDR) >> 2; if (off < g_screen_w * g_screen_h) return g_fb[off]; return 0; }
#endif
  return 0;
}

void npc_device_write(uint32_t addr, int len, uint32_t data) {
  if (addr == SERIAL_ADDR) {
    if (len != 1) std::printf("serial write len = %d, data = 0x%08x\n", len, data);
    std::putchar(data & 0xff); std::fflush(stdout); return;
  }
#if defined(CONFIG_VGA)
  if (addr == SYNC_ADDR) { sdl_init(); sdl_render(); return; }
  if (in_fb_range(addr)) { sdl_init(); uint32_t off = (addr - FB_ADDR) >> 2; if (off < g_screen_w * g_screen_h) g_fb[off] = data; return; }
#endif
  std::printf("npc_device_write: bad addr = 0x%08x, len = %d, data = 0x%08x\n", addr, len, data);
}
              addr, len, data);
}

#else // !CONFIG_DEVICE

// Stubs: no device support, all addresses map to normal memory
bool npc_device_in_range(uint32_t) { return false; }
uint32_t npc_device_read(uint32_t, int) { return 0; }
void npc_device_write(uint32_t, int, uint32_t) {}

#endif // CONFIG_DEVICE


