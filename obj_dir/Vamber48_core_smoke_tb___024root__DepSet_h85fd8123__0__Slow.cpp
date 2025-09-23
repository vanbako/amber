// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vamber48_core_smoke_tb.h for the primary calling header

#include "Vamber48_core_smoke_tb__pch.h"
#include "Vamber48_core_smoke_tb___024root.h"

VL_ATTR_COLD void Vamber48_core_smoke_tb___024root___eval_static(Vamber48_core_smoke_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vamber48_core_smoke_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vamber48_core_smoke_tb___024root___eval_static\n"); );
}

VL_ATTR_COLD void Vamber48_core_smoke_tb___024root___eval_initial__TOP(Vamber48_core_smoke_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vamber48_core_smoke_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vamber48_core_smoke_tb___024root___eval_initial__TOP\n"); );
    // Init
    IData/*31:0*/ amber48_core_smoke_tb__DOT__u_dmem__DOT__init_zero__DOT__i;
    amber48_core_smoke_tb__DOT__u_dmem__DOT__init_zero__DOT__i = 0;
    // Body
    vlSelf->amber48_core_smoke_tb__DOT__clk = 0U;
    VL_READMEM_N(true, 48, 256, 0, std::string{"build/amber48_smoke.hex"}
                 ,  &(vlSelf->amber48_core_smoke_tb__DOT__u_imem__DOT__rom)
                 , 0, ~0ULL);
    amber48_core_smoke_tb__DOT__u_dmem__DOT__init_zero__DOT__i = 0U;
    while ((0x100U > amber48_core_smoke_tb__DOT__u_dmem__DOT__init_zero__DOT__i)) {
        vlSelf->amber48_core_smoke_tb__DOT__u_dmem__DOT__ram[(0xffU 
                                                              & amber48_core_smoke_tb__DOT__u_dmem__DOT__init_zero__DOT__i)] = 0ULL;
        amber48_core_smoke_tb__DOT__u_dmem__DOT__init_zero__DOT__i 
            = ((IData)(1U) + amber48_core_smoke_tb__DOT__u_dmem__DOT__init_zero__DOT__i);
    }
}

