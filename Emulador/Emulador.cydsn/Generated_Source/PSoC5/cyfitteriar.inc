;
; File Name: cyfitteriar.inc
; 
; PSoC Creator  4.2
;
; Description:
; 
;
;-------------------------------------------------------------------------------
; Copyright (c) 2007-2018 Cypress Semiconductor.  All rights reserved.
; You may use this file only in accordance with the license, terms, conditions, 
; disclaimers, and limitations in the end user license agreement accompanying 
; the software package with which this file was provided.
;-------------------------------------------------------------------------------

#ifndef INCLUDED_CYFITTERIAR_INC
#define INCLUDED_CYFITTERIAR_INC
    INCLUDE cydeviceiar.inc
    INCLUDE cydeviceiar_trm.inc

/* GBCL */
GBCL_BUART_sRX_RxBitCounter__16BIT_CONTROL_AUX_CTL_REG EQU CYREG_B0_UDB10_11_ACTL
GBCL_BUART_sRX_RxBitCounter__16BIT_CONTROL_CONTROL_REG EQU CYREG_B0_UDB10_11_CTL
GBCL_BUART_sRX_RxBitCounter__16BIT_CONTROL_COUNT_REG EQU CYREG_B0_UDB10_11_CTL
GBCL_BUART_sRX_RxBitCounter__16BIT_COUNT_CONTROL_REG EQU CYREG_B0_UDB10_11_CTL
GBCL_BUART_sRX_RxBitCounter__16BIT_COUNT_COUNT_REG EQU CYREG_B0_UDB10_11_CTL
GBCL_BUART_sRX_RxBitCounter__16BIT_MASK_MASK_REG EQU CYREG_B0_UDB10_11_MSK
GBCL_BUART_sRX_RxBitCounter__16BIT_MASK_PERIOD_REG EQU CYREG_B0_UDB10_11_MSK
GBCL_BUART_sRX_RxBitCounter__16BIT_PERIOD_MASK_REG EQU CYREG_B0_UDB10_11_MSK
GBCL_BUART_sRX_RxBitCounter__16BIT_PERIOD_PERIOD_REG EQU CYREG_B0_UDB10_11_MSK
GBCL_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG EQU CYREG_B0_UDB10_ACTL
GBCL_BUART_sRX_RxBitCounter__CONTROL_REG EQU CYREG_B0_UDB10_CTL
GBCL_BUART_sRX_RxBitCounter__CONTROL_ST_REG EQU CYREG_B0_UDB10_ST_CTL
GBCL_BUART_sRX_RxBitCounter__COUNT_REG EQU CYREG_B0_UDB10_CTL
GBCL_BUART_sRX_RxBitCounter__COUNT_ST_REG EQU CYREG_B0_UDB10_ST_CTL
GBCL_BUART_sRX_RxBitCounter__MASK_CTL_AUX_CTL_REG EQU CYREG_B0_UDB10_MSK_ACTL
GBCL_BUART_sRX_RxBitCounter__PER_CTL_AUX_CTL_REG EQU CYREG_B0_UDB10_MSK_ACTL
GBCL_BUART_sRX_RxBitCounter__PERIOD_REG EQU CYREG_B0_UDB10_MSK
GBCL_BUART_sRX_RxBitCounter_ST__16BIT_STATUS_AUX_CTL_REG EQU CYREG_B0_UDB10_11_ACTL
GBCL_BUART_sRX_RxBitCounter_ST__16BIT_STATUS_REG EQU CYREG_B0_UDB10_11_ST
GBCL_BUART_sRX_RxBitCounter_ST__MASK_REG EQU CYREG_B0_UDB10_MSK
GBCL_BUART_sRX_RxBitCounter_ST__MASK_ST_AUX_CTL_REG EQU CYREG_B0_UDB10_MSK_ACTL
GBCL_BUART_sRX_RxBitCounter_ST__PER_ST_AUX_CTL_REG EQU CYREG_B0_UDB10_MSK_ACTL
GBCL_BUART_sRX_RxBitCounter_ST__STATUS_AUX_CTL_REG EQU CYREG_B0_UDB10_ACTL
GBCL_BUART_sRX_RxBitCounter_ST__STATUS_CNT_REG EQU CYREG_B0_UDB10_ST_CTL
GBCL_BUART_sRX_RxBitCounter_ST__STATUS_CONTROL_REG EQU CYREG_B0_UDB10_ST_CTL
GBCL_BUART_sRX_RxBitCounter_ST__STATUS_REG EQU CYREG_B0_UDB10_ST
GBCL_BUART_sRX_RxShifter_u0__16BIT_A0_REG EQU CYREG_B0_UDB13_14_A0
GBCL_BUART_sRX_RxShifter_u0__16BIT_A1_REG EQU CYREG_B0_UDB13_14_A1
GBCL_BUART_sRX_RxShifter_u0__16BIT_D0_REG EQU CYREG_B0_UDB13_14_D0
GBCL_BUART_sRX_RxShifter_u0__16BIT_D1_REG EQU CYREG_B0_UDB13_14_D1
GBCL_BUART_sRX_RxShifter_u0__16BIT_DP_AUX_CTL_REG EQU CYREG_B0_UDB13_14_ACTL
GBCL_BUART_sRX_RxShifter_u0__16BIT_F0_REG EQU CYREG_B0_UDB13_14_F0
GBCL_BUART_sRX_RxShifter_u0__16BIT_F1_REG EQU CYREG_B0_UDB13_14_F1
GBCL_BUART_sRX_RxShifter_u0__A0_A1_REG EQU CYREG_B0_UDB13_A0_A1
GBCL_BUART_sRX_RxShifter_u0__A0_REG EQU CYREG_B0_UDB13_A0
GBCL_BUART_sRX_RxShifter_u0__A1_REG EQU CYREG_B0_UDB13_A1
GBCL_BUART_sRX_RxShifter_u0__D0_D1_REG EQU CYREG_B0_UDB13_D0_D1
GBCL_BUART_sRX_RxShifter_u0__D0_REG EQU CYREG_B0_UDB13_D0
GBCL_BUART_sRX_RxShifter_u0__D1_REG EQU CYREG_B0_UDB13_D1
GBCL_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG EQU CYREG_B0_UDB13_ACTL
GBCL_BUART_sRX_RxShifter_u0__F0_F1_REG EQU CYREG_B0_UDB13_F0_F1
GBCL_BUART_sRX_RxShifter_u0__F0_REG EQU CYREG_B0_UDB13_F0
GBCL_BUART_sRX_RxShifter_u0__F1_REG EQU CYREG_B0_UDB13_F1
GBCL_BUART_sRX_RxSts__2__MASK EQU 0x04
GBCL_BUART_sRX_RxSts__2__POS EQU 2
GBCL_BUART_sRX_RxSts__3__MASK EQU 0x08
GBCL_BUART_sRX_RxSts__3__POS EQU 3
GBCL_BUART_sRX_RxSts__4__MASK EQU 0x10
GBCL_BUART_sRX_RxSts__4__POS EQU 4
GBCL_BUART_sRX_RxSts__5__MASK EQU 0x20
GBCL_BUART_sRX_RxSts__5__POS EQU 5
GBCL_BUART_sRX_RxSts__MASK EQU 0x3C
GBCL_BUART_sRX_RxSts__MASK_REG EQU CYREG_B0_UDB15_MSK
GBCL_BUART_sRX_RxSts__STATUS_AUX_CTL_REG EQU CYREG_B0_UDB15_ACTL
GBCL_BUART_sRX_RxSts__STATUS_REG EQU CYREG_B0_UDB15_ST
GBCL_BUART_sTX_sCLOCK_TxBitClkGen__A0_A1_REG EQU CYREG_B0_UDB15_A0_A1
GBCL_BUART_sTX_sCLOCK_TxBitClkGen__A0_REG EQU CYREG_B0_UDB15_A0
GBCL_BUART_sTX_sCLOCK_TxBitClkGen__A1_REG EQU CYREG_B0_UDB15_A1
GBCL_BUART_sTX_sCLOCK_TxBitClkGen__D0_D1_REG EQU CYREG_B0_UDB15_D0_D1
GBCL_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG EQU CYREG_B0_UDB15_D0
GBCL_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG EQU CYREG_B0_UDB15_D1
GBCL_BUART_sTX_sCLOCK_TxBitClkGen__DP_AUX_CTL_REG EQU CYREG_B0_UDB15_ACTL
GBCL_BUART_sTX_sCLOCK_TxBitClkGen__F0_F1_REG EQU CYREG_B0_UDB15_F0_F1
GBCL_BUART_sTX_sCLOCK_TxBitClkGen__F0_REG EQU CYREG_B0_UDB15_F0
GBCL_BUART_sTX_sCLOCK_TxBitClkGen__F1_REG EQU CYREG_B0_UDB15_F1
GBCL_BUART_sTX_TxShifter_u0__16BIT_A0_REG EQU CYREG_B0_UDB12_13_A0
GBCL_BUART_sTX_TxShifter_u0__16BIT_A1_REG EQU CYREG_B0_UDB12_13_A1
GBCL_BUART_sTX_TxShifter_u0__16BIT_D0_REG EQU CYREG_B0_UDB12_13_D0
GBCL_BUART_sTX_TxShifter_u0__16BIT_D1_REG EQU CYREG_B0_UDB12_13_D1
GBCL_BUART_sTX_TxShifter_u0__16BIT_DP_AUX_CTL_REG EQU CYREG_B0_UDB12_13_ACTL
GBCL_BUART_sTX_TxShifter_u0__16BIT_F0_REG EQU CYREG_B0_UDB12_13_F0
GBCL_BUART_sTX_TxShifter_u0__16BIT_F1_REG EQU CYREG_B0_UDB12_13_F1
GBCL_BUART_sTX_TxShifter_u0__A0_A1_REG EQU CYREG_B0_UDB12_A0_A1
GBCL_BUART_sTX_TxShifter_u0__A0_REG EQU CYREG_B0_UDB12_A0
GBCL_BUART_sTX_TxShifter_u0__A1_REG EQU CYREG_B0_UDB12_A1
GBCL_BUART_sTX_TxShifter_u0__D0_D1_REG EQU CYREG_B0_UDB12_D0_D1
GBCL_BUART_sTX_TxShifter_u0__D0_REG EQU CYREG_B0_UDB12_D0
GBCL_BUART_sTX_TxShifter_u0__D1_REG EQU CYREG_B0_UDB12_D1
GBCL_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG EQU CYREG_B0_UDB12_ACTL
GBCL_BUART_sTX_TxShifter_u0__F0_F1_REG EQU CYREG_B0_UDB12_F0_F1
GBCL_BUART_sTX_TxShifter_u0__F0_REG EQU CYREG_B0_UDB12_F0
GBCL_BUART_sTX_TxShifter_u0__F1_REG EQU CYREG_B0_UDB12_F1
GBCL_BUART_sTX_TxSts__0__MASK EQU 0x01
GBCL_BUART_sTX_TxSts__0__POS EQU 0
GBCL_BUART_sTX_TxSts__1__MASK EQU 0x02
GBCL_BUART_sTX_TxSts__1__POS EQU 1
GBCL_BUART_sTX_TxSts__16BIT_STATUS_AUX_CTL_REG EQU CYREG_B0_UDB12_13_ACTL
GBCL_BUART_sTX_TxSts__16BIT_STATUS_REG EQU CYREG_B0_UDB12_13_ST
GBCL_BUART_sTX_TxSts__2__MASK EQU 0x04
GBCL_BUART_sTX_TxSts__2__POS EQU 2
GBCL_BUART_sTX_TxSts__3__MASK EQU 0x08
GBCL_BUART_sTX_TxSts__3__POS EQU 3
GBCL_BUART_sTX_TxSts__MASK EQU 0x0F
GBCL_BUART_sTX_TxSts__MASK_REG EQU CYREG_B0_UDB12_MSK
GBCL_BUART_sTX_TxSts__STATUS_AUX_CTL_REG EQU CYREG_B0_UDB12_ACTL
GBCL_BUART_sTX_TxSts__STATUS_REG EQU CYREG_B0_UDB12_ST
GBCL_RXInternalInterrupt__INTC_CLR_EN_REG EQU CYREG_NVIC_CLRENA0
GBCL_RXInternalInterrupt__INTC_CLR_PD_REG EQU CYREG_NVIC_CLRPEND0
GBCL_RXInternalInterrupt__INTC_MASK EQU 0x01
GBCL_RXInternalInterrupt__INTC_NUMBER EQU 0
GBCL_RXInternalInterrupt__INTC_PRIOR_NUM EQU 7
GBCL_RXInternalInterrupt__INTC_PRIOR_REG EQU CYREG_NVIC_PRI_0
GBCL_RXInternalInterrupt__INTC_SET_EN_REG EQU CYREG_NVIC_SETENA0
GBCL_RXInternalInterrupt__INTC_SET_PD_REG EQU CYREG_NVIC_SETPEND0

