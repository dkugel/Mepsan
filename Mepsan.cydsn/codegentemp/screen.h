/*******************************************************************************
* File Name: screen.h
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


#if !defined(CY_UART_screen_H)
#define CY_UART_screen_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */


/***************************************
* Conditional Compilation Parameters
***************************************/

#define screen_RX_ENABLED                     (1u)
#define screen_TX_ENABLED                     (1u)
#define screen_HD_ENABLED                     (0u)
#define screen_RX_INTERRUPT_ENABLED           (1u)
#define screen_TX_INTERRUPT_ENABLED           (1u)
#define screen_INTERNAL_CLOCK_USED            (1u)
#define screen_RXHW_ADDRESS_ENABLED           (0u)
#define screen_OVER_SAMPLE_COUNT              (8u)
#define screen_PARITY_TYPE                    (0u)
#define screen_PARITY_TYPE_SW                 (0u)
#define screen_BREAK_DETECT                   (0u)
#define screen_BREAK_BITS_TX                  (13u)
#define screen_BREAK_BITS_RX                  (13u)
#define screen_TXCLKGEN_DP                    (1u)
#define screen_USE23POLLING                   (1u)
#define screen_FLOW_CONTROL                   (0u)
#define screen_CLK_FREQ                       (0u)
#define screen_TX_BUFFER_SIZE                 (8u)
#define screen_RX_BUFFER_SIZE                 (20u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component UART_v2_50 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#if defined(screen_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG)
    #define screen_CONTROL_REG_REMOVED            (0u)
#else
    #define screen_CONTROL_REG_REMOVED            (1u)
#endif /* End screen_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */


/***************************************
*      Data Structure Definition
***************************************/

/* Sleep Mode API Support */
typedef struct screen_backupStruct_
{
    uint8 enableState;

    #if(screen_CONTROL_REG_REMOVED == 0u)
        uint8 cr;
    #endif /* End screen_CONTROL_REG_REMOVED */

} screen_BACKUP_STRUCT;


/***************************************
*       Function Prototypes
***************************************/

void screen_Start(void) ;
void screen_Stop(void) ;
uint8 screen_ReadControlRegister(void) ;
void screen_WriteControlRegister(uint8 control) ;

void screen_Init(void) ;
void screen_Enable(void) ;
void screen_SaveConfig(void) ;
void screen_RestoreConfig(void) ;
void screen_Sleep(void) ;
void screen_Wakeup(void) ;

/* Only if RX is enabled */
#if( (screen_RX_ENABLED) || (screen_HD_ENABLED) )

    #if (screen_RX_INTERRUPT_ENABLED)
        #define screen_EnableRxInt()  CyIntEnable (screen_RX_VECT_NUM)
        #define screen_DisableRxInt() CyIntDisable(screen_RX_VECT_NUM)
        CY_ISR_PROTO(screen_RXISR);
    #endif /* screen_RX_INTERRUPT_ENABLED */

    void screen_SetRxAddressMode(uint8 addressMode)
                                                           ;
    void screen_SetRxAddress1(uint8 address) ;
    void screen_SetRxAddress2(uint8 address) ;

    void  screen_SetRxInterruptMode(uint8 intSrc) ;
    uint8 screen_ReadRxData(void) ;
    uint8 screen_ReadRxStatus(void) ;
    uint8 screen_GetChar(void) ;
    uint16 screen_GetByte(void) ;
    uint8 screen_GetRxBufferSize(void)
                                                            ;
    void screen_ClearRxBuffer(void) ;

    /* Obsolete functions, defines for backward compatible */
    #define screen_GetRxInterruptSource   screen_ReadRxStatus

#endif /* End (screen_RX_ENABLED) || (screen_HD_ENABLED) */

