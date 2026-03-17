// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtop.h for the primary calling header

#include "Vtop__pch.h"

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__ico(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG

void Vtop___024root___eval_triggers__ico(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_triggers__ico\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VicoTriggered[0U] = ((0xfffffffffffffffeULL 
                                      & vlSelfRef.__VicoTriggered
                                      [0U]) | (IData)((IData)(vlSelfRef.__VicoFirstIteration)));
    vlSelfRef.__VicoFirstIteration = 0U;
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vtop___024root___dump_triggers__ico(vlSelfRef.__VicoTriggered, "ico"s);
    }
#endif
}

bool Vtop___024root___trigger_anySet__ico(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___trigger_anySet__ico\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        if (in[n]) {
            return (1U);
        }
        n = ((IData)(1U) + n);
    } while ((1U > n));
    return (0U);
}

extern const VlUnpacked<CData/*2:0*/, 128> Vtop__ConstPool__TABLE_h2a8fa47b_0;

void Vtop___024root___ico_sequent__TOP__0(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___ico_sequent__TOP__0\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __Vfunc_top__DOT__u_exu__DOT__alu_func__0__Vfuncout;
    __Vfunc_top__DOT__u_exu__DOT__alu_func__0__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_top__DOT__u_exu__DOT__alu_func__0__a;
    __Vfunc_top__DOT__u_exu__DOT__alu_func__0__a = 0;
    IData/*31:0*/ __Vfunc_top__DOT__u_exu__DOT__alu_func__0__b;
    __Vfunc_top__DOT__u_exu__DOT__alu_func__0__b = 0;
    IData/*31:0*/ __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__Vfuncout;
    __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__Vfuncout = 0;
    CData/*2:0*/ __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_wb_sel;
    __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_wb_sel = 0;
    IData/*31:0*/ __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_pc4;
    __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_pc4 = 0;
    IData/*31:0*/ __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_alu_result;
    __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_alu_result = 0;
    CData/*6:0*/ __Vtableidx1;
    __Vtableidx1 = 0;
    // Body
    vlSelfRef.top__DOT__u_exu__DOT__rs1_data = ((0U 
                                                 == 
                                                 (0x0000001fU 
                                                  & (vlSelfRef.inst 
                                                     >> 0x0000000fU)))
                                                 ? 0U
                                                 : 
                                                vlSelfRef.top__DOT__u_regfile__DOT__rf
                                                [(0x0000001fU 
                                                  & (vlSelfRef.inst 
                                                     >> 0x0000000fU))]);
    __Vtableidx1 = (0x0000007fU & vlSelfRef.inst);
    vlSelfRef.top__DOT__imm_type = Vtop__ConstPool__TABLE_h2a8fa47b_0
        [__Vtableidx1];
    vlSelfRef.top__DOT__u_idu__DOT__npc_sel = (IData)(
                                                      (0x00000067U 
                                                       == 
                                                       (0x0000707fU 
                                                        & vlSelfRef.inst)));
    vlSelfRef.top__DOT__u_exu__DOT__imm = ((0U == (IData)(vlSelfRef.top__DOT__imm_type))
                                            ? (((- (IData)(
                                                           (vlSelfRef.inst 
                                                            >> 0x0000001fU))) 
                                                << 0x0000000cU) 
                                               | (vlSelfRef.inst 
                                                  >> 0x00000014U))
                                            : 0U);
    vlSelfRef.top__DOT__u_exu__DOT__alu_src2_imm = 
        ((IData)((0x00000013U == (0x0000707fU & vlSelfRef.inst))) 
         | (IData)(vlSelfRef.top__DOT__u_idu__DOT__npc_sel));
    if (vlSelfRef.top__DOT__u_idu__DOT__npc_sel) {
        vlSelfRef.top__DOT__u_pcreg__DOT__next_pc = 
            (0xfffffffeU & (vlSelfRef.top__DOT__u_exu__DOT__imm 
                            + vlSelfRef.top__DOT__u_exu__DOT__rs1_data));
        __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_wb_sel = 1U;
    } else {
        vlSelfRef.top__DOT__u_pcreg__DOT__next_pc = 
            ((IData)(4U) + vlSelfRef.pc);
        __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_wb_sel = 0U;
    }
    __Vfunc_top__DOT__u_exu__DOT__alu_func__0__b = 
        ((IData)(vlSelfRef.top__DOT__u_exu__DOT__alu_src2_imm)
          ? vlSelfRef.top__DOT__u_exu__DOT__imm : (
                                                   (0U 
                                                    == 
                                                    (0x0000001fU 
                                                     & (vlSelfRef.inst 
                                                        >> 0x00000014U)))
                                                    ? 0U
                                                    : 
                                                   vlSelfRef.top__DOT__u_regfile__DOT__rf
                                                   [
                                                   (0x0000001fU 
                                                    & (vlSelfRef.inst 
                                                       >> 0x00000014U))]));
    __Vfunc_top__DOT__u_exu__DOT__alu_func__0__a = vlSelfRef.top__DOT__u_exu__DOT__rs1_data;
    __Vfunc_top__DOT__u_exu__DOT__alu_func__0__Vfuncout 
        = (__Vfunc_top__DOT__u_exu__DOT__alu_func__0__a 
           + __Vfunc_top__DOT__u_exu__DOT__alu_func__0__b);
    vlSelfRef.top__DOT__alu_result = __Vfunc_top__DOT__u_exu__DOT__alu_func__0__Vfuncout;
    __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_alu_result 
        = vlSelfRef.top__DOT__alu_result;
    __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_pc4 
        = ((IData)(4U) + vlSelfRef.pc);
    __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__Vfuncout 
        = ((0U == (IData)(__Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_wb_sel))
            ? __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_alu_result
            : ((1U == (IData)(__Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_wb_sel))
                ? __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_pc4
                : 0U));
    vlSelfRef.top__DOT__wb_data = __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__Vfuncout;
}

