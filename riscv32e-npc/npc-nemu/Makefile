CC  := gcc
CXX := g++

NPC_ROOT      := ..
NPC_NEMU_HOME := $(abspath .)
NEMU_HOME     := $(abspath $(NPC_ROOT)/../nemu)
BUILD_DIR     := build
OBJ_DIR       := $(NPC_ROOT)/obj_dir
BIN           := $(BUILD_DIR)/npc-nemu

VERILATOR_ROOT := $(strip $(shell verilator -V | sed -n 's/VERILATOR_ROOT *= *//p' | head -n 1))

CFLAGS   := -O2 -g -Wall -Wextra
CXXFLAGS := -O2 -g -Wall -Wextra
LDFLAGS  := -lreadline -ldl

-include $(NPC_NEMU_HOME)/include/config/auto.conf
-include $(NPC_NEMU_HOME)/include/config/auto.conf.cmd
AUTO_CONF    := $(NPC_NEMU_HOME)/include/config/auto.conf
CONFIG_STAMP := $(BUILD_DIR)/.config.stamp

$(CONFIG_STAMP): $(AUTO_CONF) | $(BUILD_DIR)
	@echo "[npc-nemu] Config changed, force recompile..."
	@touch $@

$(BUILD_DIR):
	mkdir -p $@

# ── Kconfig 条件编译 ──
ifeq ($(CONFIG_HAS_SERIAL),y)
CFLAGS += -DCONFIG_HAS_SERIAL
endif
ifeq ($(CONFIG_HAS_TIMER),y)
CFLAGS += -DCONFIG_HAS_TIMER
endif
ifeq ($(CONFIG_HAS_VGA),y)
CXXFLAGS += -DCONFIG_HAS_VGA
LDFLAGS  += -lSDL2
endif
ifeq ($(CONFIG_HAS_KEYBOARD),y)
CXXFLAGS += -DCONFIG_HAS_KEYBOARD
endif

remove_quote = $(patsubst "%",%,$(patsubst "%",%,$1))
CFLAGS += -DITRACE_COND=$(if $(CONFIG_ITRACE_COND),$(call remove_quote,$(CONFIG_ITRACE_COND)),true)

INCLUDES := $(addprefix -I, \
  $(NPC_NEMU_HOME)/include            \
  $(NPC_NEMU_HOME)/include/cpu        \
  $(NPC_NEMU_HOME)/include/readline   \
  $(NPC_ROOT)/csrc                    \
  $(OBJ_DIR)                          \
  $(VERILATOR_ROOT)/include           \
  $(VERILATOR_ROOT)/include/vltstd    \
  $(NEMU_HOME)/tools/capstone/repo/include)

include $(NPC_NEMU_HOME)/scripts/config.mk

# ── 源文件 ──
NPC_C_SRCS   := $(shell cd $(NPC_NEMU_HOME) && find src -name "*.c")
# csrc/ 直属 .cpp (排除 main.cpp)，device/ 下面由 Kconfig 控制
NPC_CXX_SRCS := $(filter-out %/main.cpp, \
                  $(shell cd $(NPC_NEMU_HOME) && find $(NPC_ROOT)/csrc -maxdepth 1 -name "*.cpp"))

# device/ 源文件（由 Kconfig 控制）
ifeq ($(CONFIG_HAS_SERIAL),y)
NPC_CXX_SRCS += $(NPC_ROOT)/csrc/device/serial.cpp
endif
ifeq ($(CONFIG_HAS_TIMER),y)
NPC_CXX_SRCS += $(NPC_ROOT)/csrc/device/timer.cpp
endif
ifeq ($(CONFIG_HAS_VGA),y)
NPC_CXX_SRCS += $(NPC_ROOT)/csrc/device/vga.cpp
endif
ifeq ($(CONFIG_HAS_KEYBOARD),y)
NPC_CXX_SRCS += $(NPC_ROOT)/csrc/device/keyboard.cpp
endif

ALL_OBJS := \
  $(patsubst src/%.c, $(BUILD_DIR)/%.o, $(NPC_C_SRCS)) \
  $(patsubst $(NPC_ROOT)/csrc/%.cpp, $(BUILD_DIR)/%.o, $(NPC_CXX_SRCS))

$(ALL_OBJS): $(CONFIG_STAMP)

# ── 两条模式规则 ──
$(BUILD_DIR)/%.o: src/%.c
	@echo + CC $(notdir $<)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.o: $(NPC_ROOT)/csrc/%.cpp
	@echo + CXX $(notdir $<)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# ── 链接 ──
VERILATOR_OBJS := \
  $(OBJ_DIR)/Vtop__ALL.a \
  $(VERILATOR_ROOT)/include/verilated.cpp \
  $(VERILATOR_ROOT)/include/verilated_vcd_c.cpp \
  $(VERILATOR_ROOT)/include/verilated_threads.cpp

all: $(BIN)

$(OBJ_DIR)/Vtop__ALL.a:
	@echo "[npc-nemu] Vtop__ALL.a missing, building Verilator..."
	$(MAKE) -C $(NPC_ROOT) build-verilog

$(BIN): $(OBJ_DIR)/Vtop__ALL.a $(ALL_OBJS)
	$(CXX) -o $@ $(ALL_OBJS) $(VERILATOR_OBJS) $(LDFLAGS)

# ── 运行 ──
NEMU_SO := $(abspath $(NEMU_HOME)/build/riscv32-nemu-interpreter-so)
run: $(BIN)
ifeq ($(CONFIG_DIFFTEST),y)
	@if [ ! -f "$(NEMU_SO)" ]; then \
		echo "Error: NEMU .so not found at $(NEMU_SO)"; exit 1; \
	fi
	LD_LIBRARY_PATH=$(NEMU_HOME)/tools/capstone/repo:$$LD_LIBRARY_PATH \
	./$(BIN) -d $(NEMU_SO) $(IMG)
else
	LD_LIBRARY_PATH=$(NEMU_HOME)/tools/capstone/repo:$$LD_LIBRARY_PATH \
	./$(BIN) $(IMG)
endif

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean
