.PHONY: all run gdb clean latest $(ALL)

RESULT = .result
$(shell > $(RESULT))

COLOR_RED   = \033[1;31m
COLOR_GREEN = \033[1;32m
COLOR_NONE  = \033[0m

ALL = $(basename $(notdir $(shell find tests/. -name "*.c")))


ARCH ?= riscv32-nemu
NEMUFLAGS += -b

# NPC_HOME path for minirv-npc run target
NPC_HOME ?= $(shell [ -d "$(AM_HOME)/../npc" ] && echo "$(AM_HOME)/../npc" || echo "$(AM_HOME)/../../npc")


all: $(addprefix Makefile., $(ALL))
	@echo "test list [$(words $(ALL)) item(s)]:" $(ALL)

$(ALL): %: Makefile.%

Makefile.%: tests/%.c latest
	@/bin/echo -e "NAME = $*\nSRCS = $<\nCFLAGS = $(CFLAGS)\nexport CFLAGS\ninclude $${AM_HOME}/Makefile" > $@
	@if make -s -f $@ ARCH=$(ARCH) $(MAKECMDGOALS); then \
		printf "[%14s] $(COLOR_GREEN)PASS$(COLOR_NONE)\n" $* >> $(RESULT); \
	else \
		if [ "$(ARCH)" = "minirv-npc" ]; then \
			echo "[WARN] minirv-npc build may fail due to missing 32-bit riscv ABI libraries." >&2; \
			echo "[WARN] To fix, install: sudo apt-get install libc6-dev-riscv64-cross:i386 (if available)" >&2; \
			printf "[%14s] $(COLOR_RED)***FAIL***$(COLOR_NONE)\n" $* >> $(RESULT); \
		else \
			printf "[%14s] $(COLOR_RED)***FAIL***$(COLOR_NONE)\n" $* >> $(RESULT); \
		fi; \
	fi
	-@rm -f Makefile.$*

##让 cpu-tests 的 run 流程能够接 NPC，而不是只停留在“构建”。
run: all
	@if [ "$(ARCH)" = "minirv-npc" ]; then \
		echo ""; \
		echo "========== Running on NPC Simulator =========="; \
		ELF_COUNT=0; \
		for elf in build/*-minirv-npc.elf; do \
			if [ -f "$$elf" ]; then \
				ELF_COUNT=$$((ELF_COUNT + 1)); \
				echo "[NPC] Running: $$elf"; \
				if [ -x "$(NPC_HOME)/run-npc.sh" ]; then \
					$(NPC_HOME)/run-npc.sh "$$elf" 2>&1 || echo "[NPC] Execution completed with exit code $$?"; \
				else \
					echo "[NPC] Error: NPC simulator script not found at $(NPC_HOME)/run-npc.sh"; \
					echo "[NPC] Please ensure NPC is built and run-npc.sh exists"; \
				fi; \
				echo ""; \
			fi; \
		done; \
		if [ $$ELF_COUNT -eq 0 ]; then \
			echo "[NPC] Warning: No minirv-npc ELF files found in build/"; \
			echo "[NPC] This may be due to compilation errors. Check the output above."; \
		fi; \
	else \
		echo "[INFO] Skipping NPC run for ARCH=$(ARCH)"; \
	fi
	@cat $(RESULT)
	@rm $(RESULT)

gdb: all

clean:
	rm -rf Makefile.* build/

latest:
