/*******************************************************************************
* File Name: cyfitter.h
* 
* PSoC Creator  4.2
*
* Description:
* 
* This file is automatically generated by PSoC Creator.
*
********************************************************************************
* Copyright (c) 2007-2018 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#ifndef INCLUDED_CYFITTER_H
#define INCLUDED_CYFITTER_H
#include "cydevice.h"
#include "cydevice_trm.h"

/* Rx_1 */
#define Rx_1__0__INTTYPE CYREG_PICU1_INTTYPE6
#define Rx_1__0__MASK 0x40u
#define Rx_1__0__PC CYREG_PRT1_PC6
#define Rx_1__0__PORT 1u
#define Rx_1__0__SHIFT 6u
#define Rx_1__AG CYREG_PRT1_AG
#define Rx_1__AMUX CYREG_PRT1_AMUX
#define Rx_1__BIE CYREG_PRT1_BIE
#define Rx_1__BIT_MASK CYREG_PRT1_BIT_MASK
#define Rx_1__BYP CYREG_PRT1_BYP
#define Rx_1__CTL CYREG_PRT1_CTL
#define Rx_1__DM0 CYREG_PRT1_DM0
#define Rx_1__DM1 CYREG_PRT1_DM1
#define Rx_1__DM2 CYREG_PRT1_DM2
#define Rx_1__DR CYREG_PRT1_DR
#define Rx_1__INP_DIS CYREG_PRT1_INP_DIS
#define Rx_1__INTTYPE_BASE CYDEV_PICU_INTTYPE_PICU1_BASE
#define Rx_1__LCD_COM_SEG CYREG_PRT1_LCD_COM_SEG
#define Rx_1__LCD_EN CYREG_PRT1_LCD_EN
#define Rx_1__MASK 0x40u
#define Rx_1__PORT 1u
#define Rx_1__PRT CYREG_PRT1_PRT
#define Rx_1__PRTDSI__CAPS_SEL CYREG_PRT1_CAPS_SEL
#define Rx_1__PRTDSI__DBL_SYNC_IN CYREG_PRT1_DBL_SYNC_IN
#define Rx_1__PRTDSI__OE_SEL0 CYREG_PRT1_OE_SEL0
#define Rx_1__PRTDSI__OE_SEL1 CYREG_PRT1_OE_SEL1
#define Rx_1__PRTDSI__OUT_SEL0 CYREG_PRT1_OUT_SEL0
#define Rx_1__PRTDSI__OUT_SEL1 CYREG_PRT1_OUT_SEL1
#define Rx_1__PRTDSI__SYNC_OUT CYREG_PRT1_SYNC_OUT
#define Rx_1__PS CYREG_PRT1_PS
#define Rx_1__SHIFT 6u
#define Rx_1__SLW CYREG_PRT1_SLW

/* Tx_1 */
#define Tx_1__0__INTTYPE CYREG_PICU1_INTTYPE7
#define Tx_1__0__MASK 0x80u
#define Tx_1__0__PC CYREG_PRT1_PC7
#define Tx_1__0__PORT 1u
#define Tx_1__0__SHIFT 7u
#define Tx_1__AG CYREG_PRT1_AG
#define Tx_1__AMUX CYREG_PRT1_AMUX
#define Tx_1__BIE CYREG_PRT1_BIE
#define Tx_1__BIT_MASK CYREG_PRT1_BIT_MASK
#define Tx_1__BYP CYREG_PRT1_BYP
#define Tx_1__CTL CYREG_PRT1_CTL
#define Tx_1__DM0 CYREG_PRT1_DM0
#define Tx_1__DM1 CYREG_PRT1_DM1
#define Tx_1__DM2 CYREG_PRT1_DM2
#define Tx_1__DR CYREG_PRT1_DR
#define Tx_1__INP_DIS CYREG_PRT1_INP_DIS
#define Tx_1__INTTYPE_BASE CYDEV_PICU_INTTYPE_PICU1_BASE
#define Tx_1__LCD_COM_SEG CYREG_PRT1_LCD_COM_SEG
#define Tx_1__LCD_EN CYREG_PRT1_LCD_EN
#define Tx_1__MASK 0x80u
#define Tx_1__PORT 1u
#define Tx_1__PRT CYREG_PRT1_PRT
#define Tx_1__PRTDSI__CAPS_SEL CYREG_PRT1_CAPS_SEL
#define Tx_1__PRTDSI__DBL_SYNC_IN CYREG_PRT1_DBL_SYNC_IN
#define Tx_1__PRTDSI__OE_SEL0 CYREG_PRT1_OE_SEL0
#define Tx_1__PRTDSI__OE_SEL1 CYREG_PRT1_OE_SEL1
#define Tx_1__PRTDSI__OUT_SEL0 CYREG_PRT1_OUT_SEL0
#define Tx_1__PRTDSI__OUT_SEL1 CYREG_PRT1_OUT_SEL1
#define Tx_1__PRTDSI__SYNC_OUT CYREG_PRT1_SYNC_OUT
#define Tx_1__PS CYREG_PRT1_PS
#define Tx_1__SHIFT 7u
#define Tx_1__SLW CYREG_PRT1_SLW

