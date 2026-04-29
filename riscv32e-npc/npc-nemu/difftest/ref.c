#include <npc_common.h>
#include <difftest-def.h>

#include <stdio.h>

typedef struct {
  word_t pc;
  word_t gpr[32];
} DifftestCPUState;

typedef struct {
  word_t pc;
  word_t snpc;
  word_t dnpc;
  word_t inst;
  word_t gpr[32];
} RefCPUState;

typedef struct {
  word_t pc;
  word_t snpc;
  word_t dnpc;
  word_t inst;
} Decode;

static RefCPUState cpu;
static uint8_t *pmem = NULL;
static size_t pmem_size = 0;
static bool ref_halted = false;

static const uint32_t default_img[] = {
  0x00000297,  // auipc t0,0
  0x00028823,  // sb  zero,16(t0)     //把0x8000_0010高位字节写为0了 小端序
  0x0102c503,  // lbu a0,16(t0)       //读到字节为0 写入a0寄存器
  0x00100073,  // ebreak (used as nemu_trap)
  0xdeadbeef,  // some data
};

#define R(i) cpu.gpr[i]
#define Mr vaddr_read
#define Mw vaddr_write

enum {
  TYPE_I, TYPE_U, TYPE_S,
  TYPE_N,
  TYPE_J, TYPE_R, TYPE_B,
};

static inline uint32_t mem_base(void) {
  return 0x80000000u;
}

static inline uint32_t mem_off(uint32_t addr) {
  return addr - mem_base();
}

static inline uint32_t host_read(const uint8_t *p, int len) {
  uint32_t ret = 0;
  for (int i = 0; i < len; i++) {
    ret |= ((uint32_t)p[i]) << (8 * i);
  }
  return ret;
}

static inline void host_write(uint8_t *p, int len, uint32_t data) {
  for (int i = 0; i < len; i++) {
    p[i] = (data >> (8 * i)) & 0xff;
  }
}

static inline uint32_t paddr_read(uint32_t addr, int len) {
  uint32_t off = mem_off(addr);
  if (pmem == NULL || off + len > pmem_size) {
    return 0;
  }
  return host_read(pmem + off, len);
}

static inline void paddr_write(uint32_t addr, int len, uint32_t data) {
  uint32_t off = mem_off(addr);
  if (pmem == NULL || off + len > pmem_size) {
    return;
  }
  host_write(pmem + off, len, data);
}

static inline uint32_t vaddr_ifetch(uint32_t addr, int len) {
  return paddr_read(addr, len);
}

static inline uint32_t vaddr_read(uint32_t addr, int len) {
  return paddr_read(addr, len);
}

static inline void vaddr_write(uint32_t addr, int len, uint32_t data) {
  paddr_write(addr, len, data);
}

static inline uint32_t inst_fetch(uint32_t *pc, int len) {
  uint32_t inst = vaddr_ifetch(*pc, len);
  *pc += len;
  return inst;
}

static inline uint32_t sext(uint32_t x, int len) {
  uint32_t m = 1u << (len - 1);
  return (x ^ m) - m;
}

static inline uint32_t imm_i(uint32_t inst) { return sext((inst >> 20) & 0xfff, 12); }
static inline uint32_t imm_u(uint32_t inst) { return inst & 0xfffff000u; }
static inline uint32_t imm_s(uint32_t inst) {
  uint32_t imm = ((inst >> 25) << 5) | ((inst >> 7) & 0x1f);
  return sext(imm, 12);
}
static inline uint32_t imm_b(uint32_t inst) {
  uint32_t imm = (((inst >> 31) & 0x1) << 12) |
                 (((inst >> 7) & 0x1) << 11) |
                 (((inst >> 25) & 0x3f) << 5) |
                 (((inst >> 8) & 0xf) << 1);
  return sext(imm, 13);
}
static inline uint32_t imm_j(uint32_t inst) {
  uint32_t imm = (((inst >> 31) & 0x1) << 20) |
                 (((inst >> 12) & 0xff) << 12) |
                 (((inst >> 20) & 0x1) << 11) |
                 (((inst >> 21) & 0x3ff) << 1);
  return sext(imm, 21);
}