VL_ATTR_COLD void Vamber48_core_smoke_tb___024root___eval_final(Vamber48_core_smoke_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vamber48_core_smoke_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vamber48_core_smoke_tb___024root___eval_final\n"); );
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vamber48_core_smoke_tb___024root___dump_triggers__stl(Vamber48_core_smoke_tb___024root* vlSelf);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vamber48_core_smoke_tb___024root___eval_phase__stl(Vamber48_core_smoke_tb___024root* vlSelf);

VL_ATTR_COLD void Vamber48_core_smoke_tb___024root___eval_settle(Vamber48_core_smoke_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vamber48_core_smoke_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vamber48_core_smoke_tb___024root___eval_settle\n"); );
    // Init
    IData/*31:0*/ __VstlIterCount;
    CData/*0:0*/ __VstlContinue;
    // Body
    __VstlIterCount = 0U;
    vlSelf->__VstlFirstIteration = 1U;
    __VstlContinue = 1U;
    while (__VstlContinue) {
        if (VL_UNLIKELY((0x64U < __VstlIterCount))) {
#ifdef VL_DEBUG
            Vamber48_core_smoke_tb___024root___dump_triggers__stl(vlSelf);
#endif
            VL_FATAL_MT("src/0.1/sim/tb/amber48_core_tb_base.svh", 7, "", "Settle region did not converge.");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
        __VstlContinue = 0U;
        if (Vamber48_core_smoke_tb___024root___eval_phase__stl(vlSelf)) {
            __VstlContinue = 1U;
        }
        vlSelf->__VstlFirstIteration = 0U;
    }
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vamber48_core_smoke_tb___024root___dump_triggers__stl(Vamber48_core_smoke_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vamber48_core_smoke_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vamber48_core_smoke_tb___024root___dump_triggers__stl\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VstlTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelf->__VstlTriggered.word(0U))) {
        VL_DBG_MSGF("         'stl' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

extern const VlWide<9>/*287:0*/ Vamber48_core_smoke_tb__ConstPool__CONST_h2a0d1dfa_0;

VL_ATTR_COLD void Vamber48_core_smoke_tb___024root___stl_sequent__TOP__0(Vamber48_core_smoke_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vamber48_core_smoke_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vamber48_core_smoke_tb___024root___stl_sequent__TOP__0\n"); );
    // Init
    QData/*47:0*/ amber48_core_smoke_tb__DOT__u_core__DOT__rf_rs2;
    amber48_core_smoke_tb__DOT__u_core__DOT__rf_rs2 = 0;
    CData/*0:0*/ amber48_core_smoke_tb__DOT__u_core__DOT__pipeline_stall;
    amber48_core_smoke_tb__DOT__u_core__DOT__pipeline_stall = 0;
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
    VlWide<4>/*127:0*/ __Vtemp_5;
    // Body
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
    amber48_core_smoke_tb__DOT__u_core__DOT__pipeline_stall 
        = ((((~ (IData)(vlSelf->amber48_core_smoke_tb__DOT__dmem_ready)) 
             & (~ (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[0U] 
                   >> 3U))) & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[8U] 
                               >> 7U)) & (0U != (0x30U 
                                                 & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q[0U])));
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
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__pc_next 
        = (0xffffffffffffULL & (8ULL + vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__pc_q));
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[0U] 
        = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[0U];
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[1U] 
        = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[1U];
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[2U] 
        = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[2U];
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[3U] 
        = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r[3U];
    if (((vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[1U] 
          >> 0x14U) & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[4U] 
                       >> 0x1cU))) {
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__pc_next 
            = (0xffffffffffffULL & (((QData)((IData)(
                                                     vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[1U])) 
                                     << 0x1cU) | ((QData)((IData)(
                                                                  vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[0U])) 
                                                  >> 4U)));
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[0U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[1U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[2U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[3U] = 0U;
    }
    if (((vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[0U] 
          >> 3U) & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[4U] 
                    >> 0x1cU))) {
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[0U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[1U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[2U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[3U] = 0U;
    }
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[0U] 
        = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[0U];
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[1U] 
        = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[1U];
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[2U] 
        = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[2U];
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[3U] 
        = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[3U];
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
    if (((vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[1U] 
          >> 0x14U) & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[4U] 
                       >> 0x1cU))) {
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[0U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[1U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[2U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[3U] = 0U;
    }
    if (((vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[0U] 
          >> 3U) & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result[4U] 
                    >> 0x1cU))) {
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[0U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[1U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[2U] = 0U;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[3U] = 0U;
    }
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
                                                                                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[3U])) 
                                                                << 0x12U) 
                                                               | ((QData)((IData)(
                                                                                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[2U])) 
                                                                  >> 0xeU)))) 
                                                   << 0x17U));
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[7U] 
        = (((IData)((0x1ffffffffffffULL & (((QData)((IData)(
                                                            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[3U])) 
                                            << 0x12U) 
                                           | ((QData)((IData)(
                                                              vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[2U])) 
                                              >> 0xeU)))) 
            >> 9U) | ((IData)(((0x1ffffffffffffULL 
                                & (((QData)((IData)(
                                                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[3U])) 
                                    << 0x12U) | ((QData)((IData)(
                                                                 vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[2U])) 
                                                 >> 0xeU))) 
                               >> 0x20U)) << 0x17U));
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[8U] 
        = (0xffU & ((IData)(((0x1ffffffffffffULL & 
                              (((QData)((IData)(vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[3U])) 
                                << 0x12U) | ((QData)((IData)(
                                                             vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[2U])) 
                                             >> 0xeU))) 
                             >> 0x20U)) >> 9U));
    __Vtemp_5[2U] = (((IData)(((0xffffffffffffff80ULL 
                                & (((QData)((IData)(
                                                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[1U])) 
                                    << 0x21U) | (0xffffffffffffff80ULL 
                                                 & ((QData)((IData)(
                                                                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[0U])) 
                                                    << 1U)))) 
                               | (QData)((IData)((0x7fU 
                                                  & ((vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[2U] 
                                                      << 1U) 
                                                     | (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[1U] 
                                                        >> 0x1fU))))))) 
                      >> 0x10U) | ((IData)((((0xffffffffffffff80ULL 
                                              & (((QData)((IData)(
                                                                  vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[1U])) 
                                                  << 0x21U) 
                                                 | (0xffffffffffffff80ULL 
                                                    & ((QData)((IData)(
                                                                       vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[0U])) 
                                                       << 1U)))) 
                                             | (QData)((IData)(
                                                               (0x7fU 
                                                                & ((vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[2U] 
                                                                    << 1U) 
                                                                   | (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[1U] 
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
                                                        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[1U])) 
                                        << 0x21U) | 
                                       (0xffffffffffffff80ULL 
                                        & ((QData)((IData)(
                                                           vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[0U])) 
                                           << 1U)))) 
                                   | (QData)((IData)(
                                                     (0x7fU 
                                                      & ((vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[2U] 
                                                          << 1U) 
                                                         | (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[1U] 
                                                            >> 0x1fU))))))) 
                          << 0x17U) | ((IData)((amber48_core_smoke_tb__DOT__u_core__DOT__rf_rs2 
                                                >> 0x20U)) 
                                       << 7U)));
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[2U] 
        = (((0x7fU & ((IData)(((0xffffffffffffff80ULL 
                                & (((QData)((IData)(
                                                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[1U])) 
                                    << 0x21U) | (0xffffffffffffff80ULL 
                                                 & ((QData)((IData)(
                                                                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[0U])) 
                                                    << 1U)))) 
                               | (QData)((IData)((0x7fU 
                                                  & ((vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[2U] 
                                                      << 1U) 
                                                     | (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[1U] 
                                                        >> 0x1fU))))))) 
                      >> 9U)) | ((IData)((amber48_core_smoke_tb__DOT__u_core__DOT__rf_rs2 
                                          >> 0x20U)) 
                                 >> 0x19U)) | (__Vtemp_5[2U] 
                                               << 7U));
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[3U] 
        = ((0xff800000U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[3U]) 
           | ((__Vtemp_5[2U] >> 0x19U) | (0x7fff80U 
                                          & ((IData)(
                                                     (((0xffffffffffffff80ULL 
                                                        & (((QData)((IData)(
                                                                            vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[1U])) 
                                                            << 0x21U) 
                                                           | (0xffffffffffffff80ULL 
                                                              & ((QData)((IData)(
                                                                                vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[0U])) 
                                                                 << 1U)))) 
                                                       | (QData)((IData)(
                                                                         (0x7fU 
                                                                          & ((vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[2U] 
                                                                              << 1U) 
                                                                             | (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[1U] 
                                                                                >> 0x1fU)))))) 
                                                      >> 0x20U)) 
                                             >> 9U))));
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[0U] 
        = ((0xffffffbfU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[0U]) 
           | (0xffffffc0U & ((((~ (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[0U] 
                                   >> 3U)) & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[3U] 
                                              >> 0x1eU)) 
                              << 6U) & ((((IData)((0U 
                                                   == 
                                                   (0x3d0U 
                                                    & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[0U]))) 
                                          & (0U != 
                                             (0xfU 
                                              & ((vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[2U] 
                                                  << 1U) 
                                                 | (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[1U] 
                                                    >> 0x1fU))))) 
                                         << 6U) | (0xffffffc0U 
                                                   & (vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[2U] 
                                                      << 2U))))));
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[0U] 
        = ((0xffffffc0U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[0U]) 
           | (0x3fU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[0U]));
    if ((0x400U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[0U])) {
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[3U] 
            = ((0x7fffffU & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[3U]) 
               | ((IData)((0xffffffffffffULL & (((QData)((IData)(
                                                                 vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[1U])) 
                                                 << 0x15U) 
                                                | ((QData)((IData)(
                                                                   vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[0U])) 
                                                   >> 0xbU)))) 
                  << 0x17U));
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[4U] 
            = (((IData)((0xffffffffffffULL & (((QData)((IData)(
                                                               vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[1U])) 
                                               << 0x15U) 
                                              | ((QData)((IData)(
                                                                 vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[0U])) 
                                                 >> 0xbU)))) 
                >> 9U) | ((IData)(((0xffffffffffffULL 
                                    & (((QData)((IData)(
                                                        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[1U])) 
                                        << 0x15U) | 
                                       ((QData)((IData)(
                                                        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[0U])) 
                                        >> 0xbU))) 
                                   >> 0x20U)) << 0x17U));
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[5U] 
            = ((0xffffff80U & vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next[5U]) 
               | ((IData)(((0xffffffffffffULL & (((QData)((IData)(
                                                                  vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[1U])) 
                                                  << 0x15U) 
                                                 | ((QData)((IData)(
                                                                    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[0U])) 
                                                    >> 0xbU))) 
                           >> 0x20U)) >> 9U));
    }
    if (amber48_core_smoke_tb__DOT__u_core__DOT__pipeline_stall) {
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__pc_next 
            = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__pc_q;
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[0U] 
            = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[0U];
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[1U] 
            = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[1U];
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[2U] 
            = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[2U];
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next[3U] 
            = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q[3U];
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[0U] 
            = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[0U];
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[1U] 
            = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[1U];
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[2U] 
            = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[2U];
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next[3U] 
            = vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q[3U];
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

VL_ATTR_COLD void Vamber48_core_smoke_tb___024root___eval_stl(Vamber48_core_smoke_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vamber48_core_smoke_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vamber48_core_smoke_tb___024root___eval_stl\n"); );
    // Body
    if ((1ULL & vlSelf->__VstlTriggered.word(0U))) {
        Vamber48_core_smoke_tb___024root___stl_sequent__TOP__0(vlSelf);
    }
}

VL_ATTR_COLD void Vamber48_core_smoke_tb___024root___eval_triggers__stl(Vamber48_core_smoke_tb___024root* vlSelf);

VL_ATTR_COLD bool Vamber48_core_smoke_tb___024root___eval_phase__stl(Vamber48_core_smoke_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vamber48_core_smoke_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vamber48_core_smoke_tb___024root___eval_phase__stl\n"); );
    // Init
    CData/*0:0*/ __VstlExecute;
    // Body
    Vamber48_core_smoke_tb___024root___eval_triggers__stl(vlSelf);
    __VstlExecute = vlSelf->__VstlTriggered.any();
    if (__VstlExecute) {
        Vamber48_core_smoke_tb___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vamber48_core_smoke_tb___024root___dump_triggers__act(Vamber48_core_smoke_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vamber48_core_smoke_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vamber48_core_smoke_tb___024root___dump_triggers__act\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VactTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelf->__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 0 is active: @(posedge amber48_core_smoke_tb.clk)\n");
    }
    if ((2ULL & vlSelf->__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 1 is active: @(posedge amber48_core_smoke_tb.clk or negedge amber48_core_smoke_tb.rst_ni)\n");
    }
    if ((4ULL & vlSelf->__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 2 is active: @([changed] amber48_core_smoke_tb.rst_ni)\n");
    }
    if ((8ULL & vlSelf->__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 3 is active: @([true] __VdlySched.awaitingCurrentTime())\n");
    }
}
#endif  // VL_DEBUG

#ifdef VL_DEBUG
VL_ATTR_COLD void Vamber48_core_smoke_tb___024root___dump_triggers__nba(Vamber48_core_smoke_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vamber48_core_smoke_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vamber48_core_smoke_tb___024root___dump_triggers__nba\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VnbaTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelf->__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 0 is active: @(posedge amber48_core_smoke_tb.clk)\n");
    }
    if ((2ULL & vlSelf->__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 1 is active: @(posedge amber48_core_smoke_tb.clk or negedge amber48_core_smoke_tb.rst_ni)\n");
    }
    if ((4ULL & vlSelf->__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 2 is active: @([changed] amber48_core_smoke_tb.rst_ni)\n");
    }
    if ((8ULL & vlSelf->__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 3 is active: @([true] __VdlySched.awaitingCurrentTime())\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vamber48_core_smoke_tb___024root___ctor_var_reset(Vamber48_core_smoke_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vamber48_core_smoke_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vamber48_core_smoke_tb___024root___ctor_var_reset\n"); );
    // Body
    vlSelf->amber48_core_smoke_tb__DOT__clk = VL_RAND_RESET_I(1);
    vlSelf->amber48_core_smoke_tb__DOT__rst_ni = VL_RAND_RESET_I(1);
    vlSelf->amber48_core_smoke_tb__DOT__clk_en = VL_RAND_RESET_I(1);
    vlSelf->amber48_core_smoke_tb__DOT__imem_valid = VL_RAND_RESET_I(1);
    vlSelf->amber48_core_smoke_tb__DOT__dmem_req = VL_RAND_RESET_I(1);
    vlSelf->amber48_core_smoke_tb__DOT__dmem_rdata = VL_RAND_RESET_Q(48);
    vlSelf->amber48_core_smoke_tb__DOT__dmem_ready = VL_RAND_RESET_I(1);
    vlSelf->amber48_core_smoke_tb__DOT__dmem_trap = VL_RAND_RESET_I(1);
    vlSelf->amber48_core_smoke_tb__DOT__uart_tx_valid = VL_RAND_RESET_I(1);
    vlSelf->amber48_core_smoke_tb__DOT__uart_tx_data = VL_RAND_RESET_I(8);
    vlSelf->amber48_core_smoke_tb__DOT__trap_seen = VL_RAND_RESET_I(1);
    vlSelf->amber48_core_smoke_tb__DOT__cycle_count = VL_RAND_RESET_I(32);
    vlSelf->amber48_core_smoke_tb__DOT__uart_index = VL_RAND_RESET_I(32);
    vlSelf->amber48_core_smoke_tb__DOT__uart_count = VL_RAND_RESET_I(32);
    vlSelf->amber48_core_smoke_tb__DOT__actual_trap = VL_RAND_RESET_I(3);
    vlSelf->amber48_core_smoke_tb__DOT__expected_trap_e = VL_RAND_RESET_I(3);
    vlSelf->amber48_core_smoke_tb__DOT__store0_seen = VL_RAND_RESET_I(1);
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__pc_q = VL_RAND_RESET_Q(48);
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__pc_next = VL_RAND_RESET_Q(48);
    VL_RAND_RESET_W(97, vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_q);
    VL_RAND_RESET_W(97, vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__if_stage_next);
    VL_RAND_RESET_W(127, vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_q);
    VL_RAND_RESET_W(127, vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__id_stage_next);
    VL_RAND_RESET_W(264, vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_q);
    VL_RAND_RESET_W(264, vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_next);
    VL_RAND_RESET_W(157, vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__ex_stage_result);
    vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__rf_req = VL_RAND_RESET_Q(62);
    VL_RAND_RESET_W(127, vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_decoder__DOT__decode_r);
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_regfile__DOT__regs[__Vi0] = VL_RAND_RESET_Q(48);
    }
    VL_RAND_RESET_W(157, vlSelf->amber48_core_smoke_tb__DOT__u_core__DOT__u_alu__DOT__ex_r);
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        vlSelf->amber48_core_smoke_tb__DOT__u_imem__DOT__rom[__Vi0] = VL_RAND_RESET_Q(48);
    }
    vlSelf->amber48_core_smoke_tb__DOT__u_imem__DOT__data_q = VL_RAND_RESET_Q(48);
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        vlSelf->amber48_core_smoke_tb__DOT__u_dmem__DOT__ram[__Vi0] = VL_RAND_RESET_Q(48);
    }
    vlSelf->amber48_core_smoke_tb__DOT__u_dmem__DOT__led_q = VL_RAND_RESET_I(8);
    vlSelf->amber48_core_smoke_tb__DOT__u_dmem__DOT__uart_q = VL_RAND_RESET_I(8);
    vlSelf->amber48_core_smoke_tb__DOT__u_dmem__DOT__unnamedblk1__DOT__mem_rdata = VL_RAND_RESET_Q(48);
    vlSelf->amber48_core_smoke_tb__DOT__u_uart_tx__DOT__state_q = VL_RAND_RESET_I(2);
    vlSelf->amber48_core_smoke_tb__DOT__u_uart_tx__DOT__clk_cnt_q = VL_RAND_RESET_I(7);
    vlSelf->amber48_core_smoke_tb__DOT__u_uart_tx__DOT__bit_idx_q = VL_RAND_RESET_I(3);
    vlSelf->__Vdlyvset__amber48_core_smoke_tb__DOT__trap_seen__v0 = 0;
    vlSelf->__Vdlyvset__amber48_core_smoke_tb__DOT__trap_seen__v1 = 0;
    vlSelf->__Vdly__amber48_core_smoke_tb__DOT__u_uart_tx__DOT__state_q = VL_RAND_RESET_I(2);
    vlSelf->__Vtrigprevexpr___TOP__amber48_core_smoke_tb__DOT__clk__0 = VL_RAND_RESET_I(1);
    vlSelf->__Vtrigprevexpr___TOP__amber48_core_smoke_tb__DOT__rst_ni__0 = VL_RAND_RESET_I(1);
    vlSelf->__VactDidInit = 0;
}
