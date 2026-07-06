#include "npc_device.h"
#include "generated/autoconf.h"

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <ctime>

// ===== Skip 机制 =====
static bool is_skip = false;
void npc_skip_set(void) { is_skip = true; }
bool npc_skip_consume(void) { bool val = is_skip; is_skip = false; return val; }

// ===== Device Implementation =====
#ifdef CONFIG_DEVICE   

static constexpr uint32_t SERIAL_ADDR = 0xa00003f8u;
static constexpr uint32_t RTC_ADDR    = 0xa0000048u;

// VGA MMIO 地址（与 NEMU 兼容）
static constexpr uint32_t VGACTL_ADDR = 0xa0000100u;
static constexpr uint32_t SYNC_ADDR   = 0xa0000104u;
static constexpr uint32_t BLIT_BASE   = 0xa0000108u;  // BLIT 寄存器基址

static uint32_t g_screen_w = 400;
static uint32_t g_screen_h = 300;

static uint64_t boot_time_us = 0;

// ===== VGA / SDL2 =====
#ifdef CONFIG_HAS_VGA
#include <SDL2/SDL.h>

static SDL_Window   *g_sdl_window   = nullptr;
static SDL_Renderer *g_sdl_renderer = nullptr;
static SDL_Texture  *g_sdl_texture  = nullptr;
static uint32_t     *g_fb           = nullptr;
static bool          g_sdl_ready    = false;
static bool          g_sdl_quit     = false;

// GPU BLIT 寄存器（临时存储参数）
static uint32_t blit_src  = 0;   // pmem 源地址
static int      blit_x    = 0;
static int      blit_y    = 0;
static int      blit_w    = 0;
static int      blit_h    = 0;

// dpic.cpp 提供的批量读接口
extern "C" void pmem_read_bulk(uint32_t addr, uint32_t *dst, int count);

