/***************************************************************************************
* npc-nemu local DUT-side difftest implementation.
* The current version only compares architectural pc and gprs.
***************************************************************************************/

#include <dlfcn.h>

#include <cpu/difftest.h>
#include <cpu/npc_cpu.h>
#include <npc_isa.h>
#include <npc_utils.h>
#include <sim_bridge.h>
#include <string.h>

typedef struct {
  vaddr_t pc;
  word_t gpr[32];
} DifftestCPUState;

void (*ref_difftest_regcpy)(void *dut, bool direction) = NULL;
void (*ref_difftest_exec)(uint64_t n) = NULL;
void (*ref_difftest_raise_intr)(uint64_t NO) = NULL;

#ifdef CONFIG_DIFFTEST

static bool checkregs(DifftestCPUState *ref, vaddr_t pc, vaddr_t inst) {
  if (!difftest_check_reg("pc", pc, npc_cpu.pc, inst, ref->pc, npc_cpu.pc)) {
    npc_set_state(NPC_ABORT, pc, -1);
    npc_isa_reg_display();
    return false;
  }

  for (int i = 0; i < 32; i++) {
    char name[8];
    snprintf(name, sizeof(name), "x%d", i);
    if (!difftest_check_reg(name, pc, npc_cpu.pc, inst, ref->gpr[i], npc_cpu.gpr[i])) {
      npc_set_state(NPC_ABORT, pc, -1);
      npc_isa_reg_display();
      return false;
    }
  }

  return true;
}

void difftest_skip_ref() {}

void difftest_skip_dut(int nr_ref, int nr_dut) {
  (void)nr_ref;
  (void)nr_dut;
}

void difftest_attach() {}

void difftest_detach() {}

void init_difftest(char *ref_so_file, char *img_file, long img_size, int port) {
  (void)img_size;
  (void)port;

  if (ref_so_file == NULL) {
#ifdef CONFIG_DIFFTEST_REF_PATH
    ref_so_file = CONFIG_DIFFTEST_REF_PATH;
#endif
  }

  assert(ref_so_file != NULL);

  void *handle = dlopen(ref_so_file, RTLD_LAZY);
  assert(handle);

  ref_difftest_regcpy = (void (*)(void *, bool))dlsym(handle, "difftest_regcpy");
  assert(ref_difftest_regcpy);

  ref_difftest_exec = (void (*)(uint64_t))dlsym(handle, "difftest_exec");
  assert(ref_difftest_exec);

  ref_difftest_raise_intr = (void (*)(uint64_t))dlsym(handle, "difftest_raise_intr");
  assert(ref_difftest_raise_intr);

  void (*ref_difftest_init)(int) = (void (*)(int))dlsym(handle, "difftest_init");
  assert(ref_difftest_init);

  void (*ref_difftest_load_image)(const char *) = (void (*)(const char *))dlsym(handle, "difftest_load_image");
  assert(ref_difftest_load_image);

  Log("Differential testing: %s", ANSI_FMT("ON", ANSI_FG_GREEN));    //打印
  Log("Reference design: %s", ref_so_file);

  ref_difftest_init(port);

  ref_difftest_load_image(img_file);

  DifftestCPUState dut_init = { .pc = npc_sim_get_pc() };
  npc_sim_get_gprs(npc_cpu.gpr);
  for (int i = 0; i < 32; i++) {
    dut_init.gpr[i] = npc_cpu.gpr[i];
  }
  ref_difftest_regcpy(&dut_init, DIFFTEST_TO_REF);
}

bool difftest_step(vaddr_t pc, vaddr_t npc, vaddr_t inst) {
  (void)npc;

  DifftestCPUState ref_r;
  ref_difftest_exec(1);
  ref_difftest_regcpy(&ref_r, DIFFTEST_TO_DUT);
  return checkregs(&ref_r, pc, inst);
}

#else
/* init_difftest stub is provided by difftest.h as static inline */
#endif