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

  output reg  [2:0]         imm_type,
  
  output [3:0]              alu_op,
  output                    alu_en,     //新增控制
  output                    alu_src2_imm, //alu的第二个输入是rs2 还是imm的判断信号

  output                    mem_re,
  output                    mem_we,
  output [1:0]              mem_width, //字节有1,2,4的选择                  
  output                    mem_unsigned, //是否需要无符号扩展

  output [2:0]              wb_sel,
  output                    npc_sel,
  output [2:0]              branch_type    //BNE BLT等等的判断

);

localparam IMM_I = 3'b000;
localparam IMM_S = 3'b001;
localparam IMM_U = 3'b010;
localparam IMM_B = 3'b011;
localparam IMM_J = 3'b100;

  wire [6:0] opcode = inst[6:0];
  wire [2:0] funct3 = inst[14:12];

  //具体指令的判断译码
  wire is_addi = (opcode == 7'b0010011) && (funct3 == 3'b000);
  wire is_jalr = (opcode == 7'b1100111) && (funct3 == 3'b000);
  wire is_add  = (opcode == 7'b0110011) && (funct3 == 3'b000); 
  wire is_lui  = (opcode == 7'b0110111);

  assign rs1 = inst[19:15];
  assign rs2 = inst[24:20];
  assign rd  = inst[11:7];

  assign rs1_en = is_addi | is_jalr | is_add;
  assign rs2_en = is_add;
  assign rd_en  = is_addi | is_jalr | is_add | is_lui;
  
  /*alu related*/
  assign alu_op = 4'b0000; //0是加法 现在固定
  assign alu_src2_imm = is_addi | is_jalr | is_lui;  //add时为0， 为0时src2传入imm 而不是rs2的值 
  assign alu_en = is_add | is_addi | is_jalr;

    // npc_sel:
  // 000 -> pc + 4  // 001 -> jalr目标地址
  
  assign npc_sel = is_jalr ? 1 : 0;
  // 访存相关
  assign mem_re = 1'b0;
  assign mem_we = 1'b0;
  assign mem_width = 2'b00;
  assign mem_unsigned = 1'b0;

  // wb_sel:
  // 000 -> ALU结果（addi）
  // 001 -> pc + 4   （jalr）
  assign wb_sel = is_jalr ? 3'b001 : (is_lui ? 3'b011 : 3'b000);

    // 不支持分支
  assign branch_type = 3'b000;

  /*opcode 判断imm 类型*/
  always@(*) 
  begin
    case(opcode)
        7'b0010011: imm_type = IMM_I; // addi, I-type
        7'b0000011: imm_type = IMM_I; // lw/lbu, I-type
        7'b1100111: imm_type = IMM_I; // jalr, I-type
        7'b0100011: imm_type = IMM_S; // sw/sb, S-type
        7'b0110111: imm_type = IMM_U; // lui, U-type
        7'b1100011: imm_type = IMM_B; // branch, B-type
        7'b1101111: imm_type = IMM_J; // jal, J-type
        default:    imm_type = IMM_I;
    endcase
  end



endmodule

