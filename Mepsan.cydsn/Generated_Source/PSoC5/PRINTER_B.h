/*******************************************************************************
* File Name: PRINTER_B.h
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


#if !defined(CY_UART_PRINTER_B_H)
#define CY_UART_PRINTER_B_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */


/***************************************
* Conditional Compilation Parameters
***************************************/

#define PRINTER_B_RX_ENABLED                     (0u)
#define PRINTER_B_TX_ENABLED                     (1u)
#define PRINTER_B_HD_ENABLED                     (0u)
#define PRINTER_B_RX_INTERRUPT_ENABLED           (1u)
#define PRINTER_B_TX_INTERRUPT_ENABLED           (1u)
#define PRINTER_B_INTERNAL_CLOCK_USED            (1u)
#define PRINTER_B_RXHW_ADDRESS_ENABLED           (0u)
#define PRINTER_B_OVER_SAMPLE_COUNT              (8u)
#define PRINTER_B_PARITY_TYPE                    (0u)
#define PRINTER_B_PARITY_TYPE_SW                 (0u)
#define PRINTER_B_BREAK_DETECT                   (0u)
#define PRINTER_B_BREAK_BITS_TX                  (13u)
#define PRINTER_B_BREAK_BITS_RX                  (13u)
#define PRINTER_B_TXCLKGEN_DP                    (1u)
#define PRINTER_B_USE23POLLING                   (1u)
#define PRINTER_B_FLOW_CONTROL                   (0u)
#define PRINTER_B_CLK_FREQ                       (0u)
#define PRINTER_B_TX_BUFFER_SIZE                 (5u)
#define PRINTER_B_RX_BUFFER_SIZE                 (50u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component UART_v2_50 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#if defined(PRINTER_B_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG)
    #define PRINTER_B_CONTROL_REG_REMOVED            (0u)
#else
    #define PRINTER_B_CONTROL_REG_REMOVED            (1u)
#endif /* End PRINTER_B_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */


/***************************************
*      Data Structure Definition
***************************************/

/* Sleep Mode API Support */
typedef struct PRINTER_B_backupStruct_
{
    uint8 enableState;

    #if(PRINTER_B_CONTROL_REG_REMOVED == 0u)
        uint8 cr;
    #endif /* End PRINTER_B_CONTROL_REG_REMOVED */

} PRINTER_B_BACKUP_STRUCT;


/***************************************
*       Function Prototypes
***************************************/

void PRINTER_B_Start(void) ;
void PRINTER_B_Stop(void) ;
uint8 PRINTER_B_ReadControlRegister(void) ;
void PRINTER_B_WriteControlRegister(uint8 control) ;

void PRINTER_B_Init(void) ;
void PRINTER_B_Enable(void) ;
void PRINTER_B_SaveConfig(void) ;
void PRINTER_B_RestoreConfig(void) ;
void PRINTER_B_Sleep(void) ;
void PRINTER_B_Wakeup(void) ;

/* Only if RX is enabled */
#if( (PRINTER_B_RX_ENABLED) || (PRINTER_B_HD_ENABLED) )

    #if (PRINTER_B_RX_INTERRUPT_ENABLED)
        #define PRINTER_B_EnableRxInt()  CyIntEnable (PRINTER_B_RX_VECT_NUM)
        #define PRINTER_B_DisableRxInt() CyIntDisable(PRINTER_B_RX_VECT_NUM)
        CY_ISR_PROTO(PRINTER_B_RXISR);
    #endif /* PRINTER_B_RX_INTERRUPT_ENABLED */

    void PRINTER_B_SetRxAddressMode(uint8 addressMode)
                                                           ;
    void PRINTER_B_SetRxAddress1(uint8 address) ;
    void PRINTER_B_SetRxAddress2(uint8 address) ;

    void  PRINTER_B_SetRxInterruptMode(uint8 intSrc) ;
    uint8 PRINTER_B_ReadRxData(void) ;
    uint8 PRINTER_B_ReadRxStatus(void) ;
    uint8 PRINTER_B_GetChar(void) ;
    uint16 PRINTER_B_GetByte(void) ;
    uint8 PRINTER_B_GetRxBufferSize(void)
                                                            ;
    void PRINTER_B_ClearRxBuffer(void) ;

    /* Obsolete functions, defines for backward compatible */
    #define PRINTER_B_GetRxInterruptSource   PRINTER_B_ReadRxStatus

#endif /* End (PRINTER_B_RX_ENABLED) || (PRINTER_B_HD_ENABLED) */

