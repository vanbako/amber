// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vamber48_core_smoke_tb.h for the primary calling header

#include "Vamber48_core_smoke_tb__pch.h"
#include "Vamber48_core_smoke_tb___024root.h"

VL_ATTR_COLD void Vamber48_core_smoke_tb___024root___eval_initial__TOP(Vamber48_core_smoke_tb___024root* vlSelf);
VlCoroutine Vamber48_core_smoke_tb___024root___eval_initial__TOP__Vtiming__0(Vamber48_core_smoke_tb___024root* vlSelf);
VlCoroutine Vamber48_core_smoke_tb___024root___eval_initial__TOP__Vtiming__1(Vamber48_core_smoke_tb___024root* vlSelf);
VlCoroutine Vamber48_core_smoke_tb___024root___eval_initial__TOP__Vtiming__2(Vamber48_core_smoke_tb___024root* vlSelf);
VlCoroutine Vamber48_core_smoke_tb___024root___eval_initial__TOP__Vtiming__3(Vamber48_core_smoke_tb___024root* vlSelf);

void Vamber48_core_smoke_tb___024root___eval_initial(Vamber48_core_smoke_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vamber48_core_smoke_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vamber48_core_smoke_tb___024root___eval_initial\n"); );
    // Body
    Vamber48_core_smoke_tb___024root___eval_initial__TOP(vlSelf);
    Vamber48_core_smoke_tb___024root___eval_initial__TOP__Vtiming__0(vlSelf);
    Vamber48_core_smoke_tb___024root___eval_initial__TOP__Vtiming__1(vlSelf);
    Vamber48_core_smoke_tb___024root___eval_initial__TOP__Vtiming__2(vlSelf);
    Vamber48_core_smoke_tb___024root___eval_initial__TOP__Vtiming__3(vlSelf);
    vlSelf->__Vtrigprevexpr___TOP__amber48_core_smoke_tb__DOT__clk__0 
        = vlSelf->amber48_core_smoke_tb__DOT__clk;
    vlSelf->__Vtrigprevexpr___TOP__amber48_core_smoke_tb__DOT__rst_ni__0 
        = vlSelf->amber48_core_smoke_tb__DOT__rst_ni;
}

VL_INLINE_OPT VlCoroutine Vamber48_core_smoke_tb___024root___eval_initial__TOP__Vtiming__0(Vamber48_core_smoke_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vamber48_core_smoke_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vamber48_core_smoke_tb___024root___eval_initial__TOP__Vtiming__0\n"); );
    // Body
    vlSelf->amber48_core_smoke_tb__DOT__rst_ni = 0U;
    vlSelf->amber48_core_smoke_tb__DOT__clk_en = 1U;
    vlSelf->amber48_core_smoke_tb__DOT__trap_seen = 0U;
    vlSelf->amber48_core_smoke_tb__DOT__cycle_count = 0U;
    vlSelf->amber48_core_smoke_tb__DOT__uart_index = 0U;
    vlSelf->amber48_core_smoke_tb__DOT__uart_count = 0U;
    vlSelf->amber48_core_smoke_tb__DOT__store0_seen = 0U;
    co_await vlSelf->__VtrigSched_hb844875a__0.trigger(0U, 
                                                       nullptr, 
                                                       "@(posedge amber48_core_smoke_tb.clk)", 
                                                       "src/0.1/sim/tb/amber48_core_tb_base.svh", 
                                                       130);
    co_await vlSelf->__VtrigSched_hb844875a__0.trigger(0U, 
                                                       nullptr, 
                                                       "@(posedge amber48_core_smoke_tb.clk)", 
                                                       "src/0.1/sim/tb/amber48_core_tb_base.svh", 
                                                       130);
    co_await vlSelf->__VtrigSched_hb844875a__0.trigger(0U, 
                                                       nullptr, 
                                                       "@(posedge amber48_core_smoke_tb.clk)", 
                                                       "src/0.1/sim/tb/amber48_core_tb_base.svh", 
                                                       130);
    co_await vlSelf->__VtrigSched_hb844875a__0.trigger(0U, 
                                                       nullptr, 
                                                       "@(posedge amber48_core_smoke_tb.clk)", 
                                                       "src/0.1/sim/tb/amber48_core_tb_base.svh", 
                                                       130);
    vlSelf->amber48_core_smoke_tb__DOT__rst_ni = 1U;
}

VL_INLINE_OPT VlCoroutine Vamber48_core_smoke_tb___024root___eval_initial__TOP__Vtiming__2(Vamber48_core_smoke_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vamber48_core_smoke_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vamber48_core_smoke_tb___024root___eval_initial__TOP__Vtiming__2\n"); );
    // Body
    while (1U) {
        co_await vlSelf->__VdlySched.delay(0x1388ULL, 
                                           nullptr, 
                                           "src/0.1/sim/tb/amber48_core_tb_base.svh", 
                                           120);
        vlSelf->amber48_core_smoke_tb__DOT__clk = (1U 
                                                   & (~ (IData)(vlSelf->amber48_core_smoke_tb__DOT__clk)));
    }
}

void Vamber48_core_smoke_tb___024root___eval_act(Vamber48_core_smoke_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vamber48_core_smoke_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vamber48_core_smoke_tb___024root___eval_act\n"); );
}

extern const VlWide<9>/*287:0*/ Vamber48_core_smoke_tb__ConstPool__CONST_h2a0d1dfa_0;

