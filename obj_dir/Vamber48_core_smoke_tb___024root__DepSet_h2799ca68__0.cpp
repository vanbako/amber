// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vamber48_core_smoke_tb.h for the primary calling header

#include "Vamber48_core_smoke_tb__pch.h"
#include "Vamber48_core_smoke_tb__Syms.h"
#include "Vamber48_core_smoke_tb___024root.h"

VL_INLINE_OPT VlCoroutine Vamber48_core_smoke_tb___024root___eval_initial__TOP__Vtiming__1(Vamber48_core_smoke_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vamber48_core_smoke_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vamber48_core_smoke_tb___024root___eval_initial__TOP__Vtiming__1\n"); );
    // Body
    while ((1U & (~ (IData)(vlSelf->amber48_core_smoke_tb__DOT__rst_ni)))) {
        co_await vlSelf->__VtrigSched_hd1065de6__0.trigger(1U, 
                                                           nullptr, 
                                                           "@([changed] amber48_core_smoke_tb.rst_ni)", 
                                                           "src/0.1/sim/tb/amber48_core_tb_base.svh", 
                                                           188);
    }
    while (1U) {
        co_await vlSelf->__VtrigSched_hb844875a__0.trigger(0U, 
                                                           nullptr, 
                                                           "@(posedge amber48_core_smoke_tb.clk)", 
                                                           "src/0.1/sim/tb/amber48_core_tb_base.svh", 
                                                           190);
        vlSelf->amber48_core_smoke_tb__DOT__cycle_count 
            = ((IData)(1U) + vlSelf->amber48_core_smoke_tb__DOT__cycle_count);
        if (VL_UNLIKELY(VL_LTS_III(32, 0x4c4b40U, vlSelf->amber48_core_smoke_tb__DOT__cycle_count))) {
            VL_WRITEF("[%0t] %%Fatal: amber48_core_tb_base.svh:193: Assertion failed in %Namber48_core_smoke_tb: [%0t][smoke] Simulation timed out after %0d cycles\n",
                      64,VL_TIME_UNITED_Q(1000),-9,
                      vlSymsp->name(),64,VL_TIME_UNITED_Q(1000),
                      -9,32,vlSelf->amber48_core_smoke_tb__DOT__cycle_count);
            VL_STOP_MT("src/0.1/sim/tb/amber48_core_tb_base.svh", 193, "");
        }
    }
}

