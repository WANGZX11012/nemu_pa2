module RegisterFile #(ADDR_WIDTH = 1, DATA_WIDTH = 1) (//二读一写寄存器堆 必须有clk
  input clk,
  input [DATA_WIDTH-1:0] w_data,
  input [ADDR_WIDTH-1:0] w_addr,

  input [ADDR_WIDTH-1:0] r_addr1,
  input [ADDR_WIDTH-1:0] r_addr2,

  input wen,

  output [DATA_WIDTH-1:0]r_data1,
  output [DATA_WIDTH-1:0]r_data2

);
  reg [DATA_WIDTH-1:0] rf [2**ADDR_WIDTH-1:0];  //2**代表幂运算 也就是rf 31


  always @(posedge clk) 
  begin
    if (wen && w_addr != 0) begin
      rf[w_addr] <= w_data;
      // 用 $display 在时钟边沿立即打印本次提交值，避免 $strobe 看到边沿后组合逻辑的新值
      $display("[RF] write x%0d <= 0x%08x", w_addr, w_data);
    end
    
  end

  assign r_data1 = (r_addr1 == 0) ? 0 : rf[r_addr1];
  assign r_data2 = (r_addr2 == 0) ? 0 : rf[r_addr2];

endmodule

