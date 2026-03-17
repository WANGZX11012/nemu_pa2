module top(
  input         clk,
  input         reset,
  input  [31:0] inst,   // C++ 每拍喂进来的指令

  output [31:0] pc      // 给 C++ 取指地址
);

  // IFU
  wire [31:0] inst_out;

  //IDU
  wire [4:0] rs1, rs2, rd;
  wire       rs1_en, rs2_en, rd_en;  //rd_en is we sig
  wire [2:0] imm_type;
  wire [3:0] alu_op;
  wire       alu_src2_imm;

  wire [2:0] wb_sel;
  wire       npc_sel;    //是1就是jal触发

  // 暂未使用的控制信号先接线网，避免端口方向/位宽错误
  wire       mem_re;
  wire       mem_we;
  wire [1:0] mem_width;
  wire       mem_unsigned;
  wire [2:0] branch_type;



  //reg file

  wire [31:0] r_data1, r_data2;

  //immgen 根据不同指令type 选择输出的立即数
  
  wire [31:0] imm;


  //EXU

  wire [31:0] alu_result;
  wire [31:0] j_target;

  //WBU

  wire [31:0] wb_data;

    

  IFU u_ifu(

  .inst_in           (inst)
 ,.inst_out          (inst_out)
  
  );


  IDU u_idu(
  
  .inst          (inst_out)
 ,.rs1           (rs1)
 ,.rs2           (rs2)
 ,.rd            (rd)
 ,.rs1_en        (rs1_en)
 ,.rs2_en        (rs2_en)
 ,.rd_en         (rd_en)
 ,.imm_type      (imm_type)
 ,.alu_op        (alu_op)
 ,.alu_src2_imm  (alu_src2_imm)
 ,.wb_sel        (wb_sel)
 ,.npc_sel       (npc_sel)  
 //not used ouput
 ,.mem_re        (mem_re)
 ,.mem_we        (mem_we)
 ,.mem_width     (mem_width)
 ,.mem_unsigned  (mem_unsigned)
 ,.branch_type   (branch_type)


  );

  RegisterFile #(
    .ADDR_WIDTH(5),
    .DATA_WIDTH(32)
  ) u_regfile(

  .clk          (clk)
 ,.w_data       (wb_data)
 ,.w_addr       (rd)
 ,.r_addr1      (rs1)
 ,.r_addr2      (rs2)
 ,.wen          (rd_en)
 ,.r_data1      (r_data1)
 ,.r_data2      (r_data2)


  );

  ImmGen u_immgen(

  .inst         (inst_out)
 ,.imm_type     (imm_type)
 ,.imm          (imm)

  );

  EXU u_exu(

  .rs1_data     (r_data1)
 ,.rs2_data     (r_data2)
 ,.imm          (imm)
 ,.alu_op       (alu_op)
 ,.alu_src2_imm (alu_src2_imm)

 ,.alu_result   (alu_result)
 ,.j_target     (j_target)

  );

  // PC path
  wire [31:0] pc4;
  assign pc4 = pc + 32'd4;
  wire [31:0]  next_pc;
  assign next_pc = npc_sel ? j_target : pc4;

  PCReg u_pcreg(

  .clk          (clk)
 ,.reset        (reset)
 ,.next_pc      (next_pc)
 ,.pc           (pc)

  );

  WBU u_wbu(

  .wb_sel           (wb_sel)
 ,.pc4              (pc4)
 ,.alu_result       (alu_result)
 ,.mem_data         (32'h0)
 ,.wb_data          (wb_data)

  );








endmodule

