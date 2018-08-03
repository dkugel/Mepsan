/*******************************************************************************
* File Name: screen2.h
* Version 2.50
*
* Description:
*  Contains the function prototypes and constants available to the UART
*  user module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#if !defined(CY_UART_screen2_H)
#define CY_UART_screen2_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */


/***************************************
* Conditional Compilation Parameters
***************************************/

#define screen2_RX_ENABLED                     (1u)
#define screen2_TX_ENABLED                     (1u)
#define screen2_HD_ENABLED                     (0u)
#define screen2_RX_INTERRUPT_ENABLED           (1u)
#define screen2_TX_INTERRUPT_ENABLED           (1u)
#define screen2_INTERNAL_CLOCK_USED            (1u)
#define screen2_RXHW_ADDRESS_ENABLED           (0u)
#define screen2_OVER_SAMPLE_COUNT              (8u)
#define screen2_PARITY_TYPE                    (0u)
#define screen2_PARITY_TYPE_SW                 (0u)
#define screen2_BREAK_DETECT                   (0u)
#define screen2_BREAK_BITS_TX                  (13u)
#define screen2_BREAK_BITS_RX                  (13u)
#define screen2_TXCLKGEN_DP                    (1u)
#define screen2_USE23POLLING                   (1u)
#define screen2_FLOW_CONTROL                   (0u)
#define screen2_CLK_FREQ                       (0u)
#define screen2_TX_BUFFER_SIZE                 (8u)
#define screen2_RX_BUFFER_SIZE                 (20u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component UART_v2_50 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#if defined(screen2_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG)
    #define screen2_CONTROL_REG_REMOVED            (0u)
#else
    #define screen2_CONTROL_REG_REMOVED            (1u)
#endif /* End screen2_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */


/***************************************
*      Data Structure Definition
***************************************/

/* Sleep Mode API Support */
typedef struct screen2_backupStruct_
{
    uint8 enableState;

    #if(screen2_CONTROL_REG_REMOVED == 0u)
        uint8 cr;
    #endif /* End screen2_CONTROL_REG_REMOVED */

} screen2_BACKUP_STRUCT;


/***************************************
*       Function Prototypes
***************************************/

void screen2_Start(void) ;
void screen2_Stop(void) ;
uint8 screen2_ReadControlRegister(void) ;
void screen2_WriteControlRegister(uint8 control) ;

void screen2_Init(void) ;
void screen2_Enable(void) ;
void screen2_SaveConfig(void) ;
void screen2_RestoreConfig(void) ;
void screen2_Sleep(void) ;
void screen2_Wakeup(void) ;

/* Only if RX is enabled */
#if( (screen2_RX_ENABLED) || (screen2_HD_ENABLED) )

    #if (screen2_RX_INTERRUPT_ENABLED)
        #define screen2_EnableRxInt()  CyIntEnable (screen2_RX_VECT_NUM)
        #define screen2_DisableRxInt() CyIntDisable(screen2_RX_VECT_NUM)
        CY_ISR_PROTO(screen2_RXISR);
    #endif /* screen2_RX_INTERRUPT_ENABLED */

    void screen2_SetRxAddressMode(uint8 addressMode)
                                                           ;
    void screen2_SetRxAddress1(uint8 address) ;
    void screen2_SetRxAddress2(uint8 address) ;

    void  screen2_SetRxInterruptMode(uint8 intSrc) ;
    uint8 screen2_ReadRxData(void) ;
    uint8 screen2_ReadRxStatus(void) ;
    uint8 screen2_GetChar(void) ;
    uint16 screen2_GetByte(void) ;
    uint8 screen2_GetRxBufferSize(void)
                                                            ;
    void screen2_ClearRxBuffer(void) ;

    /* Obsolete functions, defines for backward compatible */
    #define screen2_GetRxInterruptSource   screen2_ReadRxStatus

#endif /* End (screen2_RX_ENABLED) || (screen2_HD_ENABLED) */