VL_INLINE_OPT VlCoroutine Vamber48_core_smoke_tb___024root___eval_initial__TOP__Vtiming__3(Vamber48_core_smoke_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vamber48_core_smoke_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vamber48_core_smoke_tb___024root___eval_initial__TOP__Vtiming__3\n"); );
    // Init
    std::string __Vfunc_amber48_core_smoke_tb__DOT__trap_to_string__0__Vfuncout;
    CData/*2:0*/ __Vfunc_amber48_core_smoke_tb__DOT__trap_to_string__0__trap;
    __Vfunc_amber48_core_smoke_tb__DOT__trap_to_string__0__trap = 0;
    std::string __Vfunc_amber48_core_smoke_tb__DOT__trap_to_string__1__Vfuncout;
    CData/*2:0*/ __Vfunc_amber48_core_smoke_tb__DOT__trap_to_string__1__trap;
    __Vfunc_amber48_core_smoke_tb__DOT__trap_to_string__1__trap = 0;
    std::string __Vfunc_amber48_core_smoke_tb__DOT__trap_to_string__2__Vfuncout;
    CData/*2:0*/ __Vfunc_amber48_core_smoke_tb__DOT__trap_to_string__2__trap;
    __Vfunc_amber48_core_smoke_tb__DOT__trap_to_string__2__trap = 0;
    std::string __Vtemp_1;
    std::string __Vtemp_2;
    std::string __Vtemp_3;
    // Body
    while (1U) {
        co_await vlSelf->__VtrigSched_hb844875a__0.trigger(0U, 
                                                           nullptr, 
                                                           "@(posedge amber48_core_smoke_tb.clk)", 
                                                           "src/0.1/sim/tb/amber48_core_tb_base.svh", 
                                                           157);
        if (vlSelf->amber48_core_smoke_tb__DOT__rst_ni) {
            if (VL_UNLIKELY((((vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[0U] 
                               >> 3U) & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[4U] 
                                         >> 0x1cU)) 
                             & (~ (IData)(vlSelf->amber48_core_smoke_tb__DOT__trap_seen))))) {
                vlSelf->__Vdlyvset__amber48_core_smoke_tb__DOT__trap_seen__v0 = 1U;
                vlSelf->amber48_core_smoke_tb__DOT__actual_trap 
                    = (7U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[0U]);
                vlSelf->amber48_core_smoke_tb__DOT__expected_trap_e = 1U;
                if (VL_UNLIKELY((1U != (IData)(vlSelf->amber48_core_smoke_tb__DOT__actual_trap)))) {
                    __Vtemp_1 = ([&]() {
                            __Vfunc_amber48_core_smoke_tb__DOT__trap_to_string__0__trap 
                                = vlSelf->amber48_core_smoke_tb__DOT__actual_trap;
                            __Vfunc_amber48_core_smoke_tb__DOT__trap_to_string__0__Vfuncout 
                                = ((0U == (IData)(__Vfunc_amber48_core_smoke_tb__DOT__trap_to_string__0__trap))
                                    ? std::string{"TRAP_NONE"}
                                    : ((1U == (IData)(__Vfunc_amber48_core_smoke_tb__DOT__trap_to_string__0__trap))
                                        ? std::string{"TRAP_ILLEGAL"}
                                        : ((2U == (IData)(__Vfunc_amber48_core_smoke_tb__DOT__trap_to_string__0__trap))
                                            ? std::string{"TRAP_DATA_FAULT"}
                                            : VL_SFORMATF_NX("UNKNOWN_%0#",
                                                             3,
                                                             __Vfunc_amber48_core_smoke_tb__DOT__trap_to_string__0__trap) )));
                        }(), __Vfunc_amber48_core_smoke_tb__DOT__trap_to_string__0__Vfuncout);
                    __Vtemp_2 = ([&]() {
                            __Vfunc_amber48_core_smoke_tb__DOT__trap_to_string__1__trap 
                                = vlSelf->amber48_core_smoke_tb__DOT__expected_trap_e;
                            __Vfunc_amber48_core_smoke_tb__DOT__trap_to_string__1__Vfuncout 
                                = ((0U == (IData)(__Vfunc_amber48_core_smoke_tb__DOT__trap_to_string__1__trap))
                                    ? std::string{"TRAP_NONE"}
                                    : ((1U == (IData)(__Vfunc_amber48_core_smoke_tb__DOT__trap_to_string__1__trap))
                                        ? std::string{"TRAP_ILLEGAL"}
                                        : ((2U == (IData)(__Vfunc_amber48_core_smoke_tb__DOT__trap_to_string__1__trap))
                                            ? std::string{"TRAP_DATA_FAULT"}
                                            : VL_SFORMATF_NX("UNKNOWN_%0#",
                                                             3,
                                                             __Vfunc_amber48_core_smoke_tb__DOT__trap_to_string__1__trap) )));
                        }(), __Vfunc_amber48_core_smoke_tb__DOT__trap_to_string__1__Vfuncout);
                    VL_WRITEF("[%0t] %%Fatal: amber48_core_tb_base.svh:165: Assertion failed in %Namber48_core_smoke_tb: [%0t][smoke] Unexpected trap cause %@ (expected %@)\n",
                              64,VL_TIME_UNITED_Q(1000),
                              -9,vlSymsp->name(),64,
                              VL_TIME_UNITED_Q(1000),
                              -9,-1,&(__Vtemp_1),-1,
                              &(__Vtemp_2));
                    VL_STOP_MT("src/0.1/sim/tb/amber48_core_tb_base.svh", 165, "");
                }
                if (VL_LIKELY(vlSelf->amber48_core_smoke_tb__DOT__store0_seen)) {
                    VL_WRITEF("[%0t][smoke] Observed DMEM[0] store\n",
                              64,VL_TIME_UNITED_Q(1000),
                              -9);
                } else {
                    VL_WRITEF("[%0t] %%Fatal: amber48_core_tb_base.svh:170: Assertion failed in %Namber48_core_smoke_tb: [%0t][smoke] Expected DMEM[0] store not observed\n",
                              64,VL_TIME_UNITED_Q(1000),
                              -9,vlSymsp->name(),64,
                              VL_TIME_UNITED_Q(1000),
                              -9);
                    VL_STOP_MT("src/0.1/sim/tb/amber48_core_tb_base.svh", 170, "");
                }
                if (VL_UNLIKELY((0U != vlSelf->amber48_core_smoke_tb__DOT__uart_count))) {
                    VL_WRITEF("[%0t][smoke] UART produced %0d bytes\n",
                              64,VL_TIME_UNITED_Q(1000),
                              -9,32,vlSelf->amber48_core_smoke_tb__DOT__uart_count);
                }
                __Vtemp_3 = ([&]() {
                        __Vfunc_amber48_core_smoke_tb__DOT__trap_to_string__2__trap 
                            = vlSelf->amber48_core_smoke_tb__DOT__actual_trap;
                        __Vfunc_amber48_core_smoke_tb__DOT__trap_to_string__2__Vfuncout 
                            = ((0U == (IData)(__Vfunc_amber48_core_smoke_tb__DOT__trap_to_string__2__trap))
                                ? std::string{"TRAP_NONE"}
                                : ((1U == (IData)(__Vfunc_amber48_core_smoke_tb__DOT__trap_to_string__2__trap))
                                    ? std::string{"TRAP_ILLEGAL"}
                                    : ((2U == (IData)(__Vfunc_amber48_core_smoke_tb__DOT__trap_to_string__2__trap))
                                        ? std::string{"TRAP_DATA_FAULT"}
                                        : VL_SFORMATF_NX("UNKNOWN_%0#",
                                                         3,
                                                         __Vfunc_amber48_core_smoke_tb__DOT__trap_to_string__2__trap) )));
                    }(), __Vfunc_amber48_core_smoke_tb__DOT__trap_to_string__2__Vfuncout);
                VL_WRITEF("[%0t][smoke] Trap asserted (cause=%@), ending simulation\n",
                          64,VL_TIME_UNITED_Q(1000),
                          -9,-1,&(__Vtemp_3));
                co_await vlSelf->__VtrigSched_hb844875a__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge amber48_core_smoke_tb.clk)", 
                                                                   "src/0.1/sim/tb/amber48_core_tb_base.svh", 
                                                                   182);
                co_await vlSelf->__VtrigSched_hb844875a__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge amber48_core_smoke_tb.clk)", 
                                                                   "src/0.1/sim/tb/amber48_core_tb_base.svh", 
                                                                   182);
                co_await vlSelf->__VtrigSched_hb844875a__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge amber48_core_smoke_tb.clk)", 
                                                                   "src/0.1/sim/tb/amber48_core_tb_base.svh", 
                                                                   182);
                co_await vlSelf->__VtrigSched_hb844875a__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge amber48_core_smoke_tb.clk)", 
                                                                   "src/0.1/sim/tb/amber48_core_tb_base.svh", 
                                                                   182);
                co_await vlSelf->__VtrigSched_hb844875a__0.trigger(0U, 
                                                                   nullptr, 
                                                                   "@(posedge amber48_core_smoke_tb.clk)", 
                                                                   "src/0.1/sim/tb/amber48_core_tb_base.svh", 
                                                                   182);
                VL_FINISH_MT("src/0.1/sim/tb/amber48_core_tb_base.svh", 183, "");
            }
        } else {
            vlSelf->__Vdlyvset__amber48_core_smoke_tb__DOT__trap_seen__v1 = 1U;
        }
    }
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vamber48_core_smoke_tb___024root___dump_triggers__act(Vamber48_core_smoke_tb___024root* vlSelf);
#endif  // VL_DEBUG

