module top(
  input         clk,
  input         reset,
  input  [31:0] inst,   // C++ 每拍喂进来的指令

  output [31:0] pc      // 给 C++ 取指地址
);

  wire [31:0] inst_dbg;
  wire [31:0] a0_dbg;
  wire        invalid_dbg;

  core u_core(
    .clk        (clk),
    .reset      (reset),
    .inst       (inst),
    .pc         (pc),
    .inst_dbg   (inst_dbg),
    .a0_dbg     (a0_dbg),
    .invalid_dbg(invalid_dbg)
  );

  dpic_ebreak u_dpic(
    .clk   (clk),
    .inst  (inst_dbg),
    .pc    (pc),
    .r_a0  (a0_dbg)
  );

  dpic_invalid u_dpic_invalid(
    .clk    (clk),
    .reset  (reset),
    .invalid(invalid_dbg)
  );

endmodule
