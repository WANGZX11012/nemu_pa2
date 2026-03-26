#include<stdint.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>


#define N 32

uint8_t data[N];

void reset()
{
    int i;
    for(i = 0; i < N; i++)
    {
        data[i] = i + 1;
    }
}

// 检查[l,r)区间中的值是否依次为val, val + 1, val + 2...
void check_seq(int l, int r, int val) 
{
  int i;
  for (i = l; i < r; i ++) 
  {
    assert(data[i] == val + i - l);
  }
}

// 检查[l,r)区间中的值是否均为val
void check_eq(int l, int r, int val) 
{
  int i;
  for (i = l; i < r; i ++) 
  {
    assert(data[i] == val);
  }
}

void test_memset() 
{
  int l, r;
  for (l = 0; l < N; l ++) 
  {
    for (r = l + 1; r <= N; r ++) 
    {
      reset();
      uint8_t val = (l + r) / 2;
      memset(data + l, val, r - l);
      check_seq(0, l, 1);
      check_eq(l, r, val);
      check_seq(r, N, r + 1);
    }
  }
}

void test_strlen()
{
  assert(strlen("") == 0);
  assert(strlen("a") == 1);
  assert(strlen("abc") == 3);
  assert(strlen("hello world") == 11);
  assert(strlen("a!b?") == 4);

  char s1[] = {'a', 'b', '\0', 'c', '\0'};
  assert(strlen(s1) == 2);
}

void test_str()
{
  // ---------- strcpy ----------
  {
    char buf[20];
    char *ret = strcpy(buf, "hello123");
    assert(ret == buf);
    assert(strcmp(buf, "hello123") == 0);
    assert(buf[8] == '\0');
  }

  // ---------- strncpy ----------
  {
    char buf[20];

    // 情况1：src长度 < n，后面应该补 '\0'
    memset(buf, 'x', sizeof(buf));
    char *ret = strncpy(buf, "abc", 6);
    assert(ret == buf);
    assert(buf[0] == 'a');
    assert(buf[1] == 'b');
    assert(buf[2] == 'c');
    assert(buf[3] == '\0');
    assert(buf[4] == '\0');
    assert(buf[5] == '\0');

    // 情况2：src长度 >= n，不会自动补额外 '\0'
    memset(buf, 'x', sizeof(buf));
    ret = strncpy(buf, "hello123", 5);
    assert(ret == buf);
    assert(buf[0] == 'h');
    assert(buf[1] == 'e');
    assert(buf[2] == 'l');
    assert(buf[3] == 'l');
    assert(buf[4] == 'o');
    assert(buf[5] == 'x');
  }

  // ---------- strcat ----------
  {
    char buf[30];
    char *ret = strcpy(buf, "hello");
    assert(ret == buf);

    ret = strcat(buf, "123");
    assert(ret == buf);
    assert(strcmp(buf, "hello123") == 0);
  }

  // ---------- strcmp ----------
  {
    assert(strcmp("abc", "abc") == 0);
    assert(strcmp("abc", "abd") < 0);
    assert(strcmp("abd", "abc") > 0);
    assert(strcmp("abc", "abcd") < 0);
    assert(strcmp("abcd", "abc") > 0);
    assert(strcmp("", "") == 0);
    assert(strcmp("", "a") < 0);
    assert(strcmp("a", "") > 0);
  }
}

int main()
{
    test_memset();
    test_str();
    test_strlen();

    return 0;
}