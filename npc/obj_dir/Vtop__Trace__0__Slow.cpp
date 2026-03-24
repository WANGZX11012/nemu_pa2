// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vtop__Syms.h"


VL_ATTR_COLD void Vtop___024root__trace_init_sub__TOP__0(Vtop___024root* vlSelf, VerilatedVcd* tracep) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root__trace_init_sub__TOP__0\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const int c = vlSymsp->__Vm_baseCode;
    tracep->pushPrefix("$rootio", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBit(c+62,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+63,0,"reset",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+64,0,"inst",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+65,0,"pc",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("top", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBit(c+62,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+63,0,"reset",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+64,0,"inst",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+65,0,"pc",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+64,0,"inst_out",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+66,0,"rs1",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+67,0,"rs2",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+68,0,"rd",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBit(c+69,0,"rd_en",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+1,0,"imm",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+2,0,"alu_op",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 3,0);
    tracep->declBit(c+70,0,"alu_src2_imm",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+3,0,"alu_en",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+71,0,"wb_sel",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBit(c+4,0,"npc_sel",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+5,0,"mem_re",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+6,0,"mem_we",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+7,0,"mem_width",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBit(c+72,0,"idu_rs1_en_unused",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+8,0,"idu_rs2_en_unused",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+91,0,"idu_branch_type_unused",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+25,0,"r_data1",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+73,0,"r_data2",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+26,0,"alu_result",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+27,0,"j_target",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+28,0,"wb_data",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+29,0,"rdata",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+74,0,"pc4",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+75,0,"next_pc",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+76,0,"adj_j_target",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->pushPrefix("u_dpic", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBit(c+62,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+64,0,"inst",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+65,0,"pc",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+92,0,"EBREAK",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("u_exu", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+25,0,"rs1_data",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+73,0,"rs2_data",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+1,0,"imm",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBit(c+3,0,"alu_en",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+2,0,"alu_op",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 3,0);
    tracep->declBit(c+70,0,"alu_src2_imm",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+26,0,"alu_result",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+27,0,"j_target",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+25,0,"src1",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+77,0,"src2",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("u_idu", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+64,0,"inst",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+66,0,"rs1",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+67,0,"rs2",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+68,0,"rd",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBit(c+72,0,"rs1_en",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+8,0,"rs2_en",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+69,0,"rd_en",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+1,0,"imm",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+2,0,"alu_op",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 3,0);
    tracep->declBit(c+3,0,"alu_en",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+70,0,"alu_src2_imm",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+78,0,"jal",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+5,0,"mem_re",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+6,0,"mem_we",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+7,0,"mem_width",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBus(c+71,0,"wb_sel",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBit(c+4,0,"npc_sel",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+91,0,"branch_type",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+91,0,"IMM_I",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+93,0,"IMM_S",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+94,0,"IMM_U",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+95,0,"IMM_B",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+96,0,"IMM_J",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+79,0,"opcode",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 6,0);
    tracep->declBus(c+80,0,"funct3",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+81,0,"funct7",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 6,0);
    tracep->declBus(c+9,0,"imm_type",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBit(c+10,0,"is_addi",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+4,0,"is_jalr",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+78,0,"is_jal",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+11,0,"is_add",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+12,0,"is_sub",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+13,0,"is_and",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+14,0,"is_or",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+15,0,"is_xor",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+16,0,"is_slt",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+17,0,"is_sltu",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+18,0,"is_sll",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+19,0,"is_srl",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+20,0,"is_sra",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+82,0,"is_lui",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+83,0,"is_slli",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+84,0,"is_srli",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+85,0,"is_srai",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+21,0,"is_sb",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+22,0,"is_sw",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+23,0,"is_lbu",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+24,0,"is_lw",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+86,0,"imm_i",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+87,0,"imm_s",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+88,0,"imm_u",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+89,0,"imm_b",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+90,0,"imm_j",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("u_ifu", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+64,0,"inst_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+64,0,"inst_out",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("u_lsu", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBit(c+62,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+5,0,"mem_re",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+6,0,"mem_we",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+7,0,"mem_width",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBus(c+73,0,"wdata",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+26,0,"addr",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+29,0,"rdata",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("u_pcreg", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBit(c+62,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+63,0,"reset",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+75,0,"next_pc",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+65,0,"pc",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("u_regfile", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+97,0,"ADDR_WIDTH",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+98,0,"DATA_WIDTH",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBit(c+62,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+28,0,"w_data",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+68,0,"w_addr",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+66,0,"r_addr1",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+67,0,"r_addr2",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBit(c+69,0,"wen",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+25,0,"r_data1",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+73,0,"r_data2",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->pushPrefix("rf", VerilatedTracePrefixType::ARRAY_UNPACKED);
    for (int i = 0; i < 32; ++i) {
        tracep->declBus(c+30+i*1,0,"",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, true,(i+0), 31,0);
    }
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->pushPrefix("u_wbu", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+74,0,"pc4",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+26,0,"alu_result",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+29,0,"mem_data",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+1,0,"imm",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+71,0,"wb_sel",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+28,0,"wb_data",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->popPrefix();
    tracep->popPrefix();
}

VL_ATTR_COLD void Vtop___024root__trace_init_top(Vtop___024root* vlSelf, VerilatedVcd* tracep) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root__trace_init_top\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    Vtop___024root__trace_init_sub__TOP__0(vlSelf, tracep);
}

VL_ATTR_COLD void Vtop___024root__trace_const_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
VL_ATTR_COLD void Vtop___024root__trace_full_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void Vtop___024root__trace_chg_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void Vtop___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/);

VL_ATTR_COLD void Vtop___024root__trace_register(Vtop___024root* vlSelf, VerilatedVcd* tracep) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root__trace_register\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    tracep->addConstCb(&Vtop___024root__trace_const_0, 0, vlSelf);
    tracep->addFullCb(&Vtop___024root__trace_full_0, 0, vlSelf);
    tracep->addChgCb(&Vtop___024root__trace_chg_0, 0, vlSelf);
    tracep->addCleanupCb(&Vtop___024root__trace_cleanup, vlSelf);
}

VL_ATTR_COLD void Vtop___024root__trace_const_0_sub_0(Vtop___024root* vlSelf, VerilatedVcd::Buffer* bufp);

VL_ATTR_COLD void Vtop___024root__trace_const_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root__trace_const_0\n"); );
    // Body
    Vtop___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vtop___024root*>(voidSelf);
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    Vtop___024root__trace_const_0_sub_0((&vlSymsp->TOP), bufp);
}

VL_ATTR_COLD void Vtop___024root__trace_const_0_sub_0(Vtop___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root__trace_const_0_sub_0\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode);
    bufp->fullCData(oldp+91,(0U),3);
    bufp->fullIData(oldp+92,(0x00100073U),32);
    bufp->fullCData(oldp+93,(1U),3);
    bufp->fullCData(oldp+94,(2U),3);
    bufp->fullCData(oldp+95,(3U),3);
    bufp->fullCData(oldp+96,(4U),3);
    bufp->fullIData(oldp+97,(5U),32);
    bufp->fullIData(oldp+98,(0x00000020U),32);
}

VL_ATTR_COLD void Vtop___024root__trace_full_0_sub_0(Vtop___024root* vlSelf, VerilatedVcd::Buffer* bufp);

VL_ATTR_COLD void Vtop___024root__trace_full_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root__trace_full_0\n"); );
    // Body
    Vtop___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vtop___024root*>(voidSelf);
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    Vtop___024root__trace_full_0_sub_0((&vlSymsp->TOP), bufp);
}

VL_ATTR_COLD void Vtop___024root__trace_full_0_sub_0(Vtop___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root__trace_full_0_sub_0\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode);
    bufp->fullIData(oldp+1,(vlSelfRef.top__DOT__u_exu__DOT__imm),32);
    bufp->fullCData(oldp+2,(vlSelfRef.top__DOT__alu_op),4);
    bufp->fullBit(oldp+3,(((IData)(vlSelfRef.top__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_15) 
                           | ((IData)(vlSelfRef.top__DOT__u_idu__DOT__is_addi) 
                              | ((IData)(vlSelfRef.top__DOT__u_idu__DOT__npc_sel) 
                                 | ((IData)(vlSelfRef.top__DOT__u_idu__DOT__is_sb) 
                                    | ((IData)(vlSelfRef.top__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_16) 
                                       | ((IData)(vlSelfRef.top__DOT__u_idu__DOT__is_lbu) 
                                          | ((IData)(vlSelfRef.top__DOT__u_idu__DOT__is_and) 
                                             | (IData)(vlSelfRef.top__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_20))))))))));
    bufp->fullBit(oldp+4,(vlSelfRef.top__DOT__u_idu__DOT__npc_sel));
    bufp->fullBit(oldp+5,(vlSelfRef.top__DOT__u_idu__DOT__mem_re));
    bufp->fullBit(oldp+6,(((IData)(vlSelfRef.top__DOT__u_idu__DOT__is_sb) 
                           | (IData)(vlSelfRef.top__DOT__u_idu__DOT__is_sw))));
    bufp->fullCData(oldp+7,(vlSelfRef.top__DOT__u_idu__DOT__mem_width),2);
    bufp->fullBit(oldp+8,(((IData)(vlSelfRef.top__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_15) 
                           | ((IData)(vlSelfRef.top__DOT__u_idu__DOT__is_and) 
                              | ((IData)(vlSelfRef.top__DOT__u_idu__DOT__is_or) 
                                 | ((IData)(vlSelfRef.top__DOT__u_idu__DOT__is_xor) 
                                    | ((IData)(vlSelfRef.top__DOT__u_idu__DOT__is_slt) 
                                       | ((IData)(vlSelfRef.top__DOT__u_idu__DOT__is_sltu) 
                                          | ((IData)(vlSelfRef.top__DOT__u_idu__DOT__is_sll) 
                                             | ((IData)(vlSelfRef.top__DOT__u_idu__DOT__is_srl) 
                                                | ((IData)(vlSelfRef.top__DOT__u_idu__DOT__is_sra) 
                                                   | ((IData)(vlSelfRef.top__DOT__u_idu__DOT__is_sw) 
                                                      | (IData)(vlSelfRef.top__DOT__u_idu__DOT__is_sb)))))))))))));
    bufp->fullCData(oldp+9,(vlSelfRef.top__DOT__u_idu__DOT__imm_type),3);
    bufp->fullBit(oldp+10,(vlSelfRef.top__DOT__u_idu__DOT__is_addi));
    bufp->fullBit(oldp+11,(vlSelfRef.top__DOT__u_idu__DOT__is_add));
    bufp->fullBit(oldp+12,(vlSelfRef.top__DOT__u_idu__DOT__is_sub));
    bufp->fullBit(oldp+13,(vlSelfRef.top__DOT__u_idu__DOT__is_and));
    bufp->fullBit(oldp+14,(vlSelfRef.top__DOT__u_idu__DOT__is_or));
    bufp->fullBit(oldp+15,(vlSelfRef.top__DOT__u_idu__DOT__is_xor));
    bufp->fullBit(oldp+16,(vlSelfRef.top__DOT__u_idu__DOT__is_slt));
    bufp->fullBit(oldp+17,(vlSelfRef.top__DOT__u_idu__DOT__is_sltu));
    bufp->fullBit(oldp+18,(vlSelfRef.top__DOT__u_idu__DOT__is_sll));
    bufp->fullBit(oldp+19,(vlSelfRef.top__DOT__u_idu__DOT__is_srl));
    bufp->fullBit(oldp+20,(vlSelfRef.top__DOT__u_idu__DOT__is_sra));
    bufp->fullBit(oldp+21,(vlSelfRef.top__DOT__u_idu__DOT__is_sb));
    bufp->fullBit(oldp+22,(vlSelfRef.top__DOT__u_idu__DOT__is_sw));
    bufp->fullBit(oldp+23,(vlSelfRef.top__DOT__u_idu__DOT__is_lbu));
    bufp->fullBit(oldp+24,(vlSelfRef.top__DOT__u_idu__DOT__is_lw));
    bufp->fullIData(oldp+25,(vlSelfRef.top__DOT__u_exu__DOT__rs1_data),32);
    bufp->fullIData(oldp+26,(vlSelfRef.top__DOT__alu_result),32);
    bufp->fullIData(oldp+27,(vlSelfRef.top__DOT__u_exu__DOT__j_target),32);
    bufp->fullIData(oldp+28,(vlSelfRef.top__DOT__wb_data),32);
    bufp->fullIData(oldp+29,(vlSelfRef.top__DOT__rdata),32);
    bufp->fullIData(oldp+30,(vlSelfRef.top__DOT__u_regfile__DOT__rf[0]),32);
    bufp->fullIData(oldp+31,(vlSelfRef.top__DOT__u_regfile__DOT__rf[1]),32);
    bufp->fullIData(oldp+32,(vlSelfRef.top__DOT__u_regfile__DOT__rf[2]),32);
    bufp->fullIData(oldp+33,(vlSelfRef.top__DOT__u_regfile__DOT__rf[3]),32);
    bufp->fullIData(oldp+34,(vlSelfRef.top__DOT__u_regfile__DOT__rf[4]),32);
    bufp->fullIData(oldp+35,(vlSelfRef.top__DOT__u_regfile__DOT__rf[5]),32);
    bufp->fullIData(oldp+36,(vlSelfRef.top__DOT__u_regfile__DOT__rf[6]),32);
    bufp->fullIData(oldp+37,(vlSelfRef.top__DOT__u_regfile__DOT__rf[7]),32);
    bufp->fullIData(oldp+38,(vlSelfRef.top__DOT__u_regfile__DOT__rf[8]),32);
    bufp->fullIData(oldp+39,(vlSelfRef.top__DOT__u_regfile__DOT__rf[9]),32);
    bufp->fullIData(oldp+40,(vlSelfRef.top__DOT__u_regfile__DOT__rf[10]),32);
    bufp->fullIData(oldp+41,(vlSelfRef.top__DOT__u_regfile__DOT__rf[11]),32);
    bufp->fullIData(oldp+42,(vlSelfRef.top__DOT__u_regfile__DOT__rf[12]),32);
    bufp->fullIData(oldp+43,(vlSelfRef.top__DOT__u_regfile__DOT__rf[13]),32);
    bufp->fullIData(oldp+44,(vlSelfRef.top__DOT__u_regfile__DOT__rf[14]),32);
    bufp->fullIData(oldp+45,(vlSelfRef.top__DOT__u_regfile__DOT__rf[15]),32);
    bufp->fullIData(oldp+46,(vlSelfRef.top__DOT__u_regfile__DOT__rf[16]),32);
    bufp->fullIData(oldp+47,(vlSelfRef.top__DOT__u_regfile__DOT__rf[17]),32);
    bufp->fullIData(oldp+48,(vlSelfRef.top__DOT__u_regfile__DOT__rf[18]),32);
    bufp->fullIData(oldp+49,(vlSelfRef.top__DOT__u_regfile__DOT__rf[19]),32);
    bufp->fullIData(oldp+50,(vlSelfRef.top__DOT__u_regfile__DOT__rf[20]),32);
    bufp->fullIData(oldp+51,(vlSelfRef.top__DOT__u_regfile__DOT__rf[21]),32);
    bufp->fullIData(oldp+52,(vlSelfRef.top__DOT__u_regfile__DOT__rf[22]),32);
    bufp->fullIData(oldp+53,(vlSelfRef.top__DOT__u_regfile__DOT__rf[23]),32);
    bufp->fullIData(oldp+54,(vlSelfRef.top__DOT__u_regfile__DOT__rf[24]),32);
    bufp->fullIData(oldp+55,(vlSelfRef.top__DOT__u_regfile__DOT__rf[25]),32);
    bufp->fullIData(oldp+56,(vlSelfRef.top__DOT__u_regfile__DOT__rf[26]),32);
    bufp->fullIData(oldp+57,(vlSelfRef.top__DOT__u_regfile__DOT__rf[27]),32);
    bufp->fullIData(oldp+58,(vlSelfRef.top__DOT__u_regfile__DOT__rf[28]),32);
    bufp->fullIData(oldp+59,(vlSelfRef.top__DOT__u_regfile__DOT__rf[29]),32);
    bufp->fullIData(oldp+60,(vlSelfRef.top__DOT__u_regfile__DOT__rf[30]),32);
    bufp->fullIData(oldp+61,(vlSelfRef.top__DOT__u_regfile__DOT__rf[31]),32);
    bufp->fullBit(oldp+62,(vlSelfRef.clk));
    bufp->fullBit(oldp+63,(vlSelfRef.reset));
    bufp->fullIData(oldp+64,(vlSelfRef.inst),32);
    bufp->fullIData(oldp+65,(vlSelfRef.pc),32);
    bufp->fullCData(oldp+66,((0x0000001fU & (vlSelfRef.inst 
                                             >> 0x0000000fU))),5);
    bufp->fullCData(oldp+67,((0x0000001fU & (vlSelfRef.inst 
                                             >> 0x00000014U))),5);
    bufp->fullCData(oldp+68,((0x0000001fU & (vlSelfRef.inst 
                                             >> 7U))),5);
    bufp->fullBit(oldp+69,(((IData)(vlSelfRef.top__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_21) 
                            | ((0x37U == (0x0000007fU 
                                          & vlSelfRef.inst)) 
                               | ((IData)(vlSelfRef.top__DOT__u_idu__DOT__mem_re) 
                                  | ((0x6fU == (0x0000007fU 
                                                & vlSelfRef.inst)) 
                                     | (IData)(vlSelfRef.top__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_18)))))));
    bufp->fullBit(oldp+70,(((IData)(vlSelfRef.top__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_14) 
                            | ((0x37U == (0x0000007fU 
                                          & vlSelfRef.inst)) 
                               | ((IData)(vlSelfRef.top__DOT__u_idu__DOT__is_sw) 
                                  | ((IData)(vlSelfRef.top__DOT__u_idu__DOT__is_sb) 
                                     | ((IData)(vlSelfRef.top__DOT__u_idu__DOT__is_lw) 
                                        | ((IData)(vlSelfRef.top__DOT__u_idu__DOT__is_lbu) 
                                           | (IData)(vlSelfRef.top__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_18)))))))));
    bufp->fullCData(oldp+71,((((IData)(vlSelfRef.top__DOT__u_idu__DOT__is_addi) 
                               | ((IData)(vlSelfRef.top__DOT__u_idu__DOT__is_add) 
                                  | ((IData)(vlSelfRef.top__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_19) 
                                     | (IData)(vlSelfRef.top__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_20))))
                               ? 0U : ((IData)(vlSelfRef.top__DOT__u_idu__DOT__npc_sel)
                                        ? 1U : ((0x6fU 
                                                 == 
                                                 (0x0000007fU 
                                                  & vlSelfRef.inst))
                                                 ? 1U
                                                 : 
                                                (((IData)(vlSelfRef.top__DOT__u_idu__DOT__is_lw) 
                                                  | (IData)(vlSelfRef.top__DOT__u_idu__DOT__is_lbu))
                                                  ? 2U
                                                  : 
                                                 ((0x37U 
                                                   == 
                                                   (0x0000007fU 
                                                    & vlSelfRef.inst))
                                                   ? 3U
                                                   : 0U)))))),3);
    bufp->fullBit(oldp+72,(((IData)(vlSelfRef.top__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_21) 
                            | ((IData)(vlSelfRef.top__DOT__u_idu__DOT__is_slt) 
                               | ((IData)(vlSelfRef.top__DOT__u_idu__DOT__is_sltu) 
                                  | ((IData)(vlSelfRef.top__DOT__u_idu__DOT__is_sll) 
                                     | ((IData)(vlSelfRef.top__DOT__u_idu__DOT__is_srl) 
                                        | ((IData)(vlSelfRef.top__DOT__u_idu__DOT__is_sra) 
                                           | ((0x37U 
                                               == (0x0000007fU 
                                                   & vlSelfRef.inst)) 
                                              | ((IData)(vlSelfRef.top__DOT__u_idu__DOT__is_lw) 
                                                 | ((IData)(vlSelfRef.top__DOT__u_idu__DOT__is_lbu) 
                                                    | ((IData)(vlSelfRef.top__DOT__u_idu__DOT__is_sw) 
                                                       | ((IData)(vlSelfRef.top__DOT__u_idu__DOT__is_sb) 
                                                          | (IData)(vlSelfRef.top__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_18))))))))))))));
    bufp->fullIData(oldp+73,(((0U == (0x0000001fU & 
                                      (vlSelfRef.inst 
                                       >> 0x00000014U)))
                               ? 0U : vlSelfRef.top__DOT__u_regfile__DOT__rf
                              [(0x0000001fU & (vlSelfRef.inst 
                                               >> 0x00000014U))])),32);
    bufp->fullIData(oldp+74,(((IData)(4U) + vlSelfRef.pc)),32);
    bufp->fullIData(oldp+75,(((IData)(vlSelfRef.top__DOT__u_idu__DOT__npc_sel)
                               ? (((~ (vlSelfRef.top__DOT__u_exu__DOT__j_target 
                                       >> 0x0000001fU)) 
                                   & (vlSelfRef.pc 
                                      >> 0x0000001fU))
                                   ? (0x80000000U | vlSelfRef.top__DOT__u_exu__DOT__j_target)
                                   : vlSelfRef.top__DOT__u_exu__DOT__j_target)
                               : ((IData)(4U) + vlSelfRef.pc))),32);
    bufp->fullIData(oldp+76,((((~ (vlSelfRef.top__DOT__u_exu__DOT__j_target 
                                   >> 0x0000001fU)) 
                               & (vlSelfRef.pc >> 0x0000001fU))
                               ? (0x80000000U | vlSelfRef.top__DOT__u_exu__DOT__j_target)
                               : vlSelfRef.top__DOT__u_exu__DOT__j_target)),32);
    bufp->fullIData(oldp+77,((((IData)(vlSelfRef.top__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_14) 
                               | ((0x37U == (0x0000007fU 
                                             & vlSelfRef.inst)) 
                                  | ((IData)(vlSelfRef.top__DOT__u_idu__DOT__is_sw) 
                                     | ((IData)(vlSelfRef.top__DOT__u_idu__DOT__is_sb) 
                                        | ((IData)(vlSelfRef.top__DOT__u_idu__DOT__is_lw) 
                                           | ((IData)(vlSelfRef.top__DOT__u_idu__DOT__is_lbu) 
                                              | (IData)(vlSelfRef.top__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_18)))))))
                               ? vlSelfRef.top__DOT__u_exu__DOT__imm
                               : ((0U == (0x0000001fU 
                                          & (vlSelfRef.inst 
                                             >> 0x00000014U)))
                                   ? 0U : vlSelfRef.top__DOT__u_regfile__DOT__rf
                                  [(0x0000001fU & (vlSelfRef.inst 
                                                   >> 0x00000014U))]))),32);
    bufp->fullBit(oldp+78,((0x6fU == (0x0000007fU & vlSelfRef.inst))));
    bufp->fullCData(oldp+79,((0x0000007fU & vlSelfRef.inst)),7);
    bufp->fullCData(oldp+80,((7U & (vlSelfRef.inst 
                                    >> 0x0000000cU))),3);
    bufp->fullCData(oldp+81,((vlSelfRef.inst >> 0x00000019U)),7);
    bufp->fullBit(oldp+82,((0x37U == (0x0000007fU & vlSelfRef.inst))));
    bufp->fullBit(oldp+83,((IData)((0x00001013U == 
                                    (0x0000707fU & vlSelfRef.inst)))));
    bufp->fullBit(oldp+84,(((IData)(vlSelfRef.top__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_11) 
                            & (0U == (vlSelfRef.inst 
                                      >> 0x00000019U)))));
    bufp->fullBit(oldp+85,(((IData)(vlSelfRef.top__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_11) 
                            & (0x20U == (vlSelfRef.inst 
                                         >> 0x00000019U)))));
    bufp->fullIData(oldp+86,((((- (IData)((vlSelfRef.inst 
                                           >> 0x0000001fU))) 
                               << 0x0000000cU) | (vlSelfRef.inst 
                                                  >> 0x00000014U))),32);
    bufp->fullIData(oldp+87,((((- (IData)((vlSelfRef.inst 
                                           >> 0x0000001fU))) 
                               << 0x0000000cU) | ((0x00000fe0U 
                                                   & (vlSelfRef.inst 
                                                      >> 0x00000014U)) 
                                                  | (0x0000001fU 
                                                     & (vlSelfRef.inst 
                                                        >> 7U))))),32);
    bufp->fullIData(oldp+88,((0xfffff000U & vlSelfRef.inst)),32);
    bufp->fullIData(oldp+89,((((- (IData)((vlSelfRef.inst 
                                           >> 0x0000001fU))) 
                               << 0x0000000cU) | ((0x00000fe0U 
                                                   & (vlSelfRef.inst 
                                                      >> 0x00000014U)) 
                                                  | (0x0000001eU 
                                                     & (vlSelfRef.inst 
                                                        >> 7U))))),32);
    bufp->fullIData(oldp+90,(((((0x00000ffeU & ((- (IData)(
                                                           (vlSelfRef.inst 
                                                            >> 0x0000001fU))) 
                                                << 1U)) 
                                | (vlSelfRef.inst >> 0x0000001fU)) 
                               << 0x00000014U) | ((
                                                   ((0x000001feU 
                                                     & (vlSelfRef.inst 
                                                        >> 0x0000000bU)) 
                                                    | (1U 
                                                       & (vlSelfRef.inst 
                                                          >> 0x00000014U))) 
                                                   << 0x0000000bU) 
                                                  | (0x000007feU 
                                                     & (vlSelfRef.inst 
                                                        >> 0x00000014U))))),32);
}
