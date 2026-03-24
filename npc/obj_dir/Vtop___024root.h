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
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        VL_IN8(clk,0,0);
        VL_IN8(reset,0,0);
        CData/*3:0*/ top__DOT__alu_op;
        CData/*0:0*/ top__DOT__u_idu__DOT__mem_re;
        CData/*1:0*/ top__DOT__u_idu__DOT__mem_width;
        CData/*0:0*/ top__DOT__u_idu__DOT__npc_sel;
        CData/*2:0*/ top__DOT__u_idu__DOT__imm_type;
        CData/*0:0*/ top__DOT__u_idu__DOT__is_addi;
        CData/*0:0*/ top__DOT__u_idu__DOT__is_add;
        CData/*0:0*/ top__DOT__u_idu__DOT__is_sub;
        CData/*0:0*/ top__DOT__u_idu__DOT__is_and;
        CData/*0:0*/ top__DOT__u_idu__DOT__is_or;
        CData/*0:0*/ top__DOT__u_idu__DOT__is_xor;
        CData/*0:0*/ top__DOT__u_idu__DOT__is_slt;
        CData/*0:0*/ top__DOT__u_idu__DOT__is_sltu;
        CData/*0:0*/ top__DOT__u_idu__DOT__is_sll;
        CData/*0:0*/ top__DOT__u_idu__DOT__is_srl;
        CData/*0:0*/ top__DOT__u_idu__DOT__is_sra;
        CData/*0:0*/ top__DOT__u_idu__DOT__is_sb;
        CData/*0:0*/ top__DOT__u_idu__DOT__is_sw;
        CData/*0:0*/ top__DOT__u_idu__DOT__is_lbu;
        CData/*0:0*/ top__DOT__u_idu__DOT__is_lw;
        CData/*0:0*/ top__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_3;
        CData/*0:0*/ top__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_10;
        CData/*0:0*/ top__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_11;
        CData/*0:0*/ top__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_14;
        CData/*0:0*/ top__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_15;
        CData/*0:0*/ top__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_16;
        CData/*0:0*/ top__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_18;
        CData/*0:0*/ top__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_19;
        CData/*0:0*/ top__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_20;
        CData/*0:0*/ top__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_21;
        CData/*3:0*/ __Vfunc_top__DOT__u_exu__DOT__alu_func__0__op;
        CData/*2:0*/ __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_wb_sel;
        CData/*7:0*/ __Vfunc_pmem_read_u8__4__Vfuncout;
        CData/*6:0*/ __Vtableidx1;
        CData/*0:0*/ __VstlFirstIteration;
        CData/*0:0*/ __VicoFirstIteration;
        CData/*0:0*/ __Vtrigprevexpr___TOP__clk__0;
        SData/*8:0*/ __Vtableidx2;
        VL_IN(inst,31,0);
        VL_OUT(pc,31,0);
        IData/*31:0*/ top__DOT__alu_result;
        IData/*31:0*/ top__DOT__wb_data;
        IData/*31:0*/ top__DOT__rdata;
        IData/*31:0*/ top__DOT__u_exu__DOT__rs1_data;
        IData/*31:0*/ top__DOT__u_exu__DOT__imm;
        IData/*31:0*/ top__DOT__u_exu__DOT__j_target;
        IData/*31:0*/ top__DOT__u_pcreg__DOT__next_pc;
        IData/*31:0*/ __Vfunc_top__DOT__u_exu__DOT__alu_func__0__Vfuncout;
        IData/*31:0*/ __Vfunc_top__DOT__u_exu__DOT__alu_func__0__a;
        IData/*31:0*/ __Vfunc_top__DOT__u_exu__DOT__alu_func__0__b;
        IData/*31:0*/ __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__Vfuncout;
        IData/*31:0*/ __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_pc4;
        IData/*31:0*/ __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_alu_result;
        IData/*31:0*/ __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_mem_data;
        IData/*31:0*/ __Vfunc_top__DOT__u_wbu__DOT__wb_func__1__i_imm;
        IData/*31:0*/ __Vfunc_pmem_read_u32__5__Vfuncout;
        IData/*31:0*/ __VactIterCount;
        VlUnpacked<IData/*31:0*/, 32> top__DOT__u_regfile__DOT__rf;
        VlUnpacked<QData/*63:0*/, 1> __VstlTriggered;
        VlUnpacked<QData/*63:0*/, 1> __VicoTriggered;
        VlUnpacked<QData/*63:0*/, 1> __VactTriggered;
        VlUnpacked<QData/*63:0*/, 1> __VnbaTriggered;
    };
    struct {
        VlUnpacked<CData/*0:0*/, 3> __Vm_traceActivity;
    };

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