/* Only if TX is enabled */
#if(PRINTER_B_TX_ENABLED || PRINTER_B_HD_ENABLED)

    #if(PRINTER_B_TX_INTERRUPT_ENABLED)
        #define PRINTER_B_EnableTxInt()  CyIntEnable (PRINTER_B_TX_VECT_NUM)
        #define PRINTER_B_DisableTxInt() CyIntDisable(PRINTER_B_TX_VECT_NUM)
        #define PRINTER_B_SetPendingTxInt() CyIntSetPending(PRINTER_B_TX_VECT_NUM)
        #define PRINTER_B_ClearPendingTxInt() CyIntClearPending(PRINTER_B_TX_VECT_NUM)
        CY_ISR_PROTO(PRINTER_B_TXISR);
    #endif /* PRINTER_B_TX_INTERRUPT_ENABLED */

    void PRINTER_B_SetTxInterruptMode(uint8 intSrc) ;
    void PRINTER_B_WriteTxData(uint8 txDataByte) ;
    uint8 PRINTER_B_ReadTxStatus(void) ;
    void PRINTER_B_PutChar(uint8 txDataByte) ;
    void PRINTER_B_PutString(const char8 string[]) ;
    void PRINTER_B_PutArray(const uint8 string[], uint8 byteCount)
                                                            ;
    void PRINTER_B_PutCRLF(uint8 txDataByte) ;
    void PRINTER_B_ClearTxBuffer(void) ;
    void PRINTER_B_SetTxAddressMode(uint8 addressMode) ;
    void PRINTER_B_SendBreak(uint8 retMode) ;
    uint8 PRINTER_B_GetTxBufferSize(void)
                                                            ;
    /* Obsolete functions, defines for backward compatible */
    #define PRINTER_B_PutStringConst         PRINTER_B_PutString
    #define PRINTER_B_PutArrayConst          PRINTER_B_PutArray
    #define PRINTER_B_GetTxInterruptSource   PRINTER_B_ReadTxStatus

#endif /* End PRINTER_B_TX_ENABLED || PRINTER_B_HD_ENABLED */

#if(PRINTER_B_HD_ENABLED)
    void PRINTER_B_LoadRxConfig(void) ;
    void PRINTER_B_LoadTxConfig(void) ;
#endif /* End PRINTER_B_HD_ENABLED */


/* Communication bootloader APIs */
#if defined(CYDEV_BOOTLOADER_IO_COMP) && ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_PRINTER_B) || \
                                          (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))
    /* Physical layer functions */
    void    PRINTER_B_CyBtldrCommStart(void) CYSMALL ;
    void    PRINTER_B_CyBtldrCommStop(void) CYSMALL ;
    void    PRINTER_B_CyBtldrCommReset(void) CYSMALL ;
    cystatus PRINTER_B_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;
    cystatus PRINTER_B_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;

    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_PRINTER_B)
        #define CyBtldrCommStart    PRINTER_B_CyBtldrCommStart
        #define CyBtldrCommStop     PRINTER_B_CyBtldrCommStop
        #define CyBtldrCommReset    PRINTER_B_CyBtldrCommReset
        #define CyBtldrCommWrite    PRINTER_B_CyBtldrCommWrite
        #define CyBtldrCommRead     PRINTER_B_CyBtldrCommRead
    #endif  /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_PRINTER_B) */

    /* Byte to Byte time out for detecting end of block data from host */
    #define PRINTER_B_BYTE2BYTE_TIME_OUT (25u)
    #define PRINTER_B_PACKET_EOP         (0x17u) /* End of packet defined by bootloader */
    #define PRINTER_B_WAIT_EOP_DELAY     (5u)    /* Additional 5ms to wait for End of packet */
    #define PRINTER_B_BL_CHK_DELAY_MS    (1u)    /* Time Out quantity equal 1mS */

#endif /* CYDEV_BOOTLOADER_IO_COMP */


/***************************************
*          API Constants
***************************************/
/* Parameters for SetTxAddressMode API*/
#define PRINTER_B_SET_SPACE      (0x00u)
#define PRINTER_B_SET_MARK       (0x01u)

/* Status Register definitions */
#if( (PRINTER_B_TX_ENABLED) || (PRINTER_B_HD_ENABLED) )
    #if(PRINTER_B_TX_INTERRUPT_ENABLED)
        #define PRINTER_B_TX_VECT_NUM            (uint8)PRINTER_B_TXInternalInterrupt__INTC_NUMBER
        #define PRINTER_B_TX_PRIOR_NUM           (uint8)PRINTER_B_TXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* PRINTER_B_TX_INTERRUPT_ENABLED */

    #define PRINTER_B_TX_STS_COMPLETE_SHIFT          (0x00u)
    #define PRINTER_B_TX_STS_FIFO_EMPTY_SHIFT        (0x01u)
    #define PRINTER_B_TX_STS_FIFO_NOT_FULL_SHIFT     (0x03u)
    #if(PRINTER_B_TX_ENABLED)
        #define PRINTER_B_TX_STS_FIFO_FULL_SHIFT     (0x02u)
    #else /* (PRINTER_B_HD_ENABLED) */
        #define PRINTER_B_TX_STS_FIFO_FULL_SHIFT     (0x05u)  /* Needs MD=0 */
    #endif /* (PRINTER_B_TX_ENABLED) */

    #define PRINTER_B_TX_STS_COMPLETE            (uint8)(0x01u << PRINTER_B_TX_STS_COMPLETE_SHIFT)
    #define PRINTER_B_TX_STS_FIFO_EMPTY          (uint8)(0x01u << PRINTER_B_TX_STS_FIFO_EMPTY_SHIFT)
    #define PRINTER_B_TX_STS_FIFO_FULL           (uint8)(0x01u << PRINTER_B_TX_STS_FIFO_FULL_SHIFT)
    #define PRINTER_B_TX_STS_FIFO_NOT_FULL       (uint8)(0x01u << PRINTER_B_TX_STS_FIFO_NOT_FULL_SHIFT)