void Vtop___024root___eval_ico(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_ico\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VicoTriggered[0U])) {
        Vtop___024root___ico_sequent__TOP__0(vlSelf);
        vlSelfRef.__Vm_traceActivity[1U] = 1U;
    }
}

bool Vtop___024root___eval_phase__ico(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_phase__ico\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VicoExecute;
    // Body
    Vtop___024root___eval_triggers__ico(vlSelf);
    __VicoExecute = Vtop___024root___trigger_anySet__ico(vlSelfRef.__VicoTriggered);
    if (__VicoExecute) {
        Vtop___024root___eval_ico(vlSelf);
    }
    return (__VicoExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG

void Vtop___024root___eval_triggers__act(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_triggers__act\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VactTriggered[0U] = (QData)((IData)(
                                                    ((IData)(vlSelfRef.clk) 
                                                     & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__clk__0)))));
    vlSelfRef.__Vtrigprevexpr___TOP__clk__0 = vlSelfRef.clk;
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vtop___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
    }
#endif
}

bool Vtop___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___trigger_anySet__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        if (in[n]) {
            return (1U);
        }
        n = ((IData)(1U) + n);
    } while ((1U > n));
    return (0U);
}

void Vtop___024root___nba_sequent__TOP__0(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__0\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __Vfunc_top__DOT__u_exu__DOT__alu_func__0__Vfuncout;
    __Vfunc_top__DOT__u_exu__DOT__alu_func__0__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_top__DOT__u_exu__DOT__alu_func__0__a;
    __Vfunc_top__DOT__u_exu__DOT__alu_func__0__a = 0;
    IData/*31:0*/ __Vfunc_top__DOT__u_exu__DOT__alu_func__0__b;
    __Vfunc_top__DOT__u_exu__DOT__alu_func__0__b = 0;
    IData/*31:0*/ __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__Vfuncout;
    __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__Vfuncout = 0;
    CData/*2:0*/ __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_wb_sel;
    __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_wb_sel = 0;
    IData/*31:0*/ __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_pc4;
    __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_pc4 = 0;
    IData/*31:0*/ __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_alu_result;
    __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_alu_result = 0;
    IData/*31:0*/ __VdlyVal__top__DOT__u_regfile__DOT__rf__v0;
    __VdlyVal__top__DOT__u_regfile__DOT__rf__v0 = 0;
    CData/*4:0*/ __VdlyDim0__top__DOT__u_regfile__DOT__rf__v0;
    __VdlyDim0__top__DOT__u_regfile__DOT__rf__v0 = 0;
    CData/*0:0*/ __VdlySet__top__DOT__u_regfile__DOT__rf__v0;
    __VdlySet__top__DOT__u_regfile__DOT__rf__v0 = 0;
    // Body
    __VdlySet__top__DOT__u_regfile__DOT__rf__v0 = 0U;
    if (((IData)(vlSelfRef.top__DOT__u_exu__DOT__alu_src2_imm) 
         & (0U != (0x0000001fU & (vlSelfRef.inst >> 7U))))) {
        __VdlyVal__top__DOT__u_regfile__DOT__rf__v0 
            = vlSelfRef.top__DOT__wb_data;
        __VdlyDim0__top__DOT__u_regfile__DOT__rf__v0 
            = (0x0000001fU & (vlSelfRef.inst >> 7U));
        __VdlySet__top__DOT__u_regfile__DOT__rf__v0 = 1U;
    }
    vlSelfRef.pc = ((IData)(vlSelfRef.reset) ? 0U : vlSelfRef.top__DOT__u_pcreg__DOT__next_pc);
    if (__VdlySet__top__DOT__u_regfile__DOT__rf__v0) {
        vlSelfRef.top__DOT__u_regfile__DOT__rf[__VdlyDim0__top__DOT__u_regfile__DOT__rf__v0] 
            = __VdlyVal__top__DOT__u_regfile__DOT__rf__v0;
    }
    vlSelfRef.top__DOT__u_exu__DOT__rs1_data = ((0U 
                                                 == 
                                                 (0x0000001fU 
                                                  & (vlSelfRef.inst 
                                                     >> 0x0000000fU)))
                                                 ? 0U
                                                 : 
                                                vlSelfRef.top__DOT__u_regfile__DOT__rf
                                                [(0x0000001fU 
                                                  & (vlSelfRef.inst 
                                                     >> 0x0000000fU))]);
    if (vlSelfRef.top__DOT__u_idu__DOT__npc_sel) {
        vlSelfRef.top__DOT__u_pcreg__DOT__next_pc = 
            (0xfffffffeU & (vlSelfRef.top__DOT__u_exu__DOT__imm 
                            + vlSelfRef.top__DOT__u_exu__DOT__rs1_data));
        __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_wb_sel = 1U;
    } else {
        vlSelfRef.top__DOT__u_pcreg__DOT__next_pc = 
            ((IData)(4U) + vlSelfRef.pc);
        __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_wb_sel = 0U;
    }
    __Vfunc_top__DOT__u_exu__DOT__alu_func__0__b = 
        ((IData)(vlSelfRef.top__DOT__u_exu__DOT__alu_src2_imm)
          ? vlSelfRef.top__DOT__u_exu__DOT__imm : (
                                                   (0U 
                                                    == 
                                                    (0x0000001fU 
                                                     & (vlSelfRef.inst 
                                                        >> 0x00000014U)))
                                                    ? 0U
                                                    : 
                                                   vlSelfRef.top__DOT__u_regfile__DOT__rf
                                                   [
                                                   (0x0000001fU 
                                                    & (vlSelfRef.inst 
                                                       >> 0x00000014U))]));
    __Vfunc_top__DOT__u_exu__DOT__alu_func__0__a = vlSelfRef.top__DOT__u_exu__DOT__rs1_data;
    __Vfunc_top__DOT__u_exu__DOT__alu_func__0__Vfuncout 
        = (__Vfunc_top__DOT__u_exu__DOT__alu_func__0__a 
           + __Vfunc_top__DOT__u_exu__DOT__alu_func__0__b);
    vlSelfRef.top__DOT__alu_result = __Vfunc_top__DOT__u_exu__DOT__alu_func__0__Vfuncout;
    __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_alu_result 
        = vlSelfRef.top__DOT__alu_result;
    __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_pc4 
        = ((IData)(4U) + vlSelfRef.pc);
    __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__Vfuncout 
        = ((0U == (IData)(__Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_wb_sel))
            ? __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_alu_result
            : ((1U == (IData)(__Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_wb_sel))
                ? __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_pc4
                : 0U));
    vlSelfRef.top__DOT__wb_data = __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__Vfuncout;
}

