#!/bin/bash

set -e

src=${*: -1}
dst=${*: -2:1}
cc=$1
flags=${*: 2:$#-4}

# Prefer bare-metal toolchain for minirv; fallback to linux-gnu if not installed.
##强制这条链路尽量走 bare-metal 工具链，避开 linux-gnu 的 glibc 头依赖问题
tool_prefix=${MINIRV_TOOL_PREFIX:-riscv64-unknown-elf}
if ! command -v ${tool_prefix}-$cc > /dev/null 2>&1; then
  tool_prefix=riscv64-linux-gnu
fi

dst_S=${dst%.o}.S
if [[ "$src" == *.S ]]; then
  cp $src $dst_S
else
  ${tool_prefix}-$cc $flags -S -o $dst_S $src
fi

# replace pseudo instructions for load/store
sp="[[:space:]]*"
sp_require="[[:space:]]+"
reg="[[:alnum:]]+"
comma="$sp,$sp"
symbol="[[:alnum:]\._]+"
sed -E -i -e "s/(l[bhw]u?)${sp_require}(${reg})${comma}(${symbol})(${sp}[-+]${sp}${symbol})?${sp}\$/la \2, \3\4; \1 \2, 0(\2);/" \
          -e "s/(s[bhw])${sp_require}(${reg})${comma}(${symbol})(${sp}[-+]${sp}${symbol})?${comma}(${reg})${sp}\$/la \5, \3\4; \1 \2, 0(\5);/" $dst_S

# insert inst-replace.h to each .h files
minirv_path=$AM_HOME/tools/minirv
lut_bin_path=$minirv_path/lut.bin
sed -i "1i#include \"$minirv_path/inst-replace.h\"" $dst_S
flock "$minirv_path/.lock" bash -lc "test -e \"$lut_bin_path\" || (cd \"$minirv_path\" && gcc gen-lut.c && ./a.out && rm a.out)"

src_dir=`dirname $src`
${tool_prefix}-gcc -I$src_dir $flags -D_LUT_BIN_PATH=\"$lut_bin_path\" -Wno-trigraphs -c -o $dst $dst_S

# set a non-standard extension flag in e_flags to indicate minirv
/bin/echo -ne '\x80' | dd of=$dst bs=1 seek=39 count=1 conv=notrunc 2> /dev/null
