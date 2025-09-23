// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef VERILATED_VAMBER48_CORE_MISALIGNED_TB__SYMS_H_
#define VERILATED_VAMBER48_CORE_MISALIGNED_TB__SYMS_H_  // guard

#include "verilated.h"

// INCLUDE MODEL CLASS

#include "Vamber48_core_misaligned_tb.h"

// INCLUDE MODULE CLASSES
#include "Vamber48_core_misaligned_tb___024root.h"

// SYMS CLASS (contains all model state)
class alignas(VL_CACHE_LINE_BYTES)Vamber48_core_misaligned_tb__Syms final : public VerilatedSyms {
  public:
    // INTERNAL STATE
    Vamber48_core_misaligned_tb* const __Vm_modelp;
    VlDeleter __Vm_deleter;
    bool __Vm_didInit = false;

    // MODULE INSTANCE STATE
    Vamber48_core_misaligned_tb___024root TOP;

    // SCOPE NAMES
    VerilatedScope __Vscope_amber48_core_misaligned_tb;

    // CONSTRUCTORS
    Vamber48_core_misaligned_tb__Syms(VerilatedContext* contextp, const char* namep, Vamber48_core_misaligned_tb* modelp);
    ~Vamber48_core_misaligned_tb__Syms();

    // METHODS
    const char* name() { return TOP.name(); }
};

#endif  // guard