/* Rx_2 */
Rx_2__0__INTTYPE EQU CYREG_PICU15_INTTYPE4
Rx_2__0__MASK EQU 0x10
Rx_2__0__PC EQU CYREG_IO_PC_PRT15_PC4
Rx_2__0__PORT EQU 15
Rx_2__0__SHIFT EQU 4
Rx_2__AG EQU CYREG_PRT15_AG
Rx_2__AMUX EQU CYREG_PRT15_AMUX
Rx_2__BIE EQU CYREG_PRT15_BIE
Rx_2__BIT_MASK EQU CYREG_PRT15_BIT_MASK
Rx_2__BYP EQU CYREG_PRT15_BYP
Rx_2__CTL EQU CYREG_PRT15_CTL
Rx_2__DM0 EQU CYREG_PRT15_DM0
Rx_2__DM1 EQU CYREG_PRT15_DM1
Rx_2__DM2 EQU CYREG_PRT15_DM2
Rx_2__DR EQU CYREG_PRT15_DR
Rx_2__INP_DIS EQU CYREG_PRT15_INP_DIS
Rx_2__INTTYPE_BASE EQU CYDEV_PICU_INTTYPE_PICU15_BASE
Rx_2__LCD_COM_SEG EQU CYREG_PRT15_LCD_COM_SEG
Rx_2__LCD_EN EQU CYREG_PRT15_LCD_EN
Rx_2__MASK EQU 0x10
Rx_2__PORT EQU 15
Rx_2__PRT EQU CYREG_PRT15_PRT
Rx_2__PRTDSI__CAPS_SEL EQU CYREG_PRT15_CAPS_SEL
Rx_2__PRTDSI__DBL_SYNC_IN EQU CYREG_PRT15_DBL_SYNC_IN
Rx_2__PRTDSI__OE_SEL0 EQU CYREG_PRT15_OE_SEL0
Rx_2__PRTDSI__OE_SEL1 EQU CYREG_PRT15_OE_SEL1
Rx_2__PRTDSI__OUT_SEL0 EQU CYREG_PRT15_OUT_SEL0
Rx_2__PRTDSI__OUT_SEL1 EQU CYREG_PRT15_OUT_SEL1
Rx_2__PRTDSI__SYNC_OUT EQU CYREG_PRT15_SYNC_OUT
Rx_2__PS EQU CYREG_PRT15_PS
Rx_2__SHIFT EQU 4
Rx_2__SLW EQU CYREG_PRT15_SLW

