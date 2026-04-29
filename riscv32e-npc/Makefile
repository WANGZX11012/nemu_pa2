TOPNAME = top
VERILATOR = verilator

VSRCS = $(wildcard vsrc/*.v)  #把 vsrc/ 目录下所有 .v 文件都收集到 VSRCS 变量里。
CSRCS = $(wildcard csrc/*.cpp)

OBJ_DIR = obj_dir
BIN = $(OBJ_DIR)/V$(TOPNAME)
WAVE = $(OBJ_DIR)/wave.vcd
REF_SO = $(OBJ_DIR)/ref.so

VERILATOR_FLAGS = --cc --exe --build --trace -Wall -Wno-fatal --top-module $(TOPNAME)
NPC_NEMU_INC = $(abspath ./npc-nemu/include)
NPC_CSRCS_INC = $(abspath ./csrc)
VERILATOR_FLAGS += -CFLAGS "-I$(NPC_NEMU_INC) -I$(NPC_CSRCS_INC)"

all: build

build: $(REF_SO)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(REF_SO): FORCE | $(OBJ_DIR)
	ln -sf $(abspath ../nemu/build/riscv32-nemu-interpreter-so) $@

build-verilog:
	$(VERILATOR) $(VERILATOR_FLAGS) $(VSRCS) $(CSRCS)

build: build-verilog

run: build
	$(BIN)


#DO NOT REMOVE THIS LINE!!!
sim: build
	$(call git_commit, "sim RTL") 
	@echo "Write this Makefile by your self."
	$(BIN)
	gtkwave $(WAVE)

clean:
	rm -rf $(OBJ_DIR)
	rm ./hex/*.log

FORCE:

.PHONY: all build build-verilog run sim clean FORCE

include ../Makefile