/* Only if TX is enabled */
#if(screen2_TX_ENABLED || screen2_HD_ENABLED)

    #if(screen2_TX_INTERRUPT_ENABLED)
        #define screen2_EnableTxInt()  CyIntEnable (screen2_TX_VECT_NUM)
        #define screen2_DisableTxInt() CyIntDisable(screen2_TX_VECT_NUM)
        #define screen2_SetPendingTxInt() CyIntSetPending(screen2_TX_VECT_NUM)
        #define screen2_ClearPendingTxInt() CyIntClearPending(screen2_TX_VECT_NUM)
        CY_ISR_PROTO(screen2_TXISR);
    #endif /* screen2_TX_INTERRUPT_ENABLED */

    void screen2_SetTxInterruptMode(uint8 intSrc) ;
    void screen2_WriteTxData(uint8 txDataByte) ;
    uint8 screen2_ReadTxStatus(void) ;
    void screen2_PutChar(uint8 txDataByte) ;
    void screen2_PutString(const char8 string[]) ;
    void screen2_PutArray(const uint8 string[], uint8 byteCount)
                                                            ;
    void screen2_PutCRLF(uint8 txDataByte) ;
    void screen2_ClearTxBuffer(void) ;
    void screen2_SetTxAddressMode(uint8 addressMode) ;
    void screen2_SendBreak(uint8 retMode) ;
    uint8 screen2_GetTxBufferSize(void)
                                                            ;
    /* Obsolete functions, defines for backward compatible */
    #define screen2_PutStringConst         screen2_PutString
    #define screen2_PutArrayConst          screen2_PutArray
    #define screen2_GetTxInterruptSource   screen2_ReadTxStatus

#endif /* End screen2_TX_ENABLED || screen2_HD_ENABLED */

#if(screen2_HD_ENABLED)
    void screen2_LoadRxConfig(void) ;
    void screen2_LoadTxConfig(void) ;
#endif /* End screen2_HD_ENABLED */


/* Communication bootloader APIs */
#if defined(CYDEV_BOOTLOADER_IO_COMP) && ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_screen2) || \
                                          (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))
    /* Physical layer functions */
    void    screen2_CyBtldrCommStart(void) CYSMALL ;
    void    screen2_CyBtldrCommStop(void) CYSMALL ;
    void    screen2_CyBtldrCommReset(void) CYSMALL ;
    cystatus screen2_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;
    cystatus screen2_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;

    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_screen2)
        #define CyBtldrCommStart    screen2_CyBtldrCommStart
        #define CyBtldrCommStop     screen2_CyBtldrCommStop
        #define CyBtldrCommReset    screen2_CyBtldrCommReset
        #define CyBtldrCommWrite    screen2_CyBtldrCommWrite
        #define CyBtldrCommRead     screen2_CyBtldrCommRead
    #endif  /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_screen2) */

    /* Byte to Byte time out for detecting end of block data from host */
    #define screen2_BYTE2BYTE_TIME_OUT (25u)
    #define screen2_PACKET_EOP         (0x17u) /* End of packet defined by bootloader */
    #define screen2_WAIT_EOP_DELAY     (5u)    /* Additional 5ms to wait for End of packet */
    #define screen2_BL_CHK_DELAY_MS    (1u)    /* Time Out quantity equal 1mS */

#endif /* CYDEV_BOOTLOADER_IO_COMP */


/***************************************
*          API Constants
***************************************/
/* Parameters for SetTxAddressMode API*/
#define screen2_SET_SPACE      (0x00u)
#define screen2_SET_MARK       (0x01u)

/* Status Register definitions */
#if( (screen2_TX_ENABLED) || (screen2_HD_ENABLED) )
    #if(screen2_TX_INTERRUPT_ENABLED)
        #define screen2_TX_VECT_NUM            (uint8)screen2_TXInternalInterrupt__INTC_NUMBER
        #define screen2_TX_PRIOR_NUM           (uint8)screen2_TXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* screen2_TX_INTERRUPT_ENABLED */

    #define screen2_TX_STS_COMPLETE_SHIFT          (0x00u)
    #define screen2_TX_STS_FIFO_EMPTY_SHIFT        (0x01u)
    #define screen2_TX_STS_FIFO_NOT_FULL_SHIFT     (0x03u)
    #if(screen2_TX_ENABLED)
        #define screen2_TX_STS_FIFO_FULL_SHIFT     (0x02u)
    #else /* (screen2_HD_ENABLED) */
        #define screen2_TX_STS_FIFO_FULL_SHIFT     (0x05u)  /* Needs MD=0 */
    #endif /* (screen2_TX_ENABLED) */

    #define screen2_TX_STS_COMPLETE            (uint8)(0x01u << screen2_TX_STS_COMPLETE_SHIFT)
    #define screen2_TX_STS_FIFO_EMPTY          (uint8)(0x01u << screen2_TX_STS_FIFO_EMPTY_SHIFT)
    #define screen2_TX_STS_FIFO_FULL           (uint8)(0x01u << screen2_TX_STS_FIFO_FULL_SHIFT)
    #define screen2_TX_STS_FIFO_NOT_FULL       (uint8)(0x01u << screen2_TX_STS_FIFO_NOT_FULL_SHIFT)
