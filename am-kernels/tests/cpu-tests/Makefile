# ============================================================================
#  CPU Test Suite — Makefile
# ============================================================================
#  Usage:
#    make                            默认：增量编译缺失的测试二进制
#    make all                        全量编译所有测试
#    make run ARCH=riscv32-nemu      编译并在 NEMU 上运行所有测试
#    make run ARCH=minirv-npc        编译并在 NPC 上运行所有测试
#    make <testname>                 编译/运行单个测试，如  make add
#    make gdb                        编译所有测试（调试用）
#    make clean                      清理编译产物
# ============================================================================

# ---------------------------------------------------------------------------
#  Test discovery — scan tests/ for all .c files
#  NOTE: must be defined before .PHONY so $(ALL) expands correctly
# ---------------------------------------------------------------------------
ALL      := $(basename $(notdir $(shell find tests/. -name "*.c")))

# ---------------------------------------------------------------------------
#  Phony targets & default goal
# ---------------------------------------------------------------------------
.PHONY: build-missing all run gdb clean latest $(ALL)

.DEFAULT_GOAL := build-missing

# ---------------------------------------------------------------------------
#  Shell configuration
# ---------------------------------------------------------------------------
SHELL          := /bin/bash
.ONESHELL:

# ---------------------------------------------------------------------------
#  Result tracking
# ---------------------------------------------------------------------------
RESULT_FILE    := .result
$(shell > $(RESULT_FILE))

