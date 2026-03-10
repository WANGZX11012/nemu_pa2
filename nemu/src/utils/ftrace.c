#include "ftrace.h"
#include "utils.h"
#include "ftrace.h"
#include <memory/paddr.h>

#include <elf.h>

static int fdepth = 0;

static uint8_t *elf_buf = NULL;
static Elf32_Sym *syms = NULL;
static int nsyms;
static const char *strtab = NULL;
void init_ftrace(const char *elf_path) 
{
    if(!elf_path) 
    {
        log_write("missing elf\n");
        return;
    }

    FILE *fp = fopen(elf_path, "rb");
    if(!fp)
    {
        log_write("Cant open elf\n");
        return;
    }

    fseek(fp, 0, SEEK_END);//读写位置移到文件末尾 
    long file_size = ftell(fp);//获取文件大小

    fseek(fp, 0, SEEK_SET);//重置到文件开头

    elf_buf = malloc(file_size);//分配空间

    if(fread(elf_buf, 1, file_size, fp) != (size_t)file_size)
    {
        free(elf_buf);
        elf_buf = NULL;
        fclose(fp);
        return;
    }

    fclose(fp);

    Elf32_Ehdr *eh = (Elf32_Ehdr*)elf_buf; //将读入的内容解析为ELF文件头结构
    Elf32_Shdr *sh = (Elf32_Shdr*)(elf_buf + eh->e_shoff);
    // 把 elf_buf + e_shoff 解释成节头数组的起始地址，
    // 转换为 Elf32_Shdr * 以方便按 sh[index] 访问每个节头。

    /*ELF文件 header检验*/
    if(!(eh->e_ident[EI_MAG0] == ELFMAG0 
      && eh->e_ident[EI_MAG1] == ELFMAG1
      && eh->e_ident[EI_MAG2] == ELFMAG2
      && eh->e_ident[EI_MAG3] == ELFMAG3))
      {
        log_write("not a elf\n");
        return;
      }
    
    for(int i = 0; i < eh->e_shnum; i++)
    {
        if(sh[i].sh_type == SHT_SYMTAB)
        {
            syms = (Elf32_Sym*)(elf_buf + sh[i].sh_offset);//指向节数据的开头
            nsyms = sh[i].sh_size / sh[i].sh_entsize;//计算符号表项个数
            Elf32_Shdr *str_sh = &sh[sh[i].sh_link]; //符号表节的 sh_link字段存放与之关联的字符串表节的索引
            strtab = (const char *)(elf_buf + str_sh->sh_offset);//指向elf文件.strtab保存符号名字的字符串表
            break;
        }
    }


}

const char *ftrace_find_function(uint32_t addr, uint32_t *off)
{
    if(!syms || !strtab) return NULL;
    for(int i = 0; i < nsyms; i++)
    {
        Elf32_Sym *s = &syms[i];
        if(ELF32_ST_TYPE(s->st_info) != STT_FUNC) continue; //如果类型不匹配 继续匹配
        if(s->st_size == 0)//完全匹配
        {
            if(addr == (uint32_t)s->st_value)
            {
                if(off) *off = 0;
                return strtab+ s->st_name;//st_name其实是偏移，c字符存的地方是strtab
            }
            continue;
        }
        else
        {
            uint32_t start = (uint32_t)s->st_value;
            uint32_t end = start + (uint32_t)s->st_size;
            if(addr >= start && addr < end)
            {
                if(off) *off = addr - start;
                return strtab + s->st_name;
            }
            continue;
        }

    }

    return NULL;//一个都没匹配

}


void ftrace_call(uint32_t call_pc, uint32_t target) //不跳过trm_init和main
{ 
    for (int i = 0; i < fdepth; i++) log_write("  ");
    uint32_t off = 0;
    const char *func_name = ftrace_find_function(target, &off);
    if(func_name)
    {
      log_write("%s CALL %08x -> %08x\n", func_name, call_pc, target);
    } 

    fdepth++;
}

void ftrace_ret(uint32_t ret_pc, uint32_t target) 
{
    if (fdepth > 0) fdepth--;
    for (int i = 0; i < fdepth; i++) log_write("  ");
    uint32_t off = 0;
    const char *func_name = ftrace_find_function(target, &off);
    if(func_name)
    {
        log_write("%s RET  %08x -> %08x\n", func_name, ret_pc, target);
    }
    
}