VL_INLINE_OPT void Vamber48_core_smoke_tb___024root___nba_sequent__TOP__0(Vamber48_core_smoke_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vamber48_core_smoke_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vamber48_core_smoke_tb___024root___nba_sequent__TOP__0\n"); );
    // Init
    CData/*7:0*/ amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode;
    amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode = 0;
    CData/*3:0*/ amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__rd_field;
    amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__rd_field = 0;
    CData/*3:0*/ amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__rs1_field;
    amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__rs1_field = 0;
    CData/*3:0*/ amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__rs2_field;
    amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__rs2_field = 0;
    QData/*47:0*/ amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__imm_ext;
    amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__imm_ext = 0;
    IData/*23:0*/ amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__upper_imm_field;
    amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__upper_imm_field = 0;
    QData/*47:0*/ amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__upper_imm_value;
    amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__upper_imm_value = 0;
    QData/*47:0*/ amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__operand_b;
    amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__operand_b = 0;
    CData/*5:0*/ amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__shamt;
    amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__shamt = 0;
    QData/*47:0*/ amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__branch_cmp_b;
    amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__branch_cmp_b = 0;
    QData/*47:0*/ amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__return_addr;
    amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__return_addr = 0;
    CData/*0:0*/ amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__trap_active;
    amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__trap_active = 0;
    CData/*3:0*/ __Vdlyvdim0__amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs__v0;
    __Vdlyvdim0__amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs__v0 = 0;
    QData/*47:0*/ __Vdlyvval__amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs__v0;
    __Vdlyvval__amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs__v0 = 0;
    CData/*0:0*/ __Vdlyvset__amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs__v0;
    __Vdlyvset__amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs__v0 = 0;
    CData/*0:0*/ __Vdlyvset__amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs__v1;
    __Vdlyvset__amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs__v1 = 0;
    CData/*7:0*/ __Vdlyvdim0__amber48_core_smoke_tb__DOT__u_dmem__DOT__ram__v0;
    __Vdlyvdim0__amber48_core_smoke_tb__DOT__u_dmem__DOT__ram__v0 = 0;
    QData/*47:0*/ __Vdlyvval__amber48_core_smoke_tb__DOT__u_dmem__DOT__ram__v0;
    __Vdlyvval__amber48_core_smoke_tb__DOT__u_dmem__DOT__ram__v0 = 0;
    CData/*0:0*/ __Vdlyvset__amber48_core_smoke_tb__DOT__u_dmem__DOT__ram__v0;
    __Vdlyvset__amber48_core_smoke_tb__DOT__u_dmem__DOT__ram__v0 = 0;
    CData/*6:0*/ __Vdly__amber48_core_smoke_tb__DOT__u_uart_tx__DOT__clk_cnt_q;
    __Vdly__amber48_core_smoke_tb__DOT__u_uart_tx__DOT__clk_cnt_q = 0;
    // Body
    __Vdlyvset__amber48_core_smoke_tb__DOT__u_dmem__DOT__ram__v0 = 0U;
    __Vdlyvset__amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs__v0 = 0U;
    __Vdlyvset__amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs__v1 = 0U;
    __Vdly__amber48_core_smoke_tb__DOT__u_uart_tx__DOT__clk_cnt_q 
        = vlSelf->amber48_core_smoke_tb__DOT__u_uart_tx__DOT__clk_cnt_q;
    vlSelf->__Vdly__amber48_core_smoke_tb__DOT__u_uart_tx__DOT__state_q 
        = vlSelf->amber48_core_smoke_tb__DOT__u_uart_tx__DOT__state_q;
    if (vlSelf->amber48_core_smoke_tb__DOT__rst_ni) {
        if (((IData)((vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__rf_req 
                      >> 0x30U)) & (0U != (0xfU & (IData)(
                                                          (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__rf_req 
                                                           >> 0x31U)))))) {
            __Vdlyvval__amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs__v0 
                = (0xffffffffffffULL & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__rf_req);
            __Vdlyvset__amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs__v0 = 1U;
            __Vdlyvdim0__amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs__v0 
                = (0xfU & (IData)((vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__rf_req 
                                   >> 0x31U)));
        }
        vlSelf->amber48_core_smoke_tb__DOT__u_imem__DOT__data_q 
            = vlSelf->amber48_core_smoke_tb__DOT__u_imem__DOT__rom
            [(0xffU & (IData)((vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__pc_q 
                               >> 3U)))];
        if (vlSelf->amber48_core_smoke_tb__DOT__clk_en) {
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[0U] 
                = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[0U];
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[1U] 
                = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[1U];
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[2U] 
                = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[2U];
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[3U] 
                = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[3U];
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[0U] 
                = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[0U];
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[1U] 
                = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[1U];
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[2U] 
                = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[2U];
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[3U] 
                = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[3U];
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[0U] 
                = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[0U];
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[1U] 
                = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[1U];
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[2U] 
                = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[2U];
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U] 
                = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[3U];
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[4U] 
                = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[4U];
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U] 
                = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[5U];
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U] 
                = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[6U];
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[7U] 
                = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[7U];
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[8U] 
                = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[8U];
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__pc_q 
                = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__pc_next;
        }
        vlSelf->amber48_core_smoke_tb__DOT__dmem_trap = 0U;
        if (vlSelf->amber48_core_smoke_tb__DOT__dmem_req) {
            if ((IData)(((0U != (0x7000U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[3U])) 
                         | (0U != (0x1fffffffffULL 
                                   & (((QData)((IData)(
                                                       vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[4U])) 
                                       << 9U) | ((QData)((IData)(
                                                                 vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[3U])) 
                                                 >> 0x17U))))))) {
                vlSelf->amber48_core_smoke_tb__DOT__dmem_trap = 1U;
                vlSelf->amber48_core_smoke_tb__DOT__dmem_rdata = 0ULL;
            } else if ((0xfeU == (0xffU & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[3U] 
                                           >> 0xfU)))) {
                if ((0x200000U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[1U])) {
                    if ((0U == (IData)(vlSelf->amber48_core_smoke_tb__DOT__u_uart_tx__DOT__state_q))) {
                        vlSelf->amber48_core_smoke_tb__DOT__dmem_rdata 
                            = (QData)((IData)((vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[1U] 
                                               >> 0x18U)));
                    }
                } else {
                    vlSelf->amber48_core_smoke_tb__DOT__dmem_rdata 
                        = (QData)((IData)(vlSelf->amber48_core_smoke_tb__DOT__u_dmem__DOT__uart_q));
                }
            } else {
                vlSelf->amber48_core_smoke_tb__DOT__u_dmem__DOT__unnamedblk1__DOT__mem_rdata 
                    = vlSelf->amber48_core_smoke_tb__DOT__u_dmem__DOT__ram
                    [(0xffU & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[3U] 
                               >> 0xfU))];
                if ((0x200000U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[1U])) {
                    if ((0xffU != (0xffU & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[3U] 
                                            >> 0xfU)))) {
                        vlSelf->amber48_core_smoke_tb__DOT__u_dmem__DOT__unnamedblk1__DOT__mem_rdata 
                            = (0xffffffffffffULL & 
                               (((QData)((IData)(vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[3U])) 
                                 << 0x28U) | (((QData)((IData)(
                                                               vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[2U])) 
                                               << 8U) 
                                              | ((QData)((IData)(
                                                                 vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[1U])) 
                                                 >> 0x18U))));
                    }
                }
                vlSelf->amber48_core_smoke_tb__DOT__dmem_rdata 
                    = ((0xffU == (0xffU & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[3U] 
                                           >> 0xfU)))
                        ? (QData)((IData)(vlSelf->amber48_core_smoke_tb__DOT__u_dmem__DOT__led_q))
                        : vlSelf->amber48_core_smoke_tb__DOT__u_dmem__DOT__unnamedblk1__DOT__mem_rdata);
            }
            if ((1U & (~ (IData)(((0U != (0x7000U & 
                                          vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[3U])) 
                                  | (0U != (0x1fffffffffULL 
                                            & (((QData)((IData)(
                                                                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[4U])) 
                                                << 9U) 
                                               | ((QData)((IData)(
                                                                  vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[3U])) 
                                                  >> 0x17U))))))))) {
                if ((0xfeU != (0xffU & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[3U] 
                                        >> 0xfU)))) {
                    if ((0x200000U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[1U])) {
                        if ((0xffU != (0xffU & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[3U] 
                                                >> 0xfU)))) {
                            __Vdlyvval__amber48_core_smoke_tb__DOT__u_dmem__DOT__ram__v0 
                                = (0xffffffffffffULL 
                                   & (((QData)((IData)(
                                                       vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[3U])) 
                                       << 0x28U) | 
                                      (((QData)((IData)(
                                                        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[2U])) 
                                        << 8U) | ((QData)((IData)(
                                                                  vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[1U])) 
                                                  >> 0x18U))));
                            __Vdlyvset__amber48_core_smoke_tb__DOT__u_dmem__DOT__ram__v0 = 1U;
                            __Vdlyvdim0__amber48_core_smoke_tb__DOT__u_dmem__DOT__ram__v0 
                                = (0xffU & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[3U] 
                                            >> 0xfU));
                        }
                        if ((0xffU == (0xffU & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[3U] 
                                                >> 0xfU)))) {
                            vlSelf->amber48_core_smoke_tb__DOT__u_dmem__DOT__led_q 
                                = (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[1U] 
                                   >> 0x18U);
                        }
                    }
                }
                if ((0xfeU == (0xffU & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[3U] 
                                        >> 0xfU)))) {
                    if ((0x200000U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[1U])) {
                        vlSelf->amber48_core_smoke_tb__DOT__u_dmem__DOT__uart_q 
                            = (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[1U] 
                               >> 0x18U);
                    }
                }
            }
        }
        if ((2U & (IData)(vlSelf->amber48_core_smoke_tb__DOT__u_uart_tx__DOT__state_q))) {
            if ((1U & (IData)(vlSelf->amber48_core_smoke_tb__DOT__u_uart_tx__DOT__state_q))) {
                if ((0U == (IData)(vlSelf->amber48_core_smoke_tb__DOT__u_uart_tx__DOT__clk_cnt_q))) {
                    vlSelf->__Vdly__amber48_core_smoke_tb__DOT__u_uart_tx__DOT__state_q = 0U;
                } else {
                    __Vdly__amber48_core_smoke_tb__DOT__u_uart_tx__DOT__clk_cnt_q 
                        = (0x7fU & ((IData)(vlSelf->amber48_core_smoke_tb__DOT__u_uart_tx__DOT__clk_cnt_q) 
                                    - (IData)(1U)));
                }
            } else if ((0U == (IData)(vlSelf->amber48_core_smoke_tb__DOT__u_uart_tx__DOT__clk_cnt_q))) {
                if ((7U == (IData)(vlSelf->amber48_core_smoke_tb__DOT__u_uart_tx__DOT__bit_idx_q))) {
                    vlSelf->__Vdly__amber48_core_smoke_tb__DOT__u_uart_tx__DOT__state_q = 3U;
                    __Vdly__amber48_core_smoke_tb__DOT__u_uart_tx__DOT__clk_cnt_q = 0x63U;
                } else {
                    vlSelf->amber48_core_smoke_tb__DOT__u_uart_tx__DOT__bit_idx_q 
                        = (7U & ((IData)(1U) + (IData)(vlSelf->amber48_core_smoke_tb__DOT__u_uart_tx__DOT__bit_idx_q)));
                    __Vdly__amber48_core_smoke_tb__DOT__u_uart_tx__DOT__clk_cnt_q = 0x63U;
                }
            } else {
                __Vdly__amber48_core_smoke_tb__DOT__u_uart_tx__DOT__clk_cnt_q 
                    = (0x7fU & ((IData)(vlSelf->amber48_core_smoke_tb__DOT__u_uart_tx__DOT__clk_cnt_q) 
                                - (IData)(1U)));
            }
        } else if ((1U & (IData)(vlSelf->amber48_core_smoke_tb__DOT__u_uart_tx__DOT__state_q))) {
            if ((0U == (IData)(vlSelf->amber48_core_smoke_tb__DOT__u_uart_tx__DOT__clk_cnt_q))) {
                vlSelf->amber48_core_smoke_tb__DOT__u_uart_tx__DOT__bit_idx_q = 0U;
                vlSelf->__Vdly__amber48_core_smoke_tb__DOT__u_uart_tx__DOT__state_q = 2U;
                __Vdly__amber48_core_smoke_tb__DOT__u_uart_tx__DOT__clk_cnt_q = 0x63U;
            } else {
                __Vdly__amber48_core_smoke_tb__DOT__u_uart_tx__DOT__clk_cnt_q 
                    = (0x7fU & ((IData)(vlSelf->amber48_core_smoke_tb__DOT__u_uart_tx__DOT__clk_cnt_q) 
                                - (IData)(1U)));
            }
        } else {
            vlSelf->amber48_core_smoke_tb__DOT__u_uart_tx__DOT__bit_idx_q = 0U;
            __Vdly__amber48_core_smoke_tb__DOT__u_uart_tx__DOT__clk_cnt_q = 0x63U;
            if (vlSelf->amber48_core_smoke_tb__DOT__uart_tx_valid) {
                vlSelf->__Vdly__amber48_core_smoke_tb__DOT__u_uart_tx__DOT__state_q = 1U;
            }
        }
        vlSelf->amber48_core_smoke_tb__DOT__imem_valid = 1U;
    } else {
        __Vdlyvset__amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs__v1 = 1U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[0U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[1U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[2U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[3U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[0U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[1U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[2U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[3U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_imem__DOT__data_q = 0ULL;
        vlSelf->amber48_core_smoke_tb__DOT__dmem_trap = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[0U] 
            = Vamber48_core_smoke_tb__ConstPool__CONST_h2a0d1dfa_0[0U];
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[1U] 
            = Vamber48_core_smoke_tb__ConstPool__CONST_h2a0d1dfa_0[1U];
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[2U] 
            = Vamber48_core_smoke_tb__ConstPool__CONST_h2a0d1dfa_0[2U];
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U] 
            = Vamber48_core_smoke_tb__ConstPool__CONST_h2a0d1dfa_0[3U];
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[4U] 
            = Vamber48_core_smoke_tb__ConstPool__CONST_h2a0d1dfa_0[4U];
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U] 
            = Vamber48_core_smoke_tb__ConstPool__CONST_h2a0d1dfa_0[5U];
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U] 
            = Vamber48_core_smoke_tb__ConstPool__CONST_h2a0d1dfa_0[6U];
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[7U] 
            = Vamber48_core_smoke_tb__ConstPool__CONST_h2a0d1dfa_0[7U];
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[8U] 
            = Vamber48_core_smoke_tb__ConstPool__CONST_h2a0d1dfa_0[8U];
        vlSelf->amber48_core_smoke_tb__DOT__u_uart_tx__DOT__bit_idx_q = 0U;
        vlSelf->__Vdly__amber48_core_smoke_tb__DOT__u_uart_tx__DOT__state_q = 0U;
        __Vdly__amber48_core_smoke_tb__DOT__u_uart_tx__DOT__clk_cnt_q = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__dmem_rdata = 0ULL;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__pc_q = 0ULL;
        vlSelf->amber48_core_smoke_tb__DOT__u_dmem__DOT__uart_q = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_dmem__DOT__led_q = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__imem_valid = 0U;
    }
    if (__Vdlyvset__amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs__v0) {
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs[__Vdlyvdim0__amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs__v0] 
            = __Vdlyvval__amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs__v0;
    }
    if (__Vdlyvset__amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs__v1) {
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs[0U] = 0ULL;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs[1U] = 0ULL;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs[2U] = 0ULL;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs[3U] = 0ULL;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs[4U] = 0ULL;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs[5U] = 0ULL;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs[6U] = 0ULL;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs[7U] = 0ULL;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs[8U] = 0ULL;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs[9U] = 0ULL;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs[0xaU] = 0ULL;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs[0xbU] = 0ULL;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs[0xcU] = 0ULL;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs[0xdU] = 0ULL;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs[0xeU] = 0ULL;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs[0xfU] = 0ULL;
    }
    vlSelf->amber48_core_smoke_tb__DOT__u_uart_tx__DOT__clk_cnt_q 
        = __Vdly__amber48_core_smoke_tb__DOT__u_uart_tx__DOT__clk_cnt_q;
    if (__Vdlyvset__amber48_core_smoke_tb__DOT__u_dmem__DOT__ram__v0) {
        vlSelf->amber48_core_smoke_tb__DOT__u_dmem__DOT__ram[__Vdlyvdim0__amber48_core_smoke_tb__DOT__u_dmem__DOT__ram__v0] 
            = __Vdlyvval__amber48_core_smoke_tb__DOT__u_dmem__DOT__ram__v0;
    }
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] = 0U;
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] = 0U;
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] = 0U;
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[3U] = 0U;
    amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode 
        = (0xffU & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[1U] 
                    >> 8U));
    amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__rd_field 
        = (0xfU & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[0U] 
                   >> 0xcU));
    amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__rs1_field 
        = (0xfU & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[0U] 
                   >> 0x14U));
    amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__rs2_field 
        = (0xfU & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[0U] 
                   >> 0x10U));
    amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__imm_ext 
        = (((QData)((IData)((- (IData)((1U & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[1U] 
                                              >> 7U)))))) 
            << 0x10U) | (QData)((IData)((0xffffU & 
                                         ((vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[1U] 
                                           << 8U) | 
                                          (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[0U] 
                                           >> 0x18U))))));
    amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__upper_imm_field 
        = (0xffffffU & ((vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[1U] 
                         << 0x10U) | (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[0U] 
                                      >> 0x10U)));
    amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__upper_imm_value 
        = ((QData)((IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__upper_imm_field)) 
           << 0x18U);
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
        = ((0x3fU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]) 
           | ((IData)(((0x1ffffffffffff00ULL & (((QData)((IData)(
                                                                 vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[3U])) 
                                                 << 0x38U) 
                                                | (((QData)((IData)(
                                                                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[2U])) 
                                                    << 0x18U) 
                                                   | (0xffffffffffff00ULL 
                                                      & ((QData)((IData)(
                                                                         vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[1U])) 
                                                         >> 8U))))) 
                       | (QData)((IData)((((IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__rs1_field) 
                                           << 4U) | (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__rs2_field)))))) 
              << 6U));
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[3U] 
        = (0x7fffffffU & (((IData)(((0x1ffffffffffff00ULL 
                                     & (((QData)((IData)(
                                                         vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[3U])) 
                                         << 0x38U) 
                                        | (((QData)((IData)(
                                                            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[2U])) 
                                            << 0x18U) 
                                           | (0xffffffffffff00ULL 
                                              & ((QData)((IData)(
                                                                 vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[1U])) 
                                                 >> 8U))))) 
                                    | (QData)((IData)(
                                                      (((IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__rs1_field) 
                                                        << 4U) 
                                                       | (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__rs2_field)))))) 
                           >> 0x1aU) | ((IData)((((0x1ffffffffffff00ULL 
                                                   & (((QData)((IData)(
                                                                       vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[3U])) 
                                                       << 0x38U) 
                                                      | (((QData)((IData)(
                                                                          vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[2U])) 
                                                          << 0x18U) 
                                                         | (0xffffffffffff00ULL 
                                                            & ((QData)((IData)(
                                                                               vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[1U])) 
                                                               >> 8U))))) 
                                                  | (QData)((IData)(
                                                                    (((IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__rs1_field) 
                                                                      << 4U) 
                                                                     | (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__rs2_field))))) 
                                                 >> 0x20U)) 
                                        << 6U)));
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
        = ((0x3ffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]) 
           | ((IData)((0xe000000000000ULL | (((QData)((IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__rd_field)) 
                                              << 0x35U) 
                                             | (amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__imm_ext 
                                                << 1U)))) 
              << 0xaU));
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
        = (((IData)((0xe000000000000ULL | (((QData)((IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__rd_field)) 
                                            << 0x35U) 
                                           | (amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__imm_ext 
                                              << 1U)))) 
            >> 0x16U) | ((IData)(((0xe000000000000ULL 
                                   | (((QData)((IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__rd_field)) 
                                       << 0x35U) | 
                                      (amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__imm_ext 
                                       << 1U))) >> 0x20U)) 
                         << 0xaU));
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
        = ((0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]) 
           | ((IData)(((0xe000000000000ULL | (((QData)((IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__rd_field)) 
                                               << 0x35U) 
                                              | (amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__imm_ext 
                                                 << 1U))) 
                       >> 0x20U)) >> 0x16U));
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
        = (0xfffffc00U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]);
    if ((0x80U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
        if ((1U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[3U])) {
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                = (9U | (0xfffffff0U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]));
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                = (0x7fffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                = (0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
        }
    } else if ((0x40U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
        if ((0x20U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
            if ((0x10U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
                if ((8U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
                    if ((1U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[3U])) {
                        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                            = (9U | (0xfffffff0U & 
                                     vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]));
                        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                            = (0x7fffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
                        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                            = (0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
                    }
                } else if ((4U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
                    if ((1U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[3U])) {
                        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                            = (9U | (0xfffffff0U & 
                                     vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]));
                        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                            = (0x7fffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
                        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                            = (0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
                    }
                } else if ((2U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
                    if ((1U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
                        if ((1U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[3U])) {
                            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                                = (9U | (0xfffffff0U 
                                         & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]));
                            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                                = (0x7fffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
                            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                                = (0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
                        }
                    } else {
                        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                            = (0x40U | (0xfffffc3fU 
                                        & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]));
                        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                            = ((0xffffc03fU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]) 
                               | (0xffffffc0U & (((0U 
                                                   == (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__rs1_field))
                                                   ? 0xeU
                                                   : (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__rs1_field)) 
                                                 << 0xaU)));
                        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                            = (0x7fffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
                        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                            = (0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
                        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                            = (0x3ffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]);
                        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                            = (0xf8000000U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
                        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                            = (8U | vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
                    }
                } else if ((1U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                        = (0x40U | (0xfffffc3fU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]));
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                        = (0xffffc03fU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                        = (0x7fffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                        = (7U | (0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]));
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                        = ((0x3ffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]) 
                           | ((IData)((1ULL | (amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__imm_ext 
                                               << 1U))) 
                              << 0xaU));
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                        = ((0xf8000000U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]) 
                           | (((IData)((1ULL | (amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__imm_ext 
                                                << 1U))) 
                               >> 0x16U) | ((IData)(
                                                    ((1ULL 
                                                      | (amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__imm_ext 
                                                         << 1U)) 
                                                     >> 0x20U)) 
                                            << 0xaU)));
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                        = (0x10U | vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
                } else {
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                        = (0x40U | (0xfffffc3fU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]));
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                        = (0xffffc03fU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                        = (0x7fffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                        = (0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                        = ((0x3ffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]) 
                           | ((IData)((1ULL | (amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__imm_ext 
                                               << 1U))) 
                              << 0xaU));
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                        = ((0xf8000000U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]) 
                           | (((IData)((1ULL | (amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__imm_ext 
                                                << 1U))) 
                               >> 0x16U) | ((IData)(
                                                    ((1ULL 
                                                      | (amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__imm_ext 
                                                         << 1U)) 
                                                     >> 0x20U)) 
                                            << 0xaU)));
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                        = (0x20U | vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
                }
            } else if ((8U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
                if ((1U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[3U])) {
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                        = (9U | (0xfffffff0U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]));
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                        = (0x7fffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                        = (0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
                }
            } else if ((4U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
                if ((1U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[3U])) {
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                        = (9U | (0xfffffff0U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]));
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                        = (0x7fffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                        = (0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
                }
            } else if ((2U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
                if ((1U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[3U])) {
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                        = (9U | (0xfffffff0U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]));
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                        = (0x7fffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                        = (0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
                }
            } else if ((1U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                    = (0x10U | vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]);
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                    = (0x400U | vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]);
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                    = (0x7fffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                    = (0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
            } else {
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                    = (0x20U | vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]);
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                    = (0x400U | vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]);
            }
        } else if ((0x10U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
            if ((1U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[3U])) {
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                    = (9U | (0xfffffff0U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]));
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                    = (0x7fffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                    = (0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
            }
        } else if ((8U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
            if ((4U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
                if ((1U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[3U])) {
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                        = (9U | (0xfffffff0U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]));
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                        = (0x7fffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                        = (0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
                }
            } else if ((2U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
                if ((1U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[3U])) {
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                        = (9U | (0xfffffff0U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]));
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                        = (0x7fffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                        = (0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
                }
            } else if ((1U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
                if ((1U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[3U])) {
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                        = (9U | (0xfffffff0U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]));
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                        = (0x7fffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                        = (0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
                }
            } else {
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                    = (0x40U | (0xfffffc3fU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]));
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                    = (0x7fffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                    = (0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                    = (0xfffffc3fU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
            }
        } else if ((4U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
            if ((2U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
                if ((1U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                        = (0x240U | (0xfffffc3fU & 
                                     vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]));
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                        = (0x7fffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                        = (0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                        = (0xfffffc3fU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
                } else {
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                        = (0x200U | (0xfffffc3fU & 
                                     vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]));
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                        = (0x7fffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                        = (0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                        = (0xfffffc3fU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
                }
            } else if ((1U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                    = (0x1c0U | (0xfffffc3fU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]));
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                    = (0x7fffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                    = (0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
            } else {
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                    = (0x180U | (0xfffffc3fU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]));
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                    = (0x7fffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                    = (0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
            }
        } else if ((2U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
            if ((1U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                    = (0x140U | (0xfffffc3fU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]));
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                    = (0x7fffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                    = (0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
            } else {
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                    = (0x100U | (0xfffffc3fU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]));
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                    = (0x7fffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                    = (0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
            }
        } else if ((1U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                = (0xc0U | (0xfffffc3fU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]));
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                = (0x7fffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                = (0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
        } else {
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                = (0x80U | (0xfffffc3fU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]));
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                = (0x7fffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                = (0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
        }
    } else if ((0x20U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
        if ((0x10U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
            if ((8U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
                if ((1U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[3U])) {
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                        = (9U | (0xfffffff0U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]));
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                        = (0x7fffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                        = (0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
                }
            } else if ((4U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
                if ((1U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[3U])) {
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                        = (9U | (0xfffffff0U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]));
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                        = (0x7fffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                        = (0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
                }
            } else if ((2U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
                if ((1U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[3U])) {
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                        = (9U | (0xfffffff0U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]));
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                        = (0x7fffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                        = (0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
                }
            } else {
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                    = ((0x87ffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]) 
                       | (((1U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))
                            ? 6U : 5U) << 0x1bU));
            }
        } else if ((8U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
            if ((1U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[3U])) {
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                    = (9U | (0xfffffff0U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]));
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                    = (0x7fffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                    = (0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
            }
        } else if ((4U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
            if ((1U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[3U])) {
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                    = (9U | (0xfffffff0U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]));
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                    = (0x7fffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                    = (0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
            }
        } else if ((2U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
            if ((1U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                    = (0x20000000U | (0x87ffffffU & 
                                      vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]));
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                    = (0x400U | vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]);
            } else {
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                    = (0x20000000U | (0x87ffffffU & 
                                      vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]));
            }
        } else {
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                = ((0x87ffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]) 
                   | (((1U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))
                        ? 3U : 2U) << 0x1bU));
        }
    } else if ((0x10U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
        if ((8U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
            if ((1U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[3U])) {
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                    = (9U | (0xfffffff0U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]));
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                    = (0x7fffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                    = (0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
            }
        } else if ((4U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
            if ((1U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[3U])) {
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                    = (9U | (0xfffffff0U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]));
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                    = (0x7fffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                    = (0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
            }
        } else if ((2U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
            if ((1U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                    = (0x8000000U | (0x87ffffffU & 
                                     vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]));
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                    = (0x400U | vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]);
            } else {
                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                    = (0x8000000U | (0x87ffffffU & 
                                     vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]));
            }
        } else if ((1U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                = (0x87ffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                = (0x400U | vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]);
        } else {
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                = (0x87ffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
        }
    } else if ((8U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
        if ((1U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[3U])) {
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                = (9U | (0xfffffff0U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]));
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                = (0x7fffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                = (0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
        }
    } else if ((4U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
        if ((1U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[3U])) {
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                = (9U | (0xfffffff0U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]));
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                = (0x7fffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                = (0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
        }
    } else if ((2U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
        if ((1U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[3U])) {
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                = (9U | (0xfffffff0U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]));
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                = (0x7fffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                = (0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
        }
    } else if ((1U & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__opcode))) {
        if ((1U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[3U])) {
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
                = (9U | (0xfffffff0U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]));
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
                = (0x7fffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]);
            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
                = (0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
        }
    } else {
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
            = (0x38000000U | (0x87ffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]));
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
            = (0x400U | vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]);
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U] 
            = (0xffffc03fU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U]);
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U] 
            = ((0x7ffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U]) 
               | ((IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__upper_imm_value) 
                  << 0xbU));
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U] 
            = ((0xf8000000U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U]) 
               | (((IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__upper_imm_value) 
                   >> 0x15U) | ((IData)((amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__upper_imm_value 
                                         >> 0x20U)) 
                                << 0xbU)));
    }
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[0U] = 0U;
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[1U] = 0U;
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[2U] = 0U;
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[3U] = 0U;
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[4U] = 0U;
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[4U] 
        = ((0xfffffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[4U]) 
           | (0x10000000U & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[8U] 
                             << 0x15U)));
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[0U] 
        = ((0xfU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[0U]) 
           | ((IData)((0xffffffffffffULL & ((((QData)((IData)(
                                                              vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[8U])) 
                                              << 0x29U) 
                                             | (((QData)((IData)(
                                                                 vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[7U])) 
                                                 << 9U) 
                                                | ((QData)((IData)(
                                                                   vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                   >> 0x17U))) 
                                            + (((QData)((IData)(
                                                                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])) 
                                                << 0x3dU) 
                                               | (((QData)((IData)(
                                                                   vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])) 
                                                   << 0x1dU) 
                                                  | ((QData)((IData)(
                                                                     vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[2U])) 
                                                     >> 3U)))))) 
              << 4U));
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[1U] 
        = (((IData)((0xffffffffffffULL & ((((QData)((IData)(
                                                            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[8U])) 
                                            << 0x29U) 
                                           | (((QData)((IData)(
                                                               vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[7U])) 
                                               << 9U) 
                                              | ((QData)((IData)(
                                                                 vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                 >> 0x17U))) 
                                          + (((QData)((IData)(
                                                              vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])) 
                                              << 0x3dU) 
                                             | (((QData)((IData)(
                                                                 vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])) 
                                                 << 0x1dU) 
                                                | ((QData)((IData)(
                                                                   vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[2U])) 
                                                   >> 3U)))))) 
            >> 0x1cU) | (((IData)((0x7fffffffffffffULL 
                                   & (((QData)((IData)(
                                                       vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[1U])) 
                                       << 0x1cU) | 
                                      ((QData)((IData)(
                                                       vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[0U])) 
                                       >> 4U)))) << 0x15U) 
                         | ((IData)(((0xffffffffffffULL 
                                      & ((((QData)((IData)(
                                                           vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[8U])) 
                                           << 0x29U) 
                                          | (((QData)((IData)(
                                                              vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[7U])) 
                                              << 9U) 
                                             | ((QData)((IData)(
                                                                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                >> 0x17U))) 
                                         + (((QData)((IData)(
                                                             vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])) 
                                             << 0x3dU) 
                                            | (((QData)((IData)(
                                                                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])) 
                                                << 0x1dU) 
                                               | ((QData)((IData)(
                                                                  vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[2U])) 
                                                  >> 3U))))) 
                                     >> 0x20U)) << 4U)));
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[2U] 
        = (((0xfU & ((IData)((0x7fffffffffffffULL & 
                              (((QData)((IData)(vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[1U])) 
                                << 0x1cU) | ((QData)((IData)(
                                                             vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[0U])) 
                                             >> 4U)))) 
                     >> 0xbU)) | ((IData)(((0xffffffffffffULL 
                                            & ((((QData)((IData)(
                                                                 vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[8U])) 
                                                 << 0x29U) 
                                                | (((QData)((IData)(
                                                                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[7U])) 
                                                    << 9U) 
                                                   | ((QData)((IData)(
                                                                      vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                      >> 0x17U))) 
                                               + (((QData)((IData)(
                                                                   vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])) 
                                                   << 0x3dU) 
                                                  | (((QData)((IData)(
                                                                      vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])) 
                                                      << 0x1dU) 
                                                     | ((QData)((IData)(
                                                                        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[2U])) 
                                                        >> 3U))))) 
                                           >> 0x20U)) 
                                  >> 0x1cU)) | ((0x1ffff0U 
                                                 & ((IData)(
                                                            (0x7fffffffffffffULL 
                                                             & (((QData)((IData)(
                                                                                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[1U])) 
                                                                 << 0x1cU) 
                                                                | ((QData)((IData)(
                                                                                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[0U])) 
                                                                   >> 4U)))) 
                                                    >> 0xbU)) 
                                                | ((IData)(
                                                           ((0x7fffffffffffffULL 
                                                             & (((QData)((IData)(
                                                                                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[1U])) 
                                                                 << 0x1cU) 
                                                                | ((QData)((IData)(
                                                                                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[0U])) 
                                                                   >> 4U))) 
                                                            >> 0x20U)) 
                                                   << 0x15U)));
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[3U] 
        = ((0xfffff000U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[3U]) 
           | ((0xfU & ((IData)(((0x7fffffffffffffULL 
                                 & (((QData)((IData)(
                                                     vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[1U])) 
                                     << 0x1cU) | ((QData)((IData)(
                                                                  vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[0U])) 
                                                  >> 4U))) 
                                >> 0x20U)) >> 0xbU)) 
              | (0x1ffff0U & ((IData)(((0x7fffffffffffffULL 
                                        & (((QData)((IData)(
                                                            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[1U])) 
                                            << 0x1cU) 
                                           | ((QData)((IData)(
                                                              vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[0U])) 
                                              >> 4U))) 
                                       >> 0x20U)) >> 0xbU))));
    amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__return_addr 
        = (0xffffffffffffULL & (8ULL + (((QData)((IData)(
                                                         vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[8U])) 
                                         << 0x29U) 
                                        | (((QData)((IData)(
                                                            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[7U])) 
                                            << 9U) 
                                           | ((QData)((IData)(
                                                              vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                              >> 0x17U)))));
    amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__operand_b 
        = (0xffffffffffffULL & ((4U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[2U])
                                 ? (((QData)((IData)(
                                                     vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])) 
                                     << 0x3dU) | (((QData)((IData)(
                                                                   vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])) 
                                                   << 0x1dU) 
                                                  | ((QData)((IData)(
                                                                     vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[2U])) 
                                                     >> 3U)))
                                 : (((QData)((IData)(
                                                     vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                     << 0x29U) | (((QData)((IData)(
                                                                   vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[4U])) 
                                                   << 9U) 
                                                  | ((QData)((IData)(
                                                                     vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])) 
                                                     >> 0x17U)))));
    amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__branch_cmp_b 
        = (0xffffffffffffULL & (((QData)((IData)(vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                 << 0x29U) | (((QData)((IData)(
                                                               vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[4U])) 
                                               << 9U) 
                                              | ((QData)((IData)(
                                                                 vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])) 
                                                 >> 0x17U))));
    amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__shamt 
        = (0x3fU & (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__operand_b));
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[3U] 
        = ((0xfffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[3U]) 
           | ((IData)((0xffffffffffffULL & ((0x400000U 
                                             & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])
                                             ? (((QData)((IData)(
                                                                 vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                 << 0x39U) 
                                                | (((QData)((IData)(
                                                                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                    << 0x19U) 
                                                   | ((QData)((IData)(
                                                                      vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                      >> 7U)))
                                             : ((0x200000U 
                                                 & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])
                                                 ? 
                                                ((0x100000U 
                                                  & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])
                                                  ? 
                                                 ((0x80000U 
                                                   & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])
                                                   ? 
                                                  ((4U 
                                                    & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[2U])
                                                    ? amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__operand_b
                                                    : 
                                                   (((QData)((IData)(
                                                                     vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                     << 0x39U) 
                                                    | (((QData)((IData)(
                                                                        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                        << 0x19U) 
                                                       | ((QData)((IData)(
                                                                          vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                          >> 7U))))
                                                   : 
                                                  ((0xffffffffffffULL 
                                                    & (((QData)((IData)(
                                                                        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                        << 0x19U) 
                                                       | ((QData)((IData)(
                                                                          vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                          >> 7U))) 
                                                   >> (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__shamt)))
                                                  : 
                                                 ((0x80000U 
                                                   & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])
                                                   ? 
                                                  ((((QData)((IData)(
                                                                     vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                     << 0x39U) 
                                                    | (((QData)((IData)(
                                                                        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                        << 0x19U) 
                                                       | ((QData)((IData)(
                                                                          vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                          >> 7U))) 
                                                   << (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__shamt))
                                                   : 
                                                  ((((QData)((IData)(
                                                                     vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                     << 0x39U) 
                                                    | (((QData)((IData)(
                                                                        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                        << 0x19U) 
                                                       | ((QData)((IData)(
                                                                          vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                          >> 7U))) 
                                                   ^ amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__operand_b)))
                                                 : 
                                                ((0x100000U 
                                                  & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])
                                                  ? 
                                                 ((0x80000U 
                                                   & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])
                                                   ? 
                                                  ((((QData)((IData)(
                                                                     vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                     << 0x39U) 
                                                    | (((QData)((IData)(
                                                                        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                        << 0x19U) 
                                                       | ((QData)((IData)(
                                                                          vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                          >> 7U))) 
                                                   | amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__operand_b)
                                                   : 
                                                  ((((QData)((IData)(
                                                                     vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                     << 0x39U) 
                                                    | (((QData)((IData)(
                                                                        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                        << 0x19U) 
                                                       | ((QData)((IData)(
                                                                          vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                          >> 7U))) 
                                                   & amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__operand_b))
                                                  : 
                                                 ((0x80000U 
                                                   & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])
                                                   ? 
                                                  ((((QData)((IData)(
                                                                     vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                     << 0x39U) 
                                                    | (((QData)((IData)(
                                                                        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                        << 0x19U) 
                                                       | ((QData)((IData)(
                                                                          vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                          >> 7U))) 
                                                   - amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__operand_b)
                                                   : 
                                                  ((((QData)((IData)(
                                                                     vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                     << 0x39U) 
                                                    | (((QData)((IData)(
                                                                        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                        << 0x19U) 
                                                       | ((QData)((IData)(
                                                                          vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                          >> 7U))) 
                                                   + amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__operand_b))))))) 
              << 0xcU));
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[4U] 
        = ((0x10000000U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[4U]) 
           | (0x1fffffffU & (((IData)((0xffffffffffffULL 
                                       & ((0x400000U 
                                           & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])
                                           ? (((QData)((IData)(
                                                               vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                               << 0x39U) 
                                              | (((QData)((IData)(
                                                                  vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                  << 0x19U) 
                                                 | ((QData)((IData)(
                                                                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                    >> 7U)))
                                           : ((0x200000U 
                                               & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])
                                               ? ((0x100000U 
                                                   & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])
                                                   ? 
                                                  ((0x80000U 
                                                    & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])
                                                    ? 
                                                   ((4U 
                                                     & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[2U])
                                                     ? amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__operand_b
                                                     : 
                                                    (((QData)((IData)(
                                                                      vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                      << 0x39U) 
                                                     | (((QData)((IData)(
                                                                         vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                         << 0x19U) 
                                                        | ((QData)((IData)(
                                                                           vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                           >> 7U))))
                                                    : 
                                                   ((0xffffffffffffULL 
                                                     & (((QData)((IData)(
                                                                         vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                         << 0x19U) 
                                                        | ((QData)((IData)(
                                                                           vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                           >> 7U))) 
                                                    >> (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__shamt)))
                                                   : 
                                                  ((0x80000U 
                                                    & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])
                                                    ? 
                                                   ((((QData)((IData)(
                                                                      vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                      << 0x39U) 
                                                     | (((QData)((IData)(
                                                                         vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                         << 0x19U) 
                                                        | ((QData)((IData)(
                                                                           vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                           >> 7U))) 
                                                    << (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__shamt))
                                                    : 
                                                   ((((QData)((IData)(
                                                                      vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                      << 0x39U) 
                                                     | (((QData)((IData)(
                                                                         vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                         << 0x19U) 
                                                        | ((QData)((IData)(
                                                                           vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                           >> 7U))) 
                                                    ^ amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__operand_b)))
                                               : ((0x100000U 
                                                   & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])
                                                   ? 
                                                  ((0x80000U 
                                                    & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])
                                                    ? 
                                                   ((((QData)((IData)(
                                                                      vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                      << 0x39U) 
                                                     | (((QData)((IData)(
                                                                         vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                         << 0x19U) 
                                                        | ((QData)((IData)(
                                                                           vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                           >> 7U))) 
                                                    | amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__operand_b)
                                                    : 
                                                   ((((QData)((IData)(
                                                                      vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                      << 0x39U) 
                                                     | (((QData)((IData)(
                                                                         vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                         << 0x19U) 
                                                        | ((QData)((IData)(
                                                                           vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                           >> 7U))) 
                                                    & amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__operand_b))
                                                   : 
                                                  ((0x80000U 
                                                    & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])
                                                    ? 
                                                   ((((QData)((IData)(
                                                                      vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                      << 0x39U) 
                                                     | (((QData)((IData)(
                                                                         vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                         << 0x19U) 
                                                        | ((QData)((IData)(
                                                                           vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                           >> 7U))) 
                                                    - amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__operand_b)
                                                    : 
                                                   ((((QData)((IData)(
                                                                      vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                      << 0x39U) 
                                                     | (((QData)((IData)(
                                                                         vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                         << 0x19U) 
                                                        | ((QData)((IData)(
                                                                           vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                           >> 7U))) 
                                                    + amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__operand_b))))))) 
                              >> 0x14U) | ((IData)(
                                                   ((0xffffffffffffULL 
                                                     & ((0x400000U 
                                                         & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])
                                                         ? 
                                                        (((QData)((IData)(
                                                                          vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                          << 0x39U) 
                                                         | (((QData)((IData)(
                                                                             vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                             << 0x19U) 
                                                            | ((QData)((IData)(
                                                                               vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                               >> 7U)))
                                                         : 
                                                        ((0x200000U 
                                                          & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])
                                                          ? 
                                                         ((0x100000U 
                                                           & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])
                                                           ? 
                                                          ((0x80000U 
                                                            & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])
                                                            ? 
                                                           ((4U 
                                                             & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[2U])
                                                             ? amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__operand_b
                                                             : 
                                                            (((QData)((IData)(
                                                                              vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                              << 0x39U) 
                                                             | (((QData)((IData)(
                                                                                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                                 << 0x19U) 
                                                                | ((QData)((IData)(
                                                                                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                                   >> 7U))))
                                                            : 
                                                           ((0xffffffffffffULL 
                                                             & (((QData)((IData)(
                                                                                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                                 << 0x19U) 
                                                                | ((QData)((IData)(
                                                                                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                                   >> 7U))) 
                                                            >> (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__shamt)))
                                                           : 
                                                          ((0x80000U 
                                                            & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])
                                                            ? 
                                                           ((((QData)((IData)(
                                                                              vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                              << 0x39U) 
                                                             | (((QData)((IData)(
                                                                                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                                 << 0x19U) 
                                                                | ((QData)((IData)(
                                                                                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                                   >> 7U))) 
                                                            << (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__shamt))
                                                            : 
                                                           ((((QData)((IData)(
                                                                              vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                              << 0x39U) 
                                                             | (((QData)((IData)(
                                                                                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                                 << 0x19U) 
                                                                | ((QData)((IData)(
                                                                                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                                   >> 7U))) 
                                                            ^ amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__operand_b)))
                                                          : 
                                                         ((0x100000U 
                                                           & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])
                                                           ? 
                                                          ((0x80000U 
                                                            & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])
                                                            ? 
                                                           ((((QData)((IData)(
                                                                              vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                              << 0x39U) 
                                                             | (((QData)((IData)(
                                                                                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                                 << 0x19U) 
                                                                | ((QData)((IData)(
                                                                                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                                   >> 7U))) 
                                                            | amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__operand_b)
                                                            : 
                                                           ((((QData)((IData)(
                                                                              vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                              << 0x39U) 
                                                             | (((QData)((IData)(
                                                                                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                                 << 0x19U) 
                                                                | ((QData)((IData)(
                                                                                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                                   >> 7U))) 
                                                            & amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__operand_b))
                                                           : 
                                                          ((0x80000U 
                                                            & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U])
                                                            ? 
                                                           ((((QData)((IData)(
                                                                              vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                              << 0x39U) 
                                                             | (((QData)((IData)(
                                                                                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                                 << 0x19U) 
                                                                | ((QData)((IData)(
                                                                                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                                   >> 7U))) 
                                                            - amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__operand_b)
                                                            : 
                                                           ((((QData)((IData)(
                                                                              vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                              << 0x39U) 
                                                             | (((QData)((IData)(
                                                                                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                                 << 0x19U) 
                                                                | ((QData)((IData)(
                                                                                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                                   >> 7U))) 
                                                            + amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__operand_b)))))) 
                                                    >> 0x20U)) 
                                           << 0xcU))));
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[1U] 
        = ((0xffefffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[1U]) 
           | (((2U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[2U])
                ? ((1U & (~ vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[2U])) 
                   && ((1U & (~ (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[1U] 
                                 >> 0x1fU))) && ((0x40000000U 
                                                  & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[1U])
                                                  ? 
                                                 (0ULL 
                                                  != 
                                                  (0xffffffffffffULL 
                                                   & (((QData)((IData)(
                                                                       vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                       << 0x19U) 
                                                      | ((QData)((IData)(
                                                                         vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                         >> 7U))))
                                                  : 
                                                 (0ULL 
                                                  == 
                                                  (0xffffffffffffULL 
                                                   & (((QData)((IData)(
                                                                       vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                       << 0x19U) 
                                                      | ((QData)((IData)(
                                                                         vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                         >> 7U)))))))
                : ((1U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[2U])
                    ? ((vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[1U] 
                        >> 0x1fU) ? ((0x40000000U & 
                                      vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[1U])
                                      ? VL_GTS_IQQ(48, 
                                                   (0xffffffffffffULL 
                                                    & (((QData)((IData)(
                                                                        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                        << 0x19U) 
                                                       | ((QData)((IData)(
                                                                          vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                          >> 7U))), amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__branch_cmp_b)
                                      : ((0xffffffffffffULL 
                                          & (((QData)((IData)(
                                                              vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                              << 0x19U) 
                                             | ((QData)((IData)(
                                                                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                >> 7U))) 
                                         > amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__branch_cmp_b))
                        : ((0x40000000U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[1U])
                            ? VL_LTS_IQQ(48, (0xffffffffffffULL 
                                              & (((QData)((IData)(
                                                                  vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                  << 0x19U) 
                                                 | ((QData)((IData)(
                                                                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                    >> 7U))), amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__branch_cmp_b)
                            : ((0xffffffffffffULL & 
                                (((QData)((IData)(vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                  << 0x19U) | ((QData)((IData)(
                                                               vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                               >> 7U))) 
                               < amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__branch_cmp_b)))
                    : ((vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[1U] 
                        >> 0x1fU) ? ((0x40000000U & 
                                      vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[1U])
                                      ? ((0xffffffffffffULL 
                                          & (((QData)((IData)(
                                                              vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                              << 0x19U) 
                                             | ((QData)((IData)(
                                                                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                >> 7U))) 
                                         != amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__branch_cmp_b)
                                      : ((0xffffffffffffULL 
                                          & (((QData)((IData)(
                                                              vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                              << 0x19U) 
                                             | ((QData)((IData)(
                                                                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                >> 7U))) 
                                         == amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__branch_cmp_b))
                        : ((1U & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[1U] 
                                  >> 0x1eU)) && (1U 
                                                 & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[8U] 
                                                    >> 7U)))))) 
              << 0x14U));
    if ((0x8000000U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[1U])) {
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[0U] 
            = ((0xfU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[0U]) 
               | ((IData)((((QData)((IData)((1U & (
                                                   vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[8U] 
                                                   >> 7U)))) 
                            << 0x30U) | (0xffffffffffffULL 
                                         & (((QData)((IData)(
                                                             vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                             << 0x19U) 
                                            | ((QData)((IData)(
                                                               vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                               >> 7U))))) 
                  << 4U));
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[1U] 
            = ((0xffe00000U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[1U]) 
               | (((IData)((((QData)((IData)((1U & 
                                              (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[8U] 
                                               >> 7U)))) 
                             << 0x30U) | (0xffffffffffffULL 
                                          & (((QData)((IData)(
                                                              vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                              << 0x19U) 
                                             | ((QData)((IData)(
                                                                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                >> 7U))))) 
                   >> 0x1cU) | ((IData)(((((QData)((IData)(
                                                           (1U 
                                                            & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[8U] 
                                                               >> 7U)))) 
                                           << 0x30U) 
                                          | (0xffffffffffffULL 
                                             & (((QData)((IData)(
                                                                 vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U])) 
                                                 << 0x19U) 
                                                | ((QData)((IData)(
                                                                   vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U])) 
                                                   >> 7U)))) 
                                         >> 0x20U)) 
                                << 4U)));
    }
    if ((0x10000000U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[1U])) {
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[3U] 
            = ((0xfffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[3U]) 
               | ((IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__return_addr) 
                  << 0xcU));
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[4U] 
            = ((0x10000000U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[4U]) 
               | (0x1fffffffU & (((IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__return_addr) 
                                  >> 0x14U) | ((IData)(
                                                       (amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__return_addr 
                                                        >> 0x20U)) 
                                               << 0xcU))));
    }
    amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__trap_active 
        = (IData)((0U != (0xfU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[0U])));
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[0U] 
        = ((0xfffffff7U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[0U]) 
           | ((IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__trap_active) 
              << 3U));
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[0U] 
        = ((0xfffffff8U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[0U]) 
           | ((IData)(amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__trap_active)
               ? ((0U != (7U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[0U]))
                   ? (7U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[0U])
                   : 1U) : 0U));
}

VL_INLINE_OPT void Vamber48_core_smoke_tb___024root___nba_sequent__TOP__1(Vamber48_core_smoke_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vamber48_core_smoke_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vamber48_core_smoke_tb___024root___nba_sequent__TOP__1\n"); );
    // Body
    if (vlSelf->__Vdlyvset__amber48_core_smoke_tb__DOT__trap_seen__v0) {
        vlSelf->amber48_core_smoke_tb__DOT__trap_seen = 1U;
        vlSelf->__Vdlyvset__amber48_core_smoke_tb__DOT__trap_seen__v0 = 0U;
    }
    if (vlSelf->__Vdlyvset__amber48_core_smoke_tb__DOT__trap_seen__v1) {
        vlSelf->amber48_core_smoke_tb__DOT__trap_seen = 0U;
        vlSelf->__Vdlyvset__amber48_core_smoke_tb__DOT__trap_seen__v1 = 0U;
    }
    if (vlSelf->amber48_core_smoke_tb__DOT__rst_ni) {
        if (((IData)(vlSelf->amber48_core_smoke_tb__DOT__uart_tx_valid) 
             & (0U == (IData)(vlSelf->amber48_core_smoke_tb__DOT__u_uart_tx__DOT__state_q)))) {
            vlSelf->amber48_core_smoke_tb__DOT__uart_index 
                = ((IData)(1U) + vlSelf->amber48_core_smoke_tb__DOT__uart_index);
            vlSelf->amber48_core_smoke_tb__DOT__uart_count 
                = ((IData)(1U) + vlSelf->amber48_core_smoke_tb__DOT__uart_count);
            if (((0x20U <= (IData)(vlSelf->amber48_core_smoke_tb__DOT__uart_tx_data)) 
                 & (0x7eU >= (IData)(vlSelf->amber48_core_smoke_tb__DOT__uart_tx_data)))) {
                VL_WRITEF("[%0t][smoke] UART TX byte: 0x%0x (%c)\n",
                          64,VL_TIME_UNITED_Q(1000),
                          -9,8,(IData)(vlSelf->amber48_core_smoke_tb__DOT__uart_tx_data),
                          8,vlSelf->amber48_core_smoke_tb__DOT__uart_tx_data);
            } else {
                VL_WRITEF("[%0t][smoke] UART TX byte: 0x%0x\n",
                          64,VL_TIME_UNITED_Q(1000),
                          -9,8,(IData)(vlSelf->amber48_core_smoke_tb__DOT__uart_tx_data));
            }
        }
    } else {
        vlSelf->amber48_core_smoke_tb__DOT__uart_index = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__uart_count = 0U;
    }
    if (vlSelf->amber48_core_smoke_tb__DOT__rst_ni) {
        if (((((IData)(vlSelf->amber48_core_smoke_tb__DOT__dmem_req) 
               & (IData)(vlSelf->amber48_core_smoke_tb__DOT__dmem_ready)) 
              & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[1U] 
                 >> 0x15U)) & (0ULL == (0xffffffffffffULL 
                                        & (((QData)((IData)(
                                                            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[4U])) 
                                            << 0x14U) 
                                           | ((QData)((IData)(
                                                              vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[3U])) 
                                              >> 0xcU)))))) {
            vlSelf->amber48_core_smoke_tb__DOT__store0_seen = 1U;
        }
    } else {
        vlSelf->amber48_core_smoke_tb__DOT__store0_seen = 0U;
    }
}

VL_INLINE_OPT void Vamber48_core_smoke_tb___024root___nba_sequent__TOP__2(Vamber48_core_smoke_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vamber48_core_smoke_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vamber48_core_smoke_tb___024root___nba_sequent__TOP__2\n"); );
    // Init
    QData/*47:0*/ amber48_core_smoke_tb__DOT__u_core__DOT__rf_rs2;
    amber48_core_smoke_tb__DOT__u_core__DOT__rf_rs2 = 0;
    CData/*0:0*/ amber48_core_smoke_tb__DOT__u_core__DOT__pipeline_stall;
    amber48_core_smoke_tb__DOT__u_core__DOT__pipeline_stall = 0;
    CData/*0:0*/ amber48_core_smoke_tb__DOT__u_core__DOT__flush_branch;
    amber48_core_smoke_tb__DOT__u_core__DOT__flush_branch = 0;
    CData/*0:0*/ amber48_core_smoke_tb__DOT__u_core__DOT__flush_trap;
    amber48_core_smoke_tb__DOT__u_core__DOT__flush_trap = 0;
    VlWide<4>/*126:0*/ amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff;
    VL_ZERO_W(127, amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff);
    VlWide<4>/*127:0*/ __Vtemp_4;
    // Body
    if (vlSelf->amber48_core_smoke_tb__DOT__rst_ni) {
        vlSelf->amber48_core_smoke_tb__DOT__uart_tx_valid = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__dmem_ready = 0U;
        if (vlSelf->amber48_core_smoke_tb__DOT__dmem_req) {
            if ((1U & (~ (IData)(((0U != (0x7000U & 
                                          vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[3U])) 
                                  | (0U != (0x1fffffffffULL 
                                            & (((QData)((IData)(
                                                                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[4U])) 
                                                << 9U) 
                                               | ((QData)((IData)(
                                                                  vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[3U])) 
                                                  >> 0x17U))))))))) {
                if ((0xfeU == (0xffU & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[3U] 
                                        >> 0xfU)))) {
                    if ((0x200000U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[1U])) {
                        if ((0U == (IData)(vlSelf->amber48_core_smoke_tb__DOT__u_uart_tx__DOT__state_q))) {
                            vlSelf->amber48_core_smoke_tb__DOT__uart_tx_valid = 1U;
                            vlSelf->amber48_core_smoke_tb__DOT__uart_tx_data 
                                = (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[1U] 
                                   >> 0x18U);
                        }
                    }
                }
            }
            if ((IData)(((0U != (0x7000U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[3U])) 
                         | (0U != (0x1fffffffffULL 
                                   & (((QData)((IData)(
                                                       vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[4U])) 
                                       << 9U) | ((QData)((IData)(
                                                                 vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[3U])) 
                                                 >> 0x17U))))))) {
                vlSelf->amber48_core_smoke_tb__DOT__dmem_ready = 1U;
            } else if ((0xfeU == (0xffU & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[3U] 
                                           >> 0xfU)))) {
                if ((0x200000U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[1U])) {
                    if ((0U == (IData)(vlSelf->amber48_core_smoke_tb__DOT__u_uart_tx__DOT__state_q))) {
                        vlSelf->amber48_core_smoke_tb__DOT__dmem_ready = 1U;
                    }
                } else {
                    vlSelf->amber48_core_smoke_tb__DOT__dmem_ready = 1U;
                }
            } else {
                vlSelf->amber48_core_smoke_tb__DOT__dmem_ready = 1U;
            }
        }
    } else {
        vlSelf->amber48_core_smoke_tb__DOT__uart_tx_valid = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__uart_tx_data = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__dmem_ready = 0U;
    }
    vlSelf->amber48_core_smoke_tb__DOT__u_uart_tx__DOT__state_q 
        = vlSelf->__Vdly__amber48_core_smoke_tb__DOT__u_uart_tx__DOT__state_q;
    amber48_core_smoke_tb__DOT__u_core__DOT__pipeline_stall 
        = ((((~ (IData)(vlSelf->amber48_core_smoke_tb__DOT__dmem_ready)) 
             & (~ (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[0U] 
                   >> 3U))) & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[8U] 
                               >> 7U)) & (0U != (0x30U 
                                                 & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[0U])));
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[0U] 
        = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[0U];
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[1U] 
        = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[1U];
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[2U] 
        = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[2U];
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[3U] 
        = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[3U];
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[4U] 
        = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[4U];
    if (((((vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[4U] 
            >> 0x1cU) & (IData)((0U != (0x600000U & 
                                        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r[1U])))) 
          & (IData)(vlSelf->amber48_core_smoke_tb__DOT__dmem_ready)) 
         & (IData)(vlSelf->amber48_core_smoke_tb__DOT__dmem_trap))) {
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[0U] 
            = (0xaU | (0xfffffff0U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[0U]));
    }
    vlSelf->amber48_core_smoke_tb__DOT__dmem_req = 
        (((~ (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[0U] 
              >> 3U)) & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[4U] 
                         >> 0x1cU)) & (0U != (0x600000U 
                                              & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[1U])));
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__rf_req 
        = (((QData)((IData)((0x100U | (0xffU & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[2U] 
                                                >> 6U))))) 
            << 0x35U) | (((QData)((IData)((0xfU & (
                                                   vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[3U] 
                                                   >> 8U)))) 
                          << 0x31U) | (((QData)((IData)(
                                                        ((((~ 
                                                            (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[0U] 
                                                             >> 3U)) 
                                                           & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[1U] 
                                                              >> 0x17U)) 
                                                          & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[4U] 
                                                             >> 0x1cU)) 
                                                         & ((~ 
                                                             (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[1U] 
                                                              >> 0x16U)) 
                                                            | (IData)(vlSelf->amber48_core_smoke_tb__DOT__dmem_ready))))) 
                                        << 0x30U) | 
                                       (0xffffffffffffULL 
                                        & ((0x400000U 
                                            & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[1U])
                                            ? vlSelf->amber48_core_smoke_tb__DOT__dmem_rdata
                                            : (((QData)((IData)(
                                                                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[4U])) 
                                                << 0x34U) 
                                               | (((QData)((IData)(
                                                                   vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[4U])) 
                                                   << 0x14U) 
                                                  | ((QData)((IData)(
                                                                     vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[3U])) 
                                                     >> 0xcU))))))));
    amber48_core_smoke_tb__DOT__u_core__DOT__rf_rs2 
        = ((0U == (0xfU & (IData)((vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__rf_req 
                                   >> 0x35U)))) ? 0ULL
            : vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs
           [(0xfU & (IData)((vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__rf_req 
                             >> 0x35U)))]);
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__pc_next 
        = (0xffffffffffffULL & (8ULL + vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__pc_q));
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[0U] 
        = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[0U];
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[1U] 
        = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[1U];
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[2U] 
        = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[2U];
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[3U] 
        = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[3U];
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[0U] 
        = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U];
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[1U] 
        = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U];
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[2U] 
        = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U];
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[3U] 
        = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[3U];
    amber48_core_smoke_tb__DOT__u_core__DOT__flush_branch 
        = ((vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[1U] 
            >> 0x14U) & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[4U] 
                         >> 0x1cU));
    amber48_core_smoke_tb__DOT__u_core__DOT__flush_trap 
        = ((vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[0U] 
            >> 3U) & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[4U] 
                      >> 0x1cU));
    amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[0U] 
        = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[0U];
    amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[1U] 
        = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[1U];
    amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[2U] 
        = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[2U];
    amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[3U] 
        = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[3U];
    if (((IData)(amber48_core_smoke_tb__DOT__u_core__DOT__flush_branch) 
         | (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__flush_trap))) {
        amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[0U] = 0U;
        amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[1U] = 0U;
        amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[2U] = 0U;
        amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[3U] = 0U;
    }
    if (vlSelf->amber48_core_smoke_tb__DOT__imem_valid) {
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[0U] 
            = (IData)(vlSelf->amber48_core_smoke_tb__DOT__u_imem__DOT__data_q);
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[1U] 
            = (((IData)(vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__pc_q) 
                << 0x10U) | (IData)((vlSelf->amber48_core_smoke_tb__DOT__u_imem__DOT__data_q 
                                     >> 0x20U)));
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[2U] 
            = (((IData)(vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__pc_q) 
                >> 0x10U) | ((IData)((vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__pc_q 
                                      >> 0x20U)) << 0x10U));
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[3U] = 1U;
    } else if ((1U & (~ (IData)(amber48_core_smoke_tb__DOT__u_core__DOT__pipeline_stall)))) {
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[0U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[1U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[2U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[3U] = 0U;
    }
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[0U] 
        = Vamber48_core_smoke_tb__ConstPool__CONST_h2a0d1dfa_0[0U];
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[1U] 
        = Vamber48_core_smoke_tb__ConstPool__CONST_h2a0d1dfa_0[1U];
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[2U] 
        = Vamber48_core_smoke_tb__ConstPool__CONST_h2a0d1dfa_0[2U];
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[3U] 
        = Vamber48_core_smoke_tb__ConstPool__CONST_h2a0d1dfa_0[3U];
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[4U] 
        = Vamber48_core_smoke_tb__ConstPool__CONST_h2a0d1dfa_0[4U];
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[5U] 
        = Vamber48_core_smoke_tb__ConstPool__CONST_h2a0d1dfa_0[5U];
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[6U] 
        = Vamber48_core_smoke_tb__ConstPool__CONST_h2a0d1dfa_0[6U];
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[7U] 
        = Vamber48_core_smoke_tb__ConstPool__CONST_h2a0d1dfa_0[7U];
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[8U] 
        = Vamber48_core_smoke_tb__ConstPool__CONST_h2a0d1dfa_0[8U];
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[3U] 
        = ((0x7fffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[3U]) 
           | ((IData)(amber48_core_smoke_tb__DOT__u_core__DOT__rf_rs2) 
              << 0x17U));
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[4U] 
        = (((IData)(amber48_core_smoke_tb__DOT__u_core__DOT__rf_rs2) 
            >> 9U) | ((IData)((amber48_core_smoke_tb__DOT__u_core__DOT__rf_rs2 
                               >> 0x20U)) << 0x17U));
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[5U] 
        = (((0x7fff80U & ((IData)(((0U == (0xfU & (IData)(
                                                          (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__rf_req 
                                                           >> 0x39U))))
                                    ? 0ULL : vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs
                                   [(0xfU & (IData)(
                                                    (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__rf_req 
                                                     >> 0x39U)))])) 
                          << 7U)) | ((IData)((amber48_core_smoke_tb__DOT__u_core__DOT__rf_rs2 
                                              >> 0x20U)) 
                                     >> 9U)) | (0xff800000U 
                                                & ((IData)(
                                                           ((0U 
                                                             == 
                                                             (0xfU 
                                                              & (IData)(
                                                                        (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__rf_req 
                                                                         >> 0x39U))))
                                                             ? 0ULL
                                                             : 
                                                            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs
                                                            [
                                                            (0xfU 
                                                             & (IData)(
                                                                       (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__rf_req 
                                                                        >> 0x39U)))])) 
                                                   << 7U)));
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[6U] 
        = ((((IData)(((0U == (0xfU & (IData)((vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__rf_req 
                                              >> 0x39U))))
                       ? 0ULL : vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs
                      [(0xfU & (IData)((vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__rf_req 
                                        >> 0x39U)))])) 
             >> 0x19U) | (0x7fff80U & ((IData)((((0U 
                                                  == 
                                                  (0xfU 
                                                   & (IData)(
                                                             (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__rf_req 
                                                              >> 0x39U))))
                                                  ? 0ULL
                                                  : 
                                                 vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs
                                                 [(0xfU 
                                                   & (IData)(
                                                             (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__rf_req 
                                                              >> 0x39U)))]) 
                                                >> 0x20U)) 
                                       << 7U))) | ((IData)(
                                                           (0x1ffffffffffffULL 
                                                            & (((QData)((IData)(
                                                                                amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[3U])) 
                                                                << 0x12U) 
                                                               | ((QData)((IData)(
                                                                                amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[2U])) 
                                                                  >> 0xeU)))) 
                                                   << 0x17U));
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[7U] 
        = (((IData)((0x1ffffffffffffULL & (((QData)((IData)(
                                                            amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[3U])) 
                                            << 0x12U) 
                                           | ((QData)((IData)(
                                                              amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[2U])) 
                                              >> 0xeU)))) 
            >> 9U) | ((IData)(((0x1ffffffffffffULL 
                                & (((QData)((IData)(
                                                    amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[3U])) 
                                    << 0x12U) | ((QData)((IData)(
                                                                 amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[2U])) 
                                                 >> 0xeU))) 
                               >> 0x20U)) << 0x17U));
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[8U] 
        = (0xffU & ((IData)(((0x1ffffffffffffULL & 
                              (((QData)((IData)(amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[3U])) 
                                << 0x12U) | ((QData)((IData)(
                                                             amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[2U])) 
                                             >> 0xeU))) 
                             >> 0x20U)) >> 9U));
    __Vtemp_4[2U] = (((IData)(((0xffffffffffffff80ULL 
                                & (((QData)((IData)(
                                                    amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[1U])) 
                                    << 0x21U) | (0xffffffffffffff80ULL 
                                                 & ((QData)((IData)(
                                                                    amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[0U])) 
                                                    << 1U)))) 
                               | (QData)((IData)((0x7fU 
                                                  & ((amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[2U] 
                                                      << 1U) 
                                                     | (amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[1U] 
                                                        >> 0x1fU))))))) 
                      >> 0x10U) | ((IData)((((0xffffffffffffff80ULL 
                                              & (((QData)((IData)(
                                                                  amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[1U])) 
                                                  << 0x21U) 
                                                 | (0xffffffffffffff80ULL 
                                                    & ((QData)((IData)(
                                                                       amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[0U])) 
                                                       << 1U)))) 
                                             | (QData)((IData)(
                                                               (0x7fU 
                                                                & ((amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[2U] 
                                                                    << 1U) 
                                                                   | (amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[1U] 
                                                                      >> 0x1fU)))))) 
                                            >> 0x20U)) 
                                   << 0x10U));
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[0U] 
        = ((0x7fU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[0U]) 
           | ((IData)(amber48_core_smoke_tb__DOT__u_core__DOT__rf_rs2) 
              << 7U));
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[1U] 
        = (((IData)(amber48_core_smoke_tb__DOT__u_core__DOT__rf_rs2) 
            >> 0x19U) | (((IData)(((0xffffffffffffff80ULL 
                                    & (((QData)((IData)(
                                                        amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[1U])) 
                                        << 0x21U) | 
                                       (0xffffffffffffff80ULL 
                                        & ((QData)((IData)(
                                                           amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[0U])) 
                                           << 1U)))) 
                                   | (QData)((IData)(
                                                     (0x7fU 
                                                      & ((amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[2U] 
                                                          << 1U) 
                                                         | (amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[1U] 
                                                            >> 0x1fU))))))) 
                          << 0x17U) | ((IData)((amber48_core_smoke_tb__DOT__u_core__DOT__rf_rs2 
                                                >> 0x20U)) 
                                       << 7U)));
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[2U] 
        = (((0x7fU & ((IData)(((0xffffffffffffff80ULL 
                                & (((QData)((IData)(
                                                    amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[1U])) 
                                    << 0x21U) | (0xffffffffffffff80ULL 
                                                 & ((QData)((IData)(
                                                                    amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[0U])) 
                                                    << 1U)))) 
                               | (QData)((IData)((0x7fU 
                                                  & ((amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[2U] 
                                                      << 1U) 
                                                     | (amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[1U] 
                                                        >> 0x1fU))))))) 
                      >> 9U)) | ((IData)((amber48_core_smoke_tb__DOT__u_core__DOT__rf_rs2 
                                          >> 0x20U)) 
                                 >> 0x19U)) | (__Vtemp_4[2U] 
                                               << 7U));
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[3U] 
        = ((0xff800000U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[3U]) 
           | ((__Vtemp_4[2U] >> 0x19U) | (0x7fff80U 
                                          & ((IData)(
                                                     (((0xffffffffffffff80ULL 
                                                        & (((QData)((IData)(
                                                                            amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[1U])) 
                                                            << 0x21U) 
                                                           | (0xffffffffffffff80ULL 
                                                              & ((QData)((IData)(
                                                                                amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[0U])) 
                                                                 << 1U)))) 
                                                       | (QData)((IData)(
                                                                         (0x7fU 
                                                                          & ((amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[2U] 
                                                                              << 1U) 
                                                                             | (amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[1U] 
                                                                                >> 0x1fU)))))) 
                                                      >> 0x20U)) 
                                             >> 9U))));
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[0U] 
        = ((0xffffffbfU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[0U]) 
           | (0xffffffc0U & ((((~ (amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[0U] 
                                   >> 3U)) & (amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[3U] 
                                              >> 0x1eU)) 
                              << 6U) & ((((IData)((0U 
                                                   == 
                                                   (0x3d0U 
                                                    & amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[0U]))) 
                                          & (0U != 
                                             (0xfU 
                                              & ((amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[2U] 
                                                  << 1U) 
                                                 | (amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[1U] 
                                                    >> 0x1fU))))) 
                                         << 6U) | (0xffffffc0U 
                                                   & (amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[2U] 
                                                      << 2U))))));
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[0U] 
        = ((0xffffffc0U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[0U]) 
           | (0x3fU & amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[0U]));
    if ((0x400U & amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[0U])) {
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[3U] 
            = ((0x7fffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[3U]) 
               | ((IData)((0xffffffffffffULL & (((QData)((IData)(
                                                                 amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[1U])) 
                                                 << 0x15U) 
                                                | ((QData)((IData)(
                                                                   amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[0U])) 
                                                   >> 0xbU)))) 
                  << 0x17U));
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[4U] 
            = (((IData)((0xffffffffffffULL & (((QData)((IData)(
                                                               amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[1U])) 
                                               << 0x15U) 
                                              | ((QData)((IData)(
                                                                 amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[0U])) 
                                                 >> 0xbU)))) 
                >> 9U) | ((IData)(((0xffffffffffffULL 
                                    & (((QData)((IData)(
                                                        amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[1U])) 
                                        << 0x15U) | 
                                       ((QData)((IData)(
                                                        amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[0U])) 
                                        >> 0xbU))) 
                                   >> 0x20U)) << 0x17U));
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[5U] 
            = ((0xffffff80U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[5U]) 
               | ((IData)(((0xffffffffffffULL & (((QData)((IData)(
                                                                  amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[1U])) 
                                                  << 0x15U) 
                                                 | ((QData)((IData)(
                                                                    amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_eff[0U])) 
                                                    >> 0xbU))) 
                           >> 0x20U)) >> 9U));
    }
    if (amber48_core_smoke_tb__DOT__u_core__DOT__flush_branch) {
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__pc_next 
            = (0xffffffffffffULL & (((QData)((IData)(
                                                     vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[1U])) 
                                     << 0x1cU) | ((QData)((IData)(
                                                                  vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[0U])) 
                                                  >> 4U)));
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[0U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[1U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[2U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[3U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[0U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[1U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[2U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[3U] = 0U;
    }
    if (amber48_core_smoke_tb__DOT__u_core__DOT__flush_trap) {
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[0U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[1U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[2U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[3U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[0U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[1U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[2U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[3U] = 0U;
    }
    if (amber48_core_smoke_tb__DOT__u_core__DOT__pipeline_stall) {
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__pc_next 
            = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__pc_q;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[0U] 
            = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[0U];
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[1U] 
            = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[1U];
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[2U] 
            = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[2U];
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[3U] 
            = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[3U];
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[0U] 
            = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[0U];
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[1U] 
            = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[1U];
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[2U] 
            = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[2U];
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[3U] 
            = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[3U];
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[0U] 
            = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[0U];
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[1U] 
            = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[1U];
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[2U] 
            = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[2U];
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[3U] 
            = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[3U];
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[4U] 
            = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[4U];
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[5U] 
            = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[5U];
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[6U] 
            = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[6U];
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[7U] 
            = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[7U];
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[8U] 
            = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[8U];
    }
}

void Vamber48_core_smoke_tb___024root___eval_nba(Vamber48_core_smoke_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vamber48_core_smoke_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vamber48_core_smoke_tb___024root___eval_nba\n"); );
    // Body
    if ((2ULL & vlSelf->__VnbaTriggered.word(0U))) {
        Vamber48_core_smoke_tb___024root___nba_sequent__TOP__0(vlSelf);
    }
    if ((1ULL & vlSelf->__VnbaTriggered.word(0U))) {
        Vamber48_core_smoke_tb___024root___nba_sequent__TOP__1(vlSelf);
    }
    if ((2ULL & vlSelf->__VnbaTriggered.word(0U))) {
        Vamber48_core_smoke_tb___024root___nba_sequent__TOP__2(vlSelf);
    }
}

void Vamber48_core_smoke_tb___024root___timing_resume(Vamber48_core_smoke_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vamber48_core_smoke_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vamber48_core_smoke_tb___024root___timing_resume\n"); );
    // Body
    if ((1ULL & vlSelf->__VactTriggered.word(0U))) {
        vlSelf->__VtrigSched_hb844875a__0.resume("@(posedge amber48_core_smoke_tb.clk)");
    }
    if ((4ULL & vlSelf->__VactTriggered.word(0U))) {
        vlSelf->__VtrigSched_hd1065de6__0.resume("@([changed] amber48_core_smoke_tb.rst_ni)");
    }
    if ((8ULL & vlSelf->__VactTriggered.word(0U))) {
        vlSelf->__VdlySched.resume();
    }
}

void Vamber48_core_smoke_tb___024root___timing_commit(Vamber48_core_smoke_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vamber48_core_smoke_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vamber48_core_smoke_tb___024root___timing_commit\n"); );
    // Body
    if ((! (1ULL & vlSelf->__VactTriggered.word(0U)))) {
        vlSelf->__VtrigSched_hb844875a__0.commit("@(posedge amber48_core_smoke_tb.clk)");
    }
    if ((! (4ULL & vlSelf->__VactTriggered.word(0U)))) {
        vlSelf->__VtrigSched_hd1065de6__0.commit("@([changed] amber48_core_smoke_tb.rst_ni)");
    }
}

void Vamber48_core_smoke_tb___024root___eval_triggers__act(Vamber48_core_smoke_tb___024root* vlSelf);

bool Vamber48_core_smoke_tb___024root___eval_phase__act(Vamber48_core_smoke_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vamber48_core_smoke_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vamber48_core_smoke_tb___024root___eval_phase__act\n"); );
    // Init
    VlTriggerVec<4> __VpreTriggered;
    CData/*0:0*/ __VactExecute;
    // Body
    Vamber48_core_smoke_tb___024root___eval_triggers__act(vlSelf);
    Vamber48_core_smoke_tb___024root___timing_commit(vlSelf);
    __VactExecute = vlSelf->__VactTriggered.any();
    if (__VactExecute) {
        __VpreTriggered.andNot(vlSelf->__VactTriggered, vlSelf->__VnbaTriggered);
        vlSelf->__VnbaTriggered.thisOr(vlSelf->__VactTriggered);
        Vamber48_core_smoke_tb___024root___timing_resume(vlSelf);
        Vamber48_core_smoke_tb___024root___eval_act(vlSelf);
    }
    return (__VactExecute);
}

bool Vamber48_core_smoke_tb___024root___eval_phase__nba(Vamber48_core_smoke_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vamber48_core_smoke_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vamber48_core_smoke_tb___024root___eval_phase__nba\n"); );
    // Init
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = vlSelf->__VnbaTriggered.any();
    if (__VnbaExecute) {
        Vamber48_core_smoke_tb___024root___eval_nba(vlSelf);
        vlSelf->__VnbaTriggered.clear();
    }
    return (__VnbaExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vamber48_core_smoke_tb___024root___dump_triggers__nba(Vamber48_core_smoke_tb___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Vamber48_core_smoke_tb___024root___dump_triggers__act(Vamber48_core_smoke_tb___024root* vlSelf);
#endif  // VL_DEBUG

void Vamber48_core_smoke_tb___024root___eval(Vamber48_core_smoke_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vamber48_core_smoke_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vamber48_core_smoke_tb___024root___eval\n"); );
    // Init
    IData/*31:0*/ __VnbaIterCount;
    CData/*0:0*/ __VnbaContinue;
    // Body
    __VnbaIterCount = 0U;
    __VnbaContinue = 1U;
    while (__VnbaContinue) {
        if (VL_UNLIKELY((0x64U < __VnbaIterCount))) {
#ifdef VL_DEBUG
            Vamber48_core_smoke_tb___024root___dump_triggers__nba(vlSelf);
#endif
            VL_FATAL_MT("src/0.1/sim/tb/amber48_core_tb_base.svh", 7, "", "NBA region did not converge.");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        __VnbaContinue = 0U;
        vlSelf->__VactIterCount = 0U;
        vlSelf->__VactContinue = 1U;
        while (vlSelf->__VactContinue) {
            if (VL_UNLIKELY((0x64U < vlSelf->__VactIterCount))) {
#ifdef VL_DEBUG
                Vamber48_core_smoke_tb___024root___dump_triggers__act(vlSelf);
#endif
                VL_FATAL_MT("src/0.1/sim/tb/amber48_core_tb_base.svh", 7, "", "Active region did not converge.");
            }
            vlSelf->__VactIterCount = ((IData)(1U) 
                                       + vlSelf->__VactIterCount);
            vlSelf->__VactContinue = 0U;
            if (Vamber48_core_smoke_tb___024root___eval_phase__act(vlSelf)) {
                vlSelf->__VactContinue = 1U;
            }
        }
        if (Vamber48_core_smoke_tb___024root___eval_phase__nba(vlSelf)) {
            __VnbaContinue = 1U;
        }
    }
}

#ifdef VL_DEBUG
void Vamber48_core_smoke_tb___024root___eval_debug_assertions(Vamber48_core_smoke_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vamber48_core_smoke_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vamber48_core_smoke_tb___024root___eval_debug_assertions\n"); );
}
#endif  // VL_DEBUG