/* PWM_1 */
#define PWM_1_PWMHW__CAP0 CYREG_TMR0_CAP0
#define PWM_1_PWMHW__CAP1 CYREG_TMR0_CAP1
#define PWM_1_PWMHW__CFG0 CYREG_TMR0_CFG0
#define PWM_1_PWMHW__CFG1 CYREG_TMR0_CFG1
#define PWM_1_PWMHW__CFG2 CYREG_TMR0_CFG2
#define PWM_1_PWMHW__CNT_CMP0 CYREG_TMR0_CNT_CMP0
#define PWM_1_PWMHW__CNT_CMP1 CYREG_TMR0_CNT_CMP1
#define PWM_1_PWMHW__PER0 CYREG_TMR0_PER0
#define PWM_1_PWMHW__PER1 CYREG_TMR0_PER1
#define PWM_1_PWMHW__PM_ACT_CFG CYREG_PM_ACT_CFG3
#define PWM_1_PWMHW__PM_ACT_MSK 0x01u
#define PWM_1_PWMHW__PM_STBY_CFG CYREG_PM_STBY_CFG3
#define PWM_1_PWMHW__PM_STBY_MSK 0x01u
#define PWM_1_PWMHW__RT0 CYREG_TMR0_RT0
#define PWM_1_PWMHW__RT1 CYREG_TMR0_RT1
#define PWM_1_PWMHW__SR0 CYREG_TMR0_SR0