static inline void set_gpr(int idx, uint32_t val) {
  if (idx != 0) {
    R(idx) = val;
  }
}

static inline void decode_operand(uint32_t inst, int *rd, word_t *src1, word_t *src2, word_t *imm, int type) {
  int rs1 = (inst >> 15) & 0x1f;
  int rs2 = (inst >> 20) & 0x1f;
  *rd = (inst >> 7) & 0x1f;
  switch (type) {
    case TYPE_I: *src1 = R(rs1); *imm = imm_i(inst); break;
    case TYPE_U: *imm = imm_u(inst); break;
    case TYPE_S: *src1 = R(rs1); *src2 = R(rs2); *imm = imm_s(inst); break;
    case TYPE_J: *imm = imm_j(inst); break;
    case TYPE_R: *src1 = R(rs1); *src2 = R(rs2); break;
    case TYPE_B: *src1 = R(rs1); *src2 = R(rs2); *imm = imm_b(inst); break;
    case TYPE_N: break;
    default: assert(0);
  }
}

static inline word_t my_div(word_t src1, word_t src2)
{
  if (src2 == 0) return (word_t)-1;
  if ((int32_t)src1 == INT32_MIN && (int32_t)src2 == -1) return (word_t)INT32_MIN;
  return (word_t)((int32_t)src1 / (int32_t)src2);
}

static inline word_t my_divu(word_t src1, word_t src2)
{
  if (src2 == 0) return (word_t)-1;
  return (word_t)(src1 / src2);
}

static inline word_t my_rem(word_t src1, word_t src2)
{
  if (src2 == 0) return src1;
  int32_t a = (int32_t)src1;
  int32_t b = (int32_t)src2;
  if (a == INT32_MIN && b == -1) return 0;
  return (word_t)(a % b);
}

static inline word_t my_remu(word_t src1, word_t src2)
{
  if (src2 == 0) return src1;
  return (word_t)(src1 % src2);
}

static inline word_t my_mul(word_t a, word_t b)
{
  int64_t prod = (int64_t)(int32_t)a * (int64_t)(int32_t)b;
  return (word_t)prod;
}

static inline word_t my_mulh(word_t a, word_t b)
{
  int64_t prod = (int64_t)(int32_t)a * (int64_t)(int32_t)b;
  return (word_t)(prod >> 32);
}

static inline word_t my_mulhu(word_t a, word_t b)
{
  uint64_t prod = (uint64_t)a * (uint64_t)b;
  return (word_t)(prod >> 32);
}

static bool load_hex_program(const char *path) {
  FILE *fp = fopen(path, "r");
  if (!fp) return false;

  char line[512];
  size_t cur = 0;
  size_t max_idx = 0;

  while (fgets(line, sizeof(line), fp)) {
    char *tok = strtok(line, " \t\r\n");
    while (tok) {
      size_t len = strlen(tok);
      if (len > 0 && tok[len - 1] == ':') {
        tok[len - 1] = '\0';
        cur = strtoul(tok, NULL, 16);
      } else {
        char *end = NULL;
        strtoul(tok, &end, 16);
        if (end != tok && *end == '\0') {
          if (cur + 1 > max_idx) max_idx = cur + 1;
          cur++;
        }
      }
      tok = strtok(NULL, " \t\r\n");
    }
  }

  if (max_idx == 0 || max_idx > pmem_size / 4) {
    fclose(fp);
    return false;
  }

  memset(pmem, 0, pmem_size);
  rewind(fp);
  cur = 0;
  while (fgets(line, sizeof(line), fp)) {
    char *tok = strtok(line, " \t\r\n");
    while (tok) {
      if (tok[0] == '#') break;
      size_t len = strlen(tok);
      if (len > 0 && tok[len - 1] == ':') {
        tok[len - 1] = '\0';
        cur = strtoul(tok, NULL, 16);
      } else {
        char *end = NULL;
        unsigned long v = strtoul(tok, &end, 16);
        if (end != tok && *end == '\0') {
          ((uint32_t *)pmem)[cur++] = (uint32_t)v;
        }
      }
      tok = strtok(NULL, " \t\r\n");
    }
  }

  fclose(fp);
  return true;
}

