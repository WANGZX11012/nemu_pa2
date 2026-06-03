`include "ctrl_defs.vh"

// ============================================================================
// IFU - Instruction Fetch Unit（取指单元）
// 职责：管理 PC、输出取指地址、接收指令、计算下一 PC
// ============================================================================
module IFU (
  input         clk,
  input         reset,

  // === 指令输入（来自顶层/存储器） ===
  input  [31:0] inst_in,

  // === 下一 PC 控制信号 ===
  input  [31:0] alu_result,      // 跳转/分支目标地址（来自 EXU）
  input  [2:0]  npc_sel,        // PC 选择：PC4 / JALR / JAL / BR / ECALL / MRET
  input         branch_taken,    // 分支是否 taken
  input  [31:0] csr_mtvec,      // 异常入口地址
  input  [31:0] csr_mepc,       // mret 返回地址

  // === 输出 ===
  output [31:0] pc,              // 当前 PC（供顶层取指用）
  output [31:0] pc4,             // PC+4（供 WBU 写回用）
  output [31:0] inst_out         // 当前指令（送给 IDU 译码）
);

  // =========================================================================
  // 1. PC 寄存器（原 PCReg 功能）
  // =========================================================================
  reg [31:0] pc_reg;

  initial pc_reg = 32'h8000_0000;

  always @(posedge clk) begin
    if (reset)
      pc_reg <= 32'h8000_0000;
    else
      pc_reg <= next_pc;
  end

  assign pc   = pc_reg;
  assign pc4  = pc_reg + 32'd4;

  // =========================================================================
  // 2. 下一 PC 计算（原 NextPC 功能）
  // =========================================================================
  wire [31:0] next_pc;

  assign next_pc = (npc_sel == `NPC_JALR)              ? {alu_result[31:1], 1'b0} :
                   (npc_sel == `NPC_BR && branch_taken) ? alu_result :
                   (npc_sel == `NPC_JAL)                ? alu_result :
                   (npc_sel == `NPC_ECALL)              ? csr_mtvec :
                   (npc_sel == `NPC_MRET)               ? csr_mepc  :
                   pc4;

  // =========================================================================
  // 3. 指令透传（当前单周期设计，每周期指令均有效）
  //    后续可在此处加入 valid/ready 握手逻辑
  // =========================================================================
  assign inst_out = inst_in;

endmodule