#endif /* End (screen2_TX_ENABLED) || (screen2_HD_ENABLED)*/

#if( (screen2_RX_ENABLED) || (screen2_HD_ENABLED) )
    #if(screen2_RX_INTERRUPT_ENABLED)
        #define screen2_RX_VECT_NUM            (uint8)screen2_RXInternalInterrupt__INTC_NUMBER
        #define screen2_RX_PRIOR_NUM           (uint8)screen2_RXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* screen2_RX_INTERRUPT_ENABLED */
    #define screen2_RX_STS_MRKSPC_SHIFT            (0x00u)
    #define screen2_RX_STS_BREAK_SHIFT             (0x01u)
    #define screen2_RX_STS_PAR_ERROR_SHIFT         (0x02u)
    #define screen2_RX_STS_STOP_ERROR_SHIFT        (0x03u)
    #define screen2_RX_STS_OVERRUN_SHIFT           (0x04u)
    #define screen2_RX_STS_FIFO_NOTEMPTY_SHIFT     (0x05u)
    #define screen2_RX_STS_ADDR_MATCH_SHIFT        (0x06u)
    #define screen2_RX_STS_SOFT_BUFF_OVER_SHIFT    (0x07u)

    #define screen2_RX_STS_MRKSPC           (uint8)(0x01u << screen2_RX_STS_MRKSPC_SHIFT)
    #define screen2_RX_STS_BREAK            (uint8)(0x01u << screen2_RX_STS_BREAK_SHIFT)
    #define screen2_RX_STS_PAR_ERROR        (uint8)(0x01u << screen2_RX_STS_PAR_ERROR_SHIFT)
    #define screen2_RX_STS_STOP_ERROR       (uint8)(0x01u << screen2_RX_STS_STOP_ERROR_SHIFT)
    #define screen2_RX_STS_OVERRUN          (uint8)(0x01u << screen2_RX_STS_OVERRUN_SHIFT)
    #define screen2_RX_STS_FIFO_NOTEMPTY    (uint8)(0x01u << screen2_RX_STS_FIFO_NOTEMPTY_SHIFT)
    #define screen2_RX_STS_ADDR_MATCH       (uint8)(0x01u << screen2_RX_STS_ADDR_MATCH_SHIFT)
    #define screen2_RX_STS_SOFT_BUFF_OVER   (uint8)(0x01u << screen2_RX_STS_SOFT_BUFF_OVER_SHIFT)
    #define screen2_RX_HW_MASK                     (0x7Fu)
#endif /* End (screen2_RX_ENABLED) || (screen2_HD_ENABLED) */

/* Control Register definitions */
#define screen2_CTRL_HD_SEND_SHIFT                 (0x00u) /* 1 enable TX part in Half Duplex mode */
#define screen2_CTRL_HD_SEND_BREAK_SHIFT           (0x01u) /* 1 send BREAK signal in Half Duplez mode */
#define screen2_CTRL_MARK_SHIFT                    (0x02u) /* 1 sets mark, 0 sets space */
#define screen2_CTRL_PARITY_TYPE0_SHIFT            (0x03u) /* Defines the type of parity implemented */
#define screen2_CTRL_PARITY_TYPE1_SHIFT            (0x04u) /* Defines the type of parity implemented */
#define screen2_CTRL_RXADDR_MODE0_SHIFT            (0x05u)
#define screen2_CTRL_RXADDR_MODE1_SHIFT            (0x06u)
#define screen2_CTRL_RXADDR_MODE2_SHIFT            (0x07u)

