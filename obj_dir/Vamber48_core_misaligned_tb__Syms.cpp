// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table implementation internals

#include "Vamber48_core_misaligned_tb__pch.h"
#include "Vamber48_core_misaligned_tb.h"
#include "Vamber48_core_misaligned_tb___024root.h"

// FUNCTIONS
Vamber48_core_misaligned_tb__Syms::~Vamber48_core_misaligned_tb__Syms()
{
}

Vamber48_core_misaligned_tb__Syms::Vamber48_core_misaligned_tb__Syms(VerilatedContext* contextp, const char* namep, Vamber48_core_misaligned_tb* modelp)
    : VerilatedSyms{contextp}
    // Setup internal state of the Syms class
    , __Vm_modelp{modelp}
    // Setup module instances
    , TOP{this, namep}
{
    // Configure time unit / time precision
    _vm_contextp__->timeunit(-9);
    _vm_contextp__->timeprecision(-12);
    // Setup each module's pointers to their submodules
    // Setup each module's pointer back to symbol table (for public functions)
    TOP.__Vconfigure(true);
    // Setup scopes
    __Vscope_amber48_core_misaligned_tb.configure(this, name(), "amber48_core_misaligned_tb", "amber48_core_misaligned_tb", -9, VerilatedScope::SCOPE_OTHER);
}
