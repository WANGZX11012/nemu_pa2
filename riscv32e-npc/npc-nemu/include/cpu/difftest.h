/***************************************************************************************
* npc-nemu local difftest interface.
* Keep the style close to NEMU, but only compare pc and gprs for now.
***************************************************************************************/

#ifndef __CPU_DIFFTEST_H__
#define __CPU_DIFFTEST_H__

#include <npc_common.h>
#include <difftest-def.h>

#ifdef CONFIG_DIFFTEST
void difftest_skip_ref();
void difftest_skip_dut(int nr_ref, int nr_dut);
bool difftest_step(vaddr_t pc, vaddr_t npc, vaddr_t inst);
void difftest_detach();
void difftest_attach();
void init_difftest(char *ref_so_file, char *img_file, long img_size, int port);
#else
static inline void difftest_skip_ref() {}
static inline void difftest_skip_dut(int nr_ref, int nr_dut) {
  (void)nr_ref;
  (void)nr_dut;
}
static inline bool difftest_step(vaddr_t pc, vaddr_t npc, vaddr_t inst) {
  (void)pc;
  (void)npc;
  (void)inst;
  return true;
}
static inline void difftest_detach() {}
static inline void difftest_attach() {}
static inline void init_difftest(char *ref_so_file, char *img_file, long img_size, int port) {
  (void)ref_so_file;
  (void)img_file;
  (void)img_size;
  (void)port;
}
#endif

extern void (*ref_difftest_regcpy)(void *dut, bool direction);
extern void (*ref_difftest_exec)(uint64_t n);
extern void (*ref_difftest_raise_intr)(uint64_t NO);

static inline bool difftest_check_reg(const char *name, vaddr_t pc, vaddr_t npc, vaddr_t inst, word_t ref, word_t dut) {
  if (ref != dut) {
    Log("%s is different after executing instruction 0x%08x at pc = " FMT_WORD
        " -> next pc = " FMT_WORD ", right = " FMT_WORD ", wrong = " FMT_WORD ", diff = " FMT_WORD,
        name, (uint32_t)inst, pc, npc, ref, dut, ref ^ dut);
    return false;
  }
  return true;
}

#endif