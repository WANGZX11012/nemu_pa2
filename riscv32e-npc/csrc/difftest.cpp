// ============================================================================
// NPC DiffTest 模块 (difftest.cpp)
// 
// 职责: 加载 REF (NEMU 编译的 .so), 通过 memcpy 同步内存, 每周期比较寄存器.
//
// 数据流 (优化后, 使用 memcpy 而非 load_hex):
//   1. main.cpp 加载 hex → Verilog 内存 (唯一的数据源)
//   2. difftest_init 用 pmem_copy_out 取出 NPC 内存 → difftest_memcpy → NEMU
//   3. 每周期: REF exec 1 条指令 → regcpy 读回 → checkregs 比对
//
// 与旧方案的区别:
//   旧: REF 独立解析 hex (load_hex_to_mem), 两个解析器需维护一致性
//   新: REF 通过 memcpy 从 DUT 获取内存, 单一数据源, 不重复解析 ✓
// ============================================================================

#include "difftest.h"

#include "sim_bridge.h"
#include "dpic.h"  // pmem_copy_out, pmem_get_word_count

#include <cassert>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dlfcn.h>

#include <difftest-def.h>  // DIFFTEST_TO_REF, DIFFTEST_TO_DUT

// ── 寄存器传递格式 (与 NEMU ref.c 约定一致) ─────────────────────────────
// flat array of 33 uint32_t: [0]=pc, [1..32]=gpr[0..31]
struct RefCPUState {
  uint32_t pc;
  uint32_t gpr[32];
};

// ── 全局状态 ────────────────────────────────────────────────────────────
static RefCPUState ref_cpu = {};
static bool diff_failed = false;
static void *ref_handle = nullptr;

// ── REF 导出函数的函数指针 ──────────────────────────────────────────────
static void (*ref_difftest_memcpy)(uint32_t addr, void *buf, size_t n, bool direction) = nullptr;
static void (*ref_difftest_regcpy)(void *dut, bool direction) = nullptr;
static void (*ref_difftest_exec)(uint64_t n) = nullptr;
static void (*ref_difftest_raise_intr)(uint64_t NO) = nullptr;

// ── 物理内存基址 (与 NEMU 的 CONFIG_MBASE 一致) ─────────────────────────
static const uint32_t MEM_BASE = 0x80000000u;

// ============================================================================
// load_ref_so — 动态加载 REF 共享库, 解析所有 API 符号
// ============================================================================
static void load_ref_so() 
{
  const char *ref_so_file = "./obj_dir/ref.so";
  ref_handle = dlopen(ref_so_file, RTLD_LAZY | RTLD_LOCAL);
  // RTLD_LAZY:  延迟解析符号 (用到时才解析)
  // RTLD_LOCAL: REF 的符号不污染全局命名空间 (避免和 NPC 符号冲突)
  if (ref_handle == nullptr) 
  {
    std::fprintf(stderr, "Failed to open %s: %s\n", ref_so_file, dlerror());
    abort();
  }

  // 解析 5 个必需 API (difftest_load_image 不再使用)
  ref_difftest_memcpy      = (void (*)(uint32_t, void*, size_t, bool))dlsym(ref_handle, "difftest_memcpy");
  ref_difftest_regcpy      = (void (*)(void*, bool))dlsym(ref_handle, "difftest_regcpy");
  ref_difftest_exec        = (void (*)(uint64_t))dlsym(ref_handle, "difftest_exec");
  ref_difftest_raise_intr  = (void (*)(uint64_t))dlsym(ref_handle, "difftest_raise_intr");
  auto ref_difftest_init   = (void (*)(int))dlsym(ref_handle, "difftest_init");

  // 断言所有 API 都成功解析 (任何为 nullptr 都是 .so 版本不匹配)
  assert(ref_difftest_memcpy     != nullptr);
  assert(ref_difftest_regcpy     != nullptr);
  assert(ref_difftest_exec       != nullptr);
  assert(ref_difftest_raise_intr != nullptr);
  assert(ref_difftest_init       != nullptr);

  // 初始化 REF: 分配内存, 设置 ISA, 标记为 RUNNING
  ref_difftest_init(1234);
  // port=1234 是历史遗留参数 (预留给 socket difftest), 当前不使用
}

// ============================================================================
// checkregs — 逐寄存器比对 REF 和 DUT
// ============================================================================
static void checkregs(const RefCPUState &ref, uint32_t dut_pc, uint32_t dut_inst, const uint32_t *dut_gpr) 
{
  // PC 比对
  if (ref.pc != dut_pc) {
    std::fprintf(stderr,
      "pc is different after executing instruction 0x%08x at pc = 0x%08x, right = 0x%08x, wrong = 0x%08x, diff = 0x%08x\n",
      dut_inst, dut_pc, ref.pc, dut_pc, ref.pc ^ dut_pc);
    diff_failed = true;
    return;
  }

  // 32 个 GPR 比对
  for (int i = 0; i < 32; i++) {
    if (ref.gpr[i] != dut_gpr[i]) 
    {
      std::fprintf(stderr,
    "x%d is different after executing instruction 0x%08x at pc = 0x%08x, right = 0x%08x, wrong = 0x%08x, diff = 0x%08x\n",
        i, dut_inst, dut_pc, ref.gpr[i], dut_gpr[i], ref.gpr[i] ^ dut_gpr[i]);
      diff_failed = true;
      return;  // 第一个错误就退出, 避免刷屏
    }
  }
}

// ============================================================================
// difftest_init — 初始化 DiffTest (NPC 主程序启动时调用一次)
// ============================================================================
void difftest_init(uint32_t dut_pc) 
{
  ref_cpu = {};
  diff_failed = false;

  // ① 加载 REF .so, 解析 API 符号, 调用 ref_difftest_init()
  load_ref_so();

  // ② 内存同步: NPC → NEMU
  //    NPC 的 pmem_words[] 从逻辑偏移 0 开始 (= 物理地址 0x80000000)
  //    通过 pmem_copy_out 取出 → difftest_memcpy 写入 NEMU 物理内存
  size_t word_count = pmem_get_word_count();
  size_t byte_count = word_count * sizeof(uint32_t);
  uint32_t *pmem_buf = new uint32_t[word_count];   // 堆分配 (避免栈溢出)
  pmem_copy_out(pmem_buf, byte_count);             // 从 NPC 拷贝内存
  ref_difftest_memcpy(MEM_BASE, pmem_buf, byte_count, DIFFTEST_TO_REF);
  delete[] pmem_buf;                               // 释放临时缓冲区

  // ③ 寄存器同步: NPC 的初始 PC 和 GPR → NEMU
  RefCPUState dut_init = {};
  dut_init.pc = dut_pc;
  npc_sim_get_gprs(dut_init.gpr);
  ref_difftest_regcpy(&dut_init, DIFFTEST_TO_REF);
}

// ============================================================================
// difftest_step — 每周期调用一次: REF 执行 1 条指令, 然后比对
// ============================================================================
bool difftest_step(uint32_t dut_pc, const uint32_t *dut_gpr) 
{
  if (diff_failed) return false;  // 已经失败, 不再继续

  uint32_t dut_inst = npc_sim_get_inst();  // 取当前 DUT 执行的指令 (用于错误报告)

  // ① REF 执行一条指令
  ref_difftest_exec(1);

  // ② 读回 REF 的寄存器
  ref_difftest_regcpy(&ref_cpu, DIFFTEST_TO_DUT);

  // ③ 逐寄存器比对
  checkregs(ref_cpu, dut_pc, dut_inst, dut_gpr);

  return !diff_failed;
}
