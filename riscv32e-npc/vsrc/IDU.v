`define I_LEN 32
`define REG_WIDTH 5
// 引入统一控制信号编码：mem_width / wb_sel
`include "vsrc/ctrl_defs.vh"

module IDU (
  input  [`I_LEN-1:0]       inst,
  output [`REG_WIDTH-1:0]   rs1,
  output [`REG_WIDTH-1:0]   rs2,
  output [`REG_WIDTH-1:0]   rd,

  output                    rs1_en,
  output                    rs2_en,
  output                    rd_en,

  output [31:0]             imm,
  
  output reg [3:0]          alu_op,
  output reg                alu_en,     //新增控制
  output reg                alu_src2_imm, //alu的第二个输入是rs2 还是imm的判断信号
  output                    alu_src1_pc, // 当为 AUIPC 时，ALU 的第一个操作数由 PC 提供

  output                    mem_re,
  output                    mem_we,
  output [1:0]              mem_width, //字节有1,2,4的选择  
  output                    mem_signed, //符号加载 无符号加载控制                

  output [2:0]              wb_sel,
  output [1:0]              npc_sel,
  output [2:0]              branch_type,    //BNE BLT等等的判断
  output                    invalid

);

localparam IMM_I = 3'b000;
localparam IMM_S = 3'b001;
localparam IMM_U = 3'b010;
localparam IMM_B = 3'b011;
localparam IMM_J = 3'b100;

  wire [6:0] opcode = inst[6:0];
  wire [2:0] funct3 = inst[14:12];
  wire [6:0] funct7 = inst[31:25];
  
  reg [2:0] imm_type;// 根据指令类型生成的立即数类型控制

  //具体指令的判断译码（组合逻辑中生成 reg 标志位）指令的注册

  //跳转相关
  reg is_jal;
  reg is_jalr;
  reg is_bne; //rs1 rs2不相等 pc变为pc + imm
  reg is_beq;
  reg is_bge; //大于等于 bigger equal
  reg is_bgeu;
  reg is_blt; //branh less than
  reg is_bltu;


  //load相关
  reg is_lui;
  reg is_lbu;
  reg is_lw;
  reg is_lb;
  reg is_lh;
  reg is_lhu;

  //ebreak
  reg is_ebreak;

  //store相关
  reg is_sb;
  reg is_sw;
  reg is_sh;

  //算术相关
  reg is_auipc;//！！！
  reg is_or;
  reg is_xor;
  reg is_xori;
  reg is_add;
  reg is_addi;
  reg is_sub;
  reg is_slti;
  reg is_sltiu;
  reg is_and; // R-type and
  reg is_andi;//与立即数

  reg is_sra; //移动有关
  reg is_srai;//移动有关
  reg is_srl; //逻辑右移动
  reg is_sll; //逻辑左移
  reg is_slli; // slli (I-type)
  reg is_srli; //把rs1逻辑右移

  
  reg is_sltu;
  reg is_slt;

  always @(*) 
  begin
    is_jal    = 1'b0;
    is_jalr   = 1'b0;
    is_lui    = 1'b0;
    is_auipc  = 1'b0;
    is_sb     = 1'b0;
    is_sw     = 1'b0;
    is_lbu    = 1'b0;
    is_lw     = 1'b0;
    is_ebreak = 1'b0;
    is_xor    = 1'b0;
    is_xori   = 1'b0;
    is_add    = 1'b0;
    is_addi   = 1'b0;
    is_sub    = 1'b0;
    is_or     = 1'b0;
    is_and    = 1'b0;
    is_andi   = 1'b0;

    is_sh     = 1'b0;
    is_sll    = 1'b0;
    is_slli   = 1'b0;
    is_slti   = 1'b0;
    is_sltiu  = 1'b0;
    is_sra    = 1'b0;
    is_srl    = 1'b0;
    is_srai   = 1'b0;
    is_sltu   = 1'b0;
    is_slt    = 1'b0;
    is_srli   = 1'b0;

    is_bne    = 1'b0;
    is_beq    = 1'b0;
    is_bge    = 1'b0;
    is_bgeu   = 1'b0;
    is_blt    = 1'b0;
    is_bltu   = 1'b0;
    is_lh     = 1'b0;
    is_lhu    = 1'b0;
    is_lb     = 1'b0;



    case (opcode)
      7'b1101111: 
      begin // jal
        is_jal = 1'b1;
      end

      7'b1100111: 
      begin // jalr
        if (funct3 == 3'b000) begin
          is_jalr = 1'b1;
        end
      end
      
      7'b0110111: 
      begin // lui
        is_lui = 1'b1;
      end

      7'b0010111: 
      begin // auipc
        is_auipc = 1'b1;
      end

      7'b0100011: 
      begin // store
        case (funct3)
          3'b000: begin is_sb = 1'b1; end
          3'b010: begin is_sw = 1'b1; end
          3'b001: begin is_sh = 1'b1; end
        endcase
      end

      7'b0000011: 
      begin // load
        case (funct3)
          3'b000: begin is_lb  = 1'b1; end
          3'b010: begin is_lw  = 1'b1; end
          3'b100: begin is_lbu = 1'b1; end
          3'b001: begin is_lh  = 1'b1; end
          3'b101: begin is_lhu = 1'b1; end
        endcase
      end

      7'b0110011: 
      begin // R-type arithmetic
        case (funct3)
          3'b000: 
          begin
            if (funct7 == 7'b0000000) begin is_add = 1'b1; end // add
            if (funct7 == 7'b0100000) begin is_sub = 1'b1; end // sub
          end
          3'b011: if (funct7 == 7'b0000000) begin is_sltu = 1'b1; end // sltu
          3'b010: if (funct7 == 7'b0000000) begin is_slt = 1'b1; end // slt
          3'b100: if (funct7 == 7'b0000000) begin is_xor = 1'b1; end // xor
          3'b110: if (funct7 == 7'b0000000) begin is_or = 1'b1; end // or
          3'b111: if (funct7 == 7'b0000000) begin is_and = 1'b1; end // and
          3'b101: begin 
                    if (funct7 == 7'b0100000) is_sra = 1'b1; // sra
                    if (funct7 == 7'b0000000)  is_srl = 1'b1; // srl
                  end

          3'b001: if (funct7 == 7'b0000000) begin is_sll = 1'b1; end //sll
        endcase
      end

      7'b0010011: 
      begin // I-type arithmetic
        case (funct3)
          3'b000: begin is_addi = 1'b1; end // addi
          3'b001: begin if(funct7 == 7'b000_0000) is_slli = 1'b1; end // slli
          3'b010: begin is_slti = 1'b1; end // slti
          3'b011: begin is_sltiu = 1'b1; end // sltiu
          3'b100: begin is_xori = 1'b1; end // xori
          3'b101: begin 
                    if (funct7 == 7'b0100000) is_srai = 1'b1;// srai
                    if (funct7 == 7'b0000000)  is_srli = 1'b1; //srli
                  end 
          3'b111: begin is_andi = 1'b1; end //andi
        endcase
      end

      7'b1100011:
      begin
        case(funct3)
          3'b000: is_beq = 1'b1;
          3'b001: is_bne = 1'b1;
          3'b101: is_bge = 1'b1;
          3'b111: is_bgeu= 1'b1;
          3'b100: is_blt = 1'b1;
          3'b110: is_bltu= 1'b1;
        endcase
      end

      7'b1110011://system 相关
        if(inst == 32'h00100073) 
        begin
          is_ebreak = 1'b1;
        end

      default: ;
    endcase

  

  end
 
  

  assign rs1 = inst[19:15];
  assign rs2 = inst[24:20];
  assign rd  = inst[11:7];

  // 寄存器读使能！！
  // rs1: addi/add/jalr/lw/lbu/sw/sb/xor/xori/sub 都需要
  // rs2: add/sw/sb/xor/sub 需要
  assign rs1_en = is_addi | is_jalr | is_add | is_lw | is_lbu | is_sw | is_sb | is_xor | 
                  is_xori | is_sub | is_or | is_slti | is_sltiu | is_bne | is_beq | is_bge | 
                  is_bgeu | is_blt  | is_bltu | is_lh | is_lhu | is_lb | is_sltu | is_slt | 
                  is_sh | is_srai | is_sra | is_sll | is_srli | is_srl | is_andi | is_and | is_slli;

  assign rs2_en = is_add | is_sw | is_sb | is_xor | is_sub | is_or | is_bne | is_beq | 
                  is_bge | is_bgeu |  is_blt | is_bltu | is_sltu | is_slt | is_sh | is_sra | is_sll | is_srl | is_and; 

  assign rd_en  = is_addi | is_jal | is_jalr | is_add | is_lui | is_lbu | is_lw | is_auipc | 
                  is_xor | is_xori | is_sub | is_or | is_slti | is_sltiu | is_lh | is_lhu | is_lb | 
                  is_sltu | is_slt | is_srai | is_sra | is_sll | is_srli | is_srl | is_andi | is_and | is_slli ;//寄存器写使能逻辑
                  
  
  /*alu related*/
  // 用 case 结构解码 ALU 操作，按 opcode 分组，R-type/I-type 细化
  // 默认值
  always @(*) 
  begin
    //初始化
    alu_op = `ALU_ADD;
    alu_en = 1'b0;
    alu_src2_imm = 1'b0;

    case (opcode)
      7'b1101111:
      begin // jal：pc + imm
        alu_en = 1'b1;
        alu_src2_imm = 1'b1;
        alu_op = `ALU_ADD;
      end

      7'b1100111:
      begin // jalr：rs1 + imm
        if (funct3 == 3'b000) 
        begin
          alu_en = 1'b1;
          alu_src2_imm = 1'b1;
          alu_op = `ALU_ADD;
        end
      end

      7'b0110011: //算术相关
      begin // R-type
        alu_en = 1'b1;
        case (funct3)
          3'b000: alu_op = (funct7 == 7'b0100000) ? `ALU_SUB : `ALU_ADD; // add / sub
          3'b100: alu_op = `ALU_XOR; // xor
          3'b111: alu_op = `ALU_AND; // and
          3'b110: alu_op = `ALU_OR;  // or
          3'b001: alu_op = `ALU_SLL; // sll
          3'b101: alu_op = (funct7 == 7'b0100000) ? `ALU_SRA : `ALU_SRL; // srl / sra
          3'b010: alu_op = `ALU_SLT; // slt
          3'b011: alu_op = `ALU_SLTU; // sltu
          default: alu_op = `ALU_ADD;
        endcase
      end

      7'b0010111:
      begin // auipc
        alu_en = 1'b1;
        alu_src2_imm = 1'b1;
        alu_op = `ALU_ADD;
      end

      7'b0000011:
      begin // load address = rs1 + imm
        alu_en = 1'b1;
        alu_src2_imm = 1'b1;
        alu_op = `ALU_ADD; // lw / lbu / load相关都是一样的地址计算
      end
 
      7'b0100011:
      begin // store address = rs1 + imm
        alu_en = 1'b1;
        alu_src2_imm = 1'b1;
        alu_op = `ALU_ADD; // sw / sb  / sh 地址计算
      end


      7'b0010011: 
      begin // I-type ALU immediate
        alu_en = 1'b1;
        alu_src2_imm = 1'b1;//指令带i的置为1
        case (funct3)
          3'b000: alu_op = `ALU_ADD;  //addi
          // 3'b000: alu_op = `ALU_SUB; // addi (temporary bug injection)
          3'b100: alu_op = `ALU_XOR; // xori
          3'b111: alu_op = `ALU_AND; // andi
          3'b110: alu_op = `ALU_OR;  // ori
          3'b001: alu_op = `ALU_SLL; // slli (funct7 may be checked in EXU if needed)
          3'b101: alu_op = (funct7 == 7'b0100000) ? `ALU_SRA : `ALU_SRL; // srli/srai
          3'b010: alu_op = `ALU_SLT; // slti
          3'b011: alu_op = `ALU_SLTU; // sltiu
          default: alu_op = `ALU_ADD;
        endcase
      end

      7'b1100011:
      begin // branch address = pc + imm
        case(funct3)
          3'b000, 3'b001, 3'b101, 3'b111, 3'b100, 3'b110:
          begin // bne  bge bgeu blt bltu
            alu_en = 1'b1;
            alu_src2_imm = 1'b1;
            alu_op = `ALU_ADD;
          end
        endcase
      end

      default: 
      begin
        alu_op = `ALU_ADD;
        alu_en = 1'b0;
        alu_src2_imm = 1'b0;
      end
    endcase
  end

  assign alu_src1_pc = is_auipc | is_jal | is_bne | is_bge | is_bgeu | is_blt | is_bltu | is_beq; // auipc / jal / 分支b 由 pc 作为 ALU 第一个操作数

    // npc_sel:
  // 2'b00 -> pc + 4
  // 2'b01 -> jalr target (rs1 + imm)
  // 2'b10 -> jal target  (pc + imm)
  assign npc_sel = is_jalr ? `NPC_JALR :
                   (is_bne | is_beq | is_bge | is_bgeu | is_blt | is_bltu)  ? `NPC_BR   :
                   is_jal  ? `NPC_JAL  : `NPC_PC4;
  // 访存相关
  assign mem_re = is_lb | is_lbu | is_lw | is_lh | is_lhu;
  assign mem_we = is_sb | is_sw | is_sh;
  // 访存宽度选择：sb/lbu=字节，sw/lw=字
  // 其余默认 half（预留给后续 lh/lhu/sh）
  assign mem_width =  (is_sb | is_lbu | is_lb) ? `MEM_BYTE :
                      (is_sw | is_lw) ? `MEM_WORD : 
                      (is_lh | is_lhu | is_sh) ? `MEM_HALF : `MEM_WORD;

  assign mem_signed = is_lh | is_lb;
  // wb_sel:
  // 000 -> ALU结果（addi）
  // 001 -> pc + 4   （jal/jalr）
  // wb_sel mapping (explicit):
  // 000 -> ALU result (addi / add)
  // 001 -> pc + 4 (jal / jalr)
  // 010 -> mem_data (lw / lbu)
  // 011 -> imm (lui)
  // 写回来源选择：
  // addi/add -> ALU，jal/jalr -> pc+4，lw/lbu -> MEM，lui -> IMM
  assign wb_sel = (is_addi | is_add | is_auipc | is_xor | is_xori | is_sub | is_or | is_slti | is_sltiu | is_sltu |
                   is_slt | is_sra | is_srai | is_sll | is_srli | is_srl | is_andi | is_and | is_slli ) ? `WB_ALU :
                  
                  (is_jal | is_jalr) ? `WB_PC4 :
                  (is_lw | is_lbu | is_lh | is_lhu | is_lb) ? `WB_MEM :
                  (is_lui ? `WB_IMM : `WB_ALU);

  
  
  assign branch_type = is_beq ? `BR_BEQ : is_bne ? `BR_BNE : is_bge ? `BR_BGE : 
                       is_bgeu ? `BR_BGEU : is_blt  ? `BR_BLT :
                       is_bltu ? `BR_BLTU : `BR_NONE;

  // RV32 shift-immediate 非法编码：inst[25] (= shamt[5]) 必须为 0
  wire illegal_shift_imm = (opcode == 7'b0010011) &&
                           ((funct3 == 3'b001) || (funct3 == 3'b101)) &&
                           inst[25];

  // invalid: 仅当未匹配到任何已实现指令时为 1
  assign invalid = illegal_shift_imm |
                   ~(is_addi | is_jal | is_jalr | is_add | is_lui | is_lbu | is_lw |
                     is_auipc | is_xor | is_xori | is_sub | is_or | is_slti |
                     is_sltiu | is_sw | is_sb | is_sh | is_ebreak | is_bne | is_bge | 
                     is_bgeu | is_blt | is_bltu | is_beq | is_lh | is_lhu | is_lb | 
                     is_sltu | is_slt | is_srai | is_sra | is_sll | is_srli | is_srl | is_andi | is_and | is_slli);


  /*opcode 判断imm 类型*/
  always@(*) 
  begin
    case(opcode)
        7'b0010011: imm_type = IMM_I; // addi, I-type
        7'b0000011: imm_type = IMM_I; // lw/lbu, I-type
        7'b1100111: imm_type = IMM_I; // jalr, I-type
        7'b0010111: imm_type = IMM_U; // auipc, U-type
        7'b0100011: imm_type = IMM_S; // sw/sb, S-type
        7'b0110111: imm_type = IMM_U; // lui, U-type
        7'b1100011: imm_type = IMM_B; // branch, B-type
        7'b1101111: imm_type = IMM_J; // jal, J-type
        default:    imm_type = IMM_I;
    endcase
  end

  /*立即数生成逻辑 (集成从ImmGen)*/
  wire [31:0] imm_i = {{20{inst[31]}}, inst[31:20]};
  wire [31:0] imm_s = {{20{inst[31]}}, inst[31:25], inst[11:7]}; //没毛
  wire [31:0] imm_u = {inst[31:12], 12'b0};
  wire [31:0] imm_b = {{19{inst[31]}}, inst[31], inst[7], inst[30:25], inst[11:8], 1'b0};
  wire [31:0] imm_j = {{11{inst[31]}}, inst[31], inst[19:12], inst[20], inst[30:21], 1'b0};

  assign imm = (imm_type == IMM_I) ? imm_i :
               (imm_type == IMM_S) ? imm_s :
               (imm_type == IMM_U) ? imm_u :
               (imm_type == IMM_B) ? imm_b :
               (imm_type == IMM_J) ? imm_j :
               32'h0;

endmodule
