#ifndef __AMUNIT_H__
#define __AMUNIT_H__

#include <am.h>
#include <klib.h>
#include <klib-macros.h>

#define IOE ({ ioe_init();  })
#define CTE(h) ({ Context *h(Event, Context *); cte_init(h); })//被展开为cte_init(simple_trap);
// /AM 先记住：“以后有异常/中断，就把事件交给 simple_trap 处理”。

#define VME(f1, f2) ({ void *f1(int); void f2(void *); vme_init(f1, f2); })
#define MPE ({ mpe_init(entry); })

extern void (*entry)();

#define CASE(id, entry_, ...) \
  case id: { \
    void entry_(); \
    entry = entry_; \
    __VA_ARGS__; \
    entry_(); \
    break; \
  }

#endif