/* UART_1 */
#define UART_1_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG CYREG_B1_UDB11_ACTL
#define UART_1_BUART_sRX_RxBitCounter__CONTROL_REG CYREG_B1_UDB11_CTL
#define UART_1_BUART_sRX_RxBitCounter__CONTROL_ST_REG CYREG_B1_UDB11_ST_CTL
#define UART_1_BUART_sRX_RxBitCounter__COUNT_REG CYREG_B1_UDB11_CTL
#define UART_1_BUART_sRX_RxBitCounter__COUNT_ST_REG CYREG_B1_UDB11_ST_CTL
#define UART_1_BUART_sRX_RxBitCounter__MASK_CTL_AUX_CTL_REG CYREG_B1_UDB11_MSK_ACTL
#define UART_1_BUART_sRX_RxBitCounter__PER_CTL_AUX_CTL_REG CYREG_B1_UDB11_MSK_ACTL
#define UART_1_BUART_sRX_RxBitCounter__PERIOD_REG CYREG_B1_UDB11_MSK
#define UART_1_BUART_sRX_RxBitCounter_ST__MASK_REG CYREG_B1_UDB11_MSK
#define UART_1_BUART_sRX_RxBitCounter_ST__MASK_ST_AUX_CTL_REG CYREG_B1_UDB11_MSK_ACTL
#define UART_1_BUART_sRX_RxBitCounter_ST__PER_ST_AUX_CTL_REG CYREG_B1_UDB11_MSK_ACTL
#define UART_1_BUART_sRX_RxBitCounter_ST__STATUS_AUX_CTL_REG CYREG_B1_UDB11_ACTL
#define UART_1_BUART_sRX_RxBitCounter_ST__STATUS_CNT_REG CYREG_B1_UDB11_ST_CTL
#define UART_1_BUART_sRX_RxBitCounter_ST__STATUS_CONTROL_REG CYREG_B1_UDB11_ST_CTL
#define UART_1_BUART_sRX_RxBitCounter_ST__STATUS_REG CYREG_B1_UDB11_ST
#define UART_1_BUART_sRX_RxShifter_u0__A0_A1_REG CYREG_B1_UDB11_A0_A1
#define UART_1_BUART_sRX_RxShifter_u0__A0_REG CYREG_B1_UDB11_A0
#define UART_1_BUART_sRX_RxShifter_u0__A1_REG CYREG_B1_UDB11_A1
#define UART_1_BUART_sRX_RxShifter_u0__D0_D1_REG CYREG_B1_UDB11_D0_D1
#define UART_1_BUART_sRX_RxShifter_u0__D0_REG CYREG_B1_UDB11_D0
#define UART_1_BUART_sRX_RxShifter_u0__D1_REG CYREG_B1_UDB11_D1
#define UART_1_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG CYREG_B1_UDB11_ACTL
#define UART_1_BUART_sRX_RxShifter_u0__F0_F1_REG CYREG_B1_UDB11_F0_F1
#define UART_1_BUART_sRX_RxShifter_u0__F0_REG CYREG_B1_UDB11_F0
#define UART_1_BUART_sRX_RxShifter_u0__F1_REG CYREG_B1_UDB11_F1
#define UART_1_BUART_sRX_RxShifter_u0__MSK_DP_AUX_CTL_REG CYREG_B1_UDB11_MSK_ACTL
#define UART_1_BUART_sRX_RxShifter_u0__PER_DP_AUX_CTL_REG CYREG_B1_UDB11_MSK_ACTL
#define UART_1_BUART_sRX_RxSts__16BIT_STATUS_AUX_CTL_REG CYREG_B1_UDB10_11_ACTL
#define UART_1_BUART_sRX_RxSts__16BIT_STATUS_REG CYREG_B1_UDB10_11_ST
#define UART_1_BUART_sRX_RxSts__2__MASK 0x04u
#define UART_1_BUART_sRX_RxSts__2__POS 2
#define UART_1_BUART_sRX_RxSts__3__MASK 0x08u
#define UART_1_BUART_sRX_RxSts__3__POS 3
#define UART_1_BUART_sRX_RxSts__4__MASK 0x10u
#define UART_1_BUART_sRX_RxSts__4__POS 4
#define UART_1_BUART_sRX_RxSts__5__MASK 0x20u
#define UART_1_BUART_sRX_RxSts__5__POS 5
#define UART_1_BUART_sRX_RxSts__MASK 0x3Cu
#define UART_1_BUART_sRX_RxSts__MASK_REG CYREG_B1_UDB10_MSK
#define UART_1_BUART_sRX_RxSts__STATUS_AUX_CTL_REG CYREG_B1_UDB10_ACTL
#define UART_1_BUART_sRX_RxSts__STATUS_REG CYREG_B1_UDB10_ST
#define UART_1_BUART_sTX_sCLOCK_TxBitClkGen__16BIT_A0_REG CYREG_B1_UDB09_10_A0
#define UART_1_BUART_sTX_sCLOCK_TxBitClkGen__16BIT_A1_REG CYREG_B1_UDB09_10_A1
#define UART_1_BUART_sTX_sCLOCK_TxBitClkGen__16BIT_D0_REG CYREG_B1_UDB09_10_D0
#define UART_1_BUART_sTX_sCLOCK_TxBitClkGen__16BIT_D1_REG CYREG_B1_UDB09_10_D1
#define UART_1_BUART_sTX_sCLOCK_TxBitClkGen__16BIT_DP_AUX_CTL_REG CYREG_B1_UDB09_10_ACTL
#define UART_1_BUART_sTX_sCLOCK_TxBitClkGen__16BIT_F0_REG CYREG_B1_UDB09_10_F0
#define UART_1_BUART_sTX_sCLOCK_TxBitClkGen__16BIT_F1_REG CYREG_B1_UDB09_10_F1
#define UART_1_BUART_sTX_sCLOCK_TxBitClkGen__A0_A1_REG CYREG_B1_UDB09_A0_A1
#define UART_1_BUART_sTX_sCLOCK_TxBitClkGen__A0_REG CYREG_B1_UDB09_A0
#define UART_1_BUART_sTX_sCLOCK_TxBitClkGen__A1_REG CYREG_B1_UDB09_A1
#define UART_1_BUART_sTX_sCLOCK_TxBitClkGen__D0_D1_REG CYREG_B1_UDB09_D0_D1
#define UART_1_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG CYREG_B1_UDB09_D0
#define UART_1_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG CYREG_B1_UDB09_D1
#define UART_1_BUART_sTX_sCLOCK_TxBitClkGen__DP_AUX_CTL_REG CYREG_B1_UDB09_ACTL
#define UART_1_BUART_sTX_sCLOCK_TxBitClkGen__F0_F1_REG CYREG_B1_UDB09_F0_F1
#define UART_1_BUART_sTX_sCLOCK_TxBitClkGen__F0_REG CYREG_B1_UDB09_F0
#define UART_1_BUART_sTX_sCLOCK_TxBitClkGen__F1_REG CYREG_B1_UDB09_F1
#define UART_1_BUART_sTX_TxShifter_u0__16BIT_A0_REG CYREG_B0_UDB09_10_A0
#define UART_1_BUART_sTX_TxShifter_u0__16BIT_A1_REG CYREG_B0_UDB09_10_A1
#define UART_1_BUART_sTX_TxShifter_u0__16BIT_D0_REG CYREG_B0_UDB09_10_D0
#define UART_1_BUART_sTX_TxShifter_u0__16BIT_D1_REG CYREG_B0_UDB09_10_D1
#define UART_1_BUART_sTX_TxShifter_u0__16BIT_DP_AUX_CTL_REG CYREG_B0_UDB09_10_ACTL
#define UART_1_BUART_sTX_TxShifter_u0__16BIT_F0_REG CYREG_B0_UDB09_10_F0
#define UART_1_BUART_sTX_TxShifter_u0__16BIT_F1_REG CYREG_B0_UDB09_10_F1
#define UART_1_BUART_sTX_TxShifter_u0__A0_A1_REG CYREG_B0_UDB09_A0_A1
#define UART_1_BUART_sTX_TxShifter_u0__A0_REG CYREG_B0_UDB09_A0
#define UART_1_BUART_sTX_TxShifter_u0__A1_REG CYREG_B0_UDB09_A1
#define UART_1_BUART_sTX_TxShifter_u0__D0_D1_REG CYREG_B0_UDB09_D0_D1
#define UART_1_BUART_sTX_TxShifter_u0__D0_REG CYREG_B0_UDB09_D0
#define UART_1_BUART_sTX_TxShifter_u0__D1_REG CYREG_B0_UDB09_D1
#define UART_1_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG CYREG_B0_UDB09_ACTL
#define UART_1_BUART_sTX_TxShifter_u0__F0_F1_REG CYREG_B0_UDB09_F0_F1
#define UART_1_BUART_sTX_TxShifter_u0__F0_REG CYREG_B0_UDB09_F0
#define UART_1_BUART_sTX_TxShifter_u0__F1_REG CYREG_B0_UDB09_F1
#define UART_1_BUART_sTX_TxSts__0__MASK 0x01u
#define UART_1_BUART_sTX_TxSts__0__POS 0
#define UART_1_BUART_sTX_TxSts__1__MASK 0x02u
#define UART_1_BUART_sTX_TxSts__1__POS 1
#define UART_1_BUART_sTX_TxSts__16BIT_STATUS_AUX_CTL_REG CYREG_B0_UDB09_10_ACTL
#define UART_1_BUART_sTX_TxSts__16BIT_STATUS_REG CYREG_B0_UDB09_10_ST
#define UART_1_BUART_sTX_TxSts__2__MASK 0x04u
#define UART_1_BUART_sTX_TxSts__2__POS 2
#define UART_1_BUART_sTX_TxSts__3__MASK 0x08u
#define UART_1_BUART_sTX_TxSts__3__POS 3
#define UART_1_BUART_sTX_TxSts__MASK 0x0Fu
#define UART_1_BUART_sTX_TxSts__MASK_REG CYREG_B0_UDB09_MSK
#define UART_1_BUART_sTX_TxSts__STATUS_AUX_CTL_REG CYREG_B0_UDB09_ACTL
#define UART_1_BUART_sTX_TxSts__STATUS_REG CYREG_B0_UDB09_ST
#define UART_1_IntClock__CFG0 CYREG_CLKDIST_DCFG0_CFG0
#define UART_1_IntClock__CFG1 CYREG_CLKDIST_DCFG0_CFG1
#define UART_1_IntClock__CFG2 CYREG_CLKDIST_DCFG0_CFG2
#define UART_1_IntClock__CFG2_SRC_SEL_MASK 0x07u
#define UART_1_IntClock__INDEX 0x00u
#define UART_1_IntClock__PM_ACT_CFG CYREG_PM_ACT_CFG2
#define UART_1_IntClock__PM_ACT_MSK 0x01u
#define UART_1_IntClock__PM_STBY_CFG CYREG_PM_STBY_CFG2
#define UART_1_IntClock__PM_STBY_MSK 0x01u