#endif /* End (PRINTER_B_TX_ENABLED) || (PRINTER_B_HD_ENABLED)*/

#if( (PRINTER_B_RX_ENABLED) || (PRINTER_B_HD_ENABLED) )
    #if(PRINTER_B_RX_INTERRUPT_ENABLED)
        #define PRINTER_B_RX_VECT_NUM            (uint8)PRINTER_B_RXInternalInterrupt__INTC_NUMBER
        #define PRINTER_B_RX_PRIOR_NUM           (uint8)PRINTER_B_RXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* PRINTER_B_RX_INTERRUPT_ENABLED */
    #define PRINTER_B_RX_STS_MRKSPC_SHIFT            (0x00u)
    #define PRINTER_B_RX_STS_BREAK_SHIFT             (0x01u)
    #define PRINTER_B_RX_STS_PAR_ERROR_SHIFT         (0x02u)
    #define PRINTER_B_RX_STS_STOP_ERROR_SHIFT        (0x03u)
    #define PRINTER_B_RX_STS_OVERRUN_SHIFT           (0x04u)
    #define PRINTER_B_RX_STS_FIFO_NOTEMPTY_SHIFT     (0x05u)
    #define PRINTER_B_RX_STS_ADDR_MATCH_SHIFT        (0x06u)
    #define PRINTER_B_RX_STS_SOFT_BUFF_OVER_SHIFT    (0x07u)

    #define PRINTER_B_RX_STS_MRKSPC           (uint8)(0x01u << PRINTER_B_RX_STS_MRKSPC_SHIFT)
    #define PRINTER_B_RX_STS_BREAK            (uint8)(0x01u << PRINTER_B_RX_STS_BREAK_SHIFT)
    #define PRINTER_B_RX_STS_PAR_ERROR        (uint8)(0x01u << PRINTER_B_RX_STS_PAR_ERROR_SHIFT)
    #define PRINTER_B_RX_STS_STOP_ERROR       (uint8)(0x01u << PRINTER_B_RX_STS_STOP_ERROR_SHIFT)
    #define PRINTER_B_RX_STS_OVERRUN          (uint8)(0x01u << PRINTER_B_RX_STS_OVERRUN_SHIFT)
    #define PRINTER_B_RX_STS_FIFO_NOTEMPTY    (uint8)(0x01u << PRINTER_B_RX_STS_FIFO_NOTEMPTY_SHIFT)
    #define PRINTER_B_RX_STS_ADDR_MATCH       (uint8)(0x01u << PRINTER_B_RX_STS_ADDR_MATCH_SHIFT)
    #define PRINTER_B_RX_STS_SOFT_BUFF_OVER   (uint8)(0x01u << PRINTER_B_RX_STS_SOFT_BUFF_OVER_SHIFT)
    #define PRINTER_B_RX_HW_MASK                     (0x7Fu)
#endif /* End (PRINTER_B_RX_ENABLED) || (PRINTER_B_HD_ENABLED) */

/* Control Register definitions */
#define PRINTER_B_CTRL_HD_SEND_SHIFT                 (0x00u) /* 1 enable TX part in Half Duplex mode */
#define PRINTER_B_CTRL_HD_SEND_BREAK_SHIFT           (0x01u) /* 1 send BREAK signal in Half Duplez mode */
#define PRINTER_B_CTRL_MARK_SHIFT                    (0x02u) /* 1 sets mark, 0 sets space */
#define PRINTER_B_CTRL_PARITY_TYPE0_SHIFT            (0x03u) /* Defines the type of parity implemented */
#define PRINTER_B_CTRL_PARITY_TYPE1_SHIFT            (0x04u) /* Defines the type of parity implemented */
#define PRINTER_B_CTRL_RXADDR_MODE0_SHIFT            (0x05u)
#define PRINTER_B_CTRL_RXADDR_MODE1_SHIFT            (0x06u)
#define PRINTER_B_CTRL_RXADDR_MODE2_SHIFT            (0x07u)

