/***************************************************************************************
 *  Capstone-based disassembly for npc-nemu (RISC-V only).
 *  Ported from nemu/src/utils/disasm.c
 ***************************************************************************************/

#include <dlfcn.h>
#include <capstone/capstone.h>
#include <npc_common.h>
#include <npc_utils.h>

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

#define NEMU_HOME_TOOLS "../../nemu/tools"

static csh handle;

void init_disasm(void)
{
  void *dl_handle;
  char cs_path[256];

  /* Try relative to npc-nemu's parent (repo root) */
  snprintf(cs_path, sizeof(cs_path), "%s/capstone/repo/libcapstone." CS_LIB_SUFFIX, NEMU_HOME_TOOLS);
  dl_handle = dlopen(cs_path, RTLD_LAZY);
  if (!dl_handle) {
    /* Fallback: relative to current directory */
    dl_handle = dlopen("tools/capstone/repo/libcapstone." CS_LIB_SUFFIX, RTLD_LAZY);
  }
  Assert(dl_handle, "Cannot find libcapstone.so.5 — build it in nemu/tools/capstone/ first");

  cs_err (*cs_open_dl)(cs_arch arch, cs_mode mode, csh *handle) = NULL;
  cs_open_dl = dlsym(dl_handle, "cs_open");
  assert(cs_open_dl);

  cs_disasm_dl = dlsym(dl_handle, "cs_disasm");
  assert(cs_disasm_dl);

  cs_free_dl = dlsym(dl_handle, "cs_free");
  assert(cs_free_dl);

  cs_arch arch = CS_ARCH_RISCV;
  cs_mode mode = CS_MODE_RISCV32 | CS_MODE_RISCVC;
  int ret = cs_open_dl(arch, mode, &handle);
  Assert(ret == CS_ERR_OK, "Capstone cs_open failed: %d", ret);
}

void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte)
{
  cs_insn *insn;
  size_t count = cs_disasm_dl(handle, code, nbyte, pc, 0, &insn);
  if (count > 0) {
    snprintf(str, size, "%s", insn->mnemonic);
    if (insn->op_str[0] != '\0') {
      snprintf(str + strlen(str), size - strlen(str), "  %s", insn->op_str);
    }
    cs_free_dl(insn, count);
  } else {
    snprintf(str, size, "<unknown>");
  }
}