void Vtop___024root___eval_nba(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_nba\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_sequent__TOP__0(vlSelf);
        vlSelfRef.__Vm_traceActivity[2U] = 1U;
    }
}

void Vtop___024root___trigger_orInto__act(VlUnpacked<QData/*63:0*/, 1> &out, const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___trigger_orInto__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = (out[n] | in[n]);
        n = ((IData)(1U) + n);
    } while ((1U > n));
}

bool Vtop___024root___eval_phase__act(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_phase__act\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    Vtop___024root___eval_triggers__act(vlSelf);
    Vtop___024root___trigger_orInto__act(vlSelfRef.__VnbaTriggered, vlSelfRef.__VactTriggered);
    return (0U);
}

void Vtop___024root___trigger_clear__act(VlUnpacked<QData/*63:0*/, 1> &out) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___trigger_clear__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = 0ULL;
        n = ((IData)(1U) + n);
    } while ((1U > n));
}

bool Vtop___024root___eval_phase__nba(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_phase__nba\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = Vtop___024root___trigger_anySet__act(vlSelfRef.__VnbaTriggered);
    if (__VnbaExecute) {
        Vtop___024root___eval_nba(vlSelf);
        Vtop___024root___trigger_clear__act(vlSelfRef.__VnbaTriggered);
    }
    return (__VnbaExecute);
}

