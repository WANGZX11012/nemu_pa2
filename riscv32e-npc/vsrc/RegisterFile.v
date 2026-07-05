module RegisterFile #(
  parameter ADDR_WIDTH = 5,
  parameter DATA_WIDTH = 31
) (//二读一写寄存器堆 必须有clk
  input clk,
  input [DATA_WIDTH-1:0] w_data,
  input [ADDR_WIDTH-1:0] w_addr,

  input [ADDR_WIDTH-1:0] r_addr1,
  input [ADDR_WIDTH-1:0] r_addr2,

  input wen,

  output [DATA_WIDTH-1:0]r_data1,
  output [DATA_WIDTH-1:0]r_data2,

  output [DATA_WIDTH-1:0]r_a0

);
  reg [DATA_WIDTH-1:0] rf [2**ADDR_WIDTH-1:0];  //2**代表幂运算 也就是rf 31
  reg [DATA_WIDTH-1:0] rf_read [2**ADDR_WIDTH-1:0];



  integer i;

  initial 
  begin
    for (i = 0; i < (1 << ADDR_WIDTH); i = i + 1) 
    begin
      rf[i] = {DATA_WIDTH{1'b0}};
      rf_read[i] = {DATA_WIDTH{1'b0}};
    end
  end


  always @(posedge clk) 
  begin
    rf[0] <= {DATA_WIDTH{1'b0}};
    if (wen && w_addr != 0) 
    begin
      rf[w_addr] <= w_data;
    end
  end

  // Snapshot the committed architectural state after the writeback edge.
  // Read ports use this snapshot for the whole next cycle, preventing the
  // just-written rd value from feeding back into rs1/rs2 combinationally.
  always @(negedge clk)
  begin
    rf_read[0] <= {DATA_WIDTH{1'b0}};
    for (i = 1; i < (1 << ADDR_WIDTH); i = i + 1) 
    begin
      rf_read[i] <= rf[i];
    end
  end

  assign r_data1 = (r_addr1 == 0) ? 0 : rf_read[r_addr1];
  assign r_data2 = (r_addr2 == 0) ? 0 : rf_read[r_addr2];

  assign r_a0    = rf_read[10]; //到时候传给DPIC 来判断good trap bad trap


endmodule
