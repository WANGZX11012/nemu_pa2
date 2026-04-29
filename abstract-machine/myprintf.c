#include <stdarg.h>
// #include "./am/include/am.h"

#include <stdio.h>
static inline void putch(char ch) { putchar((int)ch); }

int myprintf(const char *fmt, ...)
{
    va_list ap; //声明可变参数列表
    va_start(ap, fmt); //让列表指向第一个可变参数
    
    const char *p = fmt; //用p获取输入的字符串

    int count = 0; //输出字符数量 返回值


    
    while(*p != '\0') //
    {
        switch (*p)
        {
        case ' ':  case '\r': case '\n':    //空格 回车 换行
            putch(*p); count++; p++;
            break;
        case '\t':
            putch(*p); count++; p++; //？ 
            break;
        
        case '%':
            {
                p++;

                switch (*p)
                {
                case '%':
                    putch(*p);
                    count++;
                    p++;
                    break;

                case 'd': 
                {
                    int arg = va_arg(ap, int);
                    long long v = arg;          /* 用更宽的类型避免 INT_MIN 取负溢出 */
                    if (v < 0) 
                    {
                        putch('-'); count++;
                        v = -v;
                    }
                    if (v == 0) 
                    {
                        putch('0'); count++;
                    } 
                    else 
                    {
                        char buf[20];
                        int i = 0;
                        while (v) 
                        {
                            buf[i++] = '0' + (v % 10);
                            v /= 10;
                        }           
                        while (i--) 
                        {
                            putch(buf[i]);
                            count++;
                        }
                    }  

                p++; /* 消费掉格式字符 'd' */
                    break;

                } //case d
                
                case 's':
                {
                    
                    const char *argstr = va_arg(ap, const char*);
                    if (argstr == NULL) argstr = "(null)";
                    while (*argstr)
                    {
                        putch(*argstr);
                        argstr++;
                        count ++;
                    }
                    p++;
                    break;
                }
                   

                default:
                    break;

                }
            
            break;

            }
        default:  //正常的单个字符
            putch(*p);
            p++;
            count++;
            break;
        

        }

    }// 最大while end

    va_end(ap);
    return count;

}

int main()
{
    char *a = "hello";
    myprintf("nihao %s, %d\n",a,123);

    return 0;
}