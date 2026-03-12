module RegisterFile #(ADDR_WIDTH = 1, DATA_WIDTH = 1) (
  input clk,
  input [DATA_WIDTH-1:0] wdata,
  input [ADDR_WIDTH-1:0] waddr,
  input wen
);
  reg [DATA_WIDTH-1:0] rf [2**ADDR_WIDTH-1:0];  //2**代表幂运算
  always @(posedge clk) begin
    if (wen && waddr != 0) rf[waddr] <= wdata;
    
  end
endmodule