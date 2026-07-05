`include "ctrl_defs.vh"

module core(
  input         clk,
  input         reset,
  input  [31:0] inst,

  output [31:0] pc,
  output [31:0] inst_dbg,
  output [31:0] a0_dbg,
  output        invalid_dbg
);

  // 取指单元 IFU（整合 PC 寄存器 + 下一 PC 计算）
  wire [31:0] inst_out;
  wire [31:0] pc4;

  // 指令译码单元 IDU
  wire [4:0] rs1, rs2, rd;
  wire       rd_en;
  wire [31:0] imm;
  wire [3:0] alu_op;
  wire       alu_src2_imm;
  wire       alu_en;
  wire       alu_src1_pc;
  wire [31:0]csr_data;
  wire [11:0]csr_idx;
  wire       csr_wen;
  wire       csr_s_w;       // IDU: 1=csrrs(置位) 0=csrrw(覆盖)
  wire [31:0]csr_mtvec;     // CSRFile → IFU: ecall 跳转目标
  wire [31:0]csr_mepc;      // CSRFile → IFU: mret 返回地址 
 
  wire [2:0] wb_sel;
  wire [2:0] npc_sel;

  wire       mem_re;
  wire       mem_we;
  wire [1:0] mem_width;
  wire       mem_signed;

  // 异常相关（无额外信号，直接内联）

  // 这些是 IDU 输出但当前顶层未使用的控制信号
  wire       idu_rs1_en_unused;
  wire       idu_rs2_en_unused;
  wire [2:0] branch_type;
  wire       idu_invalid;

  // 寄存器堆
  wire [31:0] r_data1, r_data2;

  // 执行单元 EXU
  wire [31:0] alu_result;
  wire        branch_taken;

  // 写回单元 WBU
  wire [31:0] wb_data;

  // LSU 读数据，作为 WBU 的 MEM 写回输入
  wire [31:0] rdata;

  // DPIC 判断 GOOD TRAP 的 a0 值
  wire [31:0] r_a0;

  IFU u_ifu(
    .clk          (clk),
    .reset        (reset),
    .inst_in      (inst),
    .alu_result   (alu_result),
    .npc_sel      (npc_sel),
    .branch_taken (branch_taken),
    .csr_mtvec    (csr_mtvec),
    .csr_mepc     (csr_mepc),
    .pc           (pc),
    .pc4          (pc4),
    .inst_out     (inst_out)
  );

  IDU u_idu(
    .inst          (inst_out),
    .rs1           (rs1),
    .rs2           (rs2),
    .rd            (rd),
    .rs1_en        (idu_rs1_en_unused),
    .rs2_en        (idu_rs2_en_unused),
    .rd_en         (rd_en),
    .imm           (imm),
    .alu_op        (alu_op),
    .alu_src2_imm  (alu_src2_imm),
    .alu_en        (alu_en),
    .alu_src1_pc   (alu_src1_pc),
    .wb_sel        (wb_sel),
    .npc_sel       (npc_sel),
    .mem_re        (mem_re),
    .mem_we        (mem_we),
    .mem_width     (mem_width),
    .mem_signed    (mem_signed),
    .branch_type   (branch_type),
    .invalid       (idu_invalid),
    .csr_idx       (csr_idx),
    .csr_wen       (csr_wen),
    .csr_s_w       (csr_s_w)
  );

  RegisterFile #(
    .ADDR_WIDTH(5),
    .DATA_WIDTH(32)
  ) u_regfile(
    .clk     (clk),
    .w_data  (wb_data),
    .w_addr  (rd),
    .r_addr1 (rs1),
    .r_addr2 (rs2),
    .wen     (rd_en),
    .r_data1 (r_data1),
    .r_data2 (r_data2),
    .r_a0    (r_a0)
  );

  EXU u_exu(
    .rs1_data     (r_data1),
    .rs2_data     (r_data2),
    .imm          (imm),
    .alu_en       (alu_en),
    .alu_op       (alu_op),
    .alu_src2_imm (alu_src2_imm),
    .alu_src1_pc  (alu_src1_pc),
    .pc           (pc),
    .branch_type  (branch_type),
    .alu_result   (alu_result),
    .branch_taken (branch_taken)

  );

  WBU u_wbu(
    .wb_sel     (wb_sel),  
    .pc4        (pc4),  
    .alu_result (alu_result), 
    .mem_data   (rdata),
    .csr_data   (csr_data), 
    .imm        (imm),
    .wb_data    (wb_data)  
  );

  LSU u_lsu(
    .clk       (clk),
    .mem_re    (mem_re),
    .mem_we    (mem_we),
    .mem_width (mem_width),
    .mem_signed(mem_signed),
    .wdata     (r_data2),
    .addr      (alu_result),//就是传alu的结果
    .rdata     (rdata),
    .pc        (pc)
  );

  CSRFile #(
    .ADDR_WIDTH    (12)
  ) u_CSRFile (
    .clk           (clk),
    .reset         (reset),
    .csr_wen       (csr_wen),
    .ecall_trap    (npc_sel == `NPC_ECALL),
    .ebreak_trap   (1'b0),         // EBREAK 由 DPI-C 处理，不走硬件异常
    .mret_exec     (npc_sel == `NPC_MRET),
    .ecall_pc      (pc),
    .csr_wdata     (r_data1),
    .csr_idx       (csr_idx),
    .csr_s_w       (csr_s_w),
    .csr_mtvec     (csr_mtvec),
    .csr_mepc      (csr_mepc),
    .csr_data      (csr_data)
  );

  assign inst_dbg    = inst_out;
  assign a0_dbg      = r_a0;
  assign invalid_dbg = idu_invalid;

endmodule
