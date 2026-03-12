`define I_LEN 32
`define REG_WIDTH 5

module IDU (
  input  [`I_LEN-1:0]       inst,
  output [`REG_WIDTH-1:0]   rs1,
  output [`REG_WIDTH-1:0]   rs2,
  output [`REG_WIDTH-1:0]   rd,

  output                    rs1_en,
  output                    rs2_en,
  output                    rd_en,

  output [2:0]              imm_type,
  
  output [3:0]              alu_op,

  output                    alu_src_imm, //alu的第二个输入是rs2 还是imm的判断信号

  output                    mem_re,
  output                    mem_we,
  output [1:0]              mem_width, //字节有1,2,4的选择                  
  output                    mem_unsigned, //是否需要无符号扩展

  output [2:0]              wb_sel,
  output [2:0]              npc_sel,
  output [2:0]              branch_type    //BNE BLT等等的判断

);

wire [6:0] opcode = inst[6:0];
wire [2:0] funct3 = inst[14:12];









endmodule