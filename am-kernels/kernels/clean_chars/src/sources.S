# .incbin (include Binary)      五步标准法

    .section .rodata            # 只读代码段
    .global vga_load      # 声明一个全局可见的符号
    .global vga_load_end  # 声明结束符号全局可见
    .p2align 2                  # 地址对齐，rgb像素为4字节=32位，故需要4字节对齐:2^2
vga_load:                 # 打上变量符号起始指针标签
    .incbin "sources/success.rgb"   # 对应展开文件二进制数据强制写入.bin文件
vga_load_end:             # 打上变量符号结束指针标签