/* Only if TX is enabled */
#if(screen_TX_ENABLED || screen_HD_ENABLED)

    #if(screen_TX_INTERRUPT_ENABLED)
        #define screen_EnableTxInt()  CyIntEnable (screen_TX_VECT_NUM)
        #define screen_DisableTxInt() CyIntDisable(screen_TX_VECT_NUM)
        #define screen_SetPendingTxInt() CyIntSetPending(screen_TX_VECT_NUM)
        #define screen_ClearPendingTxInt() CyIntClearPending(screen_TX_VECT_NUM)
        CY_ISR_PROTO(screen_TXISR);
    #endif /* screen_TX_INTERRUPT_ENABLED */

    void screen_SetTxInterruptMode(uint8 intSrc) ;
    void screen_WriteTxData(uint8 txDataByte) ;
    uint8 screen_ReadTxStatus(void) ;
    void screen_PutChar(uint8 txDataByte) ;
    void screen_PutString(const char8 string[]) ;
    void screen_PutArray(const uint8 string[], uint8 byteCount)
                                                            ;
    void screen_PutCRLF(uint8 txDataByte) ;
    void screen_ClearTxBuffer(void) ;
    void screen_SetTxAddressMode(uint8 addressMode) ;
    void screen_SendBreak(uint8 retMode) ;
    uint8 screen_GetTxBufferSize(void)
                                                            ;
    /* Obsolete functions, defines for backward compatible */
    #define screen_PutStringConst         screen_PutString
    #define screen_PutArrayConst          screen_PutArray
    #define screen_GetTxInterruptSource   screen_ReadTxStatus

#endif /* End screen_TX_ENABLED || screen_HD_ENABLED */

#if(screen_HD_ENABLED)
    void screen_LoadRxConfig(void) ;
    void screen_LoadTxConfig(void) ;
#endif /* End screen_HD_ENABLED */


/* Communication bootloader APIs */
#if defined(CYDEV_BOOTLOADER_IO_COMP) && ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_screen) || \
                                          (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))
    /* Physical layer functions */
    void    screen_CyBtldrCommStart(void) CYSMALL ;
    void    screen_CyBtldrCommStop(void) CYSMALL ;
    void    screen_CyBtldrCommReset(void) CYSMALL ;
    cystatus screen_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;
    cystatus screen_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;

    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_screen)
        #define CyBtldrCommStart    screen_CyBtldrCommStart
        #define CyBtldrCommStop     screen_CyBtldrCommStop
        #define CyBtldrCommReset    screen_CyBtldrCommReset
        #define CyBtldrCommWrite    screen_CyBtldrCommWrite
        #define CyBtldrCommRead     screen_CyBtldrCommRead
    #endif  /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_screen) */

    /* Byte to Byte time out for detecting end of block data from host */
    #define screen_BYTE2BYTE_TIME_OUT (25u)
    #define screen_PACKET_EOP         (0x17u) /* End of packet defined by bootloader */
    #define screen_WAIT_EOP_DELAY     (5u)    /* Additional 5ms to wait for End of packet */
    #define screen_BL_CHK_DELAY_MS    (1u)    /* Time Out quantity equal 1mS */

#endif /* CYDEV_BOOTLOADER_IO_COMP */


/***************************************
*          API Constants
***************************************/
/* Parameters for SetTxAddressMode API*/
#define screen_SET_SPACE      (0x00u)
#define screen_SET_MARK       (0x01u)

/* Status Register definitions */
#if( (screen_TX_ENABLED) || (screen_HD_ENABLED) )
    #if(screen_TX_INTERRUPT_ENABLED)
        #define screen_TX_VECT_NUM            (uint8)screen_TXInternalInterrupt__INTC_NUMBER
        #define screen_TX_PRIOR_NUM           (uint8)screen_TXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* screen_TX_INTERRUPT_ENABLED */

    #define screen_TX_STS_COMPLETE_SHIFT          (0x00u)
    #define screen_TX_STS_FIFO_EMPTY_SHIFT        (0x01u)
    #define screen_TX_STS_FIFO_NOT_FULL_SHIFT     (0x03u)
    #if(screen_TX_ENABLED)
        #define screen_TX_STS_FIFO_FULL_SHIFT     (0x02u)
    #else /* (screen_HD_ENABLED) */
        #define screen_TX_STS_FIFO_FULL_SHIFT     (0x05u)  /* Needs MD=0 */
    #endif /* (screen_TX_ENABLED) */

    #define screen_TX_STS_COMPLETE            (uint8)(0x01u << screen_TX_STS_COMPLETE_SHIFT)
    #define screen_TX_STS_FIFO_EMPTY          (uint8)(0x01u << screen_TX_STS_FIFO_EMPTY_SHIFT)
    #define screen_TX_STS_FIFO_FULL           (uint8)(0x01u << screen_TX_STS_FIFO_FULL_SHIFT)
    #define screen_TX_STS_FIFO_NOT_FULL       (uint8)(0x01u << screen_TX_STS_FIFO_NOT_FULL_SHIFT)