/* PWM_Out */
#define PWM_Out__0__INTTYPE CYREG_PICU2_INTTYPE1
#define PWM_Out__0__MASK 0x02u
#define PWM_Out__0__PC CYREG_PRT2_PC1
#define PWM_Out__0__PORT 2u
#define PWM_Out__0__SHIFT 1u
#define PWM_Out__AG CYREG_PRT2_AG
#define PWM_Out__AMUX CYREG_PRT2_AMUX
#define PWM_Out__BIE CYREG_PRT2_BIE
#define PWM_Out__BIT_MASK CYREG_PRT2_BIT_MASK
#define PWM_Out__BYP CYREG_PRT2_BYP
#define PWM_Out__CTL CYREG_PRT2_CTL
#define PWM_Out__DM0 CYREG_PRT2_DM0
#define PWM_Out__DM1 CYREG_PRT2_DM1
#define PWM_Out__DM2 CYREG_PRT2_DM2
#define PWM_Out__DR CYREG_PRT2_DR
#define PWM_Out__INP_DIS CYREG_PRT2_INP_DIS
#define PWM_Out__INTTYPE_BASE CYDEV_PICU_INTTYPE_PICU2_BASE
#define PWM_Out__LCD_COM_SEG CYREG_PRT2_LCD_COM_SEG
#define PWM_Out__LCD_EN CYREG_PRT2_LCD_EN
#define PWM_Out__MASK 0x02u
#define PWM_Out__PORT 2u
#define PWM_Out__PRT CYREG_PRT2_PRT
#define PWM_Out__PRTDSI__CAPS_SEL CYREG_PRT2_CAPS_SEL
#define PWM_Out__PRTDSI__DBL_SYNC_IN CYREG_PRT2_DBL_SYNC_IN
#define PWM_Out__PRTDSI__OE_SEL0 CYREG_PRT2_OE_SEL0
#define PWM_Out__PRTDSI__OE_SEL1 CYREG_PRT2_OE_SEL1
#define PWM_Out__PRTDSI__OUT_SEL0 CYREG_PRT2_OUT_SEL0
#define PWM_Out__PRTDSI__OUT_SEL1 CYREG_PRT2_OUT_SEL1
#define PWM_Out__PRTDSI__SYNC_OUT CYREG_PRT2_SYNC_OUT
#define PWM_Out__PS CYREG_PRT2_PS
#define PWM_Out__SHIFT 1u
#define PWM_Out__SLW CYREG_PRT2_SLW

