/***************************************************************************************
* Copyright (c) 2014-2024 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <dlfcn.h>
#include <capstone/capstone.h>
#include <common.h>

#if defined(__APPLE__)
#define CS_LIB_SUFFIX "5.dylib"
#elif defined(__linux__)
#define CS_LIB_SUFFIX "so.5"
#else
#error "Unsupported platform"
#endif

static size_t (*cs_disasm_dl)(csh handle, const uint8_t *code,
    size_t code_size, uint64_t address, size_t count, cs_insn **insn);
static void (*cs_free_dl)(cs_insn *insn, size_t count);

static csh handle;
/*
 * init_disasm()
 * 作用：在程序启动时动态加载 Capstone 库并初始化反汇编句柄。
 * 工作步骤（高层次）：
 *  1. 使用 `dlopen` 打开项目内置的 libcapstone（路径为 tools/capstone/repo/libcapstone.*）。
 *  2. 通过 `dlsym` 查找 Capstone 的 API：`cs_open`、`cs_disasm`、`cs_free`（x86 下还查找 `cs_option`）。
 *  3. 根据编译时的 ISA 宏（CONFIG_ISA_*）选择 Capstone 的 `arch` 与 `mode`，然后调用 `cs_open` 获取 `csh handle`。
 *  4. 对于 x86，还通过 `cs_option` 设置语法（AT&T）。
 * 注意事项：
 *  - 如果动态库未找到或符号解析失败，代码使用 `assert` 直接终止程序（需保证 libcapstone 在该路径可用）。
 *  - 该函数只完成运行时绑定，不执行任何反汇编操作；实际反汇编由 `disassemble()` 完成。
 */
void init_disasm() {
  void *dl_handle;
  dl_handle = dlopen("tools/capstone/repo/libcapstone." CS_LIB_SUFFIX, RTLD_LAZY);
  assert(dl_handle);

  cs_err (*cs_open_dl)(cs_arch arch, cs_mode mode, csh *handle) = NULL;
  cs_open_dl = dlsym(dl_handle, "cs_open");
  assert(cs_open_dl);

  cs_disasm_dl = dlsym(dl_handle, "cs_disasm");
  assert(cs_disasm_dl);

  cs_free_dl = dlsym(dl_handle, "cs_free");
  assert(cs_free_dl);

  cs_arch arch = MUXDEF(CONFIG_ISA_x86,      CS_ARCH_X86,
                   MUXDEF(CONFIG_ISA_mips32, CS_ARCH_MIPS,
                   MUXDEF(CONFIG_ISA_riscv,  CS_ARCH_RISCV,
                   MUXDEF(CONFIG_ISA_loongarch32r,  CS_ARCH_LOONGARCH, -1))));
  cs_mode mode = MUXDEF(CONFIG_ISA_x86,      CS_MODE_32,
                   MUXDEF(CONFIG_ISA_mips32, CS_MODE_MIPS32,
                   MUXDEF(CONFIG_ISA_riscv,  MUXDEF(CONFIG_ISA64, CS_MODE_RISCV64, CS_MODE_RISCV32) | CS_MODE_RISCVC,
                   MUXDEF(CONFIG_ISA_loongarch32r,  CS_MODE_LOONGARCH32, -1))));
	int ret = cs_open_dl(arch, mode, &handle);
  assert(ret == CS_ERR_OK);

#ifdef CONFIG_ISA_x86
  cs_err (*cs_option_dl)(csh handle, cs_opt_type type, size_t value) = NULL;
  cs_option_dl = dlsym(dl_handle, "cs_option");
  assert(cs_option_dl);

  ret = cs_option_dl(handle, CS_OPT_SYNTAX, CS_OPT_SYNTAX_ATT);
  assert(ret == CS_ERR_OK);
#endif
}

/*
 * disassemble(...)
 * 作用：把给定的机器码字节反汇编为可读的汇编文本并写入 `str`。
 * 参数说明：
 *  - `str, size`：调用者提供的缓冲区与大小，用于接收反汇编结果（mnemonic + op_str）。
 *  - `pc`：反汇编时使用的地址（有些 ISA 的相对偏移解析需要）。
 *  - `code, nbyte`：指令的机器码字节与长度。
 * 工作步骤：
 *  1. 调用在 `init_disasm()` 中绑定的 `cs_disasm`（这里是 `cs_disasm_dl`）对 `code` 进行反汇编；
 *  2. 断言 `count == 1`，取得第一条 `cs_insn`，并把 `insn->mnemonic` 与 `insn->op_str` 格式化写入 `str`；
 *  3. 调用 `cs_free` 释放 Capstone 分配的内存。
 * 注意事项：
 *  - 目前代码使用 `assert(count == 1)`，如果 Capstone 返回 0 条或多条，将触发断言并导致崩溃；
 *    对初学者而言，这里可改进为当 `count > 0` 时安全处理，否则输出占位文本以避免崩溃。
 *  - 本函数不管理 Capstone 的句柄，依赖 `init_disasm()` 在程序启动时正确初始化。
 */
void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte) {
	cs_insn *insn;
	size_t count = cs_disasm_dl(handle, code, nbyte, pc, 0, &insn);
  assert(count == 1);
  int ret = snprintf(str, size, "%s", insn->mnemonic);
  if (insn->op_str[0] != '\0') {
    snprintf(str + ret, size - ret, "\t%s", insn->op_str);
  }
  cs_free_dl(insn, count);
}
