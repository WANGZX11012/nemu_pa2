#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

// 计算字符串长度；参数 s 是以 '\0' 结尾的字符串起始地址。
size_t strlen(const char *s) 
{
  // panic("Not implemented");
  panic_on(s == NULL,"NULL inputs");
  size_t n = 0;
  while (*s != '\0')
  {
    s++;
    n++;
  }
  return n;
}

// 复制字符串 src 到 dst；参数 dst 是目标缓冲区，src 是源字符串。
char *strcpy(char *dst, const char *src) 
{
  // panic("Not implemented");
  panic_on(dst == NULL || src == NULL, "strcpy: NULL pointer");

  char *ret = dst; //获取目标地址 指向dest
  while (*src != '\0')
  {
    *dst = *src;
    dst++;
    src++;
  }
  
  *dst = '\0'; //添加终止

  return ret;
}

// 最多复制 n 个字符到 dst；参数 dst 是目标缓冲区，src 是源字符串，n 是复制上限。
char *strncpy(char *dst, const char *src, size_t n) //n >= s0
{
  panic_on(dst == NULL || src == NULL, "strncpy: NULL pointer");
  char *ret = dst;
  size_t i = 0;
  for (; i < n && src[i] != '\0'; i++) 
  {
    dst[i] = src[i];
  }
 
  for (; i < n; i++) //如果src 小于dst 填补剩下的区域 
  {
    dst[i] = '\0';
  }
  return ret;
}

// 将 src 追加到 dst 末尾；参数 dst 是目标字符串，src 是待追加字符串。
char *strcat(char *dst, const char *src) //把src 追加到 dst
{
  // panic("Not implemented");
  panic_on(dst == NULL || src == NULL, "strcat: NULL pointer");
  
  size_t len = 0;
  char *ret = dst;

  len = strlen(dst);
  dst += len;
  strcpy(dst, src);
  return ret;
  
}

// 按字典序比较两个字符串；参数 s1 和 s2 是待比较的两个字符串。
int strcmp(const char *s1, const char *s2) 
{
  panic_on(s1 == NULL || s2 == NULL, "strcmp: NULL pointer");
  while (*s1 && *s1 == *s2) 
  {
    s1++; s2++;
  }
  return (int)((unsigned char)*s1) - (int)((unsigned char)*s2); //返回0代表相等
}

// 最多比较前 n 个字符；参数 s1 和 s2 是待比较字符串，n 是比较长度上限。
int strncmp(const char *s1, const char *s2, size_t n) 
{
  panic_on(s1 == NULL || s2 == NULL, "strncmp: NULL pointer");
  size_t i = 0;
  for (; i < n; i++) 
  {
    unsigned char c1 = (unsigned char)s1[i];
    unsigned char c2 = (unsigned char)s2[i];
    if (c1 != c2) return (int)c1 - (int)c2;
    if (c1 == '\0') return 0;
  }
  return 0;
}

// 将 s 开始的 n 个字节填充为字节值 c；参数 s 是起始地址，c 是填充值，n 是字节数。
void *memset(void *s, int c, size_t n) 
{
  // panic("Not implemented");
  unsigned char *p = (unsigned char *)s;
  for(; n > 0; n--)    //正确应该是n>0 我改成n>5 把函数改错进行测试
  {
    *p = (unsigned char) c;
    p++;
  }

  return s;

}

// 处理可能重叠的内存搬运；参数 dst 是目标地址，src 是源地址，n 是搬运字节数。
void *memmove(void *dst, const void *src, size_t n) 
{
  panic_on(dst == NULL || src == NULL, "memmove: NULL pointer");

  char *d = (char *)dst;
  const char *s = (const char *)src;

  if (d > s && d < s + n)  //d的地址落在s的内容之内
  {
    // 重叠且 dst 在 src 之后：从后往前搬运 必须从后往前
    d += n;  // 指向末尾之后
    s += n;
    while (n-- > 0) 
    {
      d--; 
      s--; // 左移到有效字节
      *d = *s;
    }
  } 
  else 
  {
    // 无重叠或 dst 在 src 之前：从前往后搬运
    while (n-- > 0) 
    {
      *d = *s;
      d++; 
      s++;
    }
  }

  return dst;
}

// 复制 n 个字节且不处理重叠；参数 out 是目标地址，in 是源地址，n 是复制字节数。
void *memcpy(void *out, const void *in, size_t n) //不考虑从前或从后往前的顺序
{
  // panic("Not implemented");
  panic_on(out == NULL || in == NULL, "memcpy: NULL pointer");
  char *o = (char *)out;
  const char *i = (const char *)in;

  while (n-- > 0) 
    {
      *o = *i;
      o++; 
      i++;
    }

  return out;

}

// 比较两段内存的前 n 个字节；参数 s1 和 s2 是待比较地址，n 是比较字节数。
int memcmp(const void *s1, const void *s2, size_t n) 
{
  panic_on(s1 == NULL || s2 == NULL, "memcmp: NULL pointer");
  const unsigned char *p1 = (const unsigned char *)s1;
  const unsigned char *p2 = (const unsigned char *)s2;

  while (n-- > 0) 
  {
    if (*p1 != *p2) 
    {
      return (int)(*p1) - (int)(*p2);
    }
    p1++;
    p2++;
  }
  return 0;
}

#endif
