// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vamber48_core_smoke_tb.h for the primary calling header

#include "Vamber48_core_smoke_tb__pch.h"
#include "Vamber48_core_smoke_tb__Syms.h"
#include "Vamber48_core_smoke_tb___024root.h"

#ifdef VL_DEBUG
VL_ATTR_COLD void Vamber48_core_smoke_tb___024root___dump_triggers__stl(Vamber48_core_smoke_tb___024root* vlSelf);
#endif  // VL_DEBUG

VL_ATTR_COLD void Vamber48_core_smoke_tb___024root___eval_triggers__stl(Vamber48_core_smoke_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vamber48_core_smoke_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vamber48_core_smoke_tb___024root___eval_triggers__stl\n"); );
    // Body
    vlSelf->__VstlTriggered.set(0U, (IData)(vlSelf->__VstlFirstIteration));
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vamber48_core_smoke_tb___024root___dump_triggers__stl(vlSelf);
    }
#endif
}
