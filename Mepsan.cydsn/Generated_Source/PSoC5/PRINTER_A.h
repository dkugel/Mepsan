/*******************************************************************************
* File Name: PRINTER_A.h
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


#if !defined(CY_UART_PRINTER_A_H)
#define CY_UART_PRINTER_A_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */


/***************************************
* Conditional Compilation Parameters
***************************************/

#define PRINTER_A_RX_ENABLED                     (0u)
#define PRINTER_A_TX_ENABLED                     (1u)
#define PRINTER_A_HD_ENABLED                     (0u)
#define PRINTER_A_RX_INTERRUPT_ENABLED           (1u)
#define PRINTER_A_TX_INTERRUPT_ENABLED           (1u)
#define PRINTER_A_INTERNAL_CLOCK_USED            (1u)
#define PRINTER_A_RXHW_ADDRESS_ENABLED           (0u)
#define PRINTER_A_OVER_SAMPLE_COUNT              (8u)
#define PRINTER_A_PARITY_TYPE                    (0u)
#define PRINTER_A_PARITY_TYPE_SW                 (0u)
#define PRINTER_A_BREAK_DETECT                   (0u)
#define PRINTER_A_BREAK_BITS_TX                  (13u)
#define PRINTER_A_BREAK_BITS_RX                  (13u)
#define PRINTER_A_TXCLKGEN_DP                    (1u)
#define PRINTER_A_USE23POLLING                   (1u)
#define PRINTER_A_FLOW_CONTROL                   (0u)
#define PRINTER_A_CLK_FREQ                       (0u)
#define PRINTER_A_TX_BUFFER_SIZE                 (5u)
#define PRINTER_A_RX_BUFFER_SIZE                 (50u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component UART_v2_50 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#if defined(PRINTER_A_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG)
    #define PRINTER_A_CONTROL_REG_REMOVED            (0u)
#else
    #define PRINTER_A_CONTROL_REG_REMOVED            (1u)
#endif /* End PRINTER_A_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */


/***************************************
*      Data Structure Definition
***************************************/

/* Sleep Mode API Support */
typedef struct PRINTER_A_backupStruct_
{
    uint8 enableState;

    #if(PRINTER_A_CONTROL_REG_REMOVED == 0u)
        uint8 cr;
    #endif /* End PRINTER_A_CONTROL_REG_REMOVED */

} PRINTER_A_BACKUP_STRUCT;


/***************************************
*       Function Prototypes
***************************************/

void PRINTER_A_Start(void) ;
void PRINTER_A_Stop(void) ;
uint8 PRINTER_A_ReadControlRegister(void) ;
void PRINTER_A_WriteControlRegister(uint8 control) ;

void PRINTER_A_Init(void) ;
void PRINTER_A_Enable(void) ;
void PRINTER_A_SaveConfig(void) ;
void PRINTER_A_RestoreConfig(void) ;
void PRINTER_A_Sleep(void) ;
void PRINTER_A_Wakeup(void) ;

/* Only if RX is enabled */
#if( (PRINTER_A_RX_ENABLED) || (PRINTER_A_HD_ENABLED) )

    #if (PRINTER_A_RX_INTERRUPT_ENABLED)
        #define PRINTER_A_EnableRxInt()  CyIntEnable (PRINTER_A_RX_VECT_NUM)
        #define PRINTER_A_DisableRxInt() CyIntDisable(PRINTER_A_RX_VECT_NUM)
        CY_ISR_PROTO(PRINTER_A_RXISR);
    #endif /* PRINTER_A_RX_INTERRUPT_ENABLED */

    void PRINTER_A_SetRxAddressMode(uint8 addressMode)
                                                           ;
    void PRINTER_A_SetRxAddress1(uint8 address) ;
    void PRINTER_A_SetRxAddress2(uint8 address) ;

    void  PRINTER_A_SetRxInterruptMode(uint8 intSrc) ;
    uint8 PRINTER_A_ReadRxData(void) ;
    uint8 PRINTER_A_ReadRxStatus(void) ;
    uint8 PRINTER_A_GetChar(void) ;
    uint16 PRINTER_A_GetByte(void) ;
    uint8 PRINTER_A_GetRxBufferSize(void)
                                                            ;
    void PRINTER_A_ClearRxBuffer(void) ;

    /* Obsolete functions, defines for backward compatible */
    #define PRINTER_A_GetRxInterruptSource   PRINTER_A_ReadRxStatus

#endif /* End (PRINTER_A_RX_ENABLED) || (PRINTER_A_HD_ENABLED) */

