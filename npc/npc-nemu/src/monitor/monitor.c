#include <npc_common.h>
#include <npc_utils.h>


void npc_init_rand();
void npc_init_log(const char *log_file);
// void init_mem();
// void init_difftest(char *ref_so_file, long img_size, int port);
// void init_device();
void npc_init_sdb();
void init_disasm();
bool load_hex_program(const char *path);
void init_pmem(size_t bytes);

static void welcome() 
{
  npc_Log("Trace: %s", npc_MUXDEF(CONFIG_TRACE, ANSI_FMT("ON", ANSI_FG_GREEN), ANSI_FMT("OFF", ANSI_FG_RED)));
  npc_IFDEF(CONFIG_TRACE, npc_Log("If trace is enabled, a log file will be generated "
        "to record the trace. This may lead to a large log file. "
        "If it is not necessary, you can disable it in menuconfig"));
  npc_Log("Build time: %s, %s", __TIME__, __DATE__);
  printf("Welcome to %s-NPC-NEMU!\n", ANSI_FMT(npc_str(__GUEST_ISA__), ANSI_FG_YELLOW ANSI_BG_RED));
  printf("For help, type \"help\"\n");
 //removed 
}

#ifndef CONFIG_TARGET_AM
#include <getopt.h>

void npc_sdb_set_batch_mode();

static char *log_file = NULL;
static char *diff_so_file = NULL;
static char *img_file = NULL;
static char *elf_file = NULL;
static int difftest_port = 1234;

static long load_img() 
{
  if (img_file == NULL) 
  {
    npc_Log("No image is given. Initialize NPC pmem only.");
    init_pmem(1024 * 1024);
    return 0;
  }

  npc_Assert(load_hex_program(img_file), "Can not load '%s'", img_file);
  npc_Log("The image is %s", img_file);
  return 0;
}

static int parse_args(int argc, char *argv[]) {
  const struct option table[] = {
    {"batch"    , no_argument      , NULL, 'b'},
    {"log"      , required_argument, NULL, 'l'},
    {"diff"     , required_argument, NULL, 'd'},
    {"elf"      , required_argument, NULL, 'e'},
    {"port"     , required_argument, NULL, 'p'},
    {"help"     , no_argument      , NULL, 'h'},
    {0          , 0                , NULL,  0 },
  };
  int o;
  while ( (o = getopt_long(argc, argv, "-bhl:d:p:e:", table, NULL)) != -1) {
    switch (o) {
      case 'b': npc_sdb_set_batch_mode(); break;
      case 'p': sscanf(optarg, "%d", &difftest_port); break;
      case 'l': log_file = optarg; break;
      case 'd': diff_so_file = optarg; break;
      case 'e': elf_file = optarg; printf("elf_file is %s\n",elf_file);break;
      case 1: img_file = optarg; return 0;
      default:
        printf("Usage: %s [OPTION...] IMAGE [args]\n\n", argv[0]);
        printf("\t-b,--batch              run with batch mode\n");
        printf("\t-l,--log=FILE           output log to FILE\n");
        printf("\t-d,--diff=REF_SO        run DiffTest with reference REF_SO\n");
        printf("\t-p,--port=PORT          run DiffTest with port PORT\n");
        printf("\n");
        exit(0);
    }
  }
  return 0;
}

void npc_init_monitor(int argc, char *argv[]) 
//是程序启动与简单调试器（simple debugger/sdb）的初始化入口
//负责启动时的环境准备与交互式调试入口。
{
  /* Perform some global initialization. */

  /* Parse arguments. */
  parse_args(argc, argv);

  /* Set random seed. */
  npc_init_rand();

  /* Open the log file. */
  npc_init_log(log_file);

  /* Initialize memory. */
//   init_mem();

  /* Initialize devices. */
//   IFDEF(CONFIG_DEVICE, init_device());

  /* Perform ISA dependent initialization. */
//   init_isa();

  /* Load the image to memory. This will overwrite the built-in image. */
  load_img();

  /* Initialize ftrace if an ELF path was provided via -e */
//   if (elf_file) 
//   {
//     init_ftrace(elf_file);
//   }

  /* Initialize differential testing. */
  // init_difftest(diff_so_file, img_size, difftest_port);

  /* Initialize the simple debugger. */
  npc_init_sdb();

//   IFDEF(CONFIG_ITRACE, init_disasm());

  /* Display welcome message. */
  welcome();
}
#endif
