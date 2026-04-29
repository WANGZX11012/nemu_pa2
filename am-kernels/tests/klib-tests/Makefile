CC = gcc
CFLAGS = -std=c11 -O2 -Wall -I$(KLIB_INC)\
-I../../../abstract-machine/am/include -DARCH_H=\"arch/native.h\" -D__NATIVE_USE_KLIB__
CFLAGS += -Wno-nonnull-compare
CFLAGS += -fno-builtin-memset				#防止编译器把 memset 当作内建函数处理
CFLAGS += -fno-tree-loop-distribute-patterns   #-O2的优化也会让memset调用memset导致栈溢出
CFLAGS += -D_FORTIFY_SOURCE=0					#防止测试代码里 memset(...) 被替换成 __memset_chk(...)
CFLAGS += -fno-builtin-strlen
CFLAGS += -fno-builtin-strcpy
CFLAGS += -fno-builtin-strncpy
CFLAGS += -fno-builtin-strcat
CFLAGS += -fno-builtin-strcmp

KLIB_INC = ../../../abstract-machine/klib/include
KLIB_SRC = ../../../abstract-machine/klib/src/string.c
KLIB_OBJ = string.o

STUB_SRC = am_stub.c
STUB_OBJ = am_stub.o

TEST_SRC = tests/testall.c
TEST_OBJ = testall.o
TARGET = testall

.PHONY: all run clean

all: $(TARGET) #构建目标

run: all  #依赖all构建的目标
	@./$(TARGET)&& echo "PASS" || echo "FAIL"

$(KLIB_OBJ): $(KLIB_SRC) #要生成的obj依赖于src   $<代表前置依赖也就是src $@代表klib obj
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_OBJ): $(TEST_SRC)
	$(CC) $(CFLAGS) -c $< -o $@   

$(STUB_OBJ): $(STUB_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

#禁止使用内建lib

$(TARGET): $(TEST_OBJ) $(KLIB_OBJ) $(STUB_OBJ) #$^ 所有前置依赖
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(TARGET) *.o