/* Only if TX is enabled */
#if(PRINTER_A_TX_ENABLED || PRINTER_A_HD_ENABLED)

    #if(PRINTER_A_TX_INTERRUPT_ENABLED)
        #define PRINTER_A_EnableTxInt()  CyIntEnable (PRINTER_A_TX_VECT_NUM)
        #define PRINTER_A_DisableTxInt() CyIntDisable(PRINTER_A_TX_VECT_NUM)
        #define PRINTER_A_SetPendingTxInt() CyIntSetPending(PRINTER_A_TX_VECT_NUM)
        #define PRINTER_A_ClearPendingTxInt() CyIntClearPending(PRINTER_A_TX_VECT_NUM)
        CY_ISR_PROTO(PRINTER_A_TXISR);
    #endif /* PRINTER_A_TX_INTERRUPT_ENABLED */

    void PRINTER_A_SetTxInterruptMode(uint8 intSrc) ;
    void PRINTER_A_WriteTxData(uint8 txDataByte) ;
    uint8 PRINTER_A_ReadTxStatus(void) ;
    void PRINTER_A_PutChar(uint8 txDataByte) ;
    void PRINTER_A_PutString(const char8 string[]) ;
    void PRINTER_A_PutArray(const uint8 string[], uint8 byteCount)
                                                            ;
    void PRINTER_A_PutCRLF(uint8 txDataByte) ;
    void PRINTER_A_ClearTxBuffer(void) ;
    void PRINTER_A_SetTxAddressMode(uint8 addressMode) ;
    void PRINTER_A_SendBreak(uint8 retMode) ;
    uint8 PRINTER_A_GetTxBufferSize(void)
                                                            ;
    /* Obsolete functions, defines for backward compatible */
    #define PRINTER_A_PutStringConst         PRINTER_A_PutString
    #define PRINTER_A_PutArrayConst          PRINTER_A_PutArray
    #define PRINTER_A_GetTxInterruptSource   PRINTER_A_ReadTxStatus

#endif /* End PRINTER_A_TX_ENABLED || PRINTER_A_HD_ENABLED */

#if(PRINTER_A_HD_ENABLED)
    void PRINTER_A_LoadRxConfig(void) ;
    void PRINTER_A_LoadTxConfig(void) ;
#endif /* End PRINTER_A_HD_ENABLED */


/* Communication bootloader APIs */
#if defined(CYDEV_BOOTLOADER_IO_COMP) && ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_PRINTER_A) || \
                                          (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))
    /* Physical layer functions */
    void    PRINTER_A_CyBtldrCommStart(void) CYSMALL ;
    void    PRINTER_A_CyBtldrCommStop(void) CYSMALL ;
    void    PRINTER_A_CyBtldrCommReset(void) CYSMALL ;
    cystatus PRINTER_A_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;
    cystatus PRINTER_A_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;

    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_PRINTER_A)
        #define CyBtldrCommStart    PRINTER_A_CyBtldrCommStart
        #define CyBtldrCommStop     PRINTER_A_CyBtldrCommStop
        #define CyBtldrCommReset    PRINTER_A_CyBtldrCommReset
        #define CyBtldrCommWrite    PRINTER_A_CyBtldrCommWrite
        #define CyBtldrCommRead     PRINTER_A_CyBtldrCommRead
    #endif  /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_PRINTER_A) */

    /* Byte to Byte time out for detecting end of block data from host */
    #define PRINTER_A_BYTE2BYTE_TIME_OUT (25u)
    #define PRINTER_A_PACKET_EOP         (0x17u) /* End of packet defined by bootloader */
    #define PRINTER_A_WAIT_EOP_DELAY     (5u)    /* Additional 5ms to wait for End of packet */
    #define PRINTER_A_BL_CHK_DELAY_MS    (1u)    /* Time Out quantity equal 1mS */

#endif /* CYDEV_BOOTLOADER_IO_COMP */


/***************************************
*          API Constants
***************************************/
/* Parameters for SetTxAddressMode API*/
#define PRINTER_A_SET_SPACE      (0x00u)
#define PRINTER_A_SET_MARK       (0x01u)

/* Status Register definitions */
#if( (PRINTER_A_TX_ENABLED) || (PRINTER_A_HD_ENABLED) )
    #if(PRINTER_A_TX_INTERRUPT_ENABLED)
        #define PRINTER_A_TX_VECT_NUM            (uint8)PRINTER_A_TXInternalInterrupt__INTC_NUMBER
        #define PRINTER_A_TX_PRIOR_NUM           (uint8)PRINTER_A_TXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* PRINTER_A_TX_INTERRUPT_ENABLED */

    #define PRINTER_A_TX_STS_COMPLETE_SHIFT          (0x00u)
    #define PRINTER_A_TX_STS_FIFO_EMPTY_SHIFT        (0x01u)
    #define PRINTER_A_TX_STS_FIFO_NOT_FULL_SHIFT     (0x03u)
    #if(PRINTER_A_TX_ENABLED)
        #define PRINTER_A_TX_STS_FIFO_FULL_SHIFT     (0x02u)
    #else /* (PRINTER_A_HD_ENABLED) */
        #define PRINTER_A_TX_STS_FIFO_FULL_SHIFT     (0x05u)  /* Needs MD=0 */
    #endif /* (PRINTER_A_TX_ENABLED) */

    #define PRINTER_A_TX_STS_COMPLETE            (uint8)(0x01u << PRINTER_A_TX_STS_COMPLETE_SHIFT)
    #define PRINTER_A_TX_STS_FIFO_EMPTY          (uint8)(0x01u << PRINTER_A_TX_STS_FIFO_EMPTY_SHIFT)
    #define PRINTER_A_TX_STS_FIFO_FULL           (uint8)(0x01u << PRINTER_A_TX_STS_FIFO_FULL_SHIFT)
    #define PRINTER_A_TX_STS_FIFO_NOT_FULL       (uint8)(0x01u << PRINTER_A_TX_STS_FIFO_NOT_FULL_SHIFT)