#define PRINTER_B_CTRL_HD_SEND               (uint8)(0x01u << PRINTER_B_CTRL_HD_SEND_SHIFT)
#define PRINTER_B_CTRL_HD_SEND_BREAK         (uint8)(0x01u << PRINTER_B_CTRL_HD_SEND_BREAK_SHIFT)
#define PRINTER_B_CTRL_MARK                  (uint8)(0x01u << PRINTER_B_CTRL_MARK_SHIFT)
#define PRINTER_B_CTRL_PARITY_TYPE_MASK      (uint8)(0x03u << PRINTER_B_CTRL_PARITY_TYPE0_SHIFT)
#define PRINTER_B_CTRL_RXADDR_MODE_MASK      (uint8)(0x07u << PRINTER_B_CTRL_RXADDR_MODE0_SHIFT)

/* StatusI Register Interrupt Enable Control Bits. As defined by the Register map for the AUX Control Register */
#define PRINTER_B_INT_ENABLE                         (0x10u)

/* Bit Counter (7-bit) Control Register Bit Definitions. As defined by the Register map for the AUX Control Register */
#define PRINTER_B_CNTR_ENABLE                        (0x20u)

/*   Constants for SendBreak() "retMode" parameter  */
#define PRINTER_B_SEND_BREAK                         (0x00u)
#define PRINTER_B_WAIT_FOR_COMPLETE_REINIT           (0x01u)
#define PRINTER_B_REINIT                             (0x02u)
#define PRINTER_B_SEND_WAIT_REINIT                   (0x03u)

#define PRINTER_B_OVER_SAMPLE_8                      (8u)
#define PRINTER_B_OVER_SAMPLE_16                     (16u)

#define PRINTER_B_BIT_CENTER                         (PRINTER_B_OVER_SAMPLE_COUNT - 2u)

#define PRINTER_B_FIFO_LENGTH                        (4u)
#define PRINTER_B_NUMBER_OF_START_BIT                (1u)
#define PRINTER_B_MAX_BYTE_VALUE                     (0xFFu)

/* 8X always for count7 implementation */
#define PRINTER_B_TXBITCTR_BREAKBITS8X   ((PRINTER_B_BREAK_BITS_TX * PRINTER_B_OVER_SAMPLE_8) - 1u)
/* 8X or 16X for DP implementation */
#define PRINTER_B_TXBITCTR_BREAKBITS ((PRINTER_B_BREAK_BITS_TX * PRINTER_B_OVER_SAMPLE_COUNT) - 1u)

#define PRINTER_B_HALF_BIT_COUNT   \
                            (((PRINTER_B_OVER_SAMPLE_COUNT / 2u) + (PRINTER_B_USE23POLLING * 1u)) - 2u)
#if (PRINTER_B_OVER_SAMPLE_COUNT == PRINTER_B_OVER_SAMPLE_8)
    #define PRINTER_B_HD_TXBITCTR_INIT   (((PRINTER_B_BREAK_BITS_TX + \
                            PRINTER_B_NUMBER_OF_START_BIT) * PRINTER_B_OVER_SAMPLE_COUNT) - 1u)

    /* This parameter is increased on the 2 in 2 out of 3 mode to sample voting in the middle */
    #define PRINTER_B_RXBITCTR_INIT  ((((PRINTER_B_BREAK_BITS_RX + PRINTER_B_NUMBER_OF_START_BIT) \
                            * PRINTER_B_OVER_SAMPLE_COUNT) + PRINTER_B_HALF_BIT_COUNT) - 1u)

#else /* PRINTER_B_OVER_SAMPLE_COUNT == PRINTER_B_OVER_SAMPLE_16 */
    #define PRINTER_B_HD_TXBITCTR_INIT   ((8u * PRINTER_B_OVER_SAMPLE_COUNT) - 1u)
    /* 7bit counter need one more bit for OverSampleCount = 16 */
    #define PRINTER_B_RXBITCTR_INIT      (((7u * PRINTER_B_OVER_SAMPLE_COUNT) - 1u) + \
                                                      PRINTER_B_HALF_BIT_COUNT)
#endif /* End PRINTER_B_OVER_SAMPLE_COUNT */

#define PRINTER_B_HD_RXBITCTR_INIT                   PRINTER_B_RXBITCTR_INIT


/***************************************
* Global variables external identifier
***************************************/

extern uint8 PRINTER_B_initVar;
#if (PRINTER_B_TX_INTERRUPT_ENABLED && PRINTER_B_TX_ENABLED)
    extern volatile uint8 PRINTER_B_txBuffer[PRINTER_B_TX_BUFFER_SIZE];
    extern volatile uint8 PRINTER_B_txBufferRead;
    extern uint8 PRINTER_B_txBufferWrite;