#define screen2_CTRL_HD_SEND               (uint8)(0x01u << screen2_CTRL_HD_SEND_SHIFT)
#define screen2_CTRL_HD_SEND_BREAK         (uint8)(0x01u << screen2_CTRL_HD_SEND_BREAK_SHIFT)
#define screen2_CTRL_MARK                  (uint8)(0x01u << screen2_CTRL_MARK_SHIFT)
#define screen2_CTRL_PARITY_TYPE_MASK      (uint8)(0x03u << screen2_CTRL_PARITY_TYPE0_SHIFT)
#define screen2_CTRL_RXADDR_MODE_MASK      (uint8)(0x07u << screen2_CTRL_RXADDR_MODE0_SHIFT)

/* StatusI Register Interrupt Enable Control Bits. As defined by the Register map for the AUX Control Register */
#define screen2_INT_ENABLE                         (0x10u)

/* Bit Counter (7-bit) Control Register Bit Definitions. As defined by the Register map for the AUX Control Register */
#define screen2_CNTR_ENABLE                        (0x20u)

/*   Constants for SendBreak() "retMode" parameter  */
#define screen2_SEND_BREAK                         (0x00u)
#define screen2_WAIT_FOR_COMPLETE_REINIT           (0x01u)
#define screen2_REINIT                             (0x02u)
#define screen2_SEND_WAIT_REINIT                   (0x03u)

#define screen2_OVER_SAMPLE_8                      (8u)
#define screen2_OVER_SAMPLE_16                     (16u)

#define screen2_BIT_CENTER                         (screen2_OVER_SAMPLE_COUNT - 2u)

#define screen2_FIFO_LENGTH                        (4u)
#define screen2_NUMBER_OF_START_BIT                (1u)
#define screen2_MAX_BYTE_VALUE                     (0xFFu)

/* 8X always for count7 implementation */
#define screen2_TXBITCTR_BREAKBITS8X   ((screen2_BREAK_BITS_TX * screen2_OVER_SAMPLE_8) - 1u)
/* 8X or 16X for DP implementation */
#define screen2_TXBITCTR_BREAKBITS ((screen2_BREAK_BITS_TX * screen2_OVER_SAMPLE_COUNT) - 1u)

#define screen2_HALF_BIT_COUNT   \
                            (((screen2_OVER_SAMPLE_COUNT / 2u) + (screen2_USE23POLLING * 1u)) - 2u)
#if (screen2_OVER_SAMPLE_COUNT == screen2_OVER_SAMPLE_8)
    #define screen2_HD_TXBITCTR_INIT   (((screen2_BREAK_BITS_TX + \
                            screen2_NUMBER_OF_START_BIT) * screen2_OVER_SAMPLE_COUNT) - 1u)

    /* This parameter is increased on the 2 in 2 out of 3 mode to sample voting in the middle */
    #define screen2_RXBITCTR_INIT  ((((screen2_BREAK_BITS_RX + screen2_NUMBER_OF_START_BIT) \
                            * screen2_OVER_SAMPLE_COUNT) + screen2_HALF_BIT_COUNT) - 1u)

#else /* screen2_OVER_SAMPLE_COUNT == screen2_OVER_SAMPLE_16 */
    #define screen2_HD_TXBITCTR_INIT   ((8u * screen2_OVER_SAMPLE_COUNT) - 1u)
    /* 7bit counter need one more bit for OverSampleCount = 16 */
    #define screen2_RXBITCTR_INIT      (((7u * screen2_OVER_SAMPLE_COUNT) - 1u) + \
                                                      screen2_HALF_BIT_COUNT)
#endif /* End screen2_OVER_SAMPLE_COUNT */

#define screen2_HD_RXBITCTR_INIT                   screen2_RXBITCTR_INIT


/***************************************
* Global variables external identifier
***************************************/

extern uint8 screen2_initVar;
#if (screen2_TX_INTERRUPT_ENABLED && screen2_TX_ENABLED)
    extern volatile uint8 screen2_txBuffer[screen2_TX_BUFFER_SIZE];
    extern volatile uint8 screen2_txBufferRead;
    extern uint8 screen2_txBufferWrite;