void Vamber48_core_smoke_tb___024root___eval_triggers__act(Vamber48_core_smoke_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vamber48_core_smoke_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vamber48_core_smoke_tb___024root___eval_triggers__act\n"); );
    // Body
    vlSelf->__VactTriggered.set(0U, ((IData)(vlSelf->amber48_core_smoke_tb__DOT__clk) 
                                     & (~ (IData)(vlSelf->__Vtrigprevexpr___TOP__amber48_core_smoke_tb__DOT__clk__0))));
    vlSelf->__VactTriggered.set(1U, (((IData)(vlSelf->amber48_core_smoke_tb__DOT__clk) 
                                      & (~ (IData)(vlSelf->__Vtrigprevexpr___TOP__amber48_core_smoke_tb__DOT__clk__0))) 
                                     | ((~ (IData)(vlSelf->amber48_core_smoke_tb__DOT__rst_ni)) 
                                        & (IData)(vlSelf->__Vtrigprevexpr___TOP__amber48_core_smoke_tb__DOT__rst_ni__0))));
    vlSelf->__VactTriggered.set(2U, ((IData)(vlSelf->amber48_core_smoke_tb__DOT__rst_ni) 
                                     != (IData)(vlSelf->__Vtrigprevexpr___TOP__amber48_core_smoke_tb__DOT__rst_ni__0)));
    vlSelf->__VactTriggered.set(3U, vlSelf->__VdlySched.awaitingCurrentTime());
    vlSelf->__Vtrigprevexpr___TOP__amber48_core_smoke_tb__DOT__clk__0 
        = vlSelf->amber48_core_smoke_tb__DOT__clk;
    vlSelf->__Vtrigprevexpr___TOP__amber48_core_smoke_tb__DOT__rst_ni__0 
        = vlSelf->amber48_core_smoke_tb__DOT__rst_ni;
    if (VL_UNLIKELY((1U & (~ (IData)(vlSelf->__VactDidInit))))) {
        vlSelf->__VactDidInit = 1U;
        vlSelf->__VactTriggered.set(2U, 1U);
    }
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vamber48_core_smoke_tb___024root___dump_triggers__act(vlSelf);
    }
#endif
}