#endif /* End (PRINTER_A_TX_ENABLED) || (PRINTER_A_HD_ENABLED)*/

#if( (PRINTER_A_RX_ENABLED) || (PRINTER_A_HD_ENABLED) )
    #if(PRINTER_A_RX_INTERRUPT_ENABLED)
        #define PRINTER_A_RX_VECT_NUM            (uint8)PRINTER_A_RXInternalInterrupt__INTC_NUMBER
        #define PRINTER_A_RX_PRIOR_NUM           (uint8)PRINTER_A_RXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* PRINTER_A_RX_INTERRUPT_ENABLED */
    #define PRINTER_A_RX_STS_MRKSPC_SHIFT            (0x00u)
    #define PRINTER_A_RX_STS_BREAK_SHIFT             (0x01u)
    #define PRINTER_A_RX_STS_PAR_ERROR_SHIFT         (0x02u)
    #define PRINTER_A_RX_STS_STOP_ERROR_SHIFT        (0x03u)
    #define PRINTER_A_RX_STS_OVERRUN_SHIFT           (0x04u)
    #define PRINTER_A_RX_STS_FIFO_NOTEMPTY_SHIFT     (0x05u)
    #define PRINTER_A_RX_STS_ADDR_MATCH_SHIFT        (0x06u)
    #define PRINTER_A_RX_STS_SOFT_BUFF_OVER_SHIFT    (0x07u)

    #define PRINTER_A_RX_STS_MRKSPC           (uint8)(0x01u << PRINTER_A_RX_STS_MRKSPC_SHIFT)
    #define PRINTER_A_RX_STS_BREAK            (uint8)(0x01u << PRINTER_A_RX_STS_BREAK_SHIFT)
    #define PRINTER_A_RX_STS_PAR_ERROR        (uint8)(0x01u << PRINTER_A_RX_STS_PAR_ERROR_SHIFT)
    #define PRINTER_A_RX_STS_STOP_ERROR       (uint8)(0x01u << PRINTER_A_RX_STS_STOP_ERROR_SHIFT)
    #define PRINTER_A_RX_STS_OVERRUN          (uint8)(0x01u << PRINTER_A_RX_STS_OVERRUN_SHIFT)
    #define PRINTER_A_RX_STS_FIFO_NOTEMPTY    (uint8)(0x01u << PRINTER_A_RX_STS_FIFO_NOTEMPTY_SHIFT)
    #define PRINTER_A_RX_STS_ADDR_MATCH       (uint8)(0x01u << PRINTER_A_RX_STS_ADDR_MATCH_SHIFT)
    #define PRINTER_A_RX_STS_SOFT_BUFF_OVER   (uint8)(0x01u << PRINTER_A_RX_STS_SOFT_BUFF_OVER_SHIFT)
    #define PRINTER_A_RX_HW_MASK                     (0x7Fu)
#endif /* End (PRINTER_A_RX_ENABLED) || (PRINTER_A_HD_ENABLED) */

/* Control Register definitions */
#define PRINTER_A_CTRL_HD_SEND_SHIFT                 (0x00u) /* 1 enable TX part in Half Duplex mode */
#define PRINTER_A_CTRL_HD_SEND_BREAK_SHIFT           (0x01u) /* 1 send BREAK signal in Half Duplez mode */
#define PRINTER_A_CTRL_MARK_SHIFT                    (0x02u) /* 1 sets mark, 0 sets space */
#define PRINTER_A_CTRL_PARITY_TYPE0_SHIFT            (0x03u) /* Defines the type of parity implemented */
#define PRINTER_A_CTRL_PARITY_TYPE1_SHIFT            (0x04u) /* Defines the type of parity implemented */
#define PRINTER_A_CTRL_RXADDR_MODE0_SHIFT            (0x05u)
#define PRINTER_A_CTRL_RXADDR_MODE1_SHIFT            (0x06u)
#define PRINTER_A_CTRL_RXADDR_MODE2_SHIFT            (0x07u)