/* Tx_2 */
Tx_2__0__INTTYPE EQU CYREG_PICU15_INTTYPE5
Tx_2__0__MASK EQU 0x20
Tx_2__0__PC EQU CYREG_IO_PC_PRT15_PC5
Tx_2__0__PORT EQU 15
Tx_2__0__SHIFT EQU 5
Tx_2__AG EQU CYREG_PRT15_AG
Tx_2__AMUX EQU CYREG_PRT15_AMUX
Tx_2__BIE EQU CYREG_PRT15_BIE
Tx_2__BIT_MASK EQU CYREG_PRT15_BIT_MASK
Tx_2__BYP EQU CYREG_PRT15_BYP
Tx_2__CTL EQU CYREG_PRT15_CTL
Tx_2__DM0 EQU CYREG_PRT15_DM0
Tx_2__DM1 EQU CYREG_PRT15_DM1
Tx_2__DM2 EQU CYREG_PRT15_DM2
Tx_2__DR EQU CYREG_PRT15_DR
Tx_2__INP_DIS EQU CYREG_PRT15_INP_DIS
Tx_2__INTTYPE_BASE EQU CYDEV_PICU_INTTYPE_PICU15_BASE
Tx_2__LCD_COM_SEG EQU CYREG_PRT15_LCD_COM_SEG
Tx_2__LCD_EN EQU CYREG_PRT15_LCD_EN
Tx_2__MASK EQU 0x20
Tx_2__PORT EQU 15
Tx_2__PRT EQU CYREG_PRT15_PRT
Tx_2__PRTDSI__CAPS_SEL EQU CYREG_PRT15_CAPS_SEL
Tx_2__PRTDSI__DBL_SYNC_IN EQU CYREG_PRT15_DBL_SYNC_IN
Tx_2__PRTDSI__OE_SEL0 EQU CYREG_PRT15_OE_SEL0
Tx_2__PRTDSI__OE_SEL1 EQU CYREG_PRT15_OE_SEL1
Tx_2__PRTDSI__OUT_SEL0 EQU CYREG_PRT15_OUT_SEL0
Tx_2__PRTDSI__OUT_SEL1 EQU CYREG_PRT15_OUT_SEL1
Tx_2__PRTDSI__SYNC_OUT EQU CYREG_PRT15_SYNC_OUT
Tx_2__PS EQU CYREG_PRT15_PS
Tx_2__SHIFT EQU 5
Tx_2__SLW EQU CYREG_PRT15_SLW

