//原子性读写的CSR寄存器 要么完全执行成功 要么完全不执行

module CSRFile#(
    parameter ADDR_WIDTH = 12
) (
    //不需要读使能
    input           clk,
    input           reset,
    input           csr_wen,
    input           ecall_trap,   //trap 信号相当于 NO
    input           ebreak_trap,  //ebreak 断点异常 (mcause=3)
    input           mret_exec,    //mret 指令执行，恢复 mstatus
    input  [31:0]   ecall_pc,      //ecall 时的 PC，保存到 mepc
    input  [31:0]   csr_wdata,
    input  [11:0]   csr_idx,
    input           csr_s_w,      //是否置位的控制位

    output [31:0]   csr_mtvec, //输出的mtvec值
    output [31:0]   csr_mepc,  // 输出的mepc值，供mret/IFU使用
    output [31:0]   csr_data
    

);
    reg [31:0] csr_read;
    reg [63:0] mcycle;
    reg [31:0] mvendroid, marchid;
    reg [31:0] mcause, mepc, mstatus, mtvec;

    initial 
    begin
        mvendroid = 32'h79737978;//ysyx
        marchid = 32'h18D08CF;//我的学号
    end

	always @(posedge clk)//mcycle 自增逻辑
	begin
	   if(reset)
	       mcycle <= 64'b0;
	   else
	       mcycle <= mcycle + 64'b1;
	end

    //模仿intr.c的 isa_raise_intr
    always @(posedge clk) 
    begin
        if(reset) 
        begin
            mcause <= 0;
            mepc <= 0;
            mstatus <= 32'h1800; //初始化 
            mtvec <= 0; //由am注册的cte init 的内联汇编 csrrw来写入
        end   

        else 
        begin
            if(ecall_trap)
            begin
                mcause <= 32'd11;       // M-mode ecall
                mepc <= ecall_pc;
                mstatus[7] <= mstatus[3];       // MPIE <- MIE
                mstatus[3] <= 1'b0;             // MIE  <- 0（关中断）
                mstatus[12:11] <= 2'b11;        // MPP  <- M-mode（RISC-V 规范：trap 时保存当前特权级）
            end
            else if(ebreak_trap)
            begin
                mcause <= 32'd3;        // Breakpoint
                mepc <= ecall_pc;
                mstatus[7] <= mstatus[3];       // MPIE <- MIE
                mstatus[3] <= 1'b0;             // MIE  <- 0
                mstatus[12:11] <= 2'b11;        // MPP  <- M-mode
            end
            else if(mret_exec)
            begin
                mstatus[3]   <= mstatus[7];     // MIE  <- MPIE（恢复中断）
                mstatus[7]   <= 1'b1;           // MPIE <- 1
                mstatus[12:11] <= 2'b00;        // MPP  <- 0（最低特权级）
            end
            else if (csr_wen) 
            begin
            /* verilator lint_off CASEINCOMPLETE  */ 
            case (csr_idx)
                12'h341: mepc    <= csr_s_w ? (csr_wdata |  mepc ) : csr_wdata;  // mepc
                12'h342: mcause  <= csr_s_w ? (csr_wdata |  mcause ) : csr_wdata;  // mcause
                12'h300: mstatus <= csr_s_w ? (csr_wdata |  mstatus ) : csr_wdata;  // mstatus
                12'h305: mtvec   <= csr_s_w ? (csr_wdata |  mtvec ) : csr_wdata;  // mtvec
                default: ;
            endcase
        end
        end
    
    end

   always @(*) //随时读 
   begin
    case (csr_idx)
        12'hB00: csr_read = mcycle[31:0];
        12'hB80: csr_read = mcycle[63:32];
        12'hf11: csr_read = mvendroid;
        12'hf12: csr_read = marchid;
        12'h341: csr_read = mepc;
        12'h342: csr_read = mcause;
        12'h300: csr_read = mstatus;
        12'h305: csr_read = mtvec;
        default: csr_read = 32'b0;
    endcase

   end

    assign  csr_data = csr_read;
    assign csr_mtvec = mtvec;
    assign csr_mepc  = mepc;    //不需要+4 am的traps已经做了

endmodule

