`define REG_WIDTH 5
`include "vsrc/ctrl_defs.vh"

module EXU(

  input  [31:0]         rs1_data,  
  input  [31:0]         rs2_data,

  input  [31:0]         imm,

  input                 alu_en,
  input  [3:0]          alu_op,
  input                 alu_src2_imm,    //alu的第二个输入是imm还是rs2_data
  input                 alu_src1_pc,     //当为AUIPC时，src1由PC提供
  input  [31:0]         pc,
  input  [2:0]          branch_type,

  output [31:0]         alu_result,
  output                branch_taken

);

wire [31:0] src1 = alu_src1_pc ? pc : rs1_data;
wire [31:0] src2 = alu_src2_imm ? imm : rs2_data;

function [31:0] alu_func;
    input [3:0]     op;
    input [31:0]    a;
    input [31:0]    b;
    begin
        case(op)
            `ALU_ADD:   alu_func = a + b;                 // 正确
            // `ALU_ADD:   alu_func = a + b + (alu_src2_imm ? 32'd1 : 32'd0); // temporary bug injection
            `ALU_SUB:   alu_func = a - b;
            `ALU_AND:   alu_func = a & b;
            `ALU_OR:    alu_func = a | b;
            `ALU_XOR:   alu_func = a ^ b;
            `ALU_SLT:   alu_func = ($signed(a) < $signed(b)) ? 32'd1 : 32'd0;
            `ALU_SLTU:  alu_func = (a < b) ? 32'd1 : 32'd0;
            `ALU_SLL:   alu_func = a << b[4:0];
            `ALU_SRL:   alu_func = a >> b[4:0];
            `ALU_SRA:   alu_func = $signed(a) >>> b[4:0]; //>>>是算术移动
            default:    alu_func = 32'b0;
        endcase
    end
endfunction


 assign alu_result = alu_en ? alu_func(alu_op, src1, src2) : 32'b0;
 assign branch_taken = (branch_type == `BR_BEQ)  ? (rs1_data == rs2_data) :
                       (branch_type == `BR_BNE)  ? (rs1_data != rs2_data) : 
                       (branch_type == `BR_BGE)  ? ($signed(rs1_data) >= $signed(rs2_data))  : 
                       (branch_type == `BR_BGEU) ? ($unsigned(rs1_data) >= $unsigned(rs2_data))  :
                       (branch_type == `BR_BLT)  ? ($signed(rs1_data) < $signed(rs2_data)) :
                       (branch_type == `BR_BLTU) ? ($unsigned(rs1_data) < $unsigned(rs2_data)) : 1'b0;



endmodule


