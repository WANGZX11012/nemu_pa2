

module WBU(

    input  [31:0]           pc4,
    input  [31:0]           alu_result,
    input  [31:0]           mem_data,
    
    input  [2:0]            wb_sel, //由IDU传入 决定WB什么数据

    output [31:0]           wb_data


);

function [31:0] wb_func;
    input  [2:0]    i_wb_sel;
    input  [31:0]   i_pc4;
    input  [31:0]   i_alu_result;
    input  [31:0]   i_mem_data;
    begin
        case(i_wb_sel)
            3'b000: wb_func = i_alu_result;
            3'b001: wb_func = i_pc4;
            3'b010: wb_func = i_mem_data; // 以后load用
            default:wb_func = 32'h0;
        endcase
    end
endfunction

assign wb_data = wb_func(wb_sel, pc4, alu_result, mem_data);


endmodule