/* Timer_1 */
#define Timer_1_TimerHW__CAP0 CYREG_TMR1_CAP0
#define Timer_1_TimerHW__CAP1 CYREG_TMR1_CAP1
#define Timer_1_TimerHW__CFG0 CYREG_TMR1_CFG0
#define Timer_1_TimerHW__CFG1 CYREG_TMR1_CFG1
#define Timer_1_TimerHW__CFG2 CYREG_TMR1_CFG2
#define Timer_1_TimerHW__CNT_CMP0 CYREG_TMR1_CNT_CMP0
#define Timer_1_TimerHW__CNT_CMP1 CYREG_TMR1_CNT_CMP1
#define Timer_1_TimerHW__PER0 CYREG_TMR1_PER0
#define Timer_1_TimerHW__PER1 CYREG_TMR1_PER1
#define Timer_1_TimerHW__PM_ACT_CFG CYREG_PM_ACT_CFG3
#define Timer_1_TimerHW__PM_ACT_MSK 0x02u
#define Timer_1_TimerHW__PM_STBY_CFG CYREG_PM_STBY_CFG3
#define Timer_1_TimerHW__PM_STBY_MSK 0x02u
#define Timer_1_TimerHW__RT0 CYREG_TMR1_RT0
#define Timer_1_TimerHW__RT1 CYREG_TMR1_RT1
#define Timer_1_TimerHW__SR0 CYREG_TMR1_SR0

/* clock_1 */
#define clock_1__CFG0 CYREG_CLKDIST_DCFG1_CFG0
#define clock_1__CFG1 CYREG_CLKDIST_DCFG1_CFG1
#define clock_1__CFG2 CYREG_CLKDIST_DCFG1_CFG2
#define clock_1__CFG2_SRC_SEL_MASK 0x07u
#define clock_1__INDEX 0x01u
#define clock_1__PM_ACT_CFG CYREG_PM_ACT_CFG2
#define clock_1__PM_ACT_MSK 0x02u
#define clock_1__PM_STBY_CFG CYREG_PM_STBY_CFG2
#define clock_1__PM_STBY_MSK 0x02u