#endif /* (PRINTER_B_TX_INTERRUPT_ENABLED && PRINTER_B_TX_ENABLED) */
#if (PRINTER_B_RX_INTERRUPT_ENABLED && (PRINTER_B_RX_ENABLED || PRINTER_B_HD_ENABLED))
    extern uint8 PRINTER_B_errorStatus;
    extern volatile uint8 PRINTER_B_rxBuffer[PRINTER_B_RX_BUFFER_SIZE];
    extern volatile uint8 PRINTER_B_rxBufferRead;
    extern volatile uint8 PRINTER_B_rxBufferWrite;
    extern volatile uint8 PRINTER_B_rxBufferLoopDetect;
    extern volatile uint8 PRINTER_B_rxBufferOverflow;
    #if (PRINTER_B_RXHW_ADDRESS_ENABLED)
        extern volatile uint8 PRINTER_B_rxAddressMode;
        extern volatile uint8 PRINTER_B_rxAddressDetected;
    #endif /* (PRINTER_B_RXHW_ADDRESS_ENABLED) */
#endif /* (PRINTER_B_RX_INTERRUPT_ENABLED && (PRINTER_B_RX_ENABLED || PRINTER_B_HD_ENABLED)) */


/***************************************
* Enumerated Types and Parameters
***************************************/

#define PRINTER_B__B_UART__AM_SW_BYTE_BYTE 1
#define PRINTER_B__B_UART__AM_SW_DETECT_TO_BUFFER 2
#define PRINTER_B__B_UART__AM_HW_BYTE_BY_BYTE 3
#define PRINTER_B__B_UART__AM_HW_DETECT_TO_BUFFER 4
#define PRINTER_B__B_UART__AM_NONE 0

#define PRINTER_B__B_UART__NONE_REVB 0
#define PRINTER_B__B_UART__EVEN_REVB 1
#define PRINTER_B__B_UART__ODD_REVB 2
#define PRINTER_B__B_UART__MARK_SPACE_REVB 3



/***************************************
*    Initial Parameter Constants
***************************************/

/* UART shifts max 8 bits, Mark/Space functionality working if 9 selected */
#define PRINTER_B_NUMBER_OF_DATA_BITS    ((8u > 8u) ? 8u : 8u)
#define PRINTER_B_NUMBER_OF_STOP_BITS    (1u)

#if (PRINTER_B_RXHW_ADDRESS_ENABLED)
    #define PRINTER_B_RX_ADDRESS_MODE    (0u)
    #define PRINTER_B_RX_HW_ADDRESS1     (0u)
    #define PRINTER_B_RX_HW_ADDRESS2     (0u)
#endif /* (PRINTER_B_RXHW_ADDRESS_ENABLED) */

#define PRINTER_B_INIT_RX_INTERRUPTS_MASK \
                                  (uint8)((0 << PRINTER_B_RX_STS_FIFO_NOTEMPTY_SHIFT) \
                                        | (0 << PRINTER_B_RX_STS_MRKSPC_SHIFT) \
                                        | (0 << PRINTER_B_RX_STS_ADDR_MATCH_SHIFT) \
                                        | (0 << PRINTER_B_RX_STS_PAR_ERROR_SHIFT) \
                                        | (0 << PRINTER_B_RX_STS_STOP_ERROR_SHIFT) \
                                        | (0 << PRINTER_B_RX_STS_BREAK_SHIFT) \
                                        | (0 << PRINTER_B_RX_STS_OVERRUN_SHIFT))

#define PRINTER_B_INIT_TX_INTERRUPTS_MASK \
                                  (uint8)((0 << PRINTER_B_TX_STS_COMPLETE_SHIFT) \
                                        | (1 << PRINTER_B_TX_STS_FIFO_EMPTY_SHIFT) \
                                        | (0 << PRINTER_B_TX_STS_FIFO_FULL_SHIFT) \
                                        | (0 << PRINTER_B_TX_STS_FIFO_NOT_FULL_SHIFT))


/***************************************
*              Registers
***************************************/

