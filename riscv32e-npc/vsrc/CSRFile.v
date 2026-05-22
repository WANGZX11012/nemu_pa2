//原子性读写的CSR寄存器 要么完全执行成功 要么完全不执行

module CSRFile#(
    parameter ADDR_WIDTH = 12
) (

    input           clk,
    input           reset,
    input           csr_wen,
    //不需要读使能
    input  [31:0]   csr_wdata,
    input  [11:0]   csr_idx,
    output [31:0]   csr_data

);
    reg [31:0] csr_read;

    reg [63:0] mcycle; //合并了

    reg [31:0] mvendroid;
    reg [31:0] marchid;

    initial 
    begin
        mvendroid = 32'h79737978;
        marchid = 32'h18D08CF;
    end

	always @(posedge clk)
	begin
	   if(reset)
	       mcycle <= 64'b0;
	   else
	       mcycle <= mcycle + 64'b1;
	end

   always @(posedge clk) 
   begin
    case (csr_idx)
        12'hb0: csr_read = mcycle[31:0];
        12'hb8: csr_read = mcycle[63:32];
        12'hf11: csr_read = mvendroid;
        12'hf12: csr_read = marchid;
        default: ;
    endcase

   end

    assign  csr_data = csr_read;

endmodule