#define PRINTER_A_CTRL_HD_SEND               (uint8)(0x01u << PRINTER_A_CTRL_HD_SEND_SHIFT)
#define PRINTER_A_CTRL_HD_SEND_BREAK         (uint8)(0x01u << PRINTER_A_CTRL_HD_SEND_BREAK_SHIFT)
#define PRINTER_A_CTRL_MARK                  (uint8)(0x01u << PRINTER_A_CTRL_MARK_SHIFT)
#define PRINTER_A_CTRL_PARITY_TYPE_MASK      (uint8)(0x03u << PRINTER_A_CTRL_PARITY_TYPE0_SHIFT)
#define PRINTER_A_CTRL_RXADDR_MODE_MASK      (uint8)(0x07u << PRINTER_A_CTRL_RXADDR_MODE0_SHIFT)

/* StatusI Register Interrupt Enable Control Bits. As defined by the Register map for the AUX Control Register */
#define PRINTER_A_INT_ENABLE                         (0x10u)

/* Bit Counter (7-bit) Control Register Bit Definitions. As defined by the Register map for the AUX Control Register */
#define PRINTER_A_CNTR_ENABLE                        (0x20u)

/*   Constants for SendBreak() "retMode" parameter  */
#define PRINTER_A_SEND_BREAK                         (0x00u)
#define PRINTER_A_WAIT_FOR_COMPLETE_REINIT           (0x01u)
#define PRINTER_A_REINIT                             (0x02u)
#define PRINTER_A_SEND_WAIT_REINIT                   (0x03u)

#define PRINTER_A_OVER_SAMPLE_8                      (8u)
#define PRINTER_A_OVER_SAMPLE_16                     (16u)

#define PRINTER_A_BIT_CENTER                         (PRINTER_A_OVER_SAMPLE_COUNT - 2u)

#define PRINTER_A_FIFO_LENGTH                        (4u)
#define PRINTER_A_NUMBER_OF_START_BIT                (1u)
#define PRINTER_A_MAX_BYTE_VALUE                     (0xFFu)

/* 8X always for count7 implementation */
#define PRINTER_A_TXBITCTR_BREAKBITS8X   ((PRINTER_A_BREAK_BITS_TX * PRINTER_A_OVER_SAMPLE_8) - 1u)
/* 8X or 16X for DP implementation */
#define PRINTER_A_TXBITCTR_BREAKBITS ((PRINTER_A_BREAK_BITS_TX * PRINTER_A_OVER_SAMPLE_COUNT) - 1u)

#define PRINTER_A_HALF_BIT_COUNT   \
                            (((PRINTER_A_OVER_SAMPLE_COUNT / 2u) + (PRINTER_A_USE23POLLING * 1u)) - 2u)
#if (PRINTER_A_OVER_SAMPLE_COUNT == PRINTER_A_OVER_SAMPLE_8)
    #define PRINTER_A_HD_TXBITCTR_INIT   (((PRINTER_A_BREAK_BITS_TX + \
                            PRINTER_A_NUMBER_OF_START_BIT) * PRINTER_A_OVER_SAMPLE_COUNT) - 1u)

    /* This parameter is increased on the 2 in 2 out of 3 mode to sample voting in the middle */
    #define PRINTER_A_RXBITCTR_INIT  ((((PRINTER_A_BREAK_BITS_RX + PRINTER_A_NUMBER_OF_START_BIT) \
                            * PRINTER_A_OVER_SAMPLE_COUNT) + PRINTER_A_HALF_BIT_COUNT) - 1u)

#else /* PRINTER_A_OVER_SAMPLE_COUNT == PRINTER_A_OVER_SAMPLE_16 */
    #define PRINTER_A_HD_TXBITCTR_INIT   ((8u * PRINTER_A_OVER_SAMPLE_COUNT) - 1u)
    /* 7bit counter need one more bit for OverSampleCount = 16 */
    #define PRINTER_A_RXBITCTR_INIT      (((7u * PRINTER_A_OVER_SAMPLE_COUNT) - 1u) + \
                                                      PRINTER_A_HALF_BIT_COUNT)
#endif /* End PRINTER_A_OVER_SAMPLE_COUNT */

#define PRINTER_A_HD_RXBITCTR_INIT                   PRINTER_A_RXBITCTR_INIT


/***************************************
* Global variables external identifier
***************************************/

extern uint8 PRINTER_A_initVar;
#if (PRINTER_A_TX_INTERRUPT_ENABLED && PRINTER_A_TX_ENABLED)
    extern volatile uint8 PRINTER_A_txBuffer[PRINTER_A_TX_BUFFER_SIZE];
    extern volatile uint8 PRINTER_A_txBufferRead;
    extern uint8 PRINTER_A_txBufferWrite;
