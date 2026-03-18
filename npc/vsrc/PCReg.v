
module PCReg(

    input                   clk,
    input                   reset,
    
    input  [31:0]           next_pc,

    output reg  [31:0]      pc


);


initial pc = 32'h0; // 仿真方便

  always @(posedge clk) 
  begin
    if (reset) pc <= 32'h0;
    else       pc <= next_pc;
  end


endmodule