# ---------------------------------------------------------------------------
#  Terminal color helpers
# ---------------------------------------------------------------------------
C_RED          := \033[1;31m
C_GREEN        := \033[1;32m
C_NONE         := \033[0m

# ---------------------------------------------------------------------------
#  Architecture & simulator configuration
# ---------------------------------------------------------------------------
ARCH           ?= riscv32-nemu
NEMUFLAGS      += -b
NPC_ARCHES     := minirv-npc riscv32e-npc

# True (1) when ARCH is one of the NPC architectures
IS_NPC         := $(if $(filter $(ARCH),$(NPC_ARCHES)),1,0)

# NPC-specific settings
NPC_TIMEOUT    ?= 20s
NPC_INTERACTIVE ?= 1

# Auto-detect NPC_HOME (can be overridden on command line)
ifneq ($(origin NPC_HOME), command line)
  NPC_HOME := $(shell \
    [ -d "$(AM_HOME)/../riscv32e-npc" ] \
      && echo "$(AM_HOME)/../riscv32e-npc" \
      || echo "$(AM_HOME)/../../riscv32e-npc")
endif


# ============================================================================
#  Helper: generate a temp Makefile that includes AM's build system,
#  then invoke it with the given target(s).
#
#  The AM Makefile defines these key targets:
#    (none / image)  — compile + link the binary (default)
#    run             — compile + link + launch simulator
#    gdb             — compile + link (gdb mode)
#    clean           — remove build artifacts
# ============================================================================
#  Usage (inside a recipe):
#    $(call am_make,<test-name>,<target>)
#  Example:
#    $(call am_make,add,run)
define am_make
	printf 'NAME = %s\nSRCS = tests/%s.c\nCFLAGS = %s\nexport CFLAGS\ninclude %s/Makefile\n' \
		"$(1)" "$(1)" "$(CFLAGS)" "$(AM_HOME)" > Makefile.$(1) && \
	make -s -f Makefile.$(1) ARCH=$(ARCH) $(2); \
	rc=$$?; rm -f Makefile.$(1); [ $$rc -eq 0 ]
endef


# ============================================================================
#  build-missing  (default target)
#  Incrementally compile only tests whose .bin is absent from build/.
#  This keeps `make` fast during day-to-day development.
# ============================================================================
build-missing:
	@missing=0; \
	for name in $(ALL); do \
		if [ ! -f build/$${name}-$(ARCH).bin ]; then \
			missing=1; \
			echo "[INFO] Building missing $(ARCH): $${name}"; \
			$(call am_make,$${name},); \
			if [ $$? -ne 0 ]; then \
				echo "[ERROR] Failed to build $${name}" >&2; \
				exit 1; \
			fi; \
		fi; \
	done; \
	if [ $$missing -eq 0 ]; then \
		echo "[INFO] All $(ARCH) binaries present, nothing to build"; \
	fi


# ============================================================================
#  all  — build every test (triggers Makefile.% rule for each)
# ============================================================================
all: $(addprefix Makefile., $(ALL))
	@echo "Test list [$(words $(ALL)) item(s)]: $(ALL)"


# ============================================================================
#  <testname>  — build (and optionally run) a single test
#  e.g.  make add          -> just compiles add.c
#        make run ARCH=... -> also runs it (via MAKECMDGOALS=trick)
# ============================================================================
$(ALL): %: Makefile.%


# ============================================================================
#  Makefile.%  — temp makefile recipe for one test
#  Generates a tiny Makefile, invokes AM with $(MAKECMDGOALS), records result.
#
#  IMPORTANT: $(MAKECMDGOALS) is the goal from the *outer* make command line.
#  e.g. `make run` -> inner make receives `run`, so AM builds AND runs NEMU.
#       `make add`  -> inner make receives empty, so AM just builds (image).
#
#  NPC note: inner `run` is a stub on NPC, so we only record FAIL here;
#  the real PASS/FAIL for NPC is determined by the `run` target.
# ============================================================================
Makefile.%: tests/%.c latest
	@$(call am_make,$*,$(MAKECMDGOALS)); \
	if [ $$? -eq 0 ]; then \
		if [ "$(IS_NPC)" = "0" ]; then \
			printf "[%14s] $(C_GREEN)PASS$(C_NONE)\n" $* >> $(RESULT_FILE); \
		fi; \
	else \
		if [ "$(IS_NPC)" = "1" ]; then \
			echo "[WARN] $(ARCH) build may fail — check RISC-V toolchain" >&2; \
		fi; \
		printf "[%14s] $(C_RED)***FAIL***$(C_NONE)\n" $* >> $(RESULT_FILE); \
	fi


# ============================================================================
#  run  — compile + run all tests
#  Two completely different paths depending on simulator:
#    NPC  : pre-built binaries + run-npc.sh
#    NEMU : build-and-run per test via AM's `run` target (which launches NEMU)
# ============================================================================
run:
ifeq ($(IS_NPC),1)
# --------------------------------------------------------------------
#  NPC path
# --------------------------------------------------------------------
	@if ! ls build/*-$(ARCH).bin >/dev/null 2>&1; then \
		echo "[ERROR] No $(ARCH) binaries in build/. Run 'make' first."; \
		exit 1; \
	fi
	@echo ""; \
	echo "========== Running on NPC Simulator ($(ARCH)) =========="; \
	: > $(RESULT_FILE); \
	mkdir -p "$(NPC_HOME)/hex"; \
	bins=0; \
	for bin in build/*-$(ARCH).bin; do \
		[ -f "$$bin" ] || continue; \
		bins=$$((bins + 1)); \
		base=$$(basename "$$bin" .bin); \
		name=$${base%-$(ARCH)}; \
		log="$(NPC_HOME)/hex/$${base}.log"; \
		echo "[NPC] $$name ..."; \
		if [ -x "$(NPC_HOME)/run-npc.sh" ]; then \
			if [ "$(NPC_INTERACTIVE)" = "1" ]; then \
				{ command -v script  >/dev/null 2>&1 && script -q "$$log" -c "$(NPC_HOME)/run-npc.sh \"$$bin\" -i"; } || \
				{ command -v expect  >/dev/null 2>&1 && expect -c "log_file -a $$log; spawn \"$(NPC_HOME)/run-npc.sh\" \"$$bin\" -i; interact"; } || \
				{ $(NPC_HOME)/run-npc.sh "$$bin" -i > "$$log" 2>&1; }; \
				rc=$$?; \
			elif [ "$(NPC_TIMEOUT)" = "0" ]; then \
				$(NPC_HOME)/run-npc.sh "$$bin" -b > "$$log" 2>&1; rc=$$?; \
			else \
				timeout $(NPC_TIMEOUT) $(NPC_HOME)/run-npc.sh "$$bin" -b > "$$log" 2>&1; rc=$$?; \
			fi; \
			clean_log=$$(sed -r 's/\x1B\[[0-9;]*[A-Za-z]//g' "$$log"); \
			if echo "$$clean_log" | grep -qE 'HIT GOOD TRAP|program ended at.*code = 0'; then \
				printf "[%14s] $(C_GREEN)PASS$(C_NONE)\n" "$$name" >> $(RESULT_FILE); \
			elif [ $$rc -eq 124 ]; then \
				printf "[%14s] $(C_RED)TIMEOUT$(C_NONE)\n" "$$name" >> $(RESULT_FILE); \
			else \
				printf "[%14s] $(C_RED)***FAIL***$(C_NONE)\n" "$$name" >> $(RESULT_FILE); \
			fi; \
		else \
			echo "[NPC] Error: run-npc.sh not found at $(NPC_HOME)"; \
			printf "[%14s] $(C_RED)***FAIL***$(C_NONE)\n" "$$name" >> $(RESULT_FILE); \
		fi; \
	done; \
	[ $$bins -gt 0 ] || echo "[NPC] Warning: No $(ARCH) binaries found in build/"
else
# --------------------------------------------------------------------
#  NEMU / other simulators  — build & run each test via AM's run target
# --------------------------------------------------------------------
	@echo ""; \
	echo "========== Running on $(ARCH) =========="; \
	: > $(RESULT_FILE); \
	for name in $(ALL); do \
		echo "[$(ARCH)] $$name"; \
		$(call am_make,$$name,run); \
		if [ $$? -eq 0 ]; then \
			printf "[%14s] $(C_GREEN)PASS$(C_NONE)\n" "$$name" >> $(RESULT_FILE); \
		else \
			printf "[%14s] $(C_RED)***FAIL***$(C_NONE)\n" "$$name" >> $(RESULT_FILE); \
		fi; \
	done
endif
	@cat $(RESULT_FILE)
	@# Exit with error if any test failed
	@grep -q "FAIL" $(RESULT_FILE) && FAIL=1 || FAIL=0; \
	rm $(RESULT_FILE); \
	[ $$FAIL -eq 0 ]


# ============================================================================
#  gdb  — build all tests (debug mode)
# ============================================================================
gdb: all


# ============================================================================
#  clean  — remove all generated files
# ============================================================================
clean:
	rm -rf Makefile.* build/


# ============================================================================
#  latest  — empty phony target; forces Makefile.% to always regenerate
# ============================================================================
latest:
