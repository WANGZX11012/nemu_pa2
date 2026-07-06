## C 编译器（负责 .c）
CC  := gcc
## C++ 编译器（负责 .cpp 以及最终链接）
CXX := g++

## 工程根目录（当前目录的上一级，即 npc/）
NPC_ROOT      := ..
## 当前子工程根目录（就是 npc/npc-nemu/ 本身）
NPC_NEMU_ROOT := .
## 绝对路径版 npc-nemu 根目录
NPC_NEMU_HOME  := $(abspath $(NPC_NEMU_ROOT))
## 复用同级 nemu 工程中的 kconfig 工具链
NEMU_HOME      := $(abspath $(NPC_ROOT)/../nemu)
## 本工程中间产物目录（放 .o）
BUILD_DIR     := build
## 上层 Verilator 产物目录（放 Vtop__ALL.a 等）
OBJ_DIR       := $(NPC_ROOT)/obj_dir
## 最终可执行文件路径
BIN           := $(BUILD_DIR)/npc-nemu

## 自动探测 Verilator 安装根目录：
## 等价于执行 `verilator -V`，从输出中抽取 VERILATOR_ROOT=... 这一行
VERILATOR_ROOT := $(strip $(shell verilator -V | sed -n 's/VERILATOR_ROOT *= *//p' | head -n 1))

## C 编译选项：
## -O2 适度优化，-g 保留调试信息，-Wall/-Wextra 打开常见告警
CFLAGS   := -O2 -g -Wall -Wextra
## C++ 编译选项同上
CXXFLAGS := -O2 -g -Wall -Wextra
## 链接选项：链接 readline（给 sdb 命令行编辑/历史能力）
LDFLAGS  := -lreadline -ldl

## 先尝试读取菜单配置生成的结果；如果还没跑 menuconfig，则保持默认值
-include $(NPC_NEMU_HOME)/include/config/auto.conf
-include $(NPC_NEMU_HOME)/include/config/auto.conf.cmd
# ── 配置变化自动检测：auto.conf 变了就删 .o 强制重编 ──
AUTO_CONF    := $(NPC_NEMU_HOME)/include/config/auto.conf
CONFIG_STAMP := $(BUILD_DIR)/.config.stamp

$(CONFIG_STAMP): $(AUTO_CONF)
	@echo "[npc-nemu] Config changed, force recompile..."
	@touch $@



## VGA：当 CONFIG_HAS_VGA=y 时启用 SDL2（传宏给 C++ + 链接 SDL2）
ifeq ($(CONFIG_HAS_VGA),y)
CXXFLAGS += -DCONFIG_HAS_VGA
LDFLAGS  += -lSDL2
endif

## ITRACE_COND：从 Kconfig 字符串转成 C 编译选项
remove_quote = $(patsubst "%",%,$(patsubst "%",%,$1))
CFLAGS_TRACE += -DITRACE_COND=$(if $(CONFIG_ITRACE_COND),$(call remove_quote,$(CONFIG_ITRACE_COND)),true)
CFLAGS += $(CFLAGS_TRACE)

## 统一头文件搜索路径（像给编译器一张“地图”）：
## 1) npc-nemu 自己的 include
## 2) 子模块目录 include/cpu、include/memory、include/readline
## 3) 上层 npc 的 csrc（bridge/dpic 头）
## 4) Verilator 生成头目录 obj_dir
## 5) Verilator 官方运行时头目录
INCLUDES := \
  -I$(NPC_NEMU_ROOT)/include \
  -I$(NPC_NEMU_ROOT)/include/cpu \
  -I$(NPC_NEMU_ROOT)/include/readline \
  -I$(NPC_ROOT)/csrc \
  -I$(OBJ_DIR) \
  -I$(VERILATOR_ROOT)/include \
  -I$(VERILATOR_ROOT)/include/vltstd \
  -I$(NEMU_HOME)/tools/capstone/repo/include

## menuconfig 配套规则
include $(NPC_NEMU_HOME)/scripts/config.mk

## ------------------------------------------------------------
## 源文件清单（逻辑分组，便于维护）
## ------------------------------------------------------------

## npc-nemu 的 C 源（监控器、执行、工具等）
NPC_NEMU_C_SRCS := \
  $(NPC_NEMU_ROOT)/src/npc-nemu-main.c \
  $(NPC_NEMU_ROOT)/src/cpu/cpu-exec.c \
  $(NPC_NEMU_ROOT)/src/cpu/difftest/dut.c \
  $(NPC_NEMU_ROOT)/src/engine/interpreter/init.c \
  $(NPC_NEMU_ROOT)/src/isa/npc_reg.c \
  $(NPC_NEMU_ROOT)/src/monitor/monitor.c \
  $(NPC_NEMU_ROOT)/src/monitor/sdb/sdb.c \
  $(NPC_NEMU_ROOT)/src/monitor/sdb/expr.c \
  $(NPC_NEMU_ROOT)/src/monitor/sdb/watchpoint.c \
  $(NPC_NEMU_ROOT)/src/utils/log.c \
  $(NPC_NEMU_ROOT)/src/utils/itbuf.c \
  $(NPC_NEMU_ROOT)/src/utils/disasm.c \
  $(NPC_NEMU_ROOT)/src/utils/state.c \
  $(NPC_NEMU_ROOT)/src/utils/timer.c

## 上层 npc 的 C++ 源（DPI 接口 + 仿真桥）
NPC_CPP_SRCS := \
  $(NPC_ROOT)/csrc/dpic.cpp \
  $(NPC_ROOT)/csrc/sim_bridge.cpp \
  $(NPC_ROOT)/csrc/npc_device.cpp

