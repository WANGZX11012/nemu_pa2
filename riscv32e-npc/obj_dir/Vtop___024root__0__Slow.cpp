// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtop.h for the primary calling header

#include "Vtop__pch.h"

VL_ATTR_COLD void Vtop___024root___eval_static(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_static\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__Vtrigprevexpr___TOP__clk__0 = vlSelfRef.clk;
}

VL_ATTR_COLD void Vtop___024root___eval_initial__TOP(Vtop___024root* vlSelf);
VL_ATTR_COLD void Vtop___024root____Vm_traceActivitySetAll(Vtop___024root* vlSelf);

VL_ATTR_COLD void Vtop___024root___eval_initial(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_initial\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    Vtop___024root___eval_initial__TOP(vlSelf);
    Vtop___024root____Vm_traceActivitySetAll(vlSelf);
}

VL_ATTR_COLD void Vtop___024root___eval_initial__TOP(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_initial__TOP\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf[0U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf_read[0U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf[1U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf_read[1U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf[2U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf_read[2U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf[3U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf_read[3U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf[4U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf_read[4U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf[5U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf_read[5U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf[6U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf_read[6U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf[7U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf_read[7U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf[8U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf_read[8U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf[9U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf_read[9U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf[0x0aU] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf_read[0x0aU] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf[0x0bU] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf_read[0x0bU] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf[0x0cU] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf_read[0x0cU] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf[0x0dU] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf_read[0x0dU] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf[0x0eU] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf_read[0x0eU] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf[0x0fU] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf_read[0x0fU] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf[0x10U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf_read[0x10U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf[0x11U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf_read[0x11U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf[0x12U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf_read[0x12U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf[0x13U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf_read[0x13U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf[0x14U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf_read[0x14U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf[0x15U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf_read[0x15U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf[0x16U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf_read[0x16U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf[0x17U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf_read[0x17U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf[0x18U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf_read[0x18U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf[0x19U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf_read[0x19U] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf[0x1aU] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf_read[0x1aU] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf[0x1bU] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf_read[0x1bU] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf[0x1cU] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf_read[0x1cU] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf[0x1dU] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf_read[0x1dU] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf[0x1eU] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf_read[0x1eU] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf[0x1fU] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__rf_read[0x1fU] = 0U;
    vlSelfRef.top__DOT__u_core__DOT__u_regfile__DOT__i = 0x00000020U;
    vlSelfRef.pc = 0x80000000U;
}

VL_ATTR_COLD void Vtop___024root___eval_final(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_final\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vtop___024root___eval_phase__stl(Vtop___024root* vlSelf);

VL_ATTR_COLD void Vtop___024root___eval_settle(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_settle\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VstlIterCount;
    // Body
    __VstlIterCount = 0U;
    vlSelfRef.__VstlFirstIteration = 1U;
    do {
        if (VL_UNLIKELY(((0x00000064U < __VstlIterCount)))) {
#ifdef VL_DEBUG
            Vtop___024root___dump_triggers__stl(vlSelfRef.__VstlTriggered, "stl"s);
#endif
            VL_FATAL_MT("vsrc/top.v", 1, "", "Settle region did not converge after 100 tries");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
    } while (Vtop___024root___eval_phase__stl(vlSelf));
}

VL_ATTR_COLD void Vtop___024root___eval_triggers__stl(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_triggers__stl\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VstlTriggered[0U] = ((0xfffffffffffffffeULL 
                                      & vlSelfRef.__VstlTriggered
                                      [0U]) | (IData)((IData)(vlSelfRef.__VstlFirstIteration)));
    vlSelfRef.__VstlFirstIteration = 0U;
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vtop___024root___dump_triggers__stl(vlSelfRef.__VstlTriggered, "stl"s);
    }
#endif
}

VL_ATTR_COLD bool Vtop___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___dump_triggers__stl\n"); );
    // Body
    if ((1U & (~ (IData)(Vtop___024root___trigger_anySet__stl(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD bool Vtop___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___trigger_anySet__stl\n"); );
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

void Vtop___024root___ico_sequent__TOP__0(Vtop___024root* vlSelf);

VL_ATTR_COLD void Vtop___024root___eval_stl(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_stl\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VstlTriggered[0U])) {
        Vtop___024root___ico_sequent__TOP__0(vlSelf);
        Vtop___024root____Vm_traceActivitySetAll(vlSelf);
    }
}

VL_ATTR_COLD bool Vtop___024root___eval_phase__stl(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_phase__stl\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VstlExecute;
    // Body
    Vtop___024root___eval_triggers__stl(vlSelf);
    __VstlExecute = Vtop___024root___trigger_anySet__stl(vlSelfRef.__VstlTriggered);
    if (__VstlExecute) {
        Vtop___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

bool Vtop___024root___trigger_anySet__ico(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__ico(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___dump_triggers__ico\n"); );
    // Body
    if ((1U & (~ (IData)(Vtop___024root___trigger_anySet__ico(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: Internal 'ico' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

bool Vtop___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___dump_triggers__act\n"); );
    // Body
    if ((1U & (~ (IData)(Vtop___024root___trigger_anySet__act(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: @(posedge clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 1U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 1 is active: @(negedge clk)\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vtop___024root____Vm_traceActivitySetAll(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root____Vm_traceActivitySetAll\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__Vm_traceActivity[0U] = 1U;
    vlSelfRef.__Vm_traceActivity[1U] = 1U;
    vlSelfRef.__Vm_traceActivity[2U] = 1U;
    vlSelfRef.__Vm_traceActivity[3U] = 1U;
    vlSelfRef.__Vm_traceActivity[4U] = 1U;
}

VL_ATTR_COLD void Vtop___024root___ctor_var_reset(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___ctor_var_reset\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->name());
    vlSelf->clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16707436170211756652ull);
    vlSelf->reset = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9928399931838511862ull);
    vlSelf->inst = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9812503827101699671ull);
    vlSelf->pc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4211327832146562899ull);
    vlSelf->top__DOT__u_core__DOT__imm = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3520739126106933177ull);
    vlSelf->top__DOT__u_core__DOT__alu_op = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 3306366242686482610ull);
    vlSelf->top__DOT__u_core__DOT__alu_src2_imm = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4805873606988371846ull);
    vlSelf->top__DOT__u_core__DOT__alu_en = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11934238896666575621ull);
    vlSelf->top__DOT__u_core__DOT__npc_sel = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 8662463667445798238ull);
    vlSelf->top__DOT__u_core__DOT__mem_width = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 14096419774350806246ull);
    vlSelf->top__DOT__u_core__DOT__mem_signed = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14227848951931522759ull);
    vlSelf->top__DOT__u_core__DOT__branch_type = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 10683641873153630806ull);
    vlSelf->top__DOT__u_core__DOT__r_data1 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11555001605828081059ull);
    vlSelf->top__DOT__u_core__DOT__r_data2 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5671911980500339124ull);
    vlSelf->top__DOT__u_core__DOT__alu_result = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1155168294279643942ull);
    vlSelf->top__DOT__u_core__DOT__wb_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 652582278752513991ull);
    vlSelf->top__DOT__u_core__DOT__rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6613131841481172456ull);
    vlSelf->top__DOT__u_core__DOT__next_pc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11009252348752645947ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__imm_type = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 10777651286093629664ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_jal = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4700438736010278649ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_jalr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5911616755744786795ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_bne = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8278628661212228373ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_beq = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14625830661895210039ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_bge = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17705827921820764543ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_bgeu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12136690001490381659ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_blt = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13567842607339157131ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_bltu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3587836951906065746ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_lui = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1286915713492780378ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_lbu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7058693003741154891ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_lw = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16257821205712055673ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_lb = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10559573943346323212ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_lh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8596912780177831878ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_lhu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10803052931109656963ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_ebreak = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7305299295051968556ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_sb = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6805191172132192439ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_sw = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12353280736211899847ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_sh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9721683453050455039ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_auipc = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6594337490679511688ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_or = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3640421751231576906ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_xor = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13749992253058903320ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_xori = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2793481913425343719ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_add = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14232611244542864972ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_addi = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10631999269481575860ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_sub = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12194503831034511421ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_slti = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1184252700634015244ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_sltiu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17901600433668539917ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_and = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9700730455498750972ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_andi = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12228106381493398129ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_sra = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7737914307449062341ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_srai = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17194209821820535021ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_srl = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9647128041837258244ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_sll = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1861033851532355076ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_slli = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4112893902196594697ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_srli = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11382363868574851059ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_sltu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13425941053769988825ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT__is_slt = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12221280317410100070ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16812701022399024948ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_2 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7355725715445529996ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_3 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2869773297947823681ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_4 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4425963065332469969ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_5 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11804624413738584303ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_6 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13077037539567635656ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_7 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14633227306952317048ull);
    vlSelf->top__DOT__u_core__DOT__u_idu__DOT____VdfgRegularize_h52656aab_0_8 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3565144581648978258ull);
    for (int __Vi0 = 0; __Vi0 < 32; ++__Vi0) {
        vlSelf->top__DOT__u_core__DOT__u_regfile__DOT__rf[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1950181590513781821ull);
    }
    for (int __Vi0 = 0; __Vi0 < 32; ++__Vi0) {
        vlSelf->top__DOT__u_core__DOT__u_regfile__DOT__rf_read[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15453388396797778344ull);
    }
    vlSelf->top__DOT__u_core__DOT__u_regfile__DOT__i = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6853244472217046614ull);
    vlSelf->top__DOT__u_core__DOT__u_lsu__DOT__unnamedblk1__DOT__b = 0;
    vlSelf->top__DOT__u_core__DOT__u_lsu__DOT__unnamedblk2__DOT__half = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 4053614899770337278ull);
    vlSelf->__Vfunc_top__DOT__u_core__DOT__u_exu__DOT__alu_func__0__Vfuncout = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3214592274859737655ull);
    vlSelf->__Vfunc_top__DOT__u_core__DOT__u_exu__DOT__alu_func__0__op = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 15656321812908795410ull);
    vlSelf->__Vfunc_top__DOT__u_core__DOT__u_exu__DOT__alu_func__0__a = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8245943735474502055ull);
    vlSelf->__Vfunc_top__DOT__u_core__DOT__u_exu__DOT__alu_func__0__b = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3339212414758869393ull);
    vlSelf->__Vfunc_top__DOT__u_core__DOT__u_wbu__DOT__wb_func__1__Vfuncout = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 740704769889417577ull);
    vlSelf->__Vfunc_top__DOT__u_core__DOT__u_wbu__DOT__wb_func__1__i_wb_sel = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 15402642188813563476ull);
    vlSelf->__Vfunc_top__DOT__u_core__DOT__u_wbu__DOT__wb_func__1__i_pc4 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1229776906034142216ull);
    vlSelf->__Vfunc_top__DOT__u_core__DOT__u_wbu__DOT__wb_func__1__i_alu_result = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 160928021110964323ull);
    vlSelf->__Vfunc_top__DOT__u_core__DOT__u_wbu__DOT__wb_func__1__i_mem_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13265145759209831598ull);
    vlSelf->__Vfunc_top__DOT__u_core__DOT__u_wbu__DOT__wb_func__1__i_imm = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14316497521881984293ull);
    vlSelf->__Vfunc_pmem_read_u8__5__Vfuncout = 0;
    vlSelf->__Vfunc_pmem_read_u16__6__Vfuncout = 0;
    vlSelf->__Vfunc_pmem_read_u32__7__Vfuncout = 0;
    vlSelf->__Vtableidx1 = 0;
    vlSelf->__Vtableidx2 = 0;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VstlTriggered[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VicoTriggered[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VactTriggered[__Vi0] = 0;
    }
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9526919608049418986ull);
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VnbaTriggered[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 5; ++__Vi0) {
        vlSelf->__Vm_traceActivity[__Vi0] = 0;
    }
}
