/*DPIC*/
// 引入统一 mem_width 编码
`include "vsrc/ctrl_defs.vh"
import "DPI-C" function void pmem_write_u32(input int waddr, input int wdata);
import "DPI-C" function int  pmem_read_u32(input int raddr);

import "DPI-C" function void pmem_write_u8(input int waddr, input byte wdata);
import "DPI-C" function byte pmem_read_u8(input int raddr);

import "DPI-C" function void pmem_write_u16(input int waddr, input int wdata);
import "DPI-C" function int  pmem_read_u16(input int raddr);




/*DPIC*/


module LSU
(

    input               clk,

    input               mem_re,
    input               mem_we,
    input       [1:0]   mem_width,
    input               mem_signed, //是否符号扩展标志

    input       [31:0]  wdata,

    input       [31:0]  addr,
    input       [31:0]  pc,

    output reg  [31:0]  rdata

);

always @(posedge clk) begin
    if(mem_we)
    begin
        case(mem_width)
            `MEM_BYTE: 
            begin // SB：把 rs2[7:0] 写到字节地址 addr
                pmem_write_u8(addr, wdata[7:0]);
            end
            `MEM_WORD: pmem_write_u32(addr, wdata); // SW：按字地址写 32 位
            `MEM_HALF: pmem_write_u16(addr, wdata[15:0]);

            default: ;
        endcase

    end

end

// 读路径用组合逻辑：保证 load 当拍可通过 WBU 写回
// 说明：当前只实现 lbu/lw，因此字节读统一零扩展
always @(*) 
begin
    rdata = 32'h0;

    if (mem_re) 
    begin
        case (mem_width)
            `MEM_BYTE: 
            begin // 读取 1 字节并根据 mem_signed 扩展
                byte b;
                b = pmem_read_u8(addr);
                rdata = mem_signed ? {{24{b[7]}}, b} : {24'b0, b};
            end
            `MEM_HALF:
            begin
                logic [15:0] half;
                half = pmem_read_u16(addr) & 16'hFFFF;
                rdata = mem_signed ? {{16{half[15]}}, half} : {16'b0, half};
            end
            `MEM_WORD: 
            begin // LW：读取 32 位
                rdata = pmem_read_u32(addr);
            end
            default: rdata = 32'h0;
        endcase
    end
end


endmodule
