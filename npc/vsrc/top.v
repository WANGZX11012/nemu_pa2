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
  wire       rd_en;
  wire [31:0] imm;
  wire [3:0] alu_op;
  wire       alu_src2_imm;
  wire       alu_en;

  wire [2:0] wb_sel;
  wire       npc_sel;    //是1就是jal触发

  
  wire       mem_re;
  wire       mem_we;
  wire [1:0] mem_width;

  // 这些是 IDU 输出但当前顶层未使用的控制信号：
  // 先显式接到 *_unused，避免读代码时误以为漏连
  wire       idu_rs1_en_unused;
  wire       idu_rs2_en_unused;
  wire [2:0] idu_branch_type_unused;



  //reg file

  wire [31:0] r_data1, r_data2;


  //EXU

  wire [31:0] alu_result;
  wire [31:0] j_target;

  //WBU

  wire [31:0] wb_data;

  // LSU 读数据，作为 WBU 的 MEM 写回输入
  wire [31:0] rdata;
    

  IFU u_ifu(

  .inst_in           (inst)
 ,.inst_out          (inst_out)
  
  );


  IDU u_idu(
  
  .inst          (inst_out)
 ,.rs1           (rs1)
 ,.rs2           (rs2)
 ,.rd            (rd)
 ,.rs1_en        (idu_rs1_en_unused)
 ,.rs2_en        (idu_rs2_en_unused)
 ,.rd_en         (rd_en)
 ,.imm           (imm)
 ,.alu_op        (alu_op)
 ,.alu_src2_imm  (alu_src2_imm)
 ,.alu_en        (alu_en)
 ,.wb_sel        (wb_sel)
 ,.npc_sel       (npc_sel)  
 //not used ouput
 ,.mem_re        (mem_re)
 ,.mem_we        (mem_we)
 ,.mem_width     (mem_width)
 ,.branch_type   (idu_branch_type_unused)

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

  EXU u_exu(

  .rs1_data     (r_data1)
 ,.rs2_data     (r_data2)
 ,.imm          (imm)
 ,.alu_op       (alu_op)
 ,.alu_src2_imm (alu_src2_imm)
 ,.alu_en       (alu_en)

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
 ,.mem_data         (rdata)
 ,.imm              (imm)
 ,.wb_data          (wb_data)
 
  );



  LSU u_lsu(

  .clk          (clk)
 ,.mem_re       (mem_re)
 ,.mem_we       (mem_we)
 ,.mem_width    (mem_width)
 ,.wdata        (r_data2)
 ,.addr         (alu_result)
 ,.rdata        (rdata)

  );


  dpic_ebreak u_dpic(

  .clk          (clk)
 ,.inst         (inst_out)
 ,.pc           (pc)

  );






endmodule