/* Clock_2 */
Clock_2__CFG0 EQU CYREG_CLKDIST_DCFG0_CFG0
Clock_2__CFG1 EQU CYREG_CLKDIST_DCFG0_CFG1
Clock_2__CFG2 EQU CYREG_CLKDIST_DCFG0_CFG2
Clock_2__CFG2_SRC_SEL_MASK EQU 0x07
Clock_2__INDEX EQU 0x00
Clock_2__PM_ACT_CFG EQU CYREG_PM_ACT_CFG2
Clock_2__PM_ACT_MSK EQU 0x01
Clock_2__PM_STBY_CFG EQU CYREG_PM_STBY_CFG2
Clock_2__PM_STBY_MSK EQU 0x01

/* Miscellaneous */
BCLK__BUS_CLK__HZ EQU 24000000
BCLK__BUS_CLK__KHZ EQU 24000
BCLK__BUS_CLK__MHZ EQU 24
CYDEV_CHIP_DIE_LEOPARD EQU 1
CYDEV_CHIP_DIE_PSOC4A EQU 18
CYDEV_CHIP_DIE_PSOC5LP EQU 2
CYDEV_CHIP_DIE_PSOC5TM EQU 3
CYDEV_CHIP_DIE_TMA4 EQU 4
CYDEV_CHIP_DIE_UNKNOWN EQU 0
CYDEV_CHIP_FAMILY_FM0P EQU 5
CYDEV_CHIP_FAMILY_FM3 EQU 6
CYDEV_CHIP_FAMILY_FM4 EQU 7
CYDEV_CHIP_FAMILY_PSOC3 EQU 1
CYDEV_CHIP_FAMILY_PSOC4 EQU 2
CYDEV_CHIP_FAMILY_PSOC5 EQU 3
CYDEV_CHIP_FAMILY_PSOC6 EQU 4
CYDEV_CHIP_FAMILY_UNKNOWN EQU 0
CYDEV_CHIP_FAMILY_USED EQU CYDEV_CHIP_FAMILY_PSOC5
CYDEV_CHIP_JTAG_ID EQU 0x2E161069
CYDEV_CHIP_MEMBER_3A EQU 1
CYDEV_CHIP_MEMBER_4A EQU 18
CYDEV_CHIP_MEMBER_4D EQU 13
CYDEV_CHIP_MEMBER_4E EQU 6
CYDEV_CHIP_MEMBER_4F EQU 19
CYDEV_CHIP_MEMBER_4G EQU 4
CYDEV_CHIP_MEMBER_4H EQU 17
CYDEV_CHIP_MEMBER_4I EQU 23
CYDEV_CHIP_MEMBER_4J EQU 14
CYDEV_CHIP_MEMBER_4K EQU 15
CYDEV_CHIP_MEMBER_4L EQU 22
CYDEV_CHIP_MEMBER_4M EQU 21
CYDEV_CHIP_MEMBER_4N EQU 10
CYDEV_CHIP_MEMBER_4O EQU 7
CYDEV_CHIP_MEMBER_4P EQU 20
CYDEV_CHIP_MEMBER_4Q EQU 12
CYDEV_CHIP_MEMBER_4R EQU 8
CYDEV_CHIP_MEMBER_4S EQU 11
CYDEV_CHIP_MEMBER_4T EQU 9
CYDEV_CHIP_MEMBER_4U EQU 5
CYDEV_CHIP_MEMBER_4V EQU 16
CYDEV_CHIP_MEMBER_5A EQU 3
CYDEV_CHIP_MEMBER_5B EQU 2
CYDEV_CHIP_MEMBER_6A EQU 24
CYDEV_CHIP_MEMBER_FM3 EQU 28
CYDEV_CHIP_MEMBER_FM4 EQU 29
CYDEV_CHIP_MEMBER_PDL_FM0P_TYPE1 EQU 25
CYDEV_CHIP_MEMBER_PDL_FM0P_TYPE2 EQU 26
CYDEV_CHIP_MEMBER_PDL_FM0P_TYPE3 EQU 27
CYDEV_CHIP_MEMBER_UNKNOWN EQU 0
CYDEV_CHIP_MEMBER_USED EQU CYDEV_CHIP_MEMBER_5B
CYDEV_CHIP_DIE_EXPECT EQU CYDEV_CHIP_MEMBER_USED
CYDEV_CHIP_DIE_ACTUAL EQU CYDEV_CHIP_DIE_EXPECT
CYDEV_CHIP_REV_LEOPARD_ES1 EQU 0
CYDEV_CHIP_REV_LEOPARD_ES2 EQU 1
CYDEV_CHIP_REV_LEOPARD_ES3 EQU 3
CYDEV_CHIP_REV_LEOPARD_PRODUCTION EQU 3
CYDEV_CHIP_REV_PSOC4A_ES0 EQU 17
CYDEV_CHIP_REV_PSOC4A_PRODUCTION EQU 17
CYDEV_CHIP_REV_PSOC5LP_ES0 EQU 0
CYDEV_CHIP_REV_PSOC5LP_PRODUCTION EQU 0
CYDEV_CHIP_REV_PSOC5TM_ES0 EQU 0
CYDEV_CHIP_REV_PSOC5TM_ES1 EQU 1
CYDEV_CHIP_REV_PSOC5TM_PRODUCTION EQU 1
CYDEV_CHIP_REV_TMA4_ES EQU 17
CYDEV_CHIP_REV_TMA4_ES2 EQU 33
CYDEV_CHIP_REV_TMA4_PRODUCTION EQU 17
CYDEV_CHIP_REVISION_3A_ES1 EQU 0
CYDEV_CHIP_REVISION_3A_ES2 EQU 1
CYDEV_CHIP_REVISION_3A_ES3 EQU 3
CYDEV_CHIP_REVISION_3A_PRODUCTION EQU 3
CYDEV_CHIP_REVISION_4A_ES0 EQU 17
CYDEV_CHIP_REVISION_4A_PRODUCTION EQU 17
CYDEV_CHIP_REVISION_4D_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4E_CCG2_NO_USBPD EQU 0
CYDEV_CHIP_REVISION_4E_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4F_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4F_PRODUCTION_256DMA EQU 0
CYDEV_CHIP_REVISION_4F_PRODUCTION_256K EQU 0
CYDEV_CHIP_REVISION_4G_ES EQU 17
CYDEV_CHIP_REVISION_4G_ES2 EQU 33
CYDEV_CHIP_REVISION_4G_PRODUCTION EQU 17
CYDEV_CHIP_REVISION_4H_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4I_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4J_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4K_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4L_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4M_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4N_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4O_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4P_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4Q_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4R_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4S_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4T_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4U_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4V_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_5A_ES0 EQU 0
CYDEV_CHIP_REVISION_5A_ES1 EQU 1
CYDEV_CHIP_REVISION_5A_PRODUCTION EQU 1
CYDEV_CHIP_REVISION_5B_ES0 EQU 0
CYDEV_CHIP_REVISION_5B_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_6A_ES EQU 17
CYDEV_CHIP_REVISION_6A_NO_UDB EQU 33
CYDEV_CHIP_REVISION_6A_PRODUCTION EQU 33
CYDEV_CHIP_REVISION_FM3_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_FM4_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_PDL_FM0P_TYPE1_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_PDL_FM0P_TYPE2_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_PDL_FM0P_TYPE3_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_USED EQU CYDEV_CHIP_REVISION_5B_PRODUCTION
CYDEV_CHIP_REV_EXPECT EQU CYDEV_CHIP_REVISION_USED
CYDEV_CONFIG_FASTBOOT_ENABLED EQU 1
CYDEV_CONFIG_UNUSED_IO_AllowButWarn EQU 0
CYDEV_CONFIG_UNUSED_IO EQU CYDEV_CONFIG_UNUSED_IO_AllowButWarn
CYDEV_CONFIG_UNUSED_IO_AllowWithInfo EQU 1
CYDEV_CONFIG_UNUSED_IO_Disallowed EQU 2
CYDEV_CONFIGURATION_COMPRESSED EQU 1
CYDEV_CONFIGURATION_DMA EQU 0
CYDEV_CONFIGURATION_ECC EQU 1
CYDEV_CONFIGURATION_IMOENABLED EQU CYDEV_CONFIG_FASTBOOT_ENABLED
CYDEV_CONFIGURATION_MODE_COMPRESSED EQU 0
CYDEV_CONFIGURATION_MODE EQU CYDEV_CONFIGURATION_MODE_COMPRESSED
CYDEV_CONFIGURATION_MODE_DMA EQU 2
CYDEV_CONFIGURATION_MODE_UNCOMPRESSED EQU 1
CYDEV_DEBUG_ENABLE_MASK EQU 0x20
CYDEV_DEBUG_ENABLE_REGISTER EQU CYREG_MLOGIC_DEBUG
CYDEV_DEBUGGING_DPS_Disable EQU 3
CYDEV_DEBUGGING_DPS_JTAG_4 EQU 1
CYDEV_DEBUGGING_DPS_JTAG_5 EQU 0
CYDEV_DEBUGGING_DPS_SWD EQU 2
CYDEV_DEBUGGING_DPS_SWD_SWV EQU 6
CYDEV_DEBUGGING_DPS EQU CYDEV_DEBUGGING_DPS_SWD_SWV
CYDEV_DEBUGGING_ENABLE EQU 1
CYDEV_DEBUGGING_XRES EQU 0
CYDEV_DMA_CHANNELS_AVAILABLE EQU 24
CYDEV_ECC_ENABLE EQU 0
CYDEV_HEAP_SIZE EQU 0x80
CYDEV_INSTRUCT_CACHE_ENABLED EQU 1
CYDEV_INTR_RISING EQU 0x00000001
CYDEV_IS_EXPORTING_CODE EQU 0
CYDEV_IS_IMPORTING_CODE EQU 0
CYDEV_PROJ_TYPE EQU 0
CYDEV_PROJ_TYPE_BOOTLOADER EQU 1
CYDEV_PROJ_TYPE_LAUNCHER EQU 5
CYDEV_PROJ_TYPE_LOADABLE EQU 2
CYDEV_PROJ_TYPE_LOADABLEANDBOOTLOADER EQU 4
CYDEV_PROJ_TYPE_MULTIAPPBOOTLOADER EQU 3
CYDEV_PROJ_TYPE_STANDARD EQU 0
CYDEV_PROTECTION_ENABLE EQU 0
CYDEV_STACK_SIZE EQU 0x0800
CYDEV_USE_BUNDLED_CMSIS EQU 1
CYDEV_VARIABLE_VDDA EQU 0
CYDEV_VDDA_MV EQU 5000
CYDEV_VDDD_MV EQU 5000
CYDEV_VDDIO0_MV EQU 5000
CYDEV_VDDIO1_MV EQU 5000
CYDEV_VDDIO2_MV EQU 5000
CYDEV_VDDIO3_MV EQU 5000
CYDEV_VIO0_MV EQU 5000
CYDEV_VIO1_MV EQU 5000
CYDEV_VIO2_MV EQU 5000
CYDEV_VIO3_MV EQU 5000
CYIPBLOCK_ARM_CM3_VERSION EQU 0
CYIPBLOCK_P3_ANAIF_VERSION EQU 0
CYIPBLOCK_P3_CAN_VERSION EQU 0
CYIPBLOCK_P3_CAPSENSE_VERSION EQU 0
CYIPBLOCK_P3_COMP_VERSION EQU 0
CYIPBLOCK_P3_DECIMATOR_VERSION EQU 0
CYIPBLOCK_P3_DFB_VERSION EQU 0
CYIPBLOCK_P3_DMA_VERSION EQU 0
CYIPBLOCK_P3_DRQ_VERSION EQU 0
CYIPBLOCK_P3_DSM_VERSION EQU 0
CYIPBLOCK_P3_EMIF_VERSION EQU 0
CYIPBLOCK_P3_I2C_VERSION EQU 0
CYIPBLOCK_P3_LCD_VERSION EQU 0
CYIPBLOCK_P3_LPF_VERSION EQU 0
CYIPBLOCK_P3_OPAMP_VERSION EQU 0
CYIPBLOCK_P3_PM_VERSION EQU 0
CYIPBLOCK_P3_SCCT_VERSION EQU 0
CYIPBLOCK_P3_TIMER_VERSION EQU 0
CYIPBLOCK_P3_USB_VERSION EQU 0
CYIPBLOCK_P3_VIDAC_VERSION EQU 0
CYIPBLOCK_P3_VREF_VERSION EQU 0
CYIPBLOCK_S8_GPIO_VERSION EQU 0
CYIPBLOCK_S8_IRQ_VERSION EQU 0
CYIPBLOCK_S8_SAR_VERSION EQU 0
CYIPBLOCK_S8_SIO_VERSION EQU 0
CYIPBLOCK_S8_UDB_VERSION EQU 0
DMA_CHANNELS_USED__MASK0 EQU 0x00000000
CYDEV_BOOTLOADER_ENABLE EQU 0

#endif /* INCLUDED_CYFITTERIAR_INC */