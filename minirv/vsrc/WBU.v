// 引入统一 wb_sel 编码，和 IDU 保持一致
`include "vsrc/ctrl_defs.vh"

module WBU(

    input  [31:0]           pc4,
    input  [31:0]           alu_result,
    input  [31:0]           mem_data,
    
    input  [31:0]           imm, //for lui

    input  [2:0]            wb_sel, //由IDU传入 决定WB什么数据

    output [31:0]           wb_data


);

function [31:0] wb_func;
    input  [2:0]    i_wb_sel;
    input  [31:0]   i_pc4;
    input  [31:0]   i_alu_result;
    input  [31:0]   i_mem_data;
    input  [31:0]   i_imm;
    begin
        // 统一写回多路选择：ALU / PC+4 / MEM / IMM
        case(i_wb_sel)
            `WB_ALU: wb_func = i_alu_result;
            `WB_PC4: wb_func = i_pc4;
            `WB_MEM: wb_func = i_mem_data;
            `WB_IMM: wb_func = i_imm;
            default:wb_func = 32'h0;
        endcase
    end
endfunction

assign wb_data = wb_func(wb_sel, pc4, alu_result, mem_data, imm);//要按顺序


endmodule

