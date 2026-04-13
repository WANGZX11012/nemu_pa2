import "DPI-C" function void npc_invalid_inst();

module dpic_invalid(
  input             clk,
  input             reset,
  input             invalid

);


  always@(posedge clk)
  begin
    if(!reset && invalid)
    npc_invalid_inst();
  end

endmodule