#endif /* End (screen_TX_ENABLED) || (screen_HD_ENABLED)*/

#if( (screen_RX_ENABLED) || (screen_HD_ENABLED) )
    #if(screen_RX_INTERRUPT_ENABLED)
        #define screen_RX_VECT_NUM            (uint8)screen_RXInternalInterrupt__INTC_NUMBER
        #define screen_RX_PRIOR_NUM           (uint8)screen_RXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* screen_RX_INTERRUPT_ENABLED */
    #define screen_RX_STS_MRKSPC_SHIFT            (0x00u)
    #define screen_RX_STS_BREAK_SHIFT             (0x01u)
    #define screen_RX_STS_PAR_ERROR_SHIFT         (0x02u)
    #define screen_RX_STS_STOP_ERROR_SHIFT        (0x03u)
    #define screen_RX_STS_OVERRUN_SHIFT           (0x04u)
    #define screen_RX_STS_FIFO_NOTEMPTY_SHIFT     (0x05u)
    #define screen_RX_STS_ADDR_MATCH_SHIFT        (0x06u)
    #define screen_RX_STS_SOFT_BUFF_OVER_SHIFT    (0x07u)

    #define screen_RX_STS_MRKSPC           (uint8)(0x01u << screen_RX_STS_MRKSPC_SHIFT)
    #define screen_RX_STS_BREAK            (uint8)(0x01u << screen_RX_STS_BREAK_SHIFT)
    #define screen_RX_STS_PAR_ERROR        (uint8)(0x01u << screen_RX_STS_PAR_ERROR_SHIFT)
    #define screen_RX_STS_STOP_ERROR       (uint8)(0x01u << screen_RX_STS_STOP_ERROR_SHIFT)
    #define screen_RX_STS_OVERRUN          (uint8)(0x01u << screen_RX_STS_OVERRUN_SHIFT)
    #define screen_RX_STS_FIFO_NOTEMPTY    (uint8)(0x01u << screen_RX_STS_FIFO_NOTEMPTY_SHIFT)
    #define screen_RX_STS_ADDR_MATCH       (uint8)(0x01u << screen_RX_STS_ADDR_MATCH_SHIFT)
    #define screen_RX_STS_SOFT_BUFF_OVER   (uint8)(0x01u << screen_RX_STS_SOFT_BUFF_OVER_SHIFT)
    #define screen_RX_HW_MASK                     (0x7Fu)
#endif /* End (screen_RX_ENABLED) || (screen_HD_ENABLED) */

/* Control Register definitions */
#define screen_CTRL_HD_SEND_SHIFT                 (0x00u) /* 1 enable TX part in Half Duplex mode */
#define screen_CTRL_HD_SEND_BREAK_SHIFT           (0x01u) /* 1 send BREAK signal in Half Duplez mode */
#define screen_CTRL_MARK_SHIFT                    (0x02u) /* 1 sets mark, 0 sets space */
#define screen_CTRL_PARITY_TYPE0_SHIFT            (0x03u) /* Defines the type of parity implemented */
#define screen_CTRL_PARITY_TYPE1_SHIFT            (0x04u) /* Defines the type of parity implemented */
#define screen_CTRL_RXADDR_MODE0_SHIFT            (0x05u)
#define screen_CTRL_RXADDR_MODE1_SHIFT            (0x06u)
#define screen_CTRL_RXADDR_MODE2_SHIFT            (0x07u)