/* Kill_Switch */
#define Kill_Switch__0__INTTYPE CYREG_PICU2_INTTYPE2
#define Kill_Switch__0__MASK 0x04u
#define Kill_Switch__0__PC CYREG_PRT2_PC2
#define Kill_Switch__0__PORT 2u
#define Kill_Switch__0__SHIFT 2u
#define Kill_Switch__AG CYREG_PRT2_AG
#define Kill_Switch__AMUX CYREG_PRT2_AMUX
#define Kill_Switch__BIE CYREG_PRT2_BIE
#define Kill_Switch__BIT_MASK CYREG_PRT2_BIT_MASK
#define Kill_Switch__BYP CYREG_PRT2_BYP
#define Kill_Switch__CTL CYREG_PRT2_CTL
#define Kill_Switch__DM0 CYREG_PRT2_DM0
#define Kill_Switch__DM1 CYREG_PRT2_DM1
#define Kill_Switch__DM2 CYREG_PRT2_DM2
#define Kill_Switch__DR CYREG_PRT2_DR
#define Kill_Switch__INP_DIS CYREG_PRT2_INP_DIS
#define Kill_Switch__INTTYPE_BASE CYDEV_PICU_INTTYPE_PICU2_BASE
#define Kill_Switch__LCD_COM_SEG CYREG_PRT2_LCD_COM_SEG
#define Kill_Switch__LCD_EN CYREG_PRT2_LCD_EN
#define Kill_Switch__MASK 0x04u
#define Kill_Switch__PORT 2u
#define Kill_Switch__PRT CYREG_PRT2_PRT
#define Kill_Switch__PRTDSI__CAPS_SEL CYREG_PRT2_CAPS_SEL
#define Kill_Switch__PRTDSI__DBL_SYNC_IN CYREG_PRT2_DBL_SYNC_IN
#define Kill_Switch__PRTDSI__OE_SEL0 CYREG_PRT2_OE_SEL0
#define Kill_Switch__PRTDSI__OE_SEL1 CYREG_PRT2_OE_SEL1
#define Kill_Switch__PRTDSI__OUT_SEL0 CYREG_PRT2_OUT_SEL0
#define Kill_Switch__PRTDSI__OUT_SEL1 CYREG_PRT2_OUT_SEL1
#define Kill_Switch__PRTDSI__SYNC_OUT CYREG_PRT2_SYNC_OUT
#define Kill_Switch__PS CYREG_PRT2_PS
#define Kill_Switch__SHIFT 2u
#define Kill_Switch__SLW CYREG_PRT2_SLW