#endif /* (PRINTER_A_TX_INTERRUPT_ENABLED && PRINTER_A_TX_ENABLED) */
#if (PRINTER_A_RX_INTERRUPT_ENABLED && (PRINTER_A_RX_ENABLED || PRINTER_A_HD_ENABLED))
    extern uint8 PRINTER_A_errorStatus;
    extern volatile uint8 PRINTER_A_rxBuffer[PRINTER_A_RX_BUFFER_SIZE];
    extern volatile uint8 PRINTER_A_rxBufferRead;
    extern volatile uint8 PRINTER_A_rxBufferWrite;
    extern volatile uint8 PRINTER_A_rxBufferLoopDetect;
    extern volatile uint8 PRINTER_A_rxBufferOverflow;
    #if (PRINTER_A_RXHW_ADDRESS_ENABLED)
        extern volatile uint8 PRINTER_A_rxAddressMode;
        extern volatile uint8 PRINTER_A_rxAddressDetected;
    #endif /* (PRINTER_A_RXHW_ADDRESS_ENABLED) */
#endif /* (PRINTER_A_RX_INTERRUPT_ENABLED && (PRINTER_A_RX_ENABLED || PRINTER_A_HD_ENABLED)) */


/***************************************
* Enumerated Types and Parameters
***************************************/

#define PRINTER_A__B_UART__AM_SW_BYTE_BYTE 1
#define PRINTER_A__B_UART__AM_SW_DETECT_TO_BUFFER 2
#define PRINTER_A__B_UART__AM_HW_BYTE_BY_BYTE 3
#define PRINTER_A__B_UART__AM_HW_DETECT_TO_BUFFER 4
#define PRINTER_A__B_UART__AM_NONE 0

#define PRINTER_A__B_UART__NONE_REVB 0
#define PRINTER_A__B_UART__EVEN_REVB 1
#define PRINTER_A__B_UART__ODD_REVB 2
#define PRINTER_A__B_UART__MARK_SPACE_REVB 3



/***************************************
*    Initial Parameter Constants
***************************************/

/* UART shifts max 8 bits, Mark/Space functionality working if 9 selected */
#define PRINTER_A_NUMBER_OF_DATA_BITS    ((8u > 8u) ? 8u : 8u)
#define PRINTER_A_NUMBER_OF_STOP_BITS    (1u)

#if (PRINTER_A_RXHW_ADDRESS_ENABLED)
    #define PRINTER_A_RX_ADDRESS_MODE    (0u)
    #define PRINTER_A_RX_HW_ADDRESS1     (0u)
    #define PRINTER_A_RX_HW_ADDRESS2     (0u)
#endif /* (PRINTER_A_RXHW_ADDRESS_ENABLED) */

#define PRINTER_A_INIT_RX_INTERRUPTS_MASK \
                                  (uint8)((0 << PRINTER_A_RX_STS_FIFO_NOTEMPTY_SHIFT) \
                                        | (0 << PRINTER_A_RX_STS_MRKSPC_SHIFT) \
                                        | (0 << PRINTER_A_RX_STS_ADDR_MATCH_SHIFT) \
                                        | (0 << PRINTER_A_RX_STS_PAR_ERROR_SHIFT) \
                                        | (0 << PRINTER_A_RX_STS_STOP_ERROR_SHIFT) \
                                        | (0 << PRINTER_A_RX_STS_BREAK_SHIFT) \
                                        | (0 << PRINTER_A_RX_STS_OVERRUN_SHIFT))

#define PRINTER_A_INIT_TX_INTERRUPTS_MASK \
                                  (uint8)((0 << PRINTER_A_TX_STS_COMPLETE_SHIFT) \
                                        | (1 << PRINTER_A_TX_STS_FIFO_EMPTY_SHIFT) \
                                        | (0 << PRINTER_A_TX_STS_FIFO_FULL_SHIFT) \
                                        | (0 << PRINTER_A_TX_STS_FIFO_NOT_FULL_SHIFT))


/***************************************
*              Registers
***************************************/

