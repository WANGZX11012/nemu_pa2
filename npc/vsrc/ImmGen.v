
//逻辑组合 
module ImmGen(

  input      [31:0]         inst,
  input      [2:0]          imm_type,

  output reg [31:0]         imm

);

localparam IMM_I = 3'b000;
localparam IMM_S = 3'b001;
localparam IMM_U = 3'b010;
localparam IMM_B = 3'b011;
localparam IMM_J = 3'b100;


always@(*)
begin
    case(imm_type)
        IMM_I:      imm = {{20{inst[31]}},inst[31:20]}; //TYPE I 符号位扩成32位
        IMM_U:      imm = { inst[31:12], 12'b0 };       //符号位扩展是一样的
        default:    imm = 0;
    endcase

end


//本模块只为了从指令提取立即数 包括一些拼接和扩展

endmodule

