`include "ctrl_defs.vh"

module NextPC(
  input  [31:0] pc,
  input  [31:0] alu_result,
  input  [2:0]  npc_sel,
  input         branch_taken, //要跳转的信号
  input  [31:0] csr_mtvec, //csr ecall 跳转的地方
  input  [31:0] csr_mepc,  //csr mret 返回的地方

  output [31:0] next_pc
);

  wire [31:0] pc4;

  assign pc4 = pc + 32'd4;
  // npc_sel: `NPC_JALR -> jalr, `NPC_JAL -> jal, `NPC_BR -> branch
  // jal/jalr 的目标地址已经在 EXU 里算好，NextPC 这里只负责选择与 jalr 对齐
  assign next_pc = (npc_sel == `NPC_JALR) ? {alu_result[31:1], 1'b0} :
                   (npc_sel == `NPC_BR && branch_taken) ? alu_result :
                   (npc_sel == `NPC_JAL) ? alu_result : 
                   (npc_sel == `NPC_ECALL) ? csr_mtvec :
                   (npc_sel == `NPC_MRET)  ? csr_mepc  :
                   pc4;

endmodule