#define screen_CTRL_HD_SEND               (uint8)(0x01u << screen_CTRL_HD_SEND_SHIFT)
#define screen_CTRL_HD_SEND_BREAK         (uint8)(0x01u << screen_CTRL_HD_SEND_BREAK_SHIFT)
#define screen_CTRL_MARK                  (uint8)(0x01u << screen_CTRL_MARK_SHIFT)
#define screen_CTRL_PARITY_TYPE_MASK      (uint8)(0x03u << screen_CTRL_PARITY_TYPE0_SHIFT)
#define screen_CTRL_RXADDR_MODE_MASK      (uint8)(0x07u << screen_CTRL_RXADDR_MODE0_SHIFT)

/* StatusI Register Interrupt Enable Control Bits. As defined by the Register map for the AUX Control Register */
#define screen_INT_ENABLE                         (0x10u)

/* Bit Counter (7-bit) Control Register Bit Definitions. As defined by the Register map for the AUX Control Register */
#define screen_CNTR_ENABLE                        (0x20u)

/*   Constants for SendBreak() "retMode" parameter  */
#define screen_SEND_BREAK                         (0x00u)
#define screen_WAIT_FOR_COMPLETE_REINIT           (0x01u)
#define screen_REINIT                             (0x02u)
#define screen_SEND_WAIT_REINIT                   (0x03u)

#define screen_OVER_SAMPLE_8                      (8u)
#define screen_OVER_SAMPLE_16                     (16u)

#define screen_BIT_CENTER                         (screen_OVER_SAMPLE_COUNT - 2u)

#define screen_FIFO_LENGTH                        (4u)
#define screen_NUMBER_OF_START_BIT                (1u)
#define screen_MAX_BYTE_VALUE                     (0xFFu)

/* 8X always for count7 implementation */
#define screen_TXBITCTR_BREAKBITS8X   ((screen_BREAK_BITS_TX * screen_OVER_SAMPLE_8) - 1u)
/* 8X or 16X for DP implementation */
#define screen_TXBITCTR_BREAKBITS ((screen_BREAK_BITS_TX * screen_OVER_SAMPLE_COUNT) - 1u)

#define screen_HALF_BIT_COUNT   \
                            (((screen_OVER_SAMPLE_COUNT / 2u) + (screen_USE23POLLING * 1u)) - 2u)
#if (screen_OVER_SAMPLE_COUNT == screen_OVER_SAMPLE_8)
    #define screen_HD_TXBITCTR_INIT   (((screen_BREAK_BITS_TX + \
                            screen_NUMBER_OF_START_BIT) * screen_OVER_SAMPLE_COUNT) - 1u)

    /* This parameter is increased on the 2 in 2 out of 3 mode to sample voting in the middle */
    #define screen_RXBITCTR_INIT  ((((screen_BREAK_BITS_RX + screen_NUMBER_OF_START_BIT) \
                            * screen_OVER_SAMPLE_COUNT) + screen_HALF_BIT_COUNT) - 1u)

#else /* screen_OVER_SAMPLE_COUNT == screen_OVER_SAMPLE_16 */
    #define screen_HD_TXBITCTR_INIT   ((8u * screen_OVER_SAMPLE_COUNT) - 1u)
    /* 7bit counter need one more bit for OverSampleCount = 16 */
    #define screen_RXBITCTR_INIT      (((7u * screen_OVER_SAMPLE_COUNT) - 1u) + \
                                                      screen_HALF_BIT_COUNT)
#endif /* End screen_OVER_SAMPLE_COUNT */

#define screen_HD_RXBITCTR_INIT                   screen_RXBITCTR_INIT


/***************************************
* Global variables external identifier
***************************************/

extern uint8 screen_initVar;
#if (screen_TX_INTERRUPT_ENABLED && screen_TX_ENABLED)
    extern volatile uint8 screen_txBuffer[screen_TX_BUFFER_SIZE];
    extern volatile uint8 screen_txBufferRead;
    extern uint8 screen_txBufferWrite;
