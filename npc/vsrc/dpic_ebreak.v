import "DPI-C" function void npc_ebreak(input int code);

module dpic_ebreak(
  input          clk,
  input   [31:0] inst,
  input   [31:0] pc
);

  // EBREAK 
  localparam EBREAK = 32'h00c00067;

  always @(posedge clk) begin
    if (inst == EBREAK) begin
      // pass PC as code/context to the DPI-C handler
      npc_ebreak(pc);
    end
  end

endmodule