static void decode_exec(Decode *s)
{
  uint32_t opcode = s->inst & 0x7f;
  uint32_t funct3 = (s->inst >> 12) & 0x7;
  uint32_t funct7 = (s->inst >> 25) & 0x7f;
  int rd = 0;
  word_t src1 = 0, src2 = 0, imm = 0;

  s->dnpc = s->snpc;

  switch (opcode) {
    case 0x37: // lui
      decode_operand(s->inst, &rd, &src1, &src2, &imm, TYPE_U);
      set_gpr(rd, imm);
      break;
    case 0x17: // auipc
      decode_operand(s->inst, &rd, &src1, &src2, &imm, TYPE_U);
      set_gpr(rd, s->pc + imm);
      break;
    case 0x6f: // jal
      decode_operand(s->inst, &rd, &src1, &src2, &imm, TYPE_J);
      set_gpr(rd, s->snpc);
      s->dnpc = s->pc + imm;
      break;
    case 0x67: // jalr
      decode_operand(s->inst, &rd, &src1, &src2, &imm, TYPE_I);
      set_gpr(rd, s->snpc);
      s->dnpc = (src1 + imm) & ~1u;
      break;
    case 0x63: { // branch
      decode_operand(s->inst, &rd, &src1, &src2, &imm, TYPE_B);
      bool take = false;
      switch (funct3) {
        case 0x0: take = (src1 == src2); break;
        case 0x1: take = (src1 != src2); break;
        case 0x4: take = ((int32_t)src1 < (int32_t)src2); break;
        case 0x5: take = ((int32_t)src1 >= (int32_t)src2); break;
        case 0x6: take = (src1 < src2); break;
        case 0x7: take = (src1 >= src2); break;
        default: break;
      }
      if (take) s->dnpc = s->pc + imm;
      break;
    }
    case 0x03: { // load
      decode_operand(s->inst, &rd, &src1, &src2, &imm, TYPE_I);
      uint32_t addr = src1 + imm;
      switch (funct3) {
        case 0x0: set_gpr(rd, (int8_t)Mr(addr, 1)); break;
        case 0x1: set_gpr(rd, (int16_t)Mr(addr, 2)); break;
        case 0x2: set_gpr(rd, Mr(addr, 4)); break;
        case 0x4: set_gpr(rd, Mr(addr, 1)); break;
        case 0x5: set_gpr(rd, Mr(addr, 2)); break;
        default: break;
      }
      break;
    }
    case 0x23: { // store
      decode_operand(s->inst, &rd, &src1, &src2, &imm, TYPE_S);
      uint32_t addr = src1 + imm;
      switch (funct3) {
        case 0x0: Mw(addr, 1, src2); break;
        case 0x1: Mw(addr, 2, src2); break;
        case 0x2: Mw(addr, 4, src2); break;
        default: break;
      }
      break;
    }
    case 0x13: { // op-imm
      decode_operand(s->inst, &rd, &src1, &src2, &imm, TYPE_I);
      switch (funct3) {
        case 0x0: set_gpr(rd, src1 + imm); break;
        case 0x2: set_gpr(rd, ((int32_t)src1 < (int32_t)imm) ? 1 : 0); break;
        case 0x3: set_gpr(rd, (src1 < imm) ? 1 : 0); break;
        case 0x4: set_gpr(rd, src1 ^ imm); break;
        case 0x6: set_gpr(rd, src1 | imm); break;
        case 0x7: set_gpr(rd, src1 & imm); break;
        case 0x1: set_gpr(rd, src1 << (imm & 0x1f)); break;
        case 0x5:
          if ((imm >> 10) & 1) set_gpr(rd, ((int32_t)src1) >> (imm & 0x1f));
          else set_gpr(rd, src1 >> (imm & 0x1f));
          break;
        default: break;
      }
      break;
    }
    case 0x33: { // op
      decode_operand(s->inst, &rd, &src1, &src2, &imm, TYPE_R);
      switch (funct3) {
        case 0x0:
          if (funct7 == 0x20) set_gpr(rd, src1 - src2);
          else if (funct7 == 0x01) set_gpr(rd, (int32_t)src1 * (int32_t)src2);
          else set_gpr(rd, src1 + src2);
          break;
        case 0x1: set_gpr(rd, src1 << (src2 & 0x1f)); break;
        case 0x2: set_gpr(rd, ((int32_t)src1 < (int32_t)src2) ? 1 : 0); break;
        case 0x3: set_gpr(rd, (src1 < src2) ? 1 : 0); break;
        case 0x4:
          if (funct7 == 0x01) {
            set_gpr(rd, src2 == 0 ? (uint32_t)-1 : (uint32_t)((int32_t)src1 / (int32_t)src2));
          } else {
            set_gpr(rd, src1 ^ src2);
          }
          break;
        case 0x5:
          if (funct7 == 0x20) set_gpr(rd, ((int32_t)src1) >> (src2 & 0x1f));
          else if (funct7 == 0x01) set_gpr(rd, src2 == 0 ? (uint32_t)-1 : src1 / src2);
          else set_gpr(rd, src1 >> (src2 & 0x1f));
          break;
        case 0x6:
          if (funct7 == 0x01) {
            set_gpr(rd, src2 == 0 ? src1 : (uint32_t)((int32_t)src1 % (int32_t)src2));
          } else {
            set_gpr(rd, src1 | src2);
          }
          break;
        case 0x7:
          if (funct7 == 0x01) {
            set_gpr(rd, src2 == 0 ? src1 : (uint32_t)(src1 % src2));
          } else {
            set_gpr(rd, src1 & src2);
          }
          break;
        default: break;
      }
      break;
    }
    case 0x73: // system
      if (s->inst == 0x00100073) {
        ref_halted = true;
      } else {
        fprintf(stderr, "REF invalid system inst 0x%08x at pc 0x%08x\n", s->inst, s->pc);
        ref_halted = true;
      }
      break;
    default:
      fprintf(stderr, "REF invalid inst 0x%08x at pc 0x%08x\n", s->inst, s->pc);
      ref_halted = true;
      break;
  }

  R(0) = 0;
}

