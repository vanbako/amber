// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vamber48_core_misaligned_tb.h for the primary calling header

#ifndef VERILATED_VAMBER48_CORE_MISALIGNED_TB___024ROOT_H_
#define VERILATED_VAMBER48_CORE_MISALIGNED_TB___024ROOT_H_  // guard

#include "verilated.h"
#include "verilated_timing.h"


class Vamber48_core_misaligned_tb__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vamber48_core_misaligned_tb___024root final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    CData/*0:0*/ amber48_core_misaligned_tb__DOT__clk;
    CData/*0:0*/ amber48_core_misaligned_tb__DOT__rst_ni;
    CData/*0:0*/ amber48_core_misaligned_tb__DOT__clk_en;
    CData/*0:0*/ amber48_core_misaligned_tb__DOT__imem_valid;
    CData/*0:0*/ amber48_core_misaligned_tb__DOT__dmem_req;
    CData/*0:0*/ amber48_core_misaligned_tb__DOT__dmem_ready;
    CData/*0:0*/ amber48_core_misaligned_tb__DOT__dmem_trap;
    CData/*0:0*/ amber48_core_misaligned_tb__DOT__uart_tx_valid;
    CData/*7:0*/ amber48_core_misaligned_tb__DOT__uart_tx_data;
    CData/*0:0*/ amber48_core_misaligned_tb__DOT__trap_seen;
    CData/*2:0*/ amber48_core_misaligned_tb__DOT__actual_trap;
    CData/*2:0*/ amber48_core_misaligned_tb__DOT__expected_trap_e;
    CData/*0:0*/ amber48_core_misaligned_tb__DOT__store0_seen;
    CData/*7:0*/ amber48_core_misaligned_tb__DOT__u_dmem__DOT__led_q;
    CData/*7:0*/ amber48_core_misaligned_tb__DOT__u_dmem__DOT__uart_q;
    CData/*1:0*/ amber48_core_misaligned_tb__DOT__u_uart_tx__DOT__state_q;
    CData/*6:0*/ amber48_core_misaligned_tb__DOT__u_uart_tx__DOT__clk_cnt_q;
    CData/*2:0*/ amber48_core_misaligned_tb__DOT__u_uart_tx__DOT__bit_idx_q;
    CData/*0:0*/ __Vdlyvset__amber48_core_misaligned_tb__DOT__trap_seen__v0;
    CData/*0:0*/ __Vdlyvset__amber48_core_misaligned_tb__DOT__trap_seen__v1;
    CData/*1:0*/ __Vdly__amber48_core_misaligned_tb__DOT__u_uart_tx__DOT__state_q;
    CData/*0:0*/ __VstlFirstIteration;
    CData/*0:0*/ __Vtrigprevexpr___TOP__amber48_core_misaligned_tb__DOT__clk__0;
    CData/*0:0*/ __Vtrigprevexpr___TOP__amber48_core_misaligned_tb__DOT__rst_ni__0;
    CData/*0:0*/ __VactDidInit;
    CData/*0:0*/ __VactContinue;
    IData/*31:0*/ amber48_core_misaligned_tb__DOT__cycle_count;
    IData/*31:0*/ amber48_core_misaligned_tb__DOT__uart_index;
    IData/*31:0*/ amber48_core_misaligned_tb__DOT__uart_count;
    IData/*31:0*/ __VactIterCount;
    QData/*47:0*/ amber48_core_misaligned_tb__DOT__dmem_rdata;
    QData/*47:0*/ amber48_core_misaligned_tb__DOT__u_core__DOT__pc_q;
    QData/*47:0*/ amber48_core_misaligned_tb__DOT__u_core__DOT__pc_next;
    VlWide<4>/*96:0*/ amber48_core_misaligned_tb__DOT__u_core__DOT__if_stage_q;
    VlWide<4>/*96:0*/ amber48_core_misaligned_tb__DOT__u_core__DOT__if_stage_next;
    VlWide<4>/*126:0*/ amber48_core_misaligned_tb__DOT__u_core__DOT__id_stage_q;
    VlWide<4>/*126:0*/ amber48_core_misaligned_tb__DOT__u_core__DOT__id_stage_next;
    VlWide<9>/*263:0*/ amber48_core_misaligned_tb__DOT__u_core__DOT__ex_stage_q;
    VlWide<9>/*263:0*/ amber48_core_misaligned_tb__DOT__u_core__DOT__ex_stage_next;
    VlWide<5>/*156:0*/ amber48_core_misaligned_tb__DOT__u_core__DOT__ex_stage_result;
    QData/*61:0*/ amber48_core_misaligned_tb__DOT__u_core__DOT__rf_req;
    VlWide<4>/*126:0*/ amber48_core_misaligned_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r;
    VlWide<5>/*156:0*/ amber48_core_misaligned_tb__DOT__u_core__DOT__u_alu__DOT__ex_r;
    QData/*47:0*/ amber48_core_misaligned_tb__DOT__u_imem__DOT__data_q;
    QData/*47:0*/ amber48_core_misaligned_tb__DOT__u_dmem__DOT__unnamedblk1__DOT__mem_rdata;
    VlUnpacked<QData/*47:0*/, 16> amber48_core_misaligned_tb__DOT__u_core__DOT__u_regfile__DOT__regs;
    VlUnpacked<QData/*47:0*/, 256> amber48_core_misaligned_tb__DOT__u_imem__DOT__rom;
    VlUnpacked<QData/*47:0*/, 256> amber48_core_misaligned_tb__DOT__u_dmem__DOT__ram;
    VlDelayScheduler __VdlySched;
    VlTriggerScheduler __VtrigSched_h3eb4f950__0;
    VlTriggerScheduler __VtrigSched_he6ff30a7__0;
    VlTriggerVec<1> __VstlTriggered;
    VlTriggerVec<4> __VactTriggered;
    VlTriggerVec<4> __VnbaTriggered;

    // INTERNAL VARIABLES
    Vamber48_core_misaligned_tb__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vamber48_core_misaligned_tb___024root(Vamber48_core_misaligned_tb__Syms* symsp, const char* v__name);
    ~Vamber48_core_misaligned_tb___024root();
    VL_UNCOPYABLE(Vamber48_core_misaligned_tb___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
