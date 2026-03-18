// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtop.h for the primary calling header

#include "Vtop__pch.h"

extern "C" void npc_ebreak(int code);

void Vtop___024unit____Vdpiimwrap_npc_ebreak_TOP____024unit(IData/*31:0*/ code) {
    VL_DEBUG_IF(VL_DBG_MSGF("+        Vtop___024unit____Vdpiimwrap_npc_ebreak_TOP____024unit\n"); );
    // Body
    int code__Vcvt;
    code__Vcvt = code;
    npc_ebreak(code__Vcvt);
}