static void exec_once(void)
{
  Decode s = {0};
  s.pc = cpu.pc;
  s.snpc = s.pc + 4;
  s.inst = vaddr_ifetch(s.pc, 4);
  decode_exec(&s);
  cpu.pc = s.dnpc;
}

__EXPORT void difftest_regcpy(void *dut, bool direction) 
{
  DifftestCPUState *s = (DifftestCPUState *)dut;
  if (direction == DIFFTEST_TO_REF) {
    cpu.pc = s->pc;
    for (int i = 0; i < 32; i++) {
      cpu.gpr[i] = s->gpr[i];
    }
    cpu.gpr[0] = 0;
  } else {
    s->pc = cpu.pc;
    for (int i = 0; i < 32; i++) {
      s->gpr[i] = cpu.gpr[i];
    }
    s->gpr[0] = 0;
  }
}

__EXPORT void difftest_exec(uint64_t n) 
{
  while (n-- > 0 && !ref_halted) {
    exec_once();
  }
}

__EXPORT void difftest_raise_intr(word_t NO) 
{
  (void)NO;
}

__EXPORT void difftest_init(int port) 
{
  (void)port;
  if (pmem == NULL) {
    pmem_size = 16 * 1024 * 1024;
    pmem = (uint8_t *)malloc(pmem_size);
    assert(pmem != NULL);
    memset(pmem, 0, pmem_size);
  }
  memset(&cpu, 0, sizeof(cpu));
  cpu.pc = mem_base();
  ref_halted = false;
}

__EXPORT void difftest_load_image(const char *path)
{
  if (pmem == NULL) return;

  if (path != NULL && path[0] != '\0' && load_hex_program(path)) {
    return;
  }

  memset(pmem, 0, pmem_size);
  for (size_t i = 0; i < sizeof(default_img) / sizeof(default_img[0]); i++) {
    ((uint32_t *)pmem)[i] = default_img[i];
  }
}