#endif /* (screen_TX_INTERRUPT_ENABLED && screen_TX_ENABLED) */
#if (screen_RX_INTERRUPT_ENABLED && (screen_RX_ENABLED || screen_HD_ENABLED))
    extern uint8 screen_errorStatus;
    extern volatile uint8 screen_rxBuffer[screen_RX_BUFFER_SIZE];
    extern volatile uint8 screen_rxBufferRead;
    extern volatile uint8 screen_rxBufferWrite;
    extern volatile uint8 screen_rxBufferLoopDetect;
    extern volatile uint8 screen_rxBufferOverflow;
    #if (screen_RXHW_ADDRESS_ENABLED)
        extern volatile uint8 screen_rxAddressMode;
        extern volatile uint8 screen_rxAddressDetected;
    #endif /* (screen_RXHW_ADDRESS_ENABLED) */
#endif /* (screen_RX_INTERRUPT_ENABLED && (screen_RX_ENABLED || screen_HD_ENABLED)) */


/***************************************
* Enumerated Types and Parameters
***************************************/

#define screen__B_UART__AM_SW_BYTE_BYTE 1
#define screen__B_UART__AM_SW_DETECT_TO_BUFFER 2
#define screen__B_UART__AM_HW_BYTE_BY_BYTE 3
#define screen__B_UART__AM_HW_DETECT_TO_BUFFER 4
#define screen__B_UART__AM_NONE 0

#define screen__B_UART__NONE_REVB 0
#define screen__B_UART__EVEN_REVB 1
#define screen__B_UART__ODD_REVB 2
#define screen__B_UART__MARK_SPACE_REVB 3



/***************************************
*    Initial Parameter Constants
***************************************/

/* UART shifts max 8 bits, Mark/Space functionality working if 9 selected */
#define screen_NUMBER_OF_DATA_BITS    ((8u > 8u) ? 8u : 8u)
#define screen_NUMBER_OF_STOP_BITS    (1u)

#if (screen_RXHW_ADDRESS_ENABLED)
    #define screen_RX_ADDRESS_MODE    (0u)
    #define screen_RX_HW_ADDRESS1     (0u)
    #define screen_RX_HW_ADDRESS2     (0u)
#endif /* (screen_RXHW_ADDRESS_ENABLED) */

#define screen_INIT_RX_INTERRUPTS_MASK \
                                  (uint8)((1 << screen_RX_STS_FIFO_NOTEMPTY_SHIFT) \
                                        | (0 << screen_RX_STS_MRKSPC_SHIFT) \
                                        | (0 << screen_RX_STS_ADDR_MATCH_SHIFT) \
                                        | (0 << screen_RX_STS_PAR_ERROR_SHIFT) \
                                        | (0 << screen_RX_STS_STOP_ERROR_SHIFT) \
                                        | (0 << screen_RX_STS_BREAK_SHIFT) \
                                        | (0 << screen_RX_STS_OVERRUN_SHIFT))

#define screen_INIT_TX_INTERRUPTS_MASK \
                                  (uint8)((0 << screen_TX_STS_COMPLETE_SHIFT) \
                                        | (1 << screen_TX_STS_FIFO_EMPTY_SHIFT) \
                                        | (0 << screen_TX_STS_FIFO_FULL_SHIFT) \
                                        | (0 << screen_TX_STS_FIFO_NOT_FULL_SHIFT))


/***************************************
*              Registers
***************************************/

