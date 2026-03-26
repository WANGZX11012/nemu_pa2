import "DPI-C" function void npc_ebreak(input int code);

module dpic_ebreak(
  input          clk,
  input   [31:0] inst,

  input   [31:0] r_a0,// 判断good trap

  input   [31:0] pc
);

  // EBREAK 
  localparam EBREAK     = 32'h00100073;



  always @(posedge clk) 
  begin
    if (inst == EBREAK) 
    begin
      // pass PC as code/context to the DPI-C handler
      $display("PC is 0x%08x",pc);
      npc_ebreak(r_a0);
    end


  end

endmodule

