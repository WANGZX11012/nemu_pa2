// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vtop__Syms.h"


void Vtop___024root__trace_chg_0_sub_0(Vtop___024root* vlSelf, VerilatedVcd::Buffer* bufp);

void Vtop___024root__trace_chg_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root__trace_chg_0\n"); );
    // Body
    Vtop___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vtop___024root*>(voidSelf);
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (VL_UNLIKELY(!vlSymsp->__Vm_activity)) return;
    Vtop___024root__trace_chg_0_sub_0((&vlSymsp->TOP), bufp);
}

void Vtop___024root__trace_chg_0_sub_0(Vtop___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root__trace_chg_0_sub_0\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode + 1);
    if (VL_UNLIKELY((vlSelfRef.__Vm_traceActivity[1U]))) {
        bufp->chgBit(oldp+0,(((IData)(vlSelfRef.top__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_1) 
                              | (IData)(vlSelfRef.top__DOT__u_idu__DOT__rs2_en))));
        bufp->chgBit(oldp+1,(vlSelfRef.top__DOT__u_idu__DOT__rs2_en));
        bufp->chgCData(oldp+2,(vlSelfRef.top__DOT__imm_type),3);
        bufp->chgBit(oldp+3,(((IData)(vlSelfRef.top__DOT__u_idu__DOT__rs2_en) 
                              | (IData)(vlSelfRef.top__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_1))));
        bufp->chgBit(oldp+4,(vlSelfRef.top__DOT__u_idu__DOT__npc_sel));
        bufp->chgIData(oldp+5,(vlSelfRef.top__DOT__u_exu__DOT__imm),32);
    }
    if (VL_UNLIKELY((vlSelfRef.__Vm_traceActivity[2U]))) {
        bufp->chgIData(oldp+6,(vlSelfRef.top__DOT__u_regfile__DOT__rf[0]),32);
        bufp->chgIData(oldp+7,(vlSelfRef.top__DOT__u_regfile__DOT__rf[1]),32);
        bufp->chgIData(oldp+8,(vlSelfRef.top__DOT__u_regfile__DOT__rf[2]),32);
        bufp->chgIData(oldp+9,(vlSelfRef.top__DOT__u_regfile__DOT__rf[3]),32);
        bufp->chgIData(oldp+10,(vlSelfRef.top__DOT__u_regfile__DOT__rf[4]),32);
        bufp->chgIData(oldp+11,(vlSelfRef.top__DOT__u_regfile__DOT__rf[5]),32);
        bufp->chgIData(oldp+12,(vlSelfRef.top__DOT__u_regfile__DOT__rf[6]),32);
        bufp->chgIData(oldp+13,(vlSelfRef.top__DOT__u_regfile__DOT__rf[7]),32);
        bufp->chgIData(oldp+14,(vlSelfRef.top__DOT__u_regfile__DOT__rf[8]),32);
        bufp->chgIData(oldp+15,(vlSelfRef.top__DOT__u_regfile__DOT__rf[9]),32);
        bufp->chgIData(oldp+16,(vlSelfRef.top__DOT__u_regfile__DOT__rf[10]),32);
        bufp->chgIData(oldp+17,(vlSelfRef.top__DOT__u_regfile__DOT__rf[11]),32);
        bufp->chgIData(oldp+18,(vlSelfRef.top__DOT__u_regfile__DOT__rf[12]),32);
        bufp->chgIData(oldp+19,(vlSelfRef.top__DOT__u_regfile__DOT__rf[13]),32);
        bufp->chgIData(oldp+20,(vlSelfRef.top__DOT__u_regfile__DOT__rf[14]),32);
        bufp->chgIData(oldp+21,(vlSelfRef.top__DOT__u_regfile__DOT__rf[15]),32);
        bufp->chgIData(oldp+22,(vlSelfRef.top__DOT__u_regfile__DOT__rf[16]),32);
        bufp->chgIData(oldp+23,(vlSelfRef.top__DOT__u_regfile__DOT__rf[17]),32);
        bufp->chgIData(oldp+24,(vlSelfRef.top__DOT__u_regfile__DOT__rf[18]),32);
        bufp->chgIData(oldp+25,(vlSelfRef.top__DOT__u_regfile__DOT__rf[19]),32);
        bufp->chgIData(oldp+26,(vlSelfRef.top__DOT__u_regfile__DOT__rf[20]),32);
        bufp->chgIData(oldp+27,(vlSelfRef.top__DOT__u_regfile__DOT__rf[21]),32);
        bufp->chgIData(oldp+28,(vlSelfRef.top__DOT__u_regfile__DOT__rf[22]),32);
        bufp->chgIData(oldp+29,(vlSelfRef.top__DOT__u_regfile__DOT__rf[23]),32);
        bufp->chgIData(oldp+30,(vlSelfRef.top__DOT__u_regfile__DOT__rf[24]),32);
        bufp->chgIData(oldp+31,(vlSelfRef.top__DOT__u_regfile__DOT__rf[25]),32);
        bufp->chgIData(oldp+32,(vlSelfRef.top__DOT__u_regfile__DOT__rf[26]),32);
        bufp->chgIData(oldp+33,(vlSelfRef.top__DOT__u_regfile__DOT__rf[27]),32);
        bufp->chgIData(oldp+34,(vlSelfRef.top__DOT__u_regfile__DOT__rf[28]),32);
        bufp->chgIData(oldp+35,(vlSelfRef.top__DOT__u_regfile__DOT__rf[29]),32);
        bufp->chgIData(oldp+36,(vlSelfRef.top__DOT__u_regfile__DOT__rf[30]),32);
        bufp->chgIData(oldp+37,(vlSelfRef.top__DOT__u_regfile__DOT__rf[31]),32);
    }
    bufp->chgBit(oldp+38,(vlSelfRef.clk));
    bufp->chgBit(oldp+39,(vlSelfRef.reset));
    bufp->chgIData(oldp+40,(vlSelfRef.inst),32);
    bufp->chgIData(oldp+41,(vlSelfRef.pc),32);
    bufp->chgCData(oldp+42,((0x0000001fU & (vlSelfRef.inst 
                                            >> 0x0000000fU))),5);
    bufp->chgCData(oldp+43,((0x0000001fU & (vlSelfRef.inst 
                                            >> 0x00000014U))),5);
    bufp->chgCData(oldp+44,((0x0000001fU & (vlSelfRef.inst 
                                            >> 7U))),5);
    bufp->chgBit(oldp+45,((((IData)(vlSelfRef.top__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_1) 
                            | (IData)(vlSelfRef.top__DOT__u_idu__DOT__rs2_en)) 
                           | (0x37U == (0x0000007fU 
                                        & vlSelfRef.inst)))));
    bufp->chgBit(oldp+46,(((IData)(vlSelfRef.top__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_1) 
                           | (0x37U == (0x0000007fU 
                                        & vlSelfRef.inst)))));
    bufp->chgCData(oldp+47,(((IData)(vlSelfRef.top__DOT__u_idu__DOT__npc_sel)
                              ? 1U : ((0x37U == (0x0000007fU 
                                                 & vlSelfRef.inst))
                                       ? 3U : 0U))),3);
    bufp->chgIData(oldp+48,(vlSelfRef.top__DOT__u_exu__DOT__rs1_data),32);
    bufp->chgIData(oldp+49,(((0U == (0x0000001fU & 
                                     (vlSelfRef.inst 
                                      >> 0x00000014U)))
                              ? 0U : vlSelfRef.top__DOT__u_regfile__DOT__rf
                             [(0x0000001fU & (vlSelfRef.inst 
                                              >> 0x00000014U))])),32);
    bufp->chgIData(oldp+50,(vlSelfRef.top__DOT__alu_result),32);
    bufp->chgIData(oldp+51,((0xfffffffeU & (vlSelfRef.top__DOT__u_exu__DOT__imm 
                                            + vlSelfRef.top__DOT__u_exu__DOT__rs1_data))),32);
    bufp->chgIData(oldp+52,(vlSelfRef.top__DOT__wb_data),32);
    bufp->chgIData(oldp+53,(((IData)(4U) + vlSelfRef.pc)),32);
    bufp->chgIData(oldp+54,(((IData)(vlSelfRef.top__DOT__u_idu__DOT__npc_sel)
                              ? (0xfffffffeU & (vlSelfRef.top__DOT__u_exu__DOT__imm 
                                                + vlSelfRef.top__DOT__u_exu__DOT__rs1_data))
                              : ((IData)(4U) + vlSelfRef.pc))),32);
    bufp->chgIData(oldp+55,((((IData)(vlSelfRef.top__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_1) 
                              | (0x37U == (0x0000007fU 
                                           & vlSelfRef.inst)))
                              ? vlSelfRef.top__DOT__u_exu__DOT__imm
                              : ((0U == (0x0000001fU 
                                         & (vlSelfRef.inst 
                                            >> 0x00000014U)))
                                  ? 0U : vlSelfRef.top__DOT__u_regfile__DOT__rf
                                 [(0x0000001fU & (vlSelfRef.inst 
                                                  >> 0x00000014U))]))),32);
    bufp->chgCData(oldp+56,((0x0000007fU & vlSelfRef.inst)),7);
    bufp->chgCData(oldp+57,((7U & (vlSelfRef.inst >> 0x0000000cU))),3);
    bufp->chgBit(oldp+58,((IData)((0x00000013U == (0x0000707fU 
                                                   & vlSelfRef.inst)))));
    bufp->chgBit(oldp+59,((0x37U == (0x0000007fU & vlSelfRef.inst))));
}

void Vtop___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root__trace_cleanup\n"); );
    // Body
    Vtop___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vtop___024root*>(voidSelf);
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    vlSymsp->__Vm_activity = false;
    vlSymsp->TOP.__Vm_traceActivity[0U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[1U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[2U] = 0U;
}
