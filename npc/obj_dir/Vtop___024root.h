// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vtop.h for the primary calling header

#ifndef VERILATED_VTOP___024ROOT_H_
#define VERILATED_VTOP___024ROOT_H_  // guard

#include "verilated.h"
class Vtop___024unit;


class Vtop__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vtop___024root final : public VerilatedModule {
  public:
    // CELLS
    Vtop___024unit* __PVT____024unit;

    // DESIGN SPECIFIC STATE
    VL_IN8(clk,0,0);
    VL_IN8(reset,0,0);
    CData/*2:0*/ top__DOT__imm_type;
    CData/*0:0*/ top__DOT__u_idu__DOT__rs2_en;
    CData/*0:0*/ top__DOT__u_idu__DOT__npc_sel;
    CData/*0:0*/ top__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_1;
    CData/*0:0*/ top__DOT__u_regfile__DOT____Vstrobe0;
    CData/*2:0*/ __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_wb_sel;
    CData/*0:0*/ __VstlFirstIteration;
    CData/*0:0*/ __VicoFirstIteration;
    CData/*0:0*/ __Vtrigprevexpr___TOP__clk__0;
    VL_IN(inst,31,0);
    VL_OUT(pc,31,0);
    IData/*31:0*/ top__DOT__alu_result;
    IData/*31:0*/ top__DOT__wb_data;
    IData/*31:0*/ top__DOT__u_exu__DOT__rs1_data;
    IData/*31:0*/ top__DOT__u_exu__DOT__imm;
    IData/*31:0*/ top__DOT__u_pcreg__DOT__next_pc;
    IData/*31:0*/ __Vfunc_top__DOT__u_exu__DOT__alu_func__0__Vfuncout;
    IData/*31:0*/ __Vfunc_top__DOT__u_exu__DOT__alu_func__0__a;
    IData/*31:0*/ __Vfunc_top__DOT__u_exu__DOT__alu_func__0__b;
    IData/*31:0*/ __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__Vfuncout;
    IData/*31:0*/ __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_pc4;
    IData/*31:0*/ __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_alu_result;
    IData/*31:0*/ __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_imm;
    IData/*31:0*/ __VactIterCount;
    VlUnpacked<IData/*31:0*/, 32> top__DOT__u_regfile__DOT__rf;
    VlUnpacked<QData/*63:0*/, 1> __VstlTriggered;
    VlUnpacked<QData/*63:0*/, 1> __VicoTriggered;
    VlUnpacked<QData/*63:0*/, 1> __VactTriggered;
    VlUnpacked<QData/*63:0*/, 1> __VnbaTriggered;
    VlUnpacked<CData/*0:0*/, 3> __Vm_traceActivity;

    // INTERNAL VARIABLES
    Vtop__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vtop___024root(Vtop__Syms* symsp, const char* v__name);
    ~Vtop___024root();
    VL_UNCOPYABLE(Vtop___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