#endif /* (screen2_TX_INTERRUPT_ENABLED && screen2_TX_ENABLED) */
#if (screen2_RX_INTERRUPT_ENABLED && (screen2_RX_ENABLED || screen2_HD_ENABLED))
    extern uint8 screen2_errorStatus;
    extern volatile uint8 screen2_rxBuffer[screen2_RX_BUFFER_SIZE];
    extern volatile uint8 screen2_rxBufferRead;
    extern volatile uint8 screen2_rxBufferWrite;
    extern volatile uint8 screen2_rxBufferLoopDetect;
    extern volatile uint8 screen2_rxBufferOverflow;
    #if (screen2_RXHW_ADDRESS_ENABLED)
        extern volatile uint8 screen2_rxAddressMode;
        extern volatile uint8 screen2_rxAddressDetected;
    #endif /* (screen2_RXHW_ADDRESS_ENABLED) */
#endif /* (screen2_RX_INTERRUPT_ENABLED && (screen2_RX_ENABLED || screen2_HD_ENABLED)) */


/***************************************
* Enumerated Types and Parameters
***************************************/

#define screen2__B_UART__AM_SW_BYTE_BYTE 1
#define screen2__B_UART__AM_SW_DETECT_TO_BUFFER 2
#define screen2__B_UART__AM_HW_BYTE_BY_BYTE 3
#define screen2__B_UART__AM_HW_DETECT_TO_BUFFER 4
#define screen2__B_UART__AM_NONE 0

#define screen2__B_UART__NONE_REVB 0
#define screen2__B_UART__EVEN_REVB 1
#define screen2__B_UART__ODD_REVB 2
#define screen2__B_UART__MARK_SPACE_REVB 3



/***************************************
*    Initial Parameter Constants
***************************************/

/* UART shifts max 8 bits, Mark/Space functionality working if 9 selected */
#define screen2_NUMBER_OF_DATA_BITS    ((8u > 8u) ? 8u : 8u)
#define screen2_NUMBER_OF_STOP_BITS    (1u)

#if (screen2_RXHW_ADDRESS_ENABLED)
    #define screen2_RX_ADDRESS_MODE    (0u)
    #define screen2_RX_HW_ADDRESS1     (0u)
    #define screen2_RX_HW_ADDRESS2     (0u)
#endif /* (screen2_RXHW_ADDRESS_ENABLED) */

#define screen2_INIT_RX_INTERRUPTS_MASK \
                                  (uint8)((1 << screen2_RX_STS_FIFO_NOTEMPTY_SHIFT) \
                                        | (0 << screen2_RX_STS_MRKSPC_SHIFT) \
                                        | (0 << screen2_RX_STS_ADDR_MATCH_SHIFT) \
                                        | (0 << screen2_RX_STS_PAR_ERROR_SHIFT) \
                                        | (0 << screen2_RX_STS_STOP_ERROR_SHIFT) \
                                        | (0 << screen2_RX_STS_BREAK_SHIFT) \
                                        | (0 << screen2_RX_STS_OVERRUN_SHIFT))

#define screen2_INIT_TX_INTERRUPTS_MASK \
                                  (uint8)((0 << screen2_TX_STS_COMPLETE_SHIFT) \
                                        | (1 << screen2_TX_STS_FIFO_EMPTY_SHIFT) \
                                        | (0 << screen2_TX_STS_FIFO_FULL_SHIFT) \
                                        | (0 << screen2_TX_STS_FIFO_NOT_FULL_SHIFT))


/***************************************
*              Registers
***************************************/

