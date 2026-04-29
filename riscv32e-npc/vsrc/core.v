`include "vsrc/ctrl_defs.vh"

module core(
  input         clk,
  input         reset,
  input  [31:0] inst,

  output [31:0] pc,
  output [31:0] inst_dbg,
  output [31:0] a0_dbg,
  output        invalid_dbg
);

  // 取指单元 IFU
  wire [31:0] inst_out;

  // 指令译码单元 IDU
  wire [4:0] rs1, rs2, rd;
  wire       rd_en;
  wire [31:0] imm;
  wire [3:0] alu_op;
  wire       alu_src2_imm;
  wire       alu_en;
  wire       alu_src1_pc;

  wire [2:0] wb_sel;
  wire [1:0] npc_sel;    // `NPC_PC4:pc+4 `NPC_JALR:jalr `NPC_JAL:jal

  wire       mem_re;
  wire       mem_we;
  wire [1:0] mem_width;
  wire       mem_signed;

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
    .inst_in  (inst),
    .inst_out (inst_out)
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
    .invalid       (idu_invalid)
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

  // PC 路径单独模块化，逻辑保持不变
  wire [31:0] next_pc;
  NextPC u_nextpc(
    .pc      (pc),
    .alu_result (alu_result),
    .npc_sel (npc_sel),
    .branch_taken (branch_taken),
    .next_pc (next_pc)
  );

  wire [31:0] pc4;
  assign pc4 = pc + 32'd4;

  PCReg u_pcreg(
    .clk     (clk),
    .reset   (reset),
    .next_pc (next_pc),
    .pc      (pc)
  );

  WBU u_wbu(
    .wb_sel     (wb_sel),
    .pc4        (pc4),
    .alu_result (alu_result),
    .mem_data   (rdata),
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

  assign inst_dbg    = inst_out;
  assign a0_dbg      = r_a0;
  assign invalid_dbg = idu_invalid;


endmodule
