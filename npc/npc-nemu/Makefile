## ============================================================
## npc/npc-nemu/Makefile
## 这份 Makefile 像“舞台总导演”：
## - 左手调度 C/C++ 编译（npc-nemu 本体 + bridge）
## - 右手调用上层 npc 的 Verilator 产物（Vtop）
## - 最后把所有演员（.o/.a）链接成可执行文件 npc-nemu
## ============================================================

## C 编译器（负责 .c）
CC  := gcc
## C++ 编译器（负责 .cpp 以及最终链接）
CXX := g++

## 工程根目录（当前目录的上一级，即 npc/）
NPC_ROOT      := ..
## 当前子工程根目录（就是 npc/npc-nemu/ 本身）
NPC_NEMU_ROOT := .
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
LDFLAGS  := -lreadline

## 统一头文件搜索路径（像给编译器一张“地图”）：
## 1) npc-nemu 自己的 include
## 2) 子模块目录 include/cpu、include/memory、include/readline
## 3) 上层 npc 的 csrc（bridge/dpic 头）
## 4) Verilator 生成头目录 obj_dir
## 5) Verilator 官方运行时头目录
INCLUDES := \
  -I$(NPC_NEMU_ROOT)/include \
  -I$(NPC_NEMU_ROOT)/include/cpu \
  -I$(NPC_NEMU_ROOT)/include/memory \
  -I$(NPC_NEMU_ROOT)/include/readline \
  -I$(NPC_ROOT)/csrc \
  -I$(OBJ_DIR) \
  -I$(VERILATOR_ROOT)/include \
  -I$(VERILATOR_ROOT)/include/vltstd

## ------------------------------------------------------------
## 源文件清单（逻辑分组，便于维护）
## ------------------------------------------------------------

## npc-nemu 的 C 源（监控器、执行、工具等）
NPC_NEMU_C_SRCS := \
  $(NPC_NEMU_ROOT)/src/npc-nemu-main.c \
  $(NPC_NEMU_ROOT)/src/cpu/cpu-exec.c \
  $(NPC_NEMU_ROOT)/src/engine/interpreter/init.c \
  $(NPC_NEMU_ROOT)/src/isa/npc_isa.c \
  $(NPC_NEMU_ROOT)/src/monitor/monitor.c \
  $(NPC_NEMU_ROOT)/src/monitor/sdb/sdb.c \
  $(NPC_NEMU_ROOT)/src/utils/log.c \
  $(NPC_NEMU_ROOT)/src/utils/state.c \
  $(NPC_NEMU_ROOT)/src/utils/timer.c

## 上层 npc 的 C++ 源（DPI 接口 + 仿真桥）
NPC_CPP_SRCS := \
  $(NPC_ROOT)/csrc/dpic.cpp \
  $(NPC_ROOT)/csrc/sim_bridge.cpp

## 对应的 C 目标文件输出路径
NPC_NEMU_C_OBJS := \
  $(BUILD_DIR)/npc-nemu-main.o \
  $(BUILD_DIR)/cpu-exec.o \
  $(BUILD_DIR)/init.o \
  $(BUILD_DIR)/npc_isa.o \
  $(BUILD_DIR)/monitor.o \
  $(BUILD_DIR)/sdb.o \
  $(BUILD_DIR)/log.o \
  $(BUILD_DIR)/state.o \
  $(BUILD_DIR)/timer.o

## 对应的 C++ 目标文件输出路径
NPC_CPP_OBJS := \
  $(BUILD_DIR)/dpic.o \
  $(BUILD_DIR)/sim_bridge.o

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

## 关键依赖：
## 每次都先触发一次上层 npc 的 build，确保 Vtop__ALL.a 是最新 RTL 产物。
## 这样改了 vsrc/*.v 后，npc-nemu 不会“误吃旧库”。
$(OBJ_DIR)/Vtop__ALL.a: FORCE
	$(MAKE) -C $(NPC_ROOT) build

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

$(BUILD_DIR)/init.o: $(NPC_NEMU_ROOT)/src/engine/interpreter/init.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/npc_isa.o: $(NPC_NEMU_ROOT)/src/isa/npc_isa.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/monitor.o: $(NPC_NEMU_ROOT)/src/monitor/monitor.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/sdb.o: $(NPC_NEMU_ROOT)/src/monitor/sdb/sdb.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/log.o: $(NPC_NEMU_ROOT)/src/utils/log.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/state.o: $(NPC_NEMU_ROOT)/src/utils/state.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/timer.o: $(NPC_NEMU_ROOT)/src/utils/timer.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/dpic.o: $(NPC_ROOT)/csrc/dpic.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/sim_bridge.o: $(NPC_ROOT)/csrc/sim_bridge.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

## 最终链接：
## 把 npc-nemu 自己的对象 + bridge 对象 + Verilator 模型/运行时 一次性拼成可执行文件
$(BIN): $(OBJ_DIR)/Vtop__ALL.a $(NPC_NEMU_C_OBJS) $(NPC_CPP_OBJS)
	$(CXX) -o $@ \
		$(NPC_NEMU_C_OBJS) \
		$(NPC_CPP_OBJS) \
		$(VERILATOR_OBJS) \
		$(LDFLAGS)

## 运行目标：先确保编译好，再执行
run: $(BIN)
	./$(BIN)

## 清理目标：仅清理 npc-nemu 的 build 目录
clean:
	rm -rf $(BUILD_DIR)

## 伪目标声明：这些名字不是实际文件
.PHONY: all run clean FORCE