#ifdef screen2_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define screen2_CONTROL_REG \
                            (* (reg8 *) screen2_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
    #define screen2_CONTROL_PTR \
                            (  (reg8 *) screen2_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
#endif /* End screen2_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(screen2_TX_ENABLED)
    #define screen2_TXDATA_REG          (* (reg8 *) screen2_BUART_sTX_TxShifter_u0__F0_REG)
    #define screen2_TXDATA_PTR          (  (reg8 *) screen2_BUART_sTX_TxShifter_u0__F0_REG)
    #define screen2_TXDATA_AUX_CTL_REG  (* (reg8 *) screen2_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define screen2_TXDATA_AUX_CTL_PTR  (  (reg8 *) screen2_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define screen2_TXSTATUS_REG        (* (reg8 *) screen2_BUART_sTX_TxSts__STATUS_REG)
    #define screen2_TXSTATUS_PTR        (  (reg8 *) screen2_BUART_sTX_TxSts__STATUS_REG)
    #define screen2_TXSTATUS_MASK_REG   (* (reg8 *) screen2_BUART_sTX_TxSts__MASK_REG)
    #define screen2_TXSTATUS_MASK_PTR   (  (reg8 *) screen2_BUART_sTX_TxSts__MASK_REG)
    #define screen2_TXSTATUS_ACTL_REG   (* (reg8 *) screen2_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)
    #define screen2_TXSTATUS_ACTL_PTR   (  (reg8 *) screen2_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)

    /* DP clock */
    #if(screen2_TXCLKGEN_DP)
        #define screen2_TXBITCLKGEN_CTR_REG        \
                                        (* (reg8 *) screen2_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define screen2_TXBITCLKGEN_CTR_PTR        \
                                        (  (reg8 *) screen2_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define screen2_TXBITCLKTX_COMPLETE_REG    \
                                        (* (reg8 *) screen2_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
        #define screen2_TXBITCLKTX_COMPLETE_PTR    \
                                        (  (reg8 *) screen2_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
    #else     /* Count7 clock*/
        #define screen2_TXBITCTR_PERIOD_REG    \
                                        (* (reg8 *) screen2_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define screen2_TXBITCTR_PERIOD_PTR    \
                                        (  (reg8 *) screen2_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define screen2_TXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) screen2_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define screen2_TXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) screen2_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define screen2_TXBITCTR_COUNTER_REG   \
                                        (* (reg8 *) screen2_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
        #define screen2_TXBITCTR_COUNTER_PTR   \
                                        (  (reg8 *) screen2_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
    #endif /* screen2_TXCLKGEN_DP */

#endif /* End screen2_TX_ENABLED */

#if(screen2_HD_ENABLED)

    #define screen2_TXDATA_REG             (* (reg8 *) screen2_BUART_sRX_RxShifter_u0__F1_REG )
    #define screen2_TXDATA_PTR             (  (reg8 *) screen2_BUART_sRX_RxShifter_u0__F1_REG )
    #define screen2_TXDATA_AUX_CTL_REG     (* (reg8 *) screen2_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)
    #define screen2_TXDATA_AUX_CTL_PTR     (  (reg8 *) screen2_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define screen2_TXSTATUS_REG           (* (reg8 *) screen2_BUART_sRX_RxSts__STATUS_REG )
    #define screen2_TXSTATUS_PTR           (  (reg8 *) screen2_BUART_sRX_RxSts__STATUS_REG )
    #define screen2_TXSTATUS_MASK_REG      (* (reg8 *) screen2_BUART_sRX_RxSts__MASK_REG )
    #define screen2_TXSTATUS_MASK_PTR      (  (reg8 *) screen2_BUART_sRX_RxSts__MASK_REG )
    #define screen2_TXSTATUS_ACTL_REG      (* (reg8 *) screen2_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define screen2_TXSTATUS_ACTL_PTR      (  (reg8 *) screen2_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End screen2_HD_ENABLED */

#if( (screen2_RX_ENABLED) || (screen2_HD_ENABLED) )
    #define screen2_RXDATA_REG             (* (reg8 *) screen2_BUART_sRX_RxShifter_u0__F0_REG )
    #define screen2_RXDATA_PTR             (  (reg8 *) screen2_BUART_sRX_RxShifter_u0__F0_REG )
    #define screen2_RXADDRESS1_REG         (* (reg8 *) screen2_BUART_sRX_RxShifter_u0__D0_REG )
    #define screen2_RXADDRESS1_PTR         (  (reg8 *) screen2_BUART_sRX_RxShifter_u0__D0_REG )
    #define screen2_RXADDRESS2_REG         (* (reg8 *) screen2_BUART_sRX_RxShifter_u0__D1_REG )
    #define screen2_RXADDRESS2_PTR         (  (reg8 *) screen2_BUART_sRX_RxShifter_u0__D1_REG )
    #define screen2_RXDATA_AUX_CTL_REG     (* (reg8 *) screen2_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define screen2_RXBITCTR_PERIOD_REG    (* (reg8 *) screen2_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define screen2_RXBITCTR_PERIOD_PTR    (  (reg8 *) screen2_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define screen2_RXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) screen2_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define screen2_RXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) screen2_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define screen2_RXBITCTR_COUNTER_REG   (* (reg8 *) screen2_BUART_sRX_RxBitCounter__COUNT_REG )
    #define screen2_RXBITCTR_COUNTER_PTR   (  (reg8 *) screen2_BUART_sRX_RxBitCounter__COUNT_REG )

    #define screen2_RXSTATUS_REG           (* (reg8 *) screen2_BUART_sRX_RxSts__STATUS_REG )
    #define screen2_RXSTATUS_PTR           (  (reg8 *) screen2_BUART_sRX_RxSts__STATUS_REG )
    #define screen2_RXSTATUS_MASK_REG      (* (reg8 *) screen2_BUART_sRX_RxSts__MASK_REG )
    #define screen2_RXSTATUS_MASK_PTR      (  (reg8 *) screen2_BUART_sRX_RxSts__MASK_REG )
    #define screen2_RXSTATUS_ACTL_REG      (* (reg8 *) screen2_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define screen2_RXSTATUS_ACTL_PTR      (  (reg8 *) screen2_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End  (screen2_RX_ENABLED) || (screen2_HD_ENABLED) */

#if(screen2_INTERNAL_CLOCK_USED)
    /* Register to enable or disable the digital clocks */
    #define screen2_INTCLOCK_CLKEN_REG     (* (reg8 *) screen2_IntClock__PM_ACT_CFG)
    #define screen2_INTCLOCK_CLKEN_PTR     (  (reg8 *) screen2_IntClock__PM_ACT_CFG)

    /* Clock mask for this clock. */
    #define screen2_INTCLOCK_CLKEN_MASK    screen2_IntClock__PM_ACT_MSK
#endif /* End screen2_INTERNAL_CLOCK_USED */


/***************************************
*       Register Constants
***************************************/

#if(screen2_TX_ENABLED)
    #define screen2_TX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End screen2_TX_ENABLED */

#if(screen2_HD_ENABLED)
    #define screen2_TX_FIFO_CLR            (0x02u) /* FIFO1 CLR */
#endif /* End screen2_HD_ENABLED */

#if( (screen2_RX_ENABLED) || (screen2_HD_ENABLED) )
    #define screen2_RX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End  (screen2_RX_ENABLED) || (screen2_HD_ENABLED) */


/***************************************
* The following code is DEPRECATED and
* should not be used in new projects.
***************************************/

/* UART v2_40 obsolete definitions */
#define screen2_WAIT_1_MS      screen2_BL_CHK_DELAY_MS   

#define screen2_TXBUFFERSIZE   screen2_TX_BUFFER_SIZE
#define screen2_RXBUFFERSIZE   screen2_RX_BUFFER_SIZE

#if (screen2_RXHW_ADDRESS_ENABLED)
    #define screen2_RXADDRESSMODE  screen2_RX_ADDRESS_MODE
    #define screen2_RXHWADDRESS1   screen2_RX_HW_ADDRESS1
    #define screen2_RXHWADDRESS2   screen2_RX_HW_ADDRESS2
    /* Backward compatible define */
    #define screen2_RXAddressMode  screen2_RXADDRESSMODE
#endif /* (screen2_RXHW_ADDRESS_ENABLED) */

/* UART v2_30 obsolete definitions */
#define screen2_initvar                    screen2_initVar

#define screen2_RX_Enabled                 screen2_RX_ENABLED
#define screen2_TX_Enabled                 screen2_TX_ENABLED
#define screen2_HD_Enabled                 screen2_HD_ENABLED
#define screen2_RX_IntInterruptEnabled     screen2_RX_INTERRUPT_ENABLED
#define screen2_TX_IntInterruptEnabled     screen2_TX_INTERRUPT_ENABLED
#define screen2_InternalClockUsed          screen2_INTERNAL_CLOCK_USED
#define screen2_RXHW_Address_Enabled       screen2_RXHW_ADDRESS_ENABLED
#define screen2_OverSampleCount            screen2_OVER_SAMPLE_COUNT
#define screen2_ParityType                 screen2_PARITY_TYPE

#if( screen2_TX_ENABLED && (screen2_TXBUFFERSIZE > screen2_FIFO_LENGTH))
    #define screen2_TXBUFFER               screen2_txBuffer
    #define screen2_TXBUFFERREAD           screen2_txBufferRead
    #define screen2_TXBUFFERWRITE          screen2_txBufferWrite
#endif /* End screen2_TX_ENABLED */
#if( ( screen2_RX_ENABLED || screen2_HD_ENABLED ) && \
     (screen2_RXBUFFERSIZE > screen2_FIFO_LENGTH) )
    #define screen2_RXBUFFER               screen2_rxBuffer
    #define screen2_RXBUFFERREAD           screen2_rxBufferRead
    #define screen2_RXBUFFERWRITE          screen2_rxBufferWrite
    #define screen2_RXBUFFERLOOPDETECT     screen2_rxBufferLoopDetect
    #define screen2_RXBUFFER_OVERFLOW      screen2_rxBufferOverflow
#endif /* End screen2_RX_ENABLED */

#ifdef screen2_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define screen2_CONTROL                screen2_CONTROL_REG
#endif /* End screen2_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(screen2_TX_ENABLED)
    #define screen2_TXDATA                 screen2_TXDATA_REG
    #define screen2_TXSTATUS               screen2_TXSTATUS_REG
    #define screen2_TXSTATUS_MASK          screen2_TXSTATUS_MASK_REG
    #define screen2_TXSTATUS_ACTL          screen2_TXSTATUS_ACTL_REG
    /* DP clock */
    #if(screen2_TXCLKGEN_DP)
        #define screen2_TXBITCLKGEN_CTR        screen2_TXBITCLKGEN_CTR_REG
        #define screen2_TXBITCLKTX_COMPLETE    screen2_TXBITCLKTX_COMPLETE_REG
    #else     /* Count7 clock*/
        #define screen2_TXBITCTR_PERIOD        screen2_TXBITCTR_PERIOD_REG
        #define screen2_TXBITCTR_CONTROL       screen2_TXBITCTR_CONTROL_REG
        #define screen2_TXBITCTR_COUNTER       screen2_TXBITCTR_COUNTER_REG
    #endif /* screen2_TXCLKGEN_DP */
#endif /* End screen2_TX_ENABLED */

#if(screen2_HD_ENABLED)
    #define screen2_TXDATA                 screen2_TXDATA_REG
    #define screen2_TXSTATUS               screen2_TXSTATUS_REG
    #define screen2_TXSTATUS_MASK          screen2_TXSTATUS_MASK_REG
    #define screen2_TXSTATUS_ACTL          screen2_TXSTATUS_ACTL_REG
#endif /* End screen2_HD_ENABLED */

#if( (screen2_RX_ENABLED) || (screen2_HD_ENABLED) )
    #define screen2_RXDATA                 screen2_RXDATA_REG
    #define screen2_RXADDRESS1             screen2_RXADDRESS1_REG
    #define screen2_RXADDRESS2             screen2_RXADDRESS2_REG
    #define screen2_RXBITCTR_PERIOD        screen2_RXBITCTR_PERIOD_REG
    #define screen2_RXBITCTR_CONTROL       screen2_RXBITCTR_CONTROL_REG
    #define screen2_RXBITCTR_COUNTER       screen2_RXBITCTR_COUNTER_REG
    #define screen2_RXSTATUS               screen2_RXSTATUS_REG
    #define screen2_RXSTATUS_MASK          screen2_RXSTATUS_MASK_REG
    #define screen2_RXSTATUS_ACTL          screen2_RXSTATUS_ACTL_REG
#endif /* End  (screen2_RX_ENABLED) || (screen2_HD_ENABLED) */

#if(screen2_INTERNAL_CLOCK_USED)
    #define screen2_INTCLOCK_CLKEN         screen2_INTCLOCK_CLKEN_REG
#endif /* End screen2_INTERNAL_CLOCK_USED */

#define screen2_WAIT_FOR_COMLETE_REINIT    screen2_WAIT_FOR_COMPLETE_REINIT

#endif  /* CY_UART_screen2_H */


/* [] END OF FILE */
