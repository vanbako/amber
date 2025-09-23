// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vamber48_core_smoke_tb__pch.h"

//============================================================
// Constructors

Vamber48_core_smoke_tb::Vamber48_core_smoke_tb(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vamber48_core_smoke_tb__Syms(contextp(), _vcname__, this)}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

Vamber48_core_smoke_tb::Vamber48_core_smoke_tb(const char* _vcname__)
    : Vamber48_core_smoke_tb(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vamber48_core_smoke_tb::~Vamber48_core_smoke_tb() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vamber48_core_smoke_tb___024root___eval_debug_assertions(Vamber48_core_smoke_tb___024root* vlSelf);
#endif  // VL_DEBUG
void Vamber48_core_smoke_tb___024root___eval_static(Vamber48_core_smoke_tb___024root* vlSelf);
void Vamber48_core_smoke_tb___024root___eval_initial(Vamber48_core_smoke_tb___024root* vlSelf);
void Vamber48_core_smoke_tb___024root___eval_settle(Vamber48_core_smoke_tb___024root* vlSelf);
void Vamber48_core_smoke_tb___024root___eval(Vamber48_core_smoke_tb___024root* vlSelf);

void Vamber48_core_smoke_tb::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vamber48_core_smoke_tb::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vamber48_core_smoke_tb___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        vlSymsp->__Vm_didInit = true;
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        Vamber48_core_smoke_tb___024root___eval_static(&(vlSymsp->TOP));
        Vamber48_core_smoke_tb___024root___eval_initial(&(vlSymsp->TOP));
        Vamber48_core_smoke_tb___024root___eval_settle(&(vlSymsp->TOP));
    }
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    Vamber48_core_smoke_tb___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool Vamber48_core_smoke_tb::eventsPending() { return !vlSymsp->TOP.__VdlySched.empty(); }

uint64_t Vamber48_core_smoke_tb::nextTimeSlot() { return vlSymsp->TOP.__VdlySched.nextTimeSlot(); }

//============================================================
// Utilities

const char* Vamber48_core_smoke_tb::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void Vamber48_core_smoke_tb___024root___eval_final(Vamber48_core_smoke_tb___024root* vlSelf);

VL_ATTR_COLD void Vamber48_core_smoke_tb::final() {
    Vamber48_core_smoke_tb___024root___eval_final(&(vlSymsp->TOP));
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vamber48_core_smoke_tb::hierName() const { return vlSymsp->name(); }
const char* Vamber48_core_smoke_tb::modelName() const { return "Vamber48_core_smoke_tb"; }
unsigned Vamber48_core_smoke_tb::threads() const { return 1; }
void Vamber48_core_smoke_tb::prepareClone() const { contextp()->prepareClone(); }
void Vamber48_core_smoke_tb::atClone() const {
    contextp()->threadPoolpOnClone();
}

//============================================================
// Trace configuration

VL_ATTR_COLD void Vamber48_core_smoke_tb::trace(VerilatedVcdC* tfp, int levels, int options) {
    vl_fatal(__FILE__, __LINE__, __FILE__,"'Vamber48_core_smoke_tb::trace()' called on model that was Verilated without --trace option");
}
