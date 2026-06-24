#ifndef __NPC_MACRO_H__
#define __NPC_MACRO_H__

#include <string.h>

// macro stringizing
#define npc_str_temp(x) #x
#define npc_str(x) npc_str_temp(x)

// strlen() for string constant
#define STRLEN(CONST_STR) (sizeof(CONST_STR) - 1)

// calculate the length of an array
#define npc_ARRLEN(arr) (int)(sizeof(arr) / sizeof(arr[0]))

// macro concatenation
#define npc_concat_temp(x, y) x ## y
#define npc_concat(x, y) npc_concat_temp(x, y)
#define npc_concat3(x, y, z) npc_concat(npc_concat(x, y), z)
#define npc_concat4(x, y, z, w) npc_concat3(npc_concat(x, y), z, w)
#define npc_concat5(x, y, z, v, w) npc_concat4(npc_concat(x, y), z, v, w)

// macro testing
// See https://stackoverflow.com/questions/26099745/test-if-preprocessor-symbol-is-defined-inside-macro
#define npc_CHOOSE2nd(a, b, ...) b
#define npc_MUX_WITH_COMMA(contain_comma, a, b) npc_CHOOSE2nd(contain_comma a, b)
#define npc_MUX_MACRO_PROPERTY(p, macro, a, b) npc_MUX_WITH_COMMA(npc_concat(p, macro), a, b)
// define placeholders for some property
#define __NPC_P_DEF_0  X,
#define __NPC_P_DEF_1  X,
#define __NPC_P_ONE_1  X,
#define __NPC_P_ZERO_0 X,
// define some selection functions based on the properties of BOOLEAN macro
#define npc_MUXDEF(macro, X, Y)  npc_MUX_MACRO_PROPERTY(__NPC_P_DEF_, macro, X, Y)
#define npc_MUXNDEF(macro, X, Y) npc_MUX_MACRO_PROPERTY(__NPC_P_DEF_, macro, Y, X)
#define npc_MUXONE(macro, X, Y)  npc_MUX_MACRO_PROPERTY(__NPC_P_ONE_, macro, X, Y)
#define npc_MUXZERO(macro, X, Y) npc_MUX_MACRO_PROPERTY(__NPC_P_ZERO_,macro, X, Y)

// test if a boolean macro is defined
#define npc_ISDEF(macro) npc_MUXDEF(macro, 1, 0)
// test if a boolean macro is undefined
#define npc_ISNDEF(macro) npc_MUXNDEF(macro, 1, 0)
// test if a boolean macro is defined to 1
#define npc_ISONE(macro) npc_MUXONE(macro, 1, 0)
// test if a boolean macro is defined to 0
#define npc_ISZERO(macro) npc_MUXZERO(macro, 1, 0)
// test if a macro of ANY type is defined
// NOTE1: it ONLY works inside a function, since it calls `strcmp()`
// NOTE2: macros defined to themselves (#define A A) will get wrong results
#define npc_isdef(macro) (strcmp("" #macro, "" npc_str(macro)) != 0)

// simplification for conditional compilation
#define npc__IGNORE(...)
#define npc__KEEP(...) __VA_ARGS__
// keep the code if a boolean macro is defined
#define npc_IFDEF(macro, ...) npc_MUXDEF(macro, npc__KEEP, npc__IGNORE)(__VA_ARGS__)
// keep the code if a boolean macro is undefined
#define npc_IFNDEF(macro, ...) npc_MUXNDEF(macro, npc__KEEP, npc__IGNORE)(__VA_ARGS__)
// keep the code if a boolean macro is defined to 1
#define npc_IFONE(macro, ...) npc_MUXONE(macro, npc__KEEP, npc__IGNORE)(__VA_ARGS__)
// keep the code if a boolean macro is defined to 0
#define npc_IFZERO(macro, ...) npc_MUXZERO(macro, npc__KEEP, npc__IGNORE)(__VA_ARGS__)

// functional-programming-like macro (X-macro)
// apply the function `f` to each element in the container `c`
// NOTE1: `c` should be defined as a list like:
//   f(a0) f(a1) f(a2) ...
// NOTE2: each element in the container can be a tuple
#define npc_MAP(c, f) c(f)

#define npc_BITMASK(bits) ((1ull << (bits)) - 1)       //生成“低 bits 位全为 1”的掩码 
#define npc_BITS(x, hi, lo) (((x) >> (lo)) & npc_BITMASK((hi) - (lo) + 1)) // similar to x[hi:lo] in verilog 取lo到hi位
#define npc_SEXT(x, len) ({ struct { int64_t n : len; } __x = { .n = x }; (uint64_t)__x.n; }) //建一个只有 len 位的有符号字段 n，把 x 放进去。”
//这样 x 会被截断到 len 位，并按有符号数保存，后面读 __x.n 时就自动符号扩展。

#define npc_ROUNDUP(a, sz)   ((((uintptr_t)a) + (sz) - 1) & ~((sz) - 1))
#define npc_ROUNDDOWN(a, sz) ((((uintptr_t)a)) & ~((sz) - 1))

#define npc_PG_ALIGN __attribute((aligned(4096)))

#if !defined(likely)
#define npc_likely(cond)   __builtin_expect(cond, 1)
#define npc_unlikely(cond) __builtin_expect(cond, 0)
#endif

// for AM IOE
#define npc_io_read(reg) \
  ({ reg##_T __io_param; \
    ioe_read(reg, &__io_param); \
    __io_param; })

#define npc_io_write(reg, ...) \
  ({ reg##_T __io_param = (reg##_T) { __VA_ARGS__ }; \
    ioe_write(reg, &__io_param); })

// compatibility aliases to reduce breakage during migration
#define str(x) npc_str(x)
#define ARRLEN(arr) npc_ARRLEN(arr)
#define MUXDEF(macro, X, Y) npc_MUXDEF(macro, X, Y)
#define MUXNDEF(macro, X, Y) npc_MUXNDEF(macro, X, Y)
#define MUXONE(macro, X, Y) npc_MUXONE(macro, X, Y)
#define MUXZERO(macro, X, Y) npc_MUXZERO(macro, X, Y)
#define ISDEF(macro) npc_ISDEF(macro)
#define ISNDEF(macro) npc_ISNDEF(macro)
#define ISONE(macro) npc_ISONE(macro)
#define ISZERO(macro) npc_ISZERO(macro)
#define isdef(macro) npc_isdef(macro)
#define IFDEF(macro, ...) npc_IFDEF(macro, __VA_ARGS__)
#define IFNDEF(macro, ...) npc_IFNDEF(macro, __VA_ARGS__)
#define IFONE(macro, ...) npc_IFONE(macro, __VA_ARGS__)
#define IFZERO(macro, ...) npc_IFZERO(macro, __VA_ARGS__)
#define MAP(c, f) npc_MAP(c, f)
#define BITMASK(bits) npc_BITMASK(bits)
#define BITS(x, hi, lo) npc_BITS(x, hi, lo)
#define SEXT(x, len) npc_SEXT(x, len)
#define ROUNDUP(a, sz) npc_ROUNDUP(a, sz)
#define ROUNDDOWN(a, sz) npc_ROUNDDOWN(a, sz)
#define PG_ALIGN npc_PG_ALIGN
#define likely(cond) npc_likely(cond)
#define unlikely(cond) npc_unlikely(cond)
#define io_read(reg) npc_io_read(reg)
#define io_write(reg, ...) npc_io_write(reg, __VA_ARGS__)

#endif