#ifdef PRINTER_B_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define PRINTER_B_CONTROL_REG \
                            (* (reg8 *) PRINTER_B_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
    #define PRINTER_B_CONTROL_PTR \
                            (  (reg8 *) PRINTER_B_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
#endif /* End PRINTER_B_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(PRINTER_B_TX_ENABLED)
    #define PRINTER_B_TXDATA_REG          (* (reg8 *) PRINTER_B_BUART_sTX_TxShifter_u0__F0_REG)
    #define PRINTER_B_TXDATA_PTR          (  (reg8 *) PRINTER_B_BUART_sTX_TxShifter_u0__F0_REG)
    #define PRINTER_B_TXDATA_AUX_CTL_REG  (* (reg8 *) PRINTER_B_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define PRINTER_B_TXDATA_AUX_CTL_PTR  (  (reg8 *) PRINTER_B_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define PRINTER_B_TXSTATUS_REG        (* (reg8 *) PRINTER_B_BUART_sTX_TxSts__STATUS_REG)
    #define PRINTER_B_TXSTATUS_PTR        (  (reg8 *) PRINTER_B_BUART_sTX_TxSts__STATUS_REG)
    #define PRINTER_B_TXSTATUS_MASK_REG   (* (reg8 *) PRINTER_B_BUART_sTX_TxSts__MASK_REG)
    #define PRINTER_B_TXSTATUS_MASK_PTR   (  (reg8 *) PRINTER_B_BUART_sTX_TxSts__MASK_REG)
    #define PRINTER_B_TXSTATUS_ACTL_REG   (* (reg8 *) PRINTER_B_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)
    #define PRINTER_B_TXSTATUS_ACTL_PTR   (  (reg8 *) PRINTER_B_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)

    /* DP clock */
    #if(PRINTER_B_TXCLKGEN_DP)
        #define PRINTER_B_TXBITCLKGEN_CTR_REG        \
                                        (* (reg8 *) PRINTER_B_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define PRINTER_B_TXBITCLKGEN_CTR_PTR        \
                                        (  (reg8 *) PRINTER_B_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define PRINTER_B_TXBITCLKTX_COMPLETE_REG    \
                                        (* (reg8 *) PRINTER_B_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
        #define PRINTER_B_TXBITCLKTX_COMPLETE_PTR    \
                                        (  (reg8 *) PRINTER_B_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
    #else     /* Count7 clock*/
        #define PRINTER_B_TXBITCTR_PERIOD_REG    \
                                        (* (reg8 *) PRINTER_B_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define PRINTER_B_TXBITCTR_PERIOD_PTR    \
                                        (  (reg8 *) PRINTER_B_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define PRINTER_B_TXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) PRINTER_B_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define PRINTER_B_TXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) PRINTER_B_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define PRINTER_B_TXBITCTR_COUNTER_REG   \
                                        (* (reg8 *) PRINTER_B_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
        #define PRINTER_B_TXBITCTR_COUNTER_PTR   \
                                        (  (reg8 *) PRINTER_B_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
    #endif /* PRINTER_B_TXCLKGEN_DP */

#endif /* End PRINTER_B_TX_ENABLED */

#if(PRINTER_B_HD_ENABLED)

    #define PRINTER_B_TXDATA_REG             (* (reg8 *) PRINTER_B_BUART_sRX_RxShifter_u0__F1_REG )
    #define PRINTER_B_TXDATA_PTR             (  (reg8 *) PRINTER_B_BUART_sRX_RxShifter_u0__F1_REG )
    #define PRINTER_B_TXDATA_AUX_CTL_REG     (* (reg8 *) PRINTER_B_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)
    #define PRINTER_B_TXDATA_AUX_CTL_PTR     (  (reg8 *) PRINTER_B_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define PRINTER_B_TXSTATUS_REG           (* (reg8 *) PRINTER_B_BUART_sRX_RxSts__STATUS_REG )
    #define PRINTER_B_TXSTATUS_PTR           (  (reg8 *) PRINTER_B_BUART_sRX_RxSts__STATUS_REG )
    #define PRINTER_B_TXSTATUS_MASK_REG      (* (reg8 *) PRINTER_B_BUART_sRX_RxSts__MASK_REG )
    #define PRINTER_B_TXSTATUS_MASK_PTR      (  (reg8 *) PRINTER_B_BUART_sRX_RxSts__MASK_REG )
    #define PRINTER_B_TXSTATUS_ACTL_REG      (* (reg8 *) PRINTER_B_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define PRINTER_B_TXSTATUS_ACTL_PTR      (  (reg8 *) PRINTER_B_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End PRINTER_B_HD_ENABLED */

#if( (PRINTER_B_RX_ENABLED) || (PRINTER_B_HD_ENABLED) )
    #define PRINTER_B_RXDATA_REG             (* (reg8 *) PRINTER_B_BUART_sRX_RxShifter_u0__F0_REG )
    #define PRINTER_B_RXDATA_PTR             (  (reg8 *) PRINTER_B_BUART_sRX_RxShifter_u0__F0_REG )
    #define PRINTER_B_RXADDRESS1_REG         (* (reg8 *) PRINTER_B_BUART_sRX_RxShifter_u0__D0_REG )
    #define PRINTER_B_RXADDRESS1_PTR         (  (reg8 *) PRINTER_B_BUART_sRX_RxShifter_u0__D0_REG )
    #define PRINTER_B_RXADDRESS2_REG         (* (reg8 *) PRINTER_B_BUART_sRX_RxShifter_u0__D1_REG )
    #define PRINTER_B_RXADDRESS2_PTR         (  (reg8 *) PRINTER_B_BUART_sRX_RxShifter_u0__D1_REG )
    #define PRINTER_B_RXDATA_AUX_CTL_REG     (* (reg8 *) PRINTER_B_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define PRINTER_B_RXBITCTR_PERIOD_REG    (* (reg8 *) PRINTER_B_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define PRINTER_B_RXBITCTR_PERIOD_PTR    (  (reg8 *) PRINTER_B_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define PRINTER_B_RXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) PRINTER_B_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define PRINTER_B_RXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) PRINTER_B_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define PRINTER_B_RXBITCTR_COUNTER_REG   (* (reg8 *) PRINTER_B_BUART_sRX_RxBitCounter__COUNT_REG )
    #define PRINTER_B_RXBITCTR_COUNTER_PTR   (  (reg8 *) PRINTER_B_BUART_sRX_RxBitCounter__COUNT_REG )

    #define PRINTER_B_RXSTATUS_REG           (* (reg8 *) PRINTER_B_BUART_sRX_RxSts__STATUS_REG )
    #define PRINTER_B_RXSTATUS_PTR           (  (reg8 *) PRINTER_B_BUART_sRX_RxSts__STATUS_REG )
    #define PRINTER_B_RXSTATUS_MASK_REG      (* (reg8 *) PRINTER_B_BUART_sRX_RxSts__MASK_REG )
    #define PRINTER_B_RXSTATUS_MASK_PTR      (  (reg8 *) PRINTER_B_BUART_sRX_RxSts__MASK_REG )
    #define PRINTER_B_RXSTATUS_ACTL_REG      (* (reg8 *) PRINTER_B_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define PRINTER_B_RXSTATUS_ACTL_PTR      (  (reg8 *) PRINTER_B_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End  (PRINTER_B_RX_ENABLED) || (PRINTER_B_HD_ENABLED) */

#if(PRINTER_B_INTERNAL_CLOCK_USED)
    /* Register to enable or disable the digital clocks */
    #define PRINTER_B_INTCLOCK_CLKEN_REG     (* (reg8 *) PRINTER_B_IntClock__PM_ACT_CFG)
    #define PRINTER_B_INTCLOCK_CLKEN_PTR     (  (reg8 *) PRINTER_B_IntClock__PM_ACT_CFG)

    /* Clock mask for this clock. */
    #define PRINTER_B_INTCLOCK_CLKEN_MASK    PRINTER_B_IntClock__PM_ACT_MSK
#endif /* End PRINTER_B_INTERNAL_CLOCK_USED */


/***************************************
*       Register Constants
***************************************/

#if(PRINTER_B_TX_ENABLED)
    #define PRINTER_B_TX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End PRINTER_B_TX_ENABLED */

#if(PRINTER_B_HD_ENABLED)
    #define PRINTER_B_TX_FIFO_CLR            (0x02u) /* FIFO1 CLR */
#endif /* End PRINTER_B_HD_ENABLED */

#if( (PRINTER_B_RX_ENABLED) || (PRINTER_B_HD_ENABLED) )
    #define PRINTER_B_RX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End  (PRINTER_B_RX_ENABLED) || (PRINTER_B_HD_ENABLED) */


/***************************************
* The following code is DEPRECATED and
* should not be used in new projects.
***************************************/

/* UART v2_40 obsolete definitions */
#define PRINTER_B_WAIT_1_MS      PRINTER_B_BL_CHK_DELAY_MS   

#define PRINTER_B_TXBUFFERSIZE   PRINTER_B_TX_BUFFER_SIZE
#define PRINTER_B_RXBUFFERSIZE   PRINTER_B_RX_BUFFER_SIZE

#if (PRINTER_B_RXHW_ADDRESS_ENABLED)
    #define PRINTER_B_RXADDRESSMODE  PRINTER_B_RX_ADDRESS_MODE
    #define PRINTER_B_RXHWADDRESS1   PRINTER_B_RX_HW_ADDRESS1
    #define PRINTER_B_RXHWADDRESS2   PRINTER_B_RX_HW_ADDRESS2
    /* Backward compatible define */
    #define PRINTER_B_RXAddressMode  PRINTER_B_RXADDRESSMODE
#endif /* (PRINTER_B_RXHW_ADDRESS_ENABLED) */

/* UART v2_30 obsolete definitions */
#define PRINTER_B_initvar                    PRINTER_B_initVar

#define PRINTER_B_RX_Enabled                 PRINTER_B_RX_ENABLED
#define PRINTER_B_TX_Enabled                 PRINTER_B_TX_ENABLED
#define PRINTER_B_HD_Enabled                 PRINTER_B_HD_ENABLED
#define PRINTER_B_RX_IntInterruptEnabled     PRINTER_B_RX_INTERRUPT_ENABLED
#define PRINTER_B_TX_IntInterruptEnabled     PRINTER_B_TX_INTERRUPT_ENABLED
#define PRINTER_B_InternalClockUsed          PRINTER_B_INTERNAL_CLOCK_USED
#define PRINTER_B_RXHW_Address_Enabled       PRINTER_B_RXHW_ADDRESS_ENABLED
#define PRINTER_B_OverSampleCount            PRINTER_B_OVER_SAMPLE_COUNT
#define PRINTER_B_ParityType                 PRINTER_B_PARITY_TYPE

#if( PRINTER_B_TX_ENABLED && (PRINTER_B_TXBUFFERSIZE > PRINTER_B_FIFO_LENGTH))
    #define PRINTER_B_TXBUFFER               PRINTER_B_txBuffer
    #define PRINTER_B_TXBUFFERREAD           PRINTER_B_txBufferRead
    #define PRINTER_B_TXBUFFERWRITE          PRINTER_B_txBufferWrite
#endif /* End PRINTER_B_TX_ENABLED */
#if( ( PRINTER_B_RX_ENABLED || PRINTER_B_HD_ENABLED ) && \
     (PRINTER_B_RXBUFFERSIZE > PRINTER_B_FIFO_LENGTH) )
    #define PRINTER_B_RXBUFFER               PRINTER_B_rxBuffer
    #define PRINTER_B_RXBUFFERREAD           PRINTER_B_rxBufferRead
    #define PRINTER_B_RXBUFFERWRITE          PRINTER_B_rxBufferWrite
    #define PRINTER_B_RXBUFFERLOOPDETECT     PRINTER_B_rxBufferLoopDetect
    #define PRINTER_B_RXBUFFER_OVERFLOW      PRINTER_B_rxBufferOverflow
#endif /* End PRINTER_B_RX_ENABLED */

#ifdef PRINTER_B_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define PRINTER_B_CONTROL                PRINTER_B_CONTROL_REG
#endif /* End PRINTER_B_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(PRINTER_B_TX_ENABLED)
    #define PRINTER_B_TXDATA                 PRINTER_B_TXDATA_REG
    #define PRINTER_B_TXSTATUS               PRINTER_B_TXSTATUS_REG
    #define PRINTER_B_TXSTATUS_MASK          PRINTER_B_TXSTATUS_MASK_REG
    #define PRINTER_B_TXSTATUS_ACTL          PRINTER_B_TXSTATUS_ACTL_REG
    /* DP clock */
    #if(PRINTER_B_TXCLKGEN_DP)
        #define PRINTER_B_TXBITCLKGEN_CTR        PRINTER_B_TXBITCLKGEN_CTR_REG
        #define PRINTER_B_TXBITCLKTX_COMPLETE    PRINTER_B_TXBITCLKTX_COMPLETE_REG
    #else     /* Count7 clock*/
        #define PRINTER_B_TXBITCTR_PERIOD        PRINTER_B_TXBITCTR_PERIOD_REG
        #define PRINTER_B_TXBITCTR_CONTROL       PRINTER_B_TXBITCTR_CONTROL_REG
        #define PRINTER_B_TXBITCTR_COUNTER       PRINTER_B_TXBITCTR_COUNTER_REG
    #endif /* PRINTER_B_TXCLKGEN_DP */
#endif /* End PRINTER_B_TX_ENABLED */

#if(PRINTER_B_HD_ENABLED)
    #define PRINTER_B_TXDATA                 PRINTER_B_TXDATA_REG
    #define PRINTER_B_TXSTATUS               PRINTER_B_TXSTATUS_REG
    #define PRINTER_B_TXSTATUS_MASK          PRINTER_B_TXSTATUS_MASK_REG
    #define PRINTER_B_TXSTATUS_ACTL          PRINTER_B_TXSTATUS_ACTL_REG
#endif /* End PRINTER_B_HD_ENABLED */

#if( (PRINTER_B_RX_ENABLED) || (PRINTER_B_HD_ENABLED) )
    #define PRINTER_B_RXDATA                 PRINTER_B_RXDATA_REG
    #define PRINTER_B_RXADDRESS1             PRINTER_B_RXADDRESS1_REG
    #define PRINTER_B_RXADDRESS2             PRINTER_B_RXADDRESS2_REG
    #define PRINTER_B_RXBITCTR_PERIOD        PRINTER_B_RXBITCTR_PERIOD_REG
    #define PRINTER_B_RXBITCTR_CONTROL       PRINTER_B_RXBITCTR_CONTROL_REG
    #define PRINTER_B_RXBITCTR_COUNTER       PRINTER_B_RXBITCTR_COUNTER_REG
    #define PRINTER_B_RXSTATUS               PRINTER_B_RXSTATUS_REG
    #define PRINTER_B_RXSTATUS_MASK          PRINTER_B_RXSTATUS_MASK_REG
    #define PRINTER_B_RXSTATUS_ACTL          PRINTER_B_RXSTATUS_ACTL_REG
#endif /* End  (PRINTER_B_RX_ENABLED) || (PRINTER_B_HD_ENABLED) */

#if(PRINTER_B_INTERNAL_CLOCK_USED)
    #define PRINTER_B_INTCLOCK_CLKEN         PRINTER_B_INTCLOCK_CLKEN_REG
#endif /* End PRINTER_B_INTERNAL_CLOCK_USED */

#define PRINTER_B_WAIT_FOR_COMLETE_REINIT    PRINTER_B_WAIT_FOR_COMPLETE_REINIT

#endif  /* CY_UART_PRINTER_B_H */


/* [] END OF FILE */