#ifdef screen_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define screen_CONTROL_REG \
                            (* (reg8 *) screen_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
    #define screen_CONTROL_PTR \
                            (  (reg8 *) screen_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
#endif /* End screen_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(screen_TX_ENABLED)
    #define screen_TXDATA_REG          (* (reg8 *) screen_BUART_sTX_TxShifter_u0__F0_REG)
    #define screen_TXDATA_PTR          (  (reg8 *) screen_BUART_sTX_TxShifter_u0__F0_REG)
    #define screen_TXDATA_AUX_CTL_REG  (* (reg8 *) screen_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define screen_TXDATA_AUX_CTL_PTR  (  (reg8 *) screen_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define screen_TXSTATUS_REG        (* (reg8 *) screen_BUART_sTX_TxSts__STATUS_REG)
    #define screen_TXSTATUS_PTR        (  (reg8 *) screen_BUART_sTX_TxSts__STATUS_REG)
    #define screen_TXSTATUS_MASK_REG   (* (reg8 *) screen_BUART_sTX_TxSts__MASK_REG)
    #define screen_TXSTATUS_MASK_PTR   (  (reg8 *) screen_BUART_sTX_TxSts__MASK_REG)
    #define screen_TXSTATUS_ACTL_REG   (* (reg8 *) screen_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)
    #define screen_TXSTATUS_ACTL_PTR   (  (reg8 *) screen_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)

    /* DP clock */
    #if(screen_TXCLKGEN_DP)
        #define screen_TXBITCLKGEN_CTR_REG        \
                                        (* (reg8 *) screen_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define screen_TXBITCLKGEN_CTR_PTR        \
                                        (  (reg8 *) screen_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define screen_TXBITCLKTX_COMPLETE_REG    \
                                        (* (reg8 *) screen_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
        #define screen_TXBITCLKTX_COMPLETE_PTR    \
                                        (  (reg8 *) screen_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
    #else     /* Count7 clock*/
        #define screen_TXBITCTR_PERIOD_REG    \
                                        (* (reg8 *) screen_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define screen_TXBITCTR_PERIOD_PTR    \
                                        (  (reg8 *) screen_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define screen_TXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) screen_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define screen_TXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) screen_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define screen_TXBITCTR_COUNTER_REG   \
                                        (* (reg8 *) screen_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
        #define screen_TXBITCTR_COUNTER_PTR   \
                                        (  (reg8 *) screen_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
    #endif /* screen_TXCLKGEN_DP */

#endif /* End screen_TX_ENABLED */

#if(screen_HD_ENABLED)

    #define screen_TXDATA_REG             (* (reg8 *) screen_BUART_sRX_RxShifter_u0__F1_REG )
    #define screen_TXDATA_PTR             (  (reg8 *) screen_BUART_sRX_RxShifter_u0__F1_REG )
    #define screen_TXDATA_AUX_CTL_REG     (* (reg8 *) screen_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)
    #define screen_TXDATA_AUX_CTL_PTR     (  (reg8 *) screen_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define screen_TXSTATUS_REG           (* (reg8 *) screen_BUART_sRX_RxSts__STATUS_REG )
    #define screen_TXSTATUS_PTR           (  (reg8 *) screen_BUART_sRX_RxSts__STATUS_REG )
    #define screen_TXSTATUS_MASK_REG      (* (reg8 *) screen_BUART_sRX_RxSts__MASK_REG )
    #define screen_TXSTATUS_MASK_PTR      (  (reg8 *) screen_BUART_sRX_RxSts__MASK_REG )
    #define screen_TXSTATUS_ACTL_REG      (* (reg8 *) screen_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define screen_TXSTATUS_ACTL_PTR      (  (reg8 *) screen_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End screen_HD_ENABLED */

#if( (screen_RX_ENABLED) || (screen_HD_ENABLED) )
    #define screen_RXDATA_REG             (* (reg8 *) screen_BUART_sRX_RxShifter_u0__F0_REG )
    #define screen_RXDATA_PTR             (  (reg8 *) screen_BUART_sRX_RxShifter_u0__F0_REG )
    #define screen_RXADDRESS1_REG         (* (reg8 *) screen_BUART_sRX_RxShifter_u0__D0_REG )
    #define screen_RXADDRESS1_PTR         (  (reg8 *) screen_BUART_sRX_RxShifter_u0__D0_REG )
    #define screen_RXADDRESS2_REG         (* (reg8 *) screen_BUART_sRX_RxShifter_u0__D1_REG )
    #define screen_RXADDRESS2_PTR         (  (reg8 *) screen_BUART_sRX_RxShifter_u0__D1_REG )
    #define screen_RXDATA_AUX_CTL_REG     (* (reg8 *) screen_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define screen_RXBITCTR_PERIOD_REG    (* (reg8 *) screen_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define screen_RXBITCTR_PERIOD_PTR    (  (reg8 *) screen_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define screen_RXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) screen_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define screen_RXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) screen_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define screen_RXBITCTR_COUNTER_REG   (* (reg8 *) screen_BUART_sRX_RxBitCounter__COUNT_REG )
    #define screen_RXBITCTR_COUNTER_PTR   (  (reg8 *) screen_BUART_sRX_RxBitCounter__COUNT_REG )

    #define screen_RXSTATUS_REG           (* (reg8 *) screen_BUART_sRX_RxSts__STATUS_REG )
    #define screen_RXSTATUS_PTR           (  (reg8 *) screen_BUART_sRX_RxSts__STATUS_REG )
    #define screen_RXSTATUS_MASK_REG      (* (reg8 *) screen_BUART_sRX_RxSts__MASK_REG )
    #define screen_RXSTATUS_MASK_PTR      (  (reg8 *) screen_BUART_sRX_RxSts__MASK_REG )
    #define screen_RXSTATUS_ACTL_REG      (* (reg8 *) screen_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define screen_RXSTATUS_ACTL_PTR      (  (reg8 *) screen_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End  (screen_RX_ENABLED) || (screen_HD_ENABLED) */

#if(screen_INTERNAL_CLOCK_USED)
    /* Register to enable or disable the digital clocks */
    #define screen_INTCLOCK_CLKEN_REG     (* (reg8 *) screen_IntClock__PM_ACT_CFG)
    #define screen_INTCLOCK_CLKEN_PTR     (  (reg8 *) screen_IntClock__PM_ACT_CFG)

    /* Clock mask for this clock. */
    #define screen_INTCLOCK_CLKEN_MASK    screen_IntClock__PM_ACT_MSK
#endif /* End screen_INTERNAL_CLOCK_USED */


/***************************************
*       Register Constants
***************************************/

#if(screen_TX_ENABLED)
    #define screen_TX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End screen_TX_ENABLED */

#if(screen_HD_ENABLED)
    #define screen_TX_FIFO_CLR            (0x02u) /* FIFO1 CLR */
#endif /* End screen_HD_ENABLED */

#if( (screen_RX_ENABLED) || (screen_HD_ENABLED) )
    #define screen_RX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End  (screen_RX_ENABLED) || (screen_HD_ENABLED) */


/***************************************
* The following code is DEPRECATED and
* should not be used in new projects.
***************************************/

/* UART v2_40 obsolete definitions */
#define screen_WAIT_1_MS      screen_BL_CHK_DELAY_MS   

#define screen_TXBUFFERSIZE   screen_TX_BUFFER_SIZE
#define screen_RXBUFFERSIZE   screen_RX_BUFFER_SIZE

#if (screen_RXHW_ADDRESS_ENABLED)
    #define screen_RXADDRESSMODE  screen_RX_ADDRESS_MODE
    #define screen_RXHWADDRESS1   screen_RX_HW_ADDRESS1
    #define screen_RXHWADDRESS2   screen_RX_HW_ADDRESS2
    /* Backward compatible define */
    #define screen_RXAddressMode  screen_RXADDRESSMODE
#endif /* (screen_RXHW_ADDRESS_ENABLED) */

/* UART v2_30 obsolete definitions */
#define screen_initvar                    screen_initVar

#define screen_RX_Enabled                 screen_RX_ENABLED
#define screen_TX_Enabled                 screen_TX_ENABLED
#define screen_HD_Enabled                 screen_HD_ENABLED
#define screen_RX_IntInterruptEnabled     screen_RX_INTERRUPT_ENABLED
#define screen_TX_IntInterruptEnabled     screen_TX_INTERRUPT_ENABLED
#define screen_InternalClockUsed          screen_INTERNAL_CLOCK_USED
#define screen_RXHW_Address_Enabled       screen_RXHW_ADDRESS_ENABLED
#define screen_OverSampleCount            screen_OVER_SAMPLE_COUNT
#define screen_ParityType                 screen_PARITY_TYPE

#if( screen_TX_ENABLED && (screen_TXBUFFERSIZE > screen_FIFO_LENGTH))
    #define screen_TXBUFFER               screen_txBuffer
    #define screen_TXBUFFERREAD           screen_txBufferRead
    #define screen_TXBUFFERWRITE          screen_txBufferWrite
#endif /* End screen_TX_ENABLED */
#if( ( screen_RX_ENABLED || screen_HD_ENABLED ) && \
     (screen_RXBUFFERSIZE > screen_FIFO_LENGTH) )
    #define screen_RXBUFFER               screen_rxBuffer
    #define screen_RXBUFFERREAD           screen_rxBufferRead
    #define screen_RXBUFFERWRITE          screen_rxBufferWrite
    #define screen_RXBUFFERLOOPDETECT     screen_rxBufferLoopDetect
    #define screen_RXBUFFER_OVERFLOW      screen_rxBufferOverflow
#endif /* End screen_RX_ENABLED */

#ifdef screen_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define screen_CONTROL                screen_CONTROL_REG
#endif /* End screen_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(screen_TX_ENABLED)
    #define screen_TXDATA                 screen_TXDATA_REG
    #define screen_TXSTATUS               screen_TXSTATUS_REG
    #define screen_TXSTATUS_MASK          screen_TXSTATUS_MASK_REG
    #define screen_TXSTATUS_ACTL          screen_TXSTATUS_ACTL_REG
    /* DP clock */
    #if(screen_TXCLKGEN_DP)
        #define screen_TXBITCLKGEN_CTR        screen_TXBITCLKGEN_CTR_REG
        #define screen_TXBITCLKTX_COMPLETE    screen_TXBITCLKTX_COMPLETE_REG
    #else     /* Count7 clock*/
        #define screen_TXBITCTR_PERIOD        screen_TXBITCTR_PERIOD_REG
        #define screen_TXBITCTR_CONTROL       screen_TXBITCTR_CONTROL_REG
        #define screen_TXBITCTR_COUNTER       screen_TXBITCTR_COUNTER_REG
    #endif /* screen_TXCLKGEN_DP */
#endif /* End screen_TX_ENABLED */

#if(screen_HD_ENABLED)
    #define screen_TXDATA                 screen_TXDATA_REG
    #define screen_TXSTATUS               screen_TXSTATUS_REG
    #define screen_TXSTATUS_MASK          screen_TXSTATUS_MASK_REG
    #define screen_TXSTATUS_ACTL          screen_TXSTATUS_ACTL_REG
#endif /* End screen_HD_ENABLED */

#if( (screen_RX_ENABLED) || (screen_HD_ENABLED) )
    #define screen_RXDATA                 screen_RXDATA_REG
    #define screen_RXADDRESS1             screen_RXADDRESS1_REG
    #define screen_RXADDRESS2             screen_RXADDRESS2_REG
    #define screen_RXBITCTR_PERIOD        screen_RXBITCTR_PERIOD_REG
    #define screen_RXBITCTR_CONTROL       screen_RXBITCTR_CONTROL_REG
    #define screen_RXBITCTR_COUNTER       screen_RXBITCTR_COUNTER_REG
    #define screen_RXSTATUS               screen_RXSTATUS_REG
    #define screen_RXSTATUS_MASK          screen_RXSTATUS_MASK_REG
    #define screen_RXSTATUS_ACTL          screen_RXSTATUS_ACTL_REG
#endif /* End  (screen_RX_ENABLED) || (screen_HD_ENABLED) */

#if(screen_INTERNAL_CLOCK_USED)
    #define screen_INTCLOCK_CLKEN         screen_INTCLOCK_CLKEN_REG
#endif /* End screen_INTERNAL_CLOCK_USED */

#define screen_WAIT_FOR_COMLETE_REINIT    screen_WAIT_FOR_COMPLETE_REINIT

#endif  /* CY_UART_screen_H */


/* [] END OF FILE */