/* Miscellaneous */
#define BCLK__BUS_CLK__HZ 24000000U
#define BCLK__BUS_CLK__KHZ 24000U
#define BCLK__BUS_CLK__MHZ 24U
#define CY_PROJECT_NAME "Mepsan"
#define CY_VERSION "PSoC Creator  4.2"
#define CYDEV_CHIP_DIE_LEOPARD 1u
#define CYDEV_CHIP_DIE_PSOC4A 18u
#define CYDEV_CHIP_DIE_PSOC5LP 2u
#define CYDEV_CHIP_DIE_PSOC5TM 3u
#define CYDEV_CHIP_DIE_TMA4 4u
#define CYDEV_CHIP_DIE_UNKNOWN 0u
#define CYDEV_CHIP_FAMILY_FM0P 5u
#define CYDEV_CHIP_FAMILY_FM3 6u
#define CYDEV_CHIP_FAMILY_FM4 7u
#define CYDEV_CHIP_FAMILY_PSOC3 1u
#define CYDEV_CHIP_FAMILY_PSOC4 2u
#define CYDEV_CHIP_FAMILY_PSOC5 3u
#define CYDEV_CHIP_FAMILY_PSOC6 4u
#define CYDEV_CHIP_FAMILY_UNKNOWN 0u
#define CYDEV_CHIP_FAMILY_USED CYDEV_CHIP_FAMILY_PSOC5
#define CYDEV_CHIP_JTAG_ID 0x2E161069u
#define CYDEV_CHIP_MEMBER_3A 1u
#define CYDEV_CHIP_MEMBER_4A 18u
#define CYDEV_CHIP_MEMBER_4D 13u
#define CYDEV_CHIP_MEMBER_4E 6u
#define CYDEV_CHIP_MEMBER_4F 19u
#define CYDEV_CHIP_MEMBER_4G 4u
#define CYDEV_CHIP_MEMBER_4H 17u
#define CYDEV_CHIP_MEMBER_4I 23u
#define CYDEV_CHIP_MEMBER_4J 14u
#define CYDEV_CHIP_MEMBER_4K 15u
#define CYDEV_CHIP_MEMBER_4L 22u
#define CYDEV_CHIP_MEMBER_4M 21u
#define CYDEV_CHIP_MEMBER_4N 10u
#define CYDEV_CHIP_MEMBER_4O 7u
#define CYDEV_CHIP_MEMBER_4P 20u
#define CYDEV_CHIP_MEMBER_4Q 12u
#define CYDEV_CHIP_MEMBER_4R 8u
#define CYDEV_CHIP_MEMBER_4S 11u
#define CYDEV_CHIP_MEMBER_4T 9u
#define CYDEV_CHIP_MEMBER_4U 5u
#define CYDEV_CHIP_MEMBER_4V 16u
#define CYDEV_CHIP_MEMBER_5A 3u
#define CYDEV_CHIP_MEMBER_5B 2u
#define CYDEV_CHIP_MEMBER_6A 24u
#define CYDEV_CHIP_MEMBER_FM3 28u
#define CYDEV_CHIP_MEMBER_FM4 29u
#define CYDEV_CHIP_MEMBER_PDL_FM0P_TYPE1 25u
#define CYDEV_CHIP_MEMBER_PDL_FM0P_TYPE2 26u
#define CYDEV_CHIP_MEMBER_PDL_FM0P_TYPE3 27u
#define CYDEV_CHIP_MEMBER_UNKNOWN 0u
#define CYDEV_CHIP_MEMBER_USED CYDEV_CHIP_MEMBER_5B
#define CYDEV_CHIP_DIE_EXPECT CYDEV_CHIP_MEMBER_USED
#define CYDEV_CHIP_DIE_ACTUAL CYDEV_CHIP_DIE_EXPECT
#define CYDEV_CHIP_REV_LEOPARD_ES1 0u
#define CYDEV_CHIP_REV_LEOPARD_ES2 1u
#define CYDEV_CHIP_REV_LEOPARD_ES3 3u
#define CYDEV_CHIP_REV_LEOPARD_PRODUCTION 3u
#define CYDEV_CHIP_REV_PSOC4A_ES0 17u
#define CYDEV_CHIP_REV_PSOC4A_PRODUCTION 17u
#define CYDEV_CHIP_REV_PSOC5LP_ES0 0u
#define CYDEV_CHIP_REV_PSOC5LP_PRODUCTION 0u
#define CYDEV_CHIP_REV_PSOC5TM_ES0 0u
#define CYDEV_CHIP_REV_PSOC5TM_ES1 1u
#define CYDEV_CHIP_REV_PSOC5TM_PRODUCTION 1u
#define CYDEV_CHIP_REV_TMA4_ES 17u
#define CYDEV_CHIP_REV_TMA4_ES2 33u
#define CYDEV_CHIP_REV_TMA4_PRODUCTION 17u
#define CYDEV_CHIP_REVISION_3A_ES1 0u
#define CYDEV_CHIP_REVISION_3A_ES2 1u
#define CYDEV_CHIP_REVISION_3A_ES3 3u
#define CYDEV_CHIP_REVISION_3A_PRODUCTION 3u
#define CYDEV_CHIP_REVISION_4A_ES0 17u
#define CYDEV_CHIP_REVISION_4A_PRODUCTION 17u
#define CYDEV_CHIP_REVISION_4D_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4E_CCG2_NO_USBPD 0u
#define CYDEV_CHIP_REVISION_4E_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4F_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4F_PRODUCTION_256DMA 0u
#define CYDEV_CHIP_REVISION_4F_PRODUCTION_256K 0u
#define CYDEV_CHIP_REVISION_4G_ES 17u
#define CYDEV_CHIP_REVISION_4G_ES2 33u
#define CYDEV_CHIP_REVISION_4G_PRODUCTION 17u
#define CYDEV_CHIP_REVISION_4H_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4I_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4J_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4K_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4L_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4M_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4N_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4O_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4P_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4Q_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4R_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4S_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4T_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4U_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4V_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_5A_ES0 0u
#define CYDEV_CHIP_REVISION_5A_ES1 1u
#define CYDEV_CHIP_REVISION_5A_PRODUCTION 1u
#define CYDEV_CHIP_REVISION_5B_ES0 0u
#define CYDEV_CHIP_REVISION_5B_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_6A_ES 17u
#define CYDEV_CHIP_REVISION_6A_NO_UDB 33u
#define CYDEV_CHIP_REVISION_6A_PRODUCTION 33u
#define CYDEV_CHIP_REVISION_FM3_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_FM4_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_PDL_FM0P_TYPE1_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_PDL_FM0P_TYPE2_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_PDL_FM0P_TYPE3_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_USED CYDEV_CHIP_REVISION_5B_PRODUCTION
#define CYDEV_CHIP_REV_EXPECT CYDEV_CHIP_REVISION_USED
#define CYDEV_CONFIG_FASTBOOT_ENABLED 1
#define CYDEV_CONFIG_UNUSED_IO_AllowButWarn 0
#define CYDEV_CONFIG_UNUSED_IO CYDEV_CONFIG_UNUSED_IO_AllowButWarn
#define CYDEV_CONFIG_UNUSED_IO_AllowWithInfo 1
#define CYDEV_CONFIG_UNUSED_IO_Disallowed 2
#define CYDEV_CONFIGURATION_COMPRESSED 0
#define CYDEV_CONFIGURATION_DMA 1
#define CYDEV_CONFIGURATION_ECC 1
#define CYDEV_CONFIGURATION_IMOENABLED CYDEV_CONFIG_FASTBOOT_ENABLED
#define CYDEV_CONFIGURATION_MODE_COMPRESSED 0
#define CYDEV_CONFIGURATION_MODE_DMA 2
#define CYDEV_CONFIGURATION_MODE CYDEV_CONFIGURATION_MODE_DMA
#define CYDEV_CONFIGURATION_MODE_UNCOMPRESSED 1
#define CYDEV_DEBUG_ENABLE_MASK 0x20u
#define CYDEV_DEBUG_ENABLE_REGISTER CYREG_MLOGIC_DEBUG
#define CYDEV_DEBUGGING_DPS_Disable 3
#define CYDEV_DEBUGGING_DPS_JTAG_4 1
#define CYDEV_DEBUGGING_DPS_JTAG_5 0
#define CYDEV_DEBUGGING_DPS_SWD 2
#define CYDEV_DEBUGGING_DPS_SWD_SWV 6
#define CYDEV_DEBUGGING_DPS CYDEV_DEBUGGING_DPS_SWD_SWV
#define CYDEV_DEBUGGING_ENABLE 1
#define CYDEV_DEBUGGING_XRES 0
#define CYDEV_DMA_CHANNELS_AVAILABLE 24u
#define CYDEV_ECC_ENABLE 0
#define CYDEV_HEAP_SIZE 0x80
#define CYDEV_INSTRUCT_CACHE_ENABLED 0
#define CYDEV_INTR_RISING 0x00000000u
#define CYDEV_IS_EXPORTING_CODE 0
#define CYDEV_IS_IMPORTING_CODE 0
#define CYDEV_PROJ_TYPE 0
#define CYDEV_PROJ_TYPE_BOOTLOADER 1
#define CYDEV_PROJ_TYPE_LAUNCHER 5
#define CYDEV_PROJ_TYPE_LOADABLE 2
#define CYDEV_PROJ_TYPE_LOADABLEANDBOOTLOADER 4
#define CYDEV_PROJ_TYPE_MULTIAPPBOOTLOADER 3
#define CYDEV_PROJ_TYPE_STANDARD 0
#define CYDEV_PROTECTION_ENABLE 0
#define CYDEV_STACK_SIZE 0x0800
#define CYDEV_USE_BUNDLED_CMSIS 1
#define CYDEV_VARIABLE_VDDA 0
#define CYDEV_VDDA 5.0
#define CYDEV_VDDA_MV 5000
#define CYDEV_VDDD 5.0
#define CYDEV_VDDD_MV 5000
#define CYDEV_VDDIO0 5.0
#define CYDEV_VDDIO0_MV 5000
#define CYDEV_VDDIO1 5.0
#define CYDEV_VDDIO1_MV 5000
#define CYDEV_VDDIO2 5.0
#define CYDEV_VDDIO2_MV 5000
#define CYDEV_VDDIO3 5.0
#define CYDEV_VDDIO3_MV 5000
#define CYDEV_VIO0 5.0
#define CYDEV_VIO0_MV 5000
#define CYDEV_VIO1 5.0
#define CYDEV_VIO1_MV 5000
#define CYDEV_VIO2 5.0
#define CYDEV_VIO2_MV 5000
#define CYDEV_VIO3 5.0
#define CYDEV_VIO3_MV 5000
#define CYIPBLOCK_ARM_CM3_VERSION 0
#define CYIPBLOCK_P3_ANAIF_VERSION 0
#define CYIPBLOCK_P3_CAN_VERSION 0
#define CYIPBLOCK_P3_CAPSENSE_VERSION 0
#define CYIPBLOCK_P3_COMP_VERSION 0
#define CYIPBLOCK_P3_DECIMATOR_VERSION 0
#define CYIPBLOCK_P3_DFB_VERSION 0
#define CYIPBLOCK_P3_DMA_VERSION 0
#define CYIPBLOCK_P3_DRQ_VERSION 0
#define CYIPBLOCK_P3_DSM_VERSION 0
#define CYIPBLOCK_P3_EMIF_VERSION 0
#define CYIPBLOCK_P3_I2C_VERSION 0
#define CYIPBLOCK_P3_LCD_VERSION 0
#define CYIPBLOCK_P3_LPF_VERSION 0
#define CYIPBLOCK_P3_OPAMP_VERSION 0
#define CYIPBLOCK_P3_PM_VERSION 0
#define CYIPBLOCK_P3_SCCT_VERSION 0
#define CYIPBLOCK_P3_TIMER_VERSION 0
#define CYIPBLOCK_P3_USB_VERSION 0
#define CYIPBLOCK_P3_VIDAC_VERSION 0
#define CYIPBLOCK_P3_VREF_VERSION 0
#define CYIPBLOCK_S8_GPIO_VERSION 0
#define CYIPBLOCK_S8_IRQ_VERSION 0
#define CYIPBLOCK_S8_SAR_VERSION 0
#define CYIPBLOCK_S8_SIO_VERSION 0
#define CYIPBLOCK_S8_UDB_VERSION 0
#define DMA_CHANNELS_USED__MASK0 0x00000000u
#define CYDEV_BOOTLOADER_ENABLE 0

#endif /* INCLUDED_CYFITTER_H */
