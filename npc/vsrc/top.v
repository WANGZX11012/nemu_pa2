module top(
  input         clk,
  input         reset,
  input  [31:0] inst,   // C++ 每拍喂进来的指令

  output [31:0] pc      // 给 C++ 取指地址
);

    // 取指单元 IFU
    wire [31:0] inst_out;

    // 指令译码单元 IDU
    wire [4:0] rs1, rs2, rd;
    wire       rd_en;
    wire [31:0] imm;
    wire [3:0] alu_op;
    wire       alu_src2_imm;
    wire       alu_en;

    wire [2:0] wb_sel;
    wire [1:0] npc_sel;    // 00:pc+4 01:jalr 10:jal
  
  
    wire       mem_re;
    wire       mem_we;
    wire [1:0] mem_width;

    // 这些是 IDU 输出但当前顶层未使用的控制信号：
    // 先显式接到 *_unused，避免读代码时误以为漏连
    wire       idu_rs1_en_unused;
    wire       idu_rs2_en_unused;
    wire [2:0] idu_branch_type_unused;



    // 寄存器堆

    wire [31:0] r_data1, r_data2;


    // 执行单元 EXU

    wire [31:0] alu_result;
    wire [31:0] j_target;

    // 写回单元 WBU

    wire [31:0] wb_data;

    // LSU 读数据，作为 WBU 的 MEM 写回输入
    wire [31:0] rdata;

    // DPIC 判断 GOOD TRAP 的 a0 值

    wire [31:0] r_a0;
    

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
   // 未使用的输出
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
   ,.r_a0         (r_a0)

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

    // PC 路径
    wire [31:0] pc4;
    assign pc4 = pc + 32'd4;
    wire [31:0]  next_pc;
    wire [31:0] jal_target_raw;
    wire [31:0] adj_jal_target;
    assign jal_target_raw = pc + imm;
    // 如果运行在高地址区域（0x8000_0000+），当 jalr 目标没有设置高位时
    // 继承基地址，保证间接跳转仍然落在与当前 PC 相同的地址区间。
    wire [31:0] adj_j_target;
    assign adj_j_target = (pc[31] && !j_target[31]) ? (j_target | 32'h8000_0000) : j_target;
    assign adj_jal_target = (pc[31] && !jal_target_raw[31]) ? (jal_target_raw | 32'h8000_0000) : jal_target_raw;
    assign next_pc = (npc_sel == 2'b01) ? adj_j_target :
                   (npc_sel == 2'b10) ? adj_jal_target :
                   pc4;

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
   ,.r_a0         (r_a0)
 
    );





  endmodule

