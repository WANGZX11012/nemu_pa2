`define REG_WIDTH 5
`include "vsrc/ctrl_defs.vh"

module EXU(

  input  [31:0]         rs1_data,  
  input  [31:0]         rs2_data,

  input  [31:0]         imm,

  input                 alu_en,
  input  [3:0]          alu_op,
  input                 alu_src2_imm,    //alu的第二个输入是imm还是rs2_data

  output [31:0]         alu_result,
  output [31:0]         j_target

);

wire [31:0] src1 = rs1_data;
wire [31:0] src2 = alu_src2_imm ? imm : rs2_data;

function [31:0] alu_func;
    input [3:0]     op;
    input [31:0]    a;
    input [31:0]    b;
    begin
        case(op)
            `ALU_ADD:   alu_func = a + b;
            `ALU_SUB:   alu_func = a - b;
            `ALU_AND:   alu_func = a & b;
            `ALU_OR:    alu_func = a | b;
            `ALU_XOR:   alu_func = a ^ b;
            `ALU_SLT:   alu_func = ($signed(a) < $signed(b)) ? 32'd1 : 32'd0;
            `ALU_SLTU:  alu_func = (a < b) ? 32'd1 : 32'd0;
            `ALU_SLL:   alu_func = a << b[4:0];
            `ALU_SRL:   alu_func = a >> b[4:0];
            `ALU_SRA:   alu_func = $signed(a) >>> b[4:0];
            default:    alu_func = 32'b0;
        endcase
    end
endfunction


 assign alu_result = alu_en ? alu_func(alu_op, src1, src2) : 32'b0;
 assign j_target   = (rs1_data + imm) & 32'hffff_fffe;



endmodule