## 对应的 C 目标文件输出路径
NPC_NEMU_C_OBJS := \
  $(BUILD_DIR)/npc-nemu-main.o \
  $(BUILD_DIR)/cpu-exec.o \
  $(BUILD_DIR)/difftest.o \
  $(BUILD_DIR)/init.o \
  $(BUILD_DIR)/npc_reg.o \
  $(BUILD_DIR)/monitor.o \
  $(BUILD_DIR)/sdb.o \
  $(BUILD_DIR)/expr.o \
  $(BUILD_DIR)/watchpoint.o \
  $(BUILD_DIR)/log.o \
  $(BUILD_DIR)/itbuf.o \
  $(BUILD_DIR)/disasm.o \
  $(BUILD_DIR)/state.o \
  $(BUILD_DIR)/timer.o

## 对应的 C++ 目标文件输出路径
NPC_CPP_OBJS := \
  $(BUILD_DIR)/dpic.o \
  $(BUILD_DIR)/sim_bridge.o \
  $(BUILD_DIR)/npc_device.o

# auto.conf 变化 → CONFIG_STAMP 更新 → 所有 .o 视为过期 → 自动重编
$(NPC_NEMU_C_OBJS) $(NPC_CPP_OBJS): $(CONFIG_STAMP)

## 链接时需要的 Verilator 侧对象：
## - Vtop__ALL.a：你的 RTL 转出来的核心模型库
## - verilated*.cpp：Verilator 运行时支持（时间轮、trace、线程等）
VERILATOR_OBJS := \
  $(OBJ_DIR)/Vtop__ALL.a \
  $(VERILATOR_ROOT)/include/verilated.cpp \
  $(VERILATOR_ROOT)/include/verilated_vcd_c.cpp \
  $(VERILATOR_ROOT)/include/verilated_threads.cpp

## 默认目标：直接构建最终二进制
all: $(BIN)

## 如果 build/ 不存在就创建（mkdir -p 可重复执行）
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# 仅缺失时兜底构建，不再每次触发父目录
$(OBJ_DIR)/Vtop__ALL.a:
	@echo "[npc-nemu] Vtop__ALL.a missing, building Verilator..."
	$(MAKE) -C $(NPC_ROOT) build-verilog

## 空目标 FORCE：用来强制让依赖规则每次都执行
FORCE:

## ------------------------------------------------------------
## 分文件编译规则（把每个 .c/.cpp 编成 build/*.o）
## 说明：
## - $< 代表第一个依赖（源文件）
## - $@ 代表目标文件（输出 .o）
## - `| $(BUILD_DIR)` 是 order-only 依赖：只保证目录先存在，不参与重编译判定
## ------------------------------------------------------------

$(BUILD_DIR)/npc-nemu-main.o: $(NPC_NEMU_ROOT)/src/npc-nemu-main.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/cpu-exec.o: $(NPC_NEMU_ROOT)/src/cpu/cpu-exec.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/difftest.o: $(NPC_NEMU_ROOT)/src/cpu/difftest/dut.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/init.o: $(NPC_NEMU_ROOT)/src/engine/interpreter/init.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/npc_reg.o: $(NPC_NEMU_ROOT)/src/isa/npc_reg.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/monitor.o: $(NPC_NEMU_ROOT)/src/monitor/monitor.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/sdb.o: $(NPC_NEMU_ROOT)/src/monitor/sdb/sdb.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/expr.o: $(NPC_NEMU_ROOT)/src/monitor/sdb/expr.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/watchpoint.o: $(NPC_NEMU_ROOT)/src/monitor/sdb/watchpoint.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/log.o: $(NPC_NEMU_ROOT)/src/utils/log.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/itbuf.o: $(NPC_NEMU_ROOT)/src/utils/itbuf.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/disasm.o: $(NPC_NEMU_ROOT)/src/utils/disasm.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/state.o: $(NPC_NEMU_ROOT)/src/utils/state.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/timer.o: $(NPC_NEMU_ROOT)/src/utils/timer.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/dpic.o: $(NPC_ROOT)/csrc/dpic.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/sim_bridge.o: $(NPC_ROOT)/csrc/sim_bridge.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/npc_device.o: $(NPC_ROOT)/csrc/npc_device.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

## 最终链接：
## 把 npc-nemu 自己的对象 + bridge 对象 + Verilator 模型/运行时 一次性拼成可执行文件
$(BIN): $(OBJ_DIR)/Vtop__ALL.a $(NPC_NEMU_C_OBJS) $(NPC_CPP_OBJS) $(CONFIG_STAMP)
	$(CXX) -o $@ \
		$(NPC_NEMU_C_OBJS) \
		$(NPC_CPP_OBJS) \
		$(VERILATOR_OBJS) \
		$(LDFLAGS)

## 运行目标：先确保编译好，再执行
NEMU_SO := $(abspath $(NEMU_HOME)/build/riscv32-nemu-interpreter-so)
run: $(BIN)
ifeq ($(CONFIG_DIFFTEST),y)
	@if [ ! -f "$(NEMU_SO)" ]; then \
		echo "Error: NEMU .so not found at $(NEMU_SO)"; \
		echo "Build NEMU as shared object first: cd $(NEMU_HOME) && make menuconfig (Shared object) && make"; \
		exit 1; \
	fi
	LD_LIBRARY_PATH=$(NEMU_HOME)/tools/capstone/repo:$$LD_LIBRARY_PATH \
	./$(BIN) -d $(NEMU_SO) $(IMG)
else
	LD_LIBRARY_PATH=$(NEMU_HOME)/tools/capstone/repo:$$LD_LIBRARY_PATH \
	./$(BIN) $(IMG)
endif

## 清理目标：仅清理 npc-nemu 的 build 目录
clean:
	rm -rf $(BUILD_DIR)

## 伪目标声明：这些名字不是实际文件
.PHONY: all run clean FORCE
