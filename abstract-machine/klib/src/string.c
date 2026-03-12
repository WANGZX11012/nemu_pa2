#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

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

int strcmp(const char *s1, const char *s2) 
{
  panic_on(s1 == NULL || s2 == NULL, "strcmp: NULL pointer");
  while (*s1 && *s1 == *s2) 
  {
    s1++; s2++;
  }
  return (int)((unsigned char)*s1) - (int)((unsigned char)*s2); //返回0代表相等
}

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
