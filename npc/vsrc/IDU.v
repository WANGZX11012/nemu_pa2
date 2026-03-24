`define I_LEN 32
`define REG_WIDTH 5
// 引入统一控制信号编码：mem_width / wb_sel
`include "vsrc/ctrl_defs.vh"

module IDU (
  input  [`I_LEN-1:0]       inst,
  output [`REG_WIDTH-1:0]   rs1,
  output [`REG_WIDTH-1:0]   rs2,
  output [`REG_WIDTH-1:0]   rd,

  output                    rs1_en,
  output                    rs2_en,
  output                    rd_en,

  output [31:0]             imm,
  
  output [3:0]              alu_op,
  output                    alu_en,     //新增控制
  output                    alu_src2_imm, //alu的第二个输入是rs2 还是imm的判断信号

  output                    jal,

  output                    mem_re,
  output                    mem_we,
  output [1:0]              mem_width, //字节有1,2,4的选择                  

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
  wire [6:0] funct7 = inst[31:25];
  reg [2:0] imm_type;

  //具体指令的判断译码
  wire is_addi = (opcode == 7'b0010011) && (funct3 == 3'b000);
  wire is_jalr = (opcode == 7'b1100111) && (funct3 == 3'b000);
  wire is_jal  = (opcode == 7'b1101111);
  wire is_add  = (opcode == 7'b0110011) && (funct3 == 3'b000) && (funct7 == 7'b0000000);
  wire is_sub  = (opcode == 7'b0110011) && (funct3 == 3'b000) && (funct7 == 7'b0100000);
  wire is_and  = (opcode == 7'b0110011) && (funct3 == 3'b111);
  wire is_or   = (opcode == 7'b0110011) && (funct3 == 3'b110);
  wire is_xor  = (opcode == 7'b0110011) && (funct3 == 3'b100);
  wire is_slt  = (opcode == 7'b0110011) && (funct3 == 3'b010);
  wire is_sltu = (opcode == 7'b0110011) && (funct3 == 3'b011);
  wire is_sll  = (opcode == 7'b0110011) && (funct3 == 3'b001);
  wire is_srl  = (opcode == 7'b0110011) && (funct3 == 3'b101) && (funct7 == 7'b0000000);
  wire is_sra  = (opcode == 7'b0110011) && (funct3 == 3'b101) && (funct7 == 7'b0100000);
  wire is_lui  = (opcode == 7'b0110111);
  // I-type shifts
  wire is_slli = (opcode == 7'b0010011) && (funct3 == 3'b001);
  wire is_srli = (opcode == 7'b0010011) && (funct3 == 3'b101) && (funct7 == 7'b0000000);
  wire is_srai = (opcode == 7'b0010011) && (funct3 == 3'b101) && (funct7 == 7'b0100000);
  wire is_sb   = (opcode == 7'b0100011) && (funct3 == 3'b000);
  wire is_sw   = (opcode == 7'b0100011) && (funct3 == 3'b010);
  wire is_lbu  = (opcode == 7'b0000011) && (funct3 == 3'b100);
  wire is_lw   = (opcode == 7'b0000011) && (funct3 == 3'b010);


  
  assign rs1 = inst[19:15];
  assign rs2 = inst[24:20];
  assign rd  = inst[11:7];

  // 寄存器读使能
  assign rs1_en = is_addi | is_jalr | is_add | is_sub | is_and | is_or | is_xor |
                   is_slt | is_sltu | is_sll | is_srl | is_sra | is_lui |
                   is_lw | is_lbu | is_sw | is_sb | is_slli | is_srli | is_srai;
  assign rs2_en = is_add | is_sub | is_and | is_or | is_xor | is_slt | is_sltu |
                   is_sll | is_srl | is_sra | is_sw | is_sb;
  assign rd_en  = is_addi | is_jalr | is_add | is_sub | is_and | is_or |
                   is_xor | is_lui | is_lbu | is_lw | is_jal | is_slli | is_srli | is_srai; //寄存器写使能逻辑
  
  /*alu related*/
  assign alu_op = is_sub  ? `ALU_SUB :
                  is_and  ? `ALU_AND :
                  is_or   ? `ALU_OR  :
                  is_xor  ? `ALU_XOR :
                  is_slt  ? `ALU_SLT :
                  is_sltu ? `ALU_SLTU :
                  is_sll  ? `ALU_SLL :
                  is_srl  ? `ALU_SRL :
                  is_sra  ? `ALU_SRA :
                  `ALU_ADD;

  assign alu_src2_imm = is_addi | is_jalr | is_lui | is_sw | is_sb | is_lw | is_lbu |
                        is_slli | is_srli | is_srai;  // add时为0， 为1时src2传入imm 而不是rs2的值
  assign alu_en = is_add | is_sub | is_addi | is_jalr | is_sb | is_sw | is_lw | is_lbu |
                  is_and | is_or | is_xor | is_slt | is_sltu | is_sll | is_srl | is_sra |
                  is_slli | is_srli | is_srai;

    // npc_sel:
  // 000 -> pc + 4  // 001 -> jalr目标地址
  
  assign npc_sel = is_jalr ? 1'b1 : 1'b0;
  assign jal = is_jal;
  // 访存相关
  assign mem_re = is_lbu | is_lw;
  assign mem_we = is_sb | is_sw;
  // 访存宽度选择：sb/lbu=字节，sw/lw=字
  // 其余默认 half（预留给后续 lh/lhu/sh）
  assign mem_width = (is_sb | is_lbu) ? `MEM_BYTE :
                     ((is_sw | is_lw) ? `MEM_WORD : `MEM_HALF);
  // wb_sel:
  // 000 -> ALU结果（addi）
  // 001 -> pc + 4   （jalr）
  // wb_sel mapping (explicit):
  // 000 -> ALU result (addi / add)
  // 001 -> pc + 4 (jalr)
  // 010 -> mem_data (lw / lbu)
  // 011 -> imm (lui)
  // 写回来源选择：
  // addi/add -> ALU，jalr -> pc+4，lw/lbu -> MEM，lui -> IMM
  assign wb_sel = (is_addi | is_add | is_sub | is_and | is_or | is_xor |
                   is_slt | is_sltu | is_sll | is_srl | is_sra | is_slli |
                   is_srli | is_srai) ? `WB_ALU :
                  is_jalr  ? `WB_PC4 :
                  is_jal   ? `WB_PC4 :
                  (is_lw | is_lbu)   ? `WB_MEM :
                  (is_lui ? `WB_IMM : `WB_ALU);

  
  
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

  /*立即数生成逻辑 (集成从ImmGen)*/
  wire [31:0] imm_i = {{20{inst[31]}}, inst[31:20]};
  wire [31:0] imm_s = {{20{inst[31]}}, inst[31:25], inst[11:7]}; //没毛
  wire [31:0] imm_u = {inst[31:12], 12'b0};
  wire [31:0] imm_b = {{20{inst[31]}}, inst[31:25], inst[11:8], 1'b0};
  wire [31:0] imm_j = {{11{inst[31]}}, inst[31], inst[19:12], inst[20], inst[30:21], 1'b0};

  assign imm = (imm_type == IMM_I) ? imm_i :
               (imm_type == IMM_S) ? imm_s :
               (imm_type == IMM_U) ? imm_u :
               (imm_type == IMM_B) ? imm_b :
               (imm_type == IMM_J) ? imm_j :
               32'h0;

endmodule

