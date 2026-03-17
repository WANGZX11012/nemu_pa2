TOPNAME = top
VERILATOR = verilator

VSRCS = $(wildcard vsrc/*.v)  #把 vsrc/ 目录下所有 .v 文件都收集到 VSRCS 变量里。
CSRCS = $(wildcard csrc/*.cpp)

OBJ_DIR = obj_dir
BIN = $(OBJ_DIR)/V$(TOPNAME)
WAVE = $(OBJ_DIR)/wave.vcd

VERILATOR_FLAGS = --cc --exe --build --trace -Wall -Wno-fatal --top-module $(TOPNAME)

all:run

build:
	$(VERILATOR) $(VERILATOR_FLAGS) $(VSRCS) $(CSRCS)

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

.PHONY: all build run sim clean

include ../Makefile
