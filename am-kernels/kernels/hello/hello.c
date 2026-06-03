#include <am.h>
#include <klib.h>
#include <klib-macros.h>

int main(const char *args) {
  const char *fmt =
    "Hello, AbstractMachine!\n"
    "mainargs = '%'.\n";

  for (const char *p = fmt; *p; p++) 
  {
    (*p == '%') ? putstr(args) : putch(*p);
  }

  printf("x = 0x%x\n", 255);//加了个16进制打印测试
  return 0;
}