#ifdef PRINTER_A_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define PRINTER_A_CONTROL_REG \
                            (* (reg8 *) PRINTER_A_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
    #define PRINTER_A_CONTROL_PTR \
                            (  (reg8 *) PRINTER_A_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
#endif /* End PRINTER_A_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(PRINTER_A_TX_ENABLED)
    #define PRINTER_A_TXDATA_REG          (* (reg8 *) PRINTER_A_BUART_sTX_TxShifter_u0__F0_REG)
    #define PRINTER_A_TXDATA_PTR          (  (reg8 *) PRINTER_A_BUART_sTX_TxShifter_u0__F0_REG)
    #define PRINTER_A_TXDATA_AUX_CTL_REG  (* (reg8 *) PRINTER_A_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define PRINTER_A_TXDATA_AUX_CTL_PTR  (  (reg8 *) PRINTER_A_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define PRINTER_A_TXSTATUS_REG        (* (reg8 *) PRINTER_A_BUART_sTX_TxSts__STATUS_REG)
    #define PRINTER_A_TXSTATUS_PTR        (  (reg8 *) PRINTER_A_BUART_sTX_TxSts__STATUS_REG)
    #define PRINTER_A_TXSTATUS_MASK_REG   (* (reg8 *) PRINTER_A_BUART_sTX_TxSts__MASK_REG)
    #define PRINTER_A_TXSTATUS_MASK_PTR   (  (reg8 *) PRINTER_A_BUART_sTX_TxSts__MASK_REG)
    #define PRINTER_A_TXSTATUS_ACTL_REG   (* (reg8 *) PRINTER_A_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)
    #define PRINTER_A_TXSTATUS_ACTL_PTR   (  (reg8 *) PRINTER_A_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)

    /* DP clock */
    #if(PRINTER_A_TXCLKGEN_DP)
        #define PRINTER_A_TXBITCLKGEN_CTR_REG        \
                                        (* (reg8 *) PRINTER_A_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define PRINTER_A_TXBITCLKGEN_CTR_PTR        \
                                        (  (reg8 *) PRINTER_A_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define PRINTER_A_TXBITCLKTX_COMPLETE_REG    \
                                        (* (reg8 *) PRINTER_A_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
        #define PRINTER_A_TXBITCLKTX_COMPLETE_PTR    \
                                        (  (reg8 *) PRINTER_A_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
    #else     /* Count7 clock*/
        #define PRINTER_A_TXBITCTR_PERIOD_REG    \
                                        (* (reg8 *) PRINTER_A_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define PRINTER_A_TXBITCTR_PERIOD_PTR    \
                                        (  (reg8 *) PRINTER_A_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define PRINTER_A_TXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) PRINTER_A_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define PRINTER_A_TXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) PRINTER_A_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define PRINTER_A_TXBITCTR_COUNTER_REG   \
                                        (* (reg8 *) PRINTER_A_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
        #define PRINTER_A_TXBITCTR_COUNTER_PTR   \
                                        (  (reg8 *) PRINTER_A_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
    #endif /* PRINTER_A_TXCLKGEN_DP */

#endif /* End PRINTER_A_TX_ENABLED */

#if(PRINTER_A_HD_ENABLED)

    #define PRINTER_A_TXDATA_REG             (* (reg8 *) PRINTER_A_BUART_sRX_RxShifter_u0__F1_REG )
    #define PRINTER_A_TXDATA_PTR             (  (reg8 *) PRINTER_A_BUART_sRX_RxShifter_u0__F1_REG )
    #define PRINTER_A_TXDATA_AUX_CTL_REG     (* (reg8 *) PRINTER_A_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)
    #define PRINTER_A_TXDATA_AUX_CTL_PTR     (  (reg8 *) PRINTER_A_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define PRINTER_A_TXSTATUS_REG           (* (reg8 *) PRINTER_A_BUART_sRX_RxSts__STATUS_REG )
    #define PRINTER_A_TXSTATUS_PTR           (  (reg8 *) PRINTER_A_BUART_sRX_RxSts__STATUS_REG )
    #define PRINTER_A_TXSTATUS_MASK_REG      (* (reg8 *) PRINTER_A_BUART_sRX_RxSts__MASK_REG )
    #define PRINTER_A_TXSTATUS_MASK_PTR      (  (reg8 *) PRINTER_A_BUART_sRX_RxSts__MASK_REG )
    #define PRINTER_A_TXSTATUS_ACTL_REG      (* (reg8 *) PRINTER_A_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define PRINTER_A_TXSTATUS_ACTL_PTR      (  (reg8 *) PRINTER_A_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End PRINTER_A_HD_ENABLED */

#if( (PRINTER_A_RX_ENABLED) || (PRINTER_A_HD_ENABLED) )
    #define PRINTER_A_RXDATA_REG             (* (reg8 *) PRINTER_A_BUART_sRX_RxShifter_u0__F0_REG )
    #define PRINTER_A_RXDATA_PTR             (  (reg8 *) PRINTER_A_BUART_sRX_RxShifter_u0__F0_REG )
    #define PRINTER_A_RXADDRESS1_REG         (* (reg8 *) PRINTER_A_BUART_sRX_RxShifter_u0__D0_REG )
    #define PRINTER_A_RXADDRESS1_PTR         (  (reg8 *) PRINTER_A_BUART_sRX_RxShifter_u0__D0_REG )
    #define PRINTER_A_RXADDRESS2_REG         (* (reg8 *) PRINTER_A_BUART_sRX_RxShifter_u0__D1_REG )
    #define PRINTER_A_RXADDRESS2_PTR         (  (reg8 *) PRINTER_A_BUART_sRX_RxShifter_u0__D1_REG )
    #define PRINTER_A_RXDATA_AUX_CTL_REG     (* (reg8 *) PRINTER_A_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define PRINTER_A_RXBITCTR_PERIOD_REG    (* (reg8 *) PRINTER_A_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define PRINTER_A_RXBITCTR_PERIOD_PTR    (  (reg8 *) PRINTER_A_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define PRINTER_A_RXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) PRINTER_A_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define PRINTER_A_RXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) PRINTER_A_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define PRINTER_A_RXBITCTR_COUNTER_REG   (* (reg8 *) PRINTER_A_BUART_sRX_RxBitCounter__COUNT_REG )
    #define PRINTER_A_RXBITCTR_COUNTER_PTR   (  (reg8 *) PRINTER_A_BUART_sRX_RxBitCounter__COUNT_REG )

    #define PRINTER_A_RXSTATUS_REG           (* (reg8 *) PRINTER_A_BUART_sRX_RxSts__STATUS_REG )
    #define PRINTER_A_RXSTATUS_PTR           (  (reg8 *) PRINTER_A_BUART_sRX_RxSts__STATUS_REG )
    #define PRINTER_A_RXSTATUS_MASK_REG      (* (reg8 *) PRINTER_A_BUART_sRX_RxSts__MASK_REG )
    #define PRINTER_A_RXSTATUS_MASK_PTR      (  (reg8 *) PRINTER_A_BUART_sRX_RxSts__MASK_REG )
    #define PRINTER_A_RXSTATUS_ACTL_REG      (* (reg8 *) PRINTER_A_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define PRINTER_A_RXSTATUS_ACTL_PTR      (  (reg8 *) PRINTER_A_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End  (PRINTER_A_RX_ENABLED) || (PRINTER_A_HD_ENABLED) */

#if(PRINTER_A_INTERNAL_CLOCK_USED)
    /* Register to enable or disable the digital clocks */
    #define PRINTER_A_INTCLOCK_CLKEN_REG     (* (reg8 *) PRINTER_A_IntClock__PM_ACT_CFG)
    #define PRINTER_A_INTCLOCK_CLKEN_PTR     (  (reg8 *) PRINTER_A_IntClock__PM_ACT_CFG)

    /* Clock mask for this clock. */
    #define PRINTER_A_INTCLOCK_CLKEN_MASK    PRINTER_A_IntClock__PM_ACT_MSK
#endif /* End PRINTER_A_INTERNAL_CLOCK_USED */


/***************************************
*       Register Constants
***************************************/

#if(PRINTER_A_TX_ENABLED)
    #define PRINTER_A_TX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End PRINTER_A_TX_ENABLED */

#if(PRINTER_A_HD_ENABLED)
    #define PRINTER_A_TX_FIFO_CLR            (0x02u) /* FIFO1 CLR */
#endif /* End PRINTER_A_HD_ENABLED */

#if( (PRINTER_A_RX_ENABLED) || (PRINTER_A_HD_ENABLED) )
    #define PRINTER_A_RX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End  (PRINTER_A_RX_ENABLED) || (PRINTER_A_HD_ENABLED) */


/***************************************
* The following code is DEPRECATED and
* should not be used in new projects.
***************************************/

/* UART v2_40 obsolete definitions */
#define PRINTER_A_WAIT_1_MS      PRINTER_A_BL_CHK_DELAY_MS   

#define PRINTER_A_TXBUFFERSIZE   PRINTER_A_TX_BUFFER_SIZE
#define PRINTER_A_RXBUFFERSIZE   PRINTER_A_RX_BUFFER_SIZE

#if (PRINTER_A_RXHW_ADDRESS_ENABLED)
    #define PRINTER_A_RXADDRESSMODE  PRINTER_A_RX_ADDRESS_MODE
    #define PRINTER_A_RXHWADDRESS1   PRINTER_A_RX_HW_ADDRESS1
    #define PRINTER_A_RXHWADDRESS2   PRINTER_A_RX_HW_ADDRESS2
    /* Backward compatible define */
    #define PRINTER_A_RXAddressMode  PRINTER_A_RXADDRESSMODE
#endif /* (PRINTER_A_RXHW_ADDRESS_ENABLED) */

/* UART v2_30 obsolete definitions */
#define PRINTER_A_initvar                    PRINTER_A_initVar

#define PRINTER_A_RX_Enabled                 PRINTER_A_RX_ENABLED
#define PRINTER_A_TX_Enabled                 PRINTER_A_TX_ENABLED
#define PRINTER_A_HD_Enabled                 PRINTER_A_HD_ENABLED
#define PRINTER_A_RX_IntInterruptEnabled     PRINTER_A_RX_INTERRUPT_ENABLED
#define PRINTER_A_TX_IntInterruptEnabled     PRINTER_A_TX_INTERRUPT_ENABLED
#define PRINTER_A_InternalClockUsed          PRINTER_A_INTERNAL_CLOCK_USED
#define PRINTER_A_RXHW_Address_Enabled       PRINTER_A_RXHW_ADDRESS_ENABLED
#define PRINTER_A_OverSampleCount            PRINTER_A_OVER_SAMPLE_COUNT
#define PRINTER_A_ParityType                 PRINTER_A_PARITY_TYPE

#if( PRINTER_A_TX_ENABLED && (PRINTER_A_TXBUFFERSIZE > PRINTER_A_FIFO_LENGTH))
    #define PRINTER_A_TXBUFFER               PRINTER_A_txBuffer
    #define PRINTER_A_TXBUFFERREAD           PRINTER_A_txBufferRead
    #define PRINTER_A_TXBUFFERWRITE          PRINTER_A_txBufferWrite
#endif /* End PRINTER_A_TX_ENABLED */
#if( ( PRINTER_A_RX_ENABLED || PRINTER_A_HD_ENABLED ) && \
     (PRINTER_A_RXBUFFERSIZE > PRINTER_A_FIFO_LENGTH) )
    #define PRINTER_A_RXBUFFER               PRINTER_A_rxBuffer
    #define PRINTER_A_RXBUFFERREAD           PRINTER_A_rxBufferRead
    #define PRINTER_A_RXBUFFERWRITE          PRINTER_A_rxBufferWrite
    #define PRINTER_A_RXBUFFERLOOPDETECT     PRINTER_A_rxBufferLoopDetect
    #define PRINTER_A_RXBUFFER_OVERFLOW      PRINTER_A_rxBufferOverflow
#endif /* End PRINTER_A_RX_ENABLED */

#ifdef PRINTER_A_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define PRINTER_A_CONTROL                PRINTER_A_CONTROL_REG
#endif /* End PRINTER_A_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(PRINTER_A_TX_ENABLED)
    #define PRINTER_A_TXDATA                 PRINTER_A_TXDATA_REG
    #define PRINTER_A_TXSTATUS               PRINTER_A_TXSTATUS_REG
    #define PRINTER_A_TXSTATUS_MASK          PRINTER_A_TXSTATUS_MASK_REG
    #define PRINTER_A_TXSTATUS_ACTL          PRINTER_A_TXSTATUS_ACTL_REG
    /* DP clock */
    #if(PRINTER_A_TXCLKGEN_DP)
        #define PRINTER_A_TXBITCLKGEN_CTR        PRINTER_A_TXBITCLKGEN_CTR_REG
        #define PRINTER_A_TXBITCLKTX_COMPLETE    PRINTER_A_TXBITCLKTX_COMPLETE_REG
    #else     /* Count7 clock*/
        #define PRINTER_A_TXBITCTR_PERIOD        PRINTER_A_TXBITCTR_PERIOD_REG
        #define PRINTER_A_TXBITCTR_CONTROL       PRINTER_A_TXBITCTR_CONTROL_REG
        #define PRINTER_A_TXBITCTR_COUNTER       PRINTER_A_TXBITCTR_COUNTER_REG
    #endif /* PRINTER_A_TXCLKGEN_DP */
#endif /* End PRINTER_A_TX_ENABLED */

#if(PRINTER_A_HD_ENABLED)
    #define PRINTER_A_TXDATA                 PRINTER_A_TXDATA_REG
    #define PRINTER_A_TXSTATUS               PRINTER_A_TXSTATUS_REG
    #define PRINTER_A_TXSTATUS_MASK          PRINTER_A_TXSTATUS_MASK_REG
    #define PRINTER_A_TXSTATUS_ACTL          PRINTER_A_TXSTATUS_ACTL_REG
#endif /* End PRINTER_A_HD_ENABLED */

#if( (PRINTER_A_RX_ENABLED) || (PRINTER_A_HD_ENABLED) )
    #define PRINTER_A_RXDATA                 PRINTER_A_RXDATA_REG
    #define PRINTER_A_RXADDRESS1             PRINTER_A_RXADDRESS1_REG
    #define PRINTER_A_RXADDRESS2             PRINTER_A_RXADDRESS2_REG
    #define PRINTER_A_RXBITCTR_PERIOD        PRINTER_A_RXBITCTR_PERIOD_REG
    #define PRINTER_A_RXBITCTR_CONTROL       PRINTER_A_RXBITCTR_CONTROL_REG
    #define PRINTER_A_RXBITCTR_COUNTER       PRINTER_A_RXBITCTR_COUNTER_REG
    #define PRINTER_A_RXSTATUS               PRINTER_A_RXSTATUS_REG
    #define PRINTER_A_RXSTATUS_MASK          PRINTER_A_RXSTATUS_MASK_REG
    #define PRINTER_A_RXSTATUS_ACTL          PRINTER_A_RXSTATUS_ACTL_REG
#endif /* End  (PRINTER_A_RX_ENABLED) || (PRINTER_A_HD_ENABLED) */

#if(PRINTER_A_INTERNAL_CLOCK_USED)
    #define PRINTER_A_INTCLOCK_CLKEN         PRINTER_A_INTCLOCK_CLKEN_REG
#endif /* End PRINTER_A_INTERNAL_CLOCK_USED */

#define PRINTER_A_WAIT_FOR_COMLETE_REINIT    PRINTER_A_WAIT_FOR_COMPLETE_REINIT

#endif  /* CY_UART_PRINTER_A_H */


/* [] END OF FILE */
