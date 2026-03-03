#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) 
{
  panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) 
{
  // panic("Not implemented");
  va_list ap;
  va_start(ap, fmt);

  char *o = out;
  const char *p = fmt;

  while (*p != '\0')
  {
    if(*p != '%')//一般字符
    {
      *o = *p;
      o++; p++;
      continue;
    }

    p ++; //跳过%
    if (*p == '%') { *o++ = '%'; p++; continue; } //单独的%

    if(*p == 'd')
    {
      int argint = va_arg(ap, int);
      long long v = argint;
      if(v < 0)
      {
        *o = '-';
        o++;
        v = -v;
      }
      if(v == 0)
      {
        *o = '0';
        o++;
      }
      else
      {
        char buf[32];
        int i = 0;
        while(v)
        {
          buf[i] = '0' + (v % 10);
          i++;
          v = v/10;
        }
        while(i--)
        {
          *o = buf[i];
          o++;
        }

      }

      p++;

    }

    else if (*p == 's')
    {
      const char *s = va_arg(ap, const char *);
      if (!s) s = "(null)";
      while (*s) *o++ = *s++;
      p++;
    }
    else
    {
      /* 未实现的格式：按字面输出 '%' 和该字符（若有） */
      *o++ = '%';
      if (*p) *o++ = *p++;
    }

  }
  
  *o = '\0';
  va_end(ap);
  return (int)(o - out);

}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