static void sdl_init(void) 
{
  if (g_sdl_ready) return;
  if (SDL_Init(SDL_INIT_VIDEO) != 0) return;
  g_sdl_window = SDL_CreateWindow
  (   "NPC VGA",
      SDL_WINDOWPOS_UNDEFINED, 
      SDL_WINDOWPOS_UNDEFINED,
      (int)g_screen_w, 
      (int)g_screen_h,
      SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
  );

  if (!g_sdl_window) return;
  g_sdl_renderer = SDL_CreateRenderer(g_sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (!g_sdl_renderer) return;
  g_sdl_texture = SDL_CreateTexture(g_sdl_renderer,
      SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
      (int)g_screen_w, (int)g_screen_h);
  if (!g_sdl_texture) return;
  g_fb = new uint32_t[g_screen_w * g_screen_h]; //cpu往这里面写值
  std::memset(g_fb, 0, g_screen_w * g_screen_h * sizeof(uint32_t));
  g_sdl_ready = true;
  std::printf("[VGA] SDL initialized, screen %ux%u\n", g_screen_w, g_screen_h);
}

static void sdl_render(void) {
  if (!g_sdl_ready) return;
  SDL_UpdateTexture(g_sdl_texture, nullptr, g_fb,
                    (int)g_screen_w * (int)sizeof(uint32_t));
  SDL_RenderClear(g_sdl_renderer);
  SDL_RenderCopy(g_sdl_renderer, g_sdl_texture, nullptr, nullptr);
  SDL_RenderPresent(g_sdl_renderer);
  SDL_Event ev;
  while (SDL_PollEvent(&ev)) {
    if (ev.type == SDL_QUIT) {
      std::printf("[VGA] SDL window closed, quitting...\n");
      g_sdl_quit = true;
      SDL_DestroyTexture(g_sdl_texture);
      SDL_DestroyRenderer(g_sdl_renderer);
      SDL_DestroyWindow(g_sdl_window);
      SDL_Quit();
      g_sdl_ready = false;
    }
  }
}

// 供 sim_bridge 查询：SDL 窗口是否已被关闭
bool npc_vga_quit(void) { return g_sdl_quit; }

// BLIT: 批量从 guest pmem 拷贝像素到帧缓冲（每行一次 bulk read）
static void gpu_blit(void) {
  int x = blit_x, y = blit_y, w = blit_w, h = blit_h;
  if (w <= 0 || h <= 0) return;
  if (x < 0) x = 0;
  if (y < 0) y = 0;
  if (x + w > (int)g_screen_w) w = (int)g_screen_w - x;
  if (y + h > (int)g_screen_h) h = (int)g_screen_h - y;
  if (w <= 0 || h <= 0) return;

  uint32_t *row_buf = new uint32_t[w];
  for (int j = 0; j < h; j++) {
    pmem_read_bulk(blit_src + j * w * 4, row_buf, w);
    std::memcpy(&g_fb[(y + j) * g_screen_w + x], row_buf, w * sizeof(uint32_t));
  }
  delete[] row_buf;
}
#endif // CONFIG_HAS_VGA

// ===== Timer / RTC =====
static uint64_t get_time_us() {
  timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (uint64_t)ts.tv_sec * 1000000ull + ts.tv_nsec / 1000;
}
static uint64_t uptime_us() {
  if (boot_time_us == 0) boot_time_us = get_time_us();
  return get_time_us() - boot_time_us;
}

// ===== 设备范围判断 =====
bool npc_device_in_range(uint32_t addr) 
{
  if (addr == SERIAL_ADDR || addr == RTC_ADDR || addr == RTC_ADDR + 4)
    return true;
#ifdef CONFIG_HAS_VGA
  if (addr >= VGACTL_ADDR && addr <= BLIT_BASE + 20) return true;  // VGA ctrl + blit
#endif
  return false;
}

// ===== 设备读 =====
uint32_t npc_device_read(uint32_t addr, int) 
{
  if (addr == RTC_ADDR || addr == RTC_ADDR + 4) {
    uint64_t us = uptime_us();
    return (addr == RTC_ADDR) ? (uint32_t)us : (uint32_t)(us >> 32);
  }
#ifdef CONFIG_HAS_VGA //如果读到了VGACTL地址 就弹出sdl窗口 初始化全黑 并返回屏幕尺寸 这样am_gpu init就能读到400 300
  if (addr == VGACTL_ADDR) { sdl_init(); return (g_screen_w << 16) | g_screen_h; }
#endif
  return 0;
}

// ===== 设备写 =====
void npc_device_write(uint32_t addr, int len, uint32_t data) {
  if (addr == SERIAL_ADDR) {
    if (len != 1) std::printf("serial write len = %d, data = 0x%08x\n", len, data);
    std::putchar(data & 0xff); std::fflush(stdout);
    return;
  }
#ifdef CONFIG_HAS_VGA
  if (addr == SYNC_ADDR) { sdl_init(); sdl_render(); return; }
  if (addr == BLIT_BASE + 0)  { blit_src = data; return; }
  if (addr == BLIT_BASE + 4)  { blit_x   = (int)data; return; }
  if (addr == BLIT_BASE + 8)  { blit_y   = (int)data; return; }
  if (addr == BLIT_BASE + 12) { blit_w   = (int)data; return; }
  if (addr == BLIT_BASE + 16) { blit_h   = (int)data; return; }
  if (addr == BLIT_BASE + 20) { sdl_init(); gpu_blit(); return; }  // EXEC
#endif
  std::printf("npc_device_write: bad addr = 0x%08x, len = %d, data = 0x%08x\n",
              addr, len, data);
}

#else // !CONFIG_DEVICE  防止编译器找不到函数报错
bool     npc_device_in_range(uint32_t) { return false; }
uint32_t npc_device_read(uint32_t, int) { return 0; }
void     npc_device_write(uint32_t, int, uint32_t) {}
#endif // CONFIG_DEVICE

// npc_vga_quit stub（无 VGA 时永远返回 false）
#ifdef CONFIG_HAS_VGA
#else
bool npc_vga_quit(void) { return false; }
#endif
