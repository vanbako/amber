// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vamber48_core_smoke_tb.h for the primary calling header

#include "Vamber48_core_smoke_tb__pch.h"
#include "Vamber48_core_smoke_tb__Syms.h"
#include "Vamber48_core_smoke_tb___024root.h"

void Vamber48_core_smoke_tb___024root___ctor_var_reset(Vamber48_core_smoke_tb___024root* vlSelf);

Vamber48_core_smoke_tb___024root::Vamber48_core_smoke_tb___024root(Vamber48_core_smoke_tb__Syms* symsp, const char* v__name)
    : VerilatedModule{v__name}
    , __VdlySched{*symsp->_vm_contextp__}
    , vlSymsp{symsp}
 {
    // Reset structure values
    Vamber48_core_smoke_tb___024root___ctor_var_reset(this);
}

void Vamber48_core_smoke_tb___024root::__Vconfigure(bool first) {
    if (false && first) {}  // Prevent unused
}

Vamber48_core_smoke_tb___024root::~Vamber48_core_smoke_tb___024root() {
}