void Vtop___024root___eval(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VicoIterCount;
    IData/*31:0*/ __VnbaIterCount;
    // Body
    __VicoIterCount = 0U;
    vlSelfRef.__VicoFirstIteration = 1U;
    do {
        if (VL_UNLIKELY(((0x00000064U < __VicoIterCount)))) {
#ifdef VL_DEBUG
            Vtop___024root___dump_triggers__ico(vlSelfRef.__VicoTriggered, "ico"s);
#endif
            VL_FATAL_MT("vsrc/top.v", 1, "", "Input combinational region did not converge after 100 tries");
        }
        __VicoIterCount = ((IData)(1U) + __VicoIterCount);
    } while (Vtop___024root___eval_phase__ico(vlSelf));
    __VnbaIterCount = 0U;
    do {
        if (VL_UNLIKELY(((0x00000064U < __VnbaIterCount)))) {
#ifdef VL_DEBUG
            Vtop___024root___dump_triggers__act(vlSelfRef.__VnbaTriggered, "nba"s);
#endif
            VL_FATAL_MT("vsrc/top.v", 1, "", "NBA region did not converge after 100 tries");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        vlSelfRef.__VactIterCount = 0U;
        do {
            if (VL_UNLIKELY(((0x00000064U < vlSelfRef.__VactIterCount)))) {
#ifdef VL_DEBUG
                Vtop___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
#endif
                VL_FATAL_MT("vsrc/top.v", 1, "", "Active region did not converge after 100 tries");
            }
            vlSelfRef.__VactIterCount = ((IData)(1U) 
                                         + vlSelfRef.__VactIterCount);
        } while (Vtop___024root___eval_phase__act(vlSelf));
    } while (Vtop___024root___eval_phase__nba(vlSelf));
}

#ifdef VL_DEBUG
void Vtop___024root___eval_debug_assertions(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_debug_assertions\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (VL_UNLIKELY(((vlSelfRef.clk & 0xfeU)))) {
        Verilated::overWidthError("clk");
    }
    if (VL_UNLIKELY(((vlSelfRef.reset & 0xfeU)))) {
        Verilated::overWidthError("reset");
    }
}
#endif  // VL_DEBUG
