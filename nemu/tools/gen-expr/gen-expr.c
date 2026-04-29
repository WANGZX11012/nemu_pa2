/***************************************************************************************
* Copyright (c) 2014-2024 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>


static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; 
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

static uint32_t choose(uint32_t n)
{
  return rand() % n + 0;//生成0~n-1的随机数 
}

static void gen_num()//随机整数的生成
{
  char num_str[10];
  if(!(sprintf(num_str, "%d",(rand() % 100 + 1))))//写入num str if也会执行 
    {
      printf("error gen num");
      return;
    }
  else
  strcat(buf, num_str); //追加到buf里面
}

static void gen_hex_num()//生成随机4位十六进制数，格式 0x1111
{
  uint32_t num = rand() % 65536; // 0 到 65535
  char hex_str[10];
  sprintf(hex_str, "0x%04X", num);
  strcat(buf, hex_str);
}



static void gen_char(char c)
{
  char my_c[2] = {c, '\0'}; //一定要\0结尾 不能定义后赋值
  strcat(buf, my_c);
}

static void gen_rand_op()
{
  char *ops[] = {"+", "-", "*", "/", "<=", "==", "!=", "||", "&&"};
  strcat(buf, ops[choose(9)]);
}

static void gen_unary_ops()
{
  char my_unary[2] = {'+', '-'};
  gen_char(my_unary[choose(2)]);
}


static void gen_rand_expr(); 
static void gen_term() //生成数字 16进制 和带括号的子表达式
{
  if (strlen(buf) > 50) //太长就生成数字
  {
    gen_num();
    return;
  }

  switch (choose(3)) 
  {
  case 0:
    gen_num();
    break;
  case 1:
    gen_hex_num();
    break;

  default:
    gen_char('('); gen_rand_expr(); gen_char(')');
    break;
  }
}

static void gen_rand_expr() 
{
  if (strlen(buf) > 50) 
  {
    gen_term();
    return;
  }

  switch (choose(3)) 
  {
  case 0:
    gen_term();
    break;
  case 1:
    gen_unary_ops(); gen_char(' '); gen_term();  // 一元 + 空格 + term
    break;
  default:
    gen_term(); gen_char(' '); gen_rand_op(); gen_char(' '); gen_term();  // term + 空格 + op + 空格 + term
    break;
  }
}

int main(int argc, char *argv[]) 
{
  int seed = time(0);// 用当前时间作为随机数种子，保证每次运行生成不同的随机序列
  srand(seed);       // 初始化 rand() 使用的种子
  int loop = 1;
  if (argc > 1) // 如果命令行带参数，则把第一个参数当作循环次数
  {     
    sscanf(argv[1], "%d", &loop);
  }
 
  int i;
  for (i = 0; i < loop; i ++) 
  {
    
    buf[0] = '\0';//循环生成表达式前清空缓冲区
    gen_rand_expr();
    if(strlen(buf) >= 50000) //超出界限
    {
      printf("buffer overflow\n");
      exit(1);
    }

    sprintf(code_buf, code_format, buf);// 将生成的表达式插入到 C 源代码模板 code_format 中，生成完整的程序源码

    FILE *fp = fopen("/tmp/.code.c", "w");// 打开临时文件用于写入源码
    assert(fp != NULL);                   // 确认文件打开成功（失败则终止）
    fputs(code_buf, fp);                  // 把源码写入临时文件
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");// 调用系统命令编译临时源码为可执行文件
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");  // 运行编译出的程序，并打开管道读取其输出
    assert(fp != NULL);

    int result;
    ret = fscanf(fp, "%d", &result);// 从子进程输出中读取整数结果
    pclose(fp);

    printf("%u %s\n", result, buf);// 打印子程序返回的结果（无符号格式）以及对应的表达式
  }
  return 0;
}
