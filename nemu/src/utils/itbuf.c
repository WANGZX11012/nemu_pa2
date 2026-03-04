#include "itbuf.h"
#include <stdio.h>
#include <string.h>    // 为 strncpy
#include "debug.h"     // 为 Log 宏

#define IT_LEN 1024
#define IT_LOGSZ 256

typedef struct 
{
    vaddr_t pc;
    char logstr[IT_LOGSZ];
} itbuf;

static itbuf my_itbuf[IT_LEN];  

static int it_pos = 0;
static int it_cnt = 0; // 当前已填条目数

void itbuf_p(vaddr_t pc, const char* logbuf)
{
    if(logbuf == NULL) return;
  
  
    my_itbuf[it_pos].pc = pc;
    strncpy(my_itbuf[it_pos].logstr, logbuf, IT_LOGSZ -1);
    my_itbuf[it_pos].logstr[IT_LOGSZ - 1] = '\0';
    
    it_pos = (it_pos + 1) % IT_LEN;
    if(it_cnt < IT_LEN) it_cnt ++; //达到最大值后不再增加

}

void itbuf_d(int n)
{
    if(it_cnt == 0)
    {
        printf("empty itbuf\n");
            return;  
    }
        
    if( n <= 0)
    {
        printf("enter a positive num\n");
            return;
    }

    if( n > it_cnt) n = it_cnt;
    //上面部分处理 n
    
    int total = it_cnt < (2*n+1) ? it_cnt : (2*n+1);
    int center = (it_pos - 1 + IT_LEN) % IT_LEN;
    int half = total / 2;
    int start = (center - half + IT_LEN) % IT_LEN;
    for (int k = 0; k < total; k++) 
    {
        int idx = (start + k) % IT_LEN;
        if (k == half) printf("=> ");// 标记中心行
        printf("IT[%03d] PC=" FMT_WORD " %s\n", idx, my_itbuf[idx].pc, my_itbuf[idx].logstr);
        Log("IT[%03d] PC=" FMT_WORD "  %s", idx, my_itbuf[idx].pc, my_itbuf[idx].logstr);
    }

}