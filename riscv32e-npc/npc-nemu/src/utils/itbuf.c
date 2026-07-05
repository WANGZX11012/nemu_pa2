#include "itbuf.h"
#include <stdio.h>
#include <string.h>
#include <npc_utils.h>

#define IT_LEN 1024
#define IT_LOGSZ 256

typedef struct 
{
    vaddr_t pc;
    char logstr[IT_LOGSZ];
} itbuf;

static itbuf my_itbuf[IT_LEN];  

static int it_pos = 0;
static int it_cnt = 0;

void itbuf_p(vaddr_t pc, const char* logbuf)
{
    if(logbuf == NULL) return;

    my_itbuf[it_pos].pc = pc;
    strncpy(my_itbuf[it_pos].logstr, logbuf, IT_LOGSZ -1);
    my_itbuf[it_pos].logstr[IT_LOGSZ - 1] = '\0';

    it_pos = (it_pos + 1) % IT_LEN;
    if(it_cnt < IT_LEN) it_cnt ++;
}

void itbuf_d(int n)
{
    if (it_cnt == 0) {
        printf("empty itbuf\n");
        return;
    }
    if (n <= 0) {
        printf("enter a positive num\n");
        return;
    }

    int max_before = (it_cnt > 0) ? (it_cnt - 1) : 0;
    if (n > max_before) n = max_before;
    if (n == 0) return;

    int start = (it_pos - n + IT_LEN) % IT_LEN;
    for (int i = 0; i < n - 1; i++) {
        int idx = (start + i) % IT_LEN;
        printf("    ");
        printf("IT[%04d] PC= %-30s\n", idx,  my_itbuf[idx].logstr);
    }

    printf(ANSI_FMT(" => ", ANSI_FG_RED));
    printf("IT[%04d] PC= %-30s\n", it_pos-1,  my_itbuf[it_pos - 1].logstr);
}
