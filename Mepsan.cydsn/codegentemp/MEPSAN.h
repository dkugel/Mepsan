/*******************************************************************************
* File Name: MEPSAN.h
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


#if !defined(CY_UART_MEPSAN_H)
#define CY_UART_MEPSAN_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */


/***************************************
* Conditional Compilation Parameters
***************************************/

#define MEPSAN_RX_ENABLED                     (1u)
#define MEPSAN_TX_ENABLED                     (1u)
#define MEPSAN_HD_ENABLED                     (0u)
#define MEPSAN_RX_INTERRUPT_ENABLED           (1u)
#define MEPSAN_TX_INTERRUPT_ENABLED           (0u)
#define MEPSAN_INTERNAL_CLOCK_USED            (1u)
#define MEPSAN_RXHW_ADDRESS_ENABLED           (0u)
#define MEPSAN_OVER_SAMPLE_COUNT              (8u)
#define MEPSAN_PARITY_TYPE                    (2u)
#define MEPSAN_PARITY_TYPE_SW                 (0u)
#define MEPSAN_BREAK_DETECT                   (0u)
#define MEPSAN_BREAK_BITS_TX                  (13u)
#define MEPSAN_BREAK_BITS_RX                  (13u)
#define MEPSAN_TXCLKGEN_DP                    (1u)
#define MEPSAN_USE23POLLING                   (1u)
#define MEPSAN_FLOW_CONTROL                   (0u)
#define MEPSAN_CLK_FREQ                       (0u)
#define MEPSAN_TX_BUFFER_SIZE                 (4u)
#define MEPSAN_RX_BUFFER_SIZE                 (128u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component UART_v2_50 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#if defined(MEPSAN_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG)
    #define MEPSAN_CONTROL_REG_REMOVED            (0u)
#else
    #define MEPSAN_CONTROL_REG_REMOVED            (1u)
#endif /* End MEPSAN_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */


/***************************************
*      Data Structure Definition
***************************************/

/* Sleep Mode API Support */
typedef struct MEPSAN_backupStruct_
{
    uint8 enableState;

    #if(MEPSAN_CONTROL_REG_REMOVED == 0u)
        uint8 cr;
    #endif /* End MEPSAN_CONTROL_REG_REMOVED */

} MEPSAN_BACKUP_STRUCT;


/***************************************
*       Function Prototypes
***************************************/

void MEPSAN_Start(void) ;
void MEPSAN_Stop(void) ;
uint8 MEPSAN_ReadControlRegister(void) ;
void MEPSAN_WriteControlRegister(uint8 control) ;

void MEPSAN_Init(void) ;
void MEPSAN_Enable(void) ;
void MEPSAN_SaveConfig(void) ;
void MEPSAN_RestoreConfig(void) ;
void MEPSAN_Sleep(void) ;
void MEPSAN_Wakeup(void) ;

/* Only if RX is enabled */
#if( (MEPSAN_RX_ENABLED) || (MEPSAN_HD_ENABLED) )

    #if (MEPSAN_RX_INTERRUPT_ENABLED)
        #define MEPSAN_EnableRxInt()  CyIntEnable (MEPSAN_RX_VECT_NUM)
        #define MEPSAN_DisableRxInt() CyIntDisable(MEPSAN_RX_VECT_NUM)
        CY_ISR_PROTO(MEPSAN_RXISR);
    #endif /* MEPSAN_RX_INTERRUPT_ENABLED */

    void MEPSAN_SetRxAddressMode(uint8 addressMode)
                                                           ;
    void MEPSAN_SetRxAddress1(uint8 address) ;
    void MEPSAN_SetRxAddress2(uint8 address) ;

    void  MEPSAN_SetRxInterruptMode(uint8 intSrc) ;
    uint8 MEPSAN_ReadRxData(void) ;
    uint8 MEPSAN_ReadRxStatus(void) ;
    uint8 MEPSAN_GetChar(void) ;
    uint16 MEPSAN_GetByte(void) ;
    uint8 MEPSAN_GetRxBufferSize(void)
                                                            ;
    void MEPSAN_ClearRxBuffer(void) ;

    /* Obsolete functions, defines for backward compatible */
    #define MEPSAN_GetRxInterruptSource   MEPSAN_ReadRxStatus

#endif /* End (MEPSAN_RX_ENABLED) || (MEPSAN_HD_ENABLED) */

/* Only if TX is enabled */
#if(MEPSAN_TX_ENABLED || MEPSAN_HD_ENABLED)

    #if(MEPSAN_TX_INTERRUPT_ENABLED)
        #define MEPSAN_EnableTxInt()  CyIntEnable (MEPSAN_TX_VECT_NUM)
        #define MEPSAN_DisableTxInt() CyIntDisable(MEPSAN_TX_VECT_NUM)
        #define MEPSAN_SetPendingTxInt() CyIntSetPending(MEPSAN_TX_VECT_NUM)
        #define MEPSAN_ClearPendingTxInt() CyIntClearPending(MEPSAN_TX_VECT_NUM)
        CY_ISR_PROTO(MEPSAN_TXISR);
    #endif /* MEPSAN_TX_INTERRUPT_ENABLED */

    void MEPSAN_SetTxInterruptMode(uint8 intSrc) ;
    void MEPSAN_WriteTxData(uint8 txDataByte) ;
    uint8 MEPSAN_ReadTxStatus(void) ;
    void MEPSAN_PutChar(uint8 txDataByte) ;
    void MEPSAN_PutString(const char8 string[]) ;
    void MEPSAN_PutArray(const uint8 string[], uint8 byteCount)
                                                            ;
    void MEPSAN_PutCRLF(uint8 txDataByte) ;
    void MEPSAN_ClearTxBuffer(void) ;
    void MEPSAN_SetTxAddressMode(uint8 addressMode) ;
    void MEPSAN_SendBreak(uint8 retMode) ;
    uint8 MEPSAN_GetTxBufferSize(void)
                                                            ;
    /* Obsolete functions, defines for backward compatible */
    #define MEPSAN_PutStringConst         MEPSAN_PutString
    #define MEPSAN_PutArrayConst          MEPSAN_PutArray
    #define MEPSAN_GetTxInterruptSource   MEPSAN_ReadTxStatus

#endif /* End MEPSAN_TX_ENABLED || MEPSAN_HD_ENABLED */

#if(MEPSAN_HD_ENABLED)
    void MEPSAN_LoadRxConfig(void) ;
    void MEPSAN_LoadTxConfig(void) ;
#endif /* End MEPSAN_HD_ENABLED */


/* Communication bootloader APIs */
#if defined(CYDEV_BOOTLOADER_IO_COMP) && ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_MEPSAN) || \
                                          (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))
    /* Physical layer functions */
    void    MEPSAN_CyBtldrCommStart(void) CYSMALL ;
    void    MEPSAN_CyBtldrCommStop(void) CYSMALL ;
    void    MEPSAN_CyBtldrCommReset(void) CYSMALL ;
    cystatus MEPSAN_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;
    cystatus MEPSAN_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;

    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_MEPSAN)
        #define CyBtldrCommStart    MEPSAN_CyBtldrCommStart
        #define CyBtldrCommStop     MEPSAN_CyBtldrCommStop
        #define CyBtldrCommReset    MEPSAN_CyBtldrCommReset
        #define CyBtldrCommWrite    MEPSAN_CyBtldrCommWrite
        #define CyBtldrCommRead     MEPSAN_CyBtldrCommRead
    #endif  /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_MEPSAN) */

    /* Byte to Byte time out for detecting end of block data from host */
    #define MEPSAN_BYTE2BYTE_TIME_OUT (25u)
    #define MEPSAN_PACKET_EOP         (0x17u) /* End of packet defined by bootloader */
    #define MEPSAN_WAIT_EOP_DELAY     (5u)    /* Additional 5ms to wait for End of packet */
    #define MEPSAN_BL_CHK_DELAY_MS    (1u)    /* Time Out quantity equal 1mS */

#endif /* CYDEV_BOOTLOADER_IO_COMP */


/***************************************
*          API Constants
***************************************/
/* Parameters for SetTxAddressMode API*/
#define MEPSAN_SET_SPACE      (0x00u)
#define MEPSAN_SET_MARK       (0x01u)

/* Status Register definitions */
#if( (MEPSAN_TX_ENABLED) || (MEPSAN_HD_ENABLED) )
    #if(MEPSAN_TX_INTERRUPT_ENABLED)
        #define MEPSAN_TX_VECT_NUM            (uint8)MEPSAN_TXInternalInterrupt__INTC_NUMBER
        #define MEPSAN_TX_PRIOR_NUM           (uint8)MEPSAN_TXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* MEPSAN_TX_INTERRUPT_ENABLED */

    #define MEPSAN_TX_STS_COMPLETE_SHIFT          (0x00u)
    #define MEPSAN_TX_STS_FIFO_EMPTY_SHIFT        (0x01u)
    #define MEPSAN_TX_STS_FIFO_NOT_FULL_SHIFT     (0x03u)
    #if(MEPSAN_TX_ENABLED)
        #define MEPSAN_TX_STS_FIFO_FULL_SHIFT     (0x02u)
    #else /* (MEPSAN_HD_ENABLED) */
        #define MEPSAN_TX_STS_FIFO_FULL_SHIFT     (0x05u)  /* Needs MD=0 */
    #endif /* (MEPSAN_TX_ENABLED) */

    #define MEPSAN_TX_STS_COMPLETE            (uint8)(0x01u << MEPSAN_TX_STS_COMPLETE_SHIFT)
    #define MEPSAN_TX_STS_FIFO_EMPTY          (uint8)(0x01u << MEPSAN_TX_STS_FIFO_EMPTY_SHIFT)
    #define MEPSAN_TX_STS_FIFO_FULL           (uint8)(0x01u << MEPSAN_TX_STS_FIFO_FULL_SHIFT)
    #define MEPSAN_TX_STS_FIFO_NOT_FULL       (uint8)(0x01u << MEPSAN_TX_STS_FIFO_NOT_FULL_SHIFT)
#endif /* End (MEPSAN_TX_ENABLED) || (MEPSAN_HD_ENABLED)*/

#if( (MEPSAN_RX_ENABLED) || (MEPSAN_HD_ENABLED) )
    #if(MEPSAN_RX_INTERRUPT_ENABLED)
        #define MEPSAN_RX_VECT_NUM            (uint8)MEPSAN_RXInternalInterrupt__INTC_NUMBER
        #define MEPSAN_RX_PRIOR_NUM           (uint8)MEPSAN_RXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* MEPSAN_RX_INTERRUPT_ENABLED */
    #define MEPSAN_RX_STS_MRKSPC_SHIFT            (0x00u)
    #define MEPSAN_RX_STS_BREAK_SHIFT             (0x01u)
    #define MEPSAN_RX_STS_PAR_ERROR_SHIFT         (0x02u)
    #define MEPSAN_RX_STS_STOP_ERROR_SHIFT        (0x03u)
    #define MEPSAN_RX_STS_OVERRUN_SHIFT           (0x04u)
    #define MEPSAN_RX_STS_FIFO_NOTEMPTY_SHIFT     (0x05u)
    #define MEPSAN_RX_STS_ADDR_MATCH_SHIFT        (0x06u)
    #define MEPSAN_RX_STS_SOFT_BUFF_OVER_SHIFT    (0x07u)

    #define MEPSAN_RX_STS_MRKSPC           (uint8)(0x01u << MEPSAN_RX_STS_MRKSPC_SHIFT)
    #define MEPSAN_RX_STS_BREAK            (uint8)(0x01u << MEPSAN_RX_STS_BREAK_SHIFT)
    #define MEPSAN_RX_STS_PAR_ERROR        (uint8)(0x01u << MEPSAN_RX_STS_PAR_ERROR_SHIFT)
    #define MEPSAN_RX_STS_STOP_ERROR       (uint8)(0x01u << MEPSAN_RX_STS_STOP_ERROR_SHIFT)
    #define MEPSAN_RX_STS_OVERRUN          (uint8)(0x01u << MEPSAN_RX_STS_OVERRUN_SHIFT)
    #define MEPSAN_RX_STS_FIFO_NOTEMPTY    (uint8)(0x01u << MEPSAN_RX_STS_FIFO_NOTEMPTY_SHIFT)
    #define MEPSAN_RX_STS_ADDR_MATCH       (uint8)(0x01u << MEPSAN_RX_STS_ADDR_MATCH_SHIFT)
    #define MEPSAN_RX_STS_SOFT_BUFF_OVER   (uint8)(0x01u << MEPSAN_RX_STS_SOFT_BUFF_OVER_SHIFT)
    #define MEPSAN_RX_HW_MASK                     (0x7Fu)
#endif /* End (MEPSAN_RX_ENABLED) || (MEPSAN_HD_ENABLED) */

/* Control Register definitions */
#define MEPSAN_CTRL_HD_SEND_SHIFT                 (0x00u) /* 1 enable TX part in Half Duplex mode */
#define MEPSAN_CTRL_HD_SEND_BREAK_SHIFT           (0x01u) /* 1 send BREAK signal in Half Duplez mode */
#define MEPSAN_CTRL_MARK_SHIFT                    (0x02u) /* 1 sets mark, 0 sets space */
#define MEPSAN_CTRL_PARITY_TYPE0_SHIFT            (0x03u) /* Defines the type of parity implemented */
#define MEPSAN_CTRL_PARITY_TYPE1_SHIFT            (0x04u) /* Defines the type of parity implemented */
#define MEPSAN_CTRL_RXADDR_MODE0_SHIFT            (0x05u)
#define MEPSAN_CTRL_RXADDR_MODE1_SHIFT            (0x06u)
#define MEPSAN_CTRL_RXADDR_MODE2_SHIFT            (0x07u)

#define MEPSAN_CTRL_HD_SEND               (uint8)(0x01u << MEPSAN_CTRL_HD_SEND_SHIFT)
#define MEPSAN_CTRL_HD_SEND_BREAK         (uint8)(0x01u << MEPSAN_CTRL_HD_SEND_BREAK_SHIFT)
#define MEPSAN_CTRL_MARK                  (uint8)(0x01u << MEPSAN_CTRL_MARK_SHIFT)
#define MEPSAN_CTRL_PARITY_TYPE_MASK      (uint8)(0x03u << MEPSAN_CTRL_PARITY_TYPE0_SHIFT)
#define MEPSAN_CTRL_RXADDR_MODE_MASK      (uint8)(0x07u << MEPSAN_CTRL_RXADDR_MODE0_SHIFT)

/* StatusI Register Interrupt Enable Control Bits. As defined by the Register map for the AUX Control Register */
#define MEPSAN_INT_ENABLE                         (0x10u)

/* Bit Counter (7-bit) Control Register Bit Definitions. As defined by the Register map for the AUX Control Register */
#define MEPSAN_CNTR_ENABLE                        (0x20u)

/*   Constants for SendBreak() "retMode" parameter  */
#define MEPSAN_SEND_BREAK                         (0x00u)
#define MEPSAN_WAIT_FOR_COMPLETE_REINIT           (0x01u)
#define MEPSAN_REINIT                             (0x02u)
#define MEPSAN_SEND_WAIT_REINIT                   (0x03u)

#define MEPSAN_OVER_SAMPLE_8                      (8u)
#define MEPSAN_OVER_SAMPLE_16                     (16u)

#define MEPSAN_BIT_CENTER                         (MEPSAN_OVER_SAMPLE_COUNT - 2u)

#define MEPSAN_FIFO_LENGTH                        (4u)
#define MEPSAN_NUMBER_OF_START_BIT                (1u)
#define MEPSAN_MAX_BYTE_VALUE                     (0xFFu)

/* 8X always for count7 implementation */
#define MEPSAN_TXBITCTR_BREAKBITS8X   ((MEPSAN_BREAK_BITS_TX * MEPSAN_OVER_SAMPLE_8) - 1u)
/* 8X or 16X for DP implementation */
#define MEPSAN_TXBITCTR_BREAKBITS ((MEPSAN_BREAK_BITS_TX * MEPSAN_OVER_SAMPLE_COUNT) - 1u)

#define MEPSAN_HALF_BIT_COUNT   \
                            (((MEPSAN_OVER_SAMPLE_COUNT / 2u) + (MEPSAN_USE23POLLING * 1u)) - 2u)
#if (MEPSAN_OVER_SAMPLE_COUNT == MEPSAN_OVER_SAMPLE_8)
    #define MEPSAN_HD_TXBITCTR_INIT   (((MEPSAN_BREAK_BITS_TX + \
                            MEPSAN_NUMBER_OF_START_BIT) * MEPSAN_OVER_SAMPLE_COUNT) - 1u)

    /* This parameter is increased on the 2 in 2 out of 3 mode to sample voting in the middle */
    #define MEPSAN_RXBITCTR_INIT  ((((MEPSAN_BREAK_BITS_RX + MEPSAN_NUMBER_OF_START_BIT) \
                            * MEPSAN_OVER_SAMPLE_COUNT) + MEPSAN_HALF_BIT_COUNT) - 1u)

#else /* MEPSAN_OVER_SAMPLE_COUNT == MEPSAN_OVER_SAMPLE_16 */
    #define MEPSAN_HD_TXBITCTR_INIT   ((8u * MEPSAN_OVER_SAMPLE_COUNT) - 1u)
    /* 7bit counter need one more bit for OverSampleCount = 16 */
    #define MEPSAN_RXBITCTR_INIT      (((7u * MEPSAN_OVER_SAMPLE_COUNT) - 1u) + \
                                                      MEPSAN_HALF_BIT_COUNT)
#endif /* End MEPSAN_OVER_SAMPLE_COUNT */

#define MEPSAN_HD_RXBITCTR_INIT                   MEPSAN_RXBITCTR_INIT


/***************************************
* Global variables external identifier
***************************************/

extern uint8 MEPSAN_initVar;
#if (MEPSAN_TX_INTERRUPT_ENABLED && MEPSAN_TX_ENABLED)
    extern volatile uint8 MEPSAN_txBuffer[MEPSAN_TX_BUFFER_SIZE];
    extern volatile uint8 MEPSAN_txBufferRead;
    extern uint8 MEPSAN_txBufferWrite;
#endif /* (MEPSAN_TX_INTERRUPT_ENABLED && MEPSAN_TX_ENABLED) */
#if (MEPSAN_RX_INTERRUPT_ENABLED && (MEPSAN_RX_ENABLED || MEPSAN_HD_ENABLED))
    extern uint8 MEPSAN_errorStatus;
    extern volatile uint8 MEPSAN_rxBuffer[MEPSAN_RX_BUFFER_SIZE];
    extern volatile uint8 MEPSAN_rxBufferRead;
    extern volatile uint8 MEPSAN_rxBufferWrite;
    extern volatile uint8 MEPSAN_rxBufferLoopDetect;
    extern volatile uint8 MEPSAN_rxBufferOverflow;
    #if (MEPSAN_RXHW_ADDRESS_ENABLED)
        extern volatile uint8 MEPSAN_rxAddressMode;
        extern volatile uint8 MEPSAN_rxAddressDetected;
    #endif /* (MEPSAN_RXHW_ADDRESS_ENABLED) */
#endif /* (MEPSAN_RX_INTERRUPT_ENABLED && (MEPSAN_RX_ENABLED || MEPSAN_HD_ENABLED)) */


/***************************************
* Enumerated Types and Parameters
***************************************/

#define MEPSAN__B_UART__AM_SW_BYTE_BYTE 1
#define MEPSAN__B_UART__AM_SW_DETECT_TO_BUFFER 2
#define MEPSAN__B_UART__AM_HW_BYTE_BY_BYTE 3
#define MEPSAN__B_UART__AM_HW_DETECT_TO_BUFFER 4
#define MEPSAN__B_UART__AM_NONE 0

#define MEPSAN__B_UART__NONE_REVB 0
#define MEPSAN__B_UART__EVEN_REVB 1
#define MEPSAN__B_UART__ODD_REVB 2
#define MEPSAN__B_UART__MARK_SPACE_REVB 3



/***************************************
*    Initial Parameter Constants
***************************************/

/* UART shifts max 8 bits, Mark/Space functionality working if 9 selected */
#define MEPSAN_NUMBER_OF_DATA_BITS    ((8u > 8u) ? 8u : 8u)
#define MEPSAN_NUMBER_OF_STOP_BITS    (1u)

#if (MEPSAN_RXHW_ADDRESS_ENABLED)
    #define MEPSAN_RX_ADDRESS_MODE    (0u)
    #define MEPSAN_RX_HW_ADDRESS1     (0u)
    #define MEPSAN_RX_HW_ADDRESS2     (0u)
#endif /* (MEPSAN_RXHW_ADDRESS_ENABLED) */

#define MEPSAN_INIT_RX_INTERRUPTS_MASK \
                                  (uint8)((1 << MEPSAN_RX_STS_FIFO_NOTEMPTY_SHIFT) \
                                        | (0 << MEPSAN_RX_STS_MRKSPC_SHIFT) \
                                        | (0 << MEPSAN_RX_STS_ADDR_MATCH_SHIFT) \
                                        | (0 << MEPSAN_RX_STS_PAR_ERROR_SHIFT) \
                                        | (0 << MEPSAN_RX_STS_STOP_ERROR_SHIFT) \
                                        | (0 << MEPSAN_RX_STS_BREAK_SHIFT) \
                                        | (0 << MEPSAN_RX_STS_OVERRUN_SHIFT))

#define MEPSAN_INIT_TX_INTERRUPTS_MASK \
                                  (uint8)((0 << MEPSAN_TX_STS_COMPLETE_SHIFT) \
                                        | (0 << MEPSAN_TX_STS_FIFO_EMPTY_SHIFT) \
                                        | (0 << MEPSAN_TX_STS_FIFO_FULL_SHIFT) \
                                        | (0 << MEPSAN_TX_STS_FIFO_NOT_FULL_SHIFT))


/***************************************
*              Registers
***************************************/

#ifdef MEPSAN_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define MEPSAN_CONTROL_REG \
                            (* (reg8 *) MEPSAN_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
    #define MEPSAN_CONTROL_PTR \
                            (  (reg8 *) MEPSAN_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
#endif /* End MEPSAN_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(MEPSAN_TX_ENABLED)
    #define MEPSAN_TXDATA_REG          (* (reg8 *) MEPSAN_BUART_sTX_TxShifter_u0__F0_REG)
    #define MEPSAN_TXDATA_PTR          (  (reg8 *) MEPSAN_BUART_sTX_TxShifter_u0__F0_REG)
    #define MEPSAN_TXDATA_AUX_CTL_REG  (* (reg8 *) MEPSAN_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define MEPSAN_TXDATA_AUX_CTL_PTR  (  (reg8 *) MEPSAN_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define MEPSAN_TXSTATUS_REG        (* (reg8 *) MEPSAN_BUART_sTX_TxSts__STATUS_REG)
    #define MEPSAN_TXSTATUS_PTR        (  (reg8 *) MEPSAN_BUART_sTX_TxSts__STATUS_REG)
    #define MEPSAN_TXSTATUS_MASK_REG   (* (reg8 *) MEPSAN_BUART_sTX_TxSts__MASK_REG)
    #define MEPSAN_TXSTATUS_MASK_PTR   (  (reg8 *) MEPSAN_BUART_sTX_TxSts__MASK_REG)
    #define MEPSAN_TXSTATUS_ACTL_REG   (* (reg8 *) MEPSAN_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)
    #define MEPSAN_TXSTATUS_ACTL_PTR   (  (reg8 *) MEPSAN_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)

    /* DP clock */
    #if(MEPSAN_TXCLKGEN_DP)
        #define MEPSAN_TXBITCLKGEN_CTR_REG        \
                                        (* (reg8 *) MEPSAN_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define MEPSAN_TXBITCLKGEN_CTR_PTR        \
                                        (  (reg8 *) MEPSAN_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define MEPSAN_TXBITCLKTX_COMPLETE_REG    \
                                        (* (reg8 *) MEPSAN_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
        #define MEPSAN_TXBITCLKTX_COMPLETE_PTR    \
                                        (  (reg8 *) MEPSAN_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
    #else     /* Count7 clock*/
        #define MEPSAN_TXBITCTR_PERIOD_REG    \
                                        (* (reg8 *) MEPSAN_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define MEPSAN_TXBITCTR_PERIOD_PTR    \
                                        (  (reg8 *) MEPSAN_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define MEPSAN_TXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) MEPSAN_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define MEPSAN_TXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) MEPSAN_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define MEPSAN_TXBITCTR_COUNTER_REG   \
                                        (* (reg8 *) MEPSAN_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
        #define MEPSAN_TXBITCTR_COUNTER_PTR   \
                                        (  (reg8 *) MEPSAN_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
    #endif /* MEPSAN_TXCLKGEN_DP */

#endif /* End MEPSAN_TX_ENABLED */

#if(MEPSAN_HD_ENABLED)

    #define MEPSAN_TXDATA_REG             (* (reg8 *) MEPSAN_BUART_sRX_RxShifter_u0__F1_REG )
    #define MEPSAN_TXDATA_PTR             (  (reg8 *) MEPSAN_BUART_sRX_RxShifter_u0__F1_REG )
    #define MEPSAN_TXDATA_AUX_CTL_REG     (* (reg8 *) MEPSAN_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)
    #define MEPSAN_TXDATA_AUX_CTL_PTR     (  (reg8 *) MEPSAN_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define MEPSAN_TXSTATUS_REG           (* (reg8 *) MEPSAN_BUART_sRX_RxSts__STATUS_REG )
    #define MEPSAN_TXSTATUS_PTR           (  (reg8 *) MEPSAN_BUART_sRX_RxSts__STATUS_REG )
    #define MEPSAN_TXSTATUS_MASK_REG      (* (reg8 *) MEPSAN_BUART_sRX_RxSts__MASK_REG )
    #define MEPSAN_TXSTATUS_MASK_PTR      (  (reg8 *) MEPSAN_BUART_sRX_RxSts__MASK_REG )
    #define MEPSAN_TXSTATUS_ACTL_REG      (* (reg8 *) MEPSAN_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define MEPSAN_TXSTATUS_ACTL_PTR      (  (reg8 *) MEPSAN_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End MEPSAN_HD_ENABLED */

#if( (MEPSAN_RX_ENABLED) || (MEPSAN_HD_ENABLED) )
    #define MEPSAN_RXDATA_REG             (* (reg8 *) MEPSAN_BUART_sRX_RxShifter_u0__F0_REG )
    #define MEPSAN_RXDATA_PTR             (  (reg8 *) MEPSAN_BUART_sRX_RxShifter_u0__F0_REG )
    #define MEPSAN_RXADDRESS1_REG         (* (reg8 *) MEPSAN_BUART_sRX_RxShifter_u0__D0_REG )
    #define MEPSAN_RXADDRESS1_PTR         (  (reg8 *) MEPSAN_BUART_sRX_RxShifter_u0__D0_REG )
    #define MEPSAN_RXADDRESS2_REG         (* (reg8 *) MEPSAN_BUART_sRX_RxShifter_u0__D1_REG )
    #define MEPSAN_RXADDRESS2_PTR         (  (reg8 *) MEPSAN_BUART_sRX_RxShifter_u0__D1_REG )
    #define MEPSAN_RXDATA_AUX_CTL_REG     (* (reg8 *) MEPSAN_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define MEPSAN_RXBITCTR_PERIOD_REG    (* (reg8 *) MEPSAN_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define MEPSAN_RXBITCTR_PERIOD_PTR    (  (reg8 *) MEPSAN_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define MEPSAN_RXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) MEPSAN_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define MEPSAN_RXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) MEPSAN_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define MEPSAN_RXBITCTR_COUNTER_REG   (* (reg8 *) MEPSAN_BUART_sRX_RxBitCounter__COUNT_REG )
    #define MEPSAN_RXBITCTR_COUNTER_PTR   (  (reg8 *) MEPSAN_BUART_sRX_RxBitCounter__COUNT_REG )

    #define MEPSAN_RXSTATUS_REG           (* (reg8 *) MEPSAN_BUART_sRX_RxSts__STATUS_REG )
    #define MEPSAN_RXSTATUS_PTR           (  (reg8 *) MEPSAN_BUART_sRX_RxSts__STATUS_REG )
    #define MEPSAN_RXSTATUS_MASK_REG      (* (reg8 *) MEPSAN_BUART_sRX_RxSts__MASK_REG )
    #define MEPSAN_RXSTATUS_MASK_PTR      (  (reg8 *) MEPSAN_BUART_sRX_RxSts__MASK_REG )
    #define MEPSAN_RXSTATUS_ACTL_REG      (* (reg8 *) MEPSAN_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define MEPSAN_RXSTATUS_ACTL_PTR      (  (reg8 *) MEPSAN_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End  (MEPSAN_RX_ENABLED) || (MEPSAN_HD_ENABLED) */

#if(MEPSAN_INTERNAL_CLOCK_USED)
    /* Register to enable or disable the digital clocks */
    #define MEPSAN_INTCLOCK_CLKEN_REG     (* (reg8 *) MEPSAN_IntClock__PM_ACT_CFG)
    #define MEPSAN_INTCLOCK_CLKEN_PTR     (  (reg8 *) MEPSAN_IntClock__PM_ACT_CFG)

    /* Clock mask for this clock. */
    #define MEPSAN_INTCLOCK_CLKEN_MASK    MEPSAN_IntClock__PM_ACT_MSK
#endif /* End MEPSAN_INTERNAL_CLOCK_USED */


/***************************************
*       Register Constants
***************************************/

#if(MEPSAN_TX_ENABLED)
    #define MEPSAN_TX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End MEPSAN_TX_ENABLED */

#if(MEPSAN_HD_ENABLED)
    #define MEPSAN_TX_FIFO_CLR            (0x02u) /* FIFO1 CLR */
#endif /* End MEPSAN_HD_ENABLED */

#if( (MEPSAN_RX_ENABLED) || (MEPSAN_HD_ENABLED) )
    #define MEPSAN_RX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End  (MEPSAN_RX_ENABLED) || (MEPSAN_HD_ENABLED) */


/***************************************
* The following code is DEPRECATED and
* should not be used in new projects.
***************************************/

/* UART v2_40 obsolete definitions */
#define MEPSAN_WAIT_1_MS      MEPSAN_BL_CHK_DELAY_MS   

#define MEPSAN_TXBUFFERSIZE   MEPSAN_TX_BUFFER_SIZE
#define MEPSAN_RXBUFFERSIZE   MEPSAN_RX_BUFFER_SIZE

#if (MEPSAN_RXHW_ADDRESS_ENABLED)
    #define MEPSAN_RXADDRESSMODE  MEPSAN_RX_ADDRESS_MODE
    #define MEPSAN_RXHWADDRESS1   MEPSAN_RX_HW_ADDRESS1
    #define MEPSAN_RXHWADDRESS2   MEPSAN_RX_HW_ADDRESS2
    /* Backward compatible define */
    #define MEPSAN_RXAddressMode  MEPSAN_RXADDRESSMODE
#endif /* (MEPSAN_RXHW_ADDRESS_ENABLED) */

/* UART v2_30 obsolete definitions */
#define MEPSAN_initvar                    MEPSAN_initVar

#define MEPSAN_RX_Enabled                 MEPSAN_RX_ENABLED
#define MEPSAN_TX_Enabled                 MEPSAN_TX_ENABLED
#define MEPSAN_HD_Enabled                 MEPSAN_HD_ENABLED
#define MEPSAN_RX_IntInterruptEnabled     MEPSAN_RX_INTERRUPT_ENABLED
#define MEPSAN_TX_IntInterruptEnabled     MEPSAN_TX_INTERRUPT_ENABLED
#define MEPSAN_InternalClockUsed          MEPSAN_INTERNAL_CLOCK_USED
#define MEPSAN_RXHW_Address_Enabled       MEPSAN_RXHW_ADDRESS_ENABLED
#define MEPSAN_OverSampleCount            MEPSAN_OVER_SAMPLE_COUNT
#define MEPSAN_ParityType                 MEPSAN_PARITY_TYPE

#if( MEPSAN_TX_ENABLED && (MEPSAN_TXBUFFERSIZE > MEPSAN_FIFO_LENGTH))
    #define MEPSAN_TXBUFFER               MEPSAN_txBuffer
    #define MEPSAN_TXBUFFERREAD           MEPSAN_txBufferRead
    #define MEPSAN_TXBUFFERWRITE          MEPSAN_txBufferWrite
#endif /* End MEPSAN_TX_ENABLED */
#if( ( MEPSAN_RX_ENABLED || MEPSAN_HD_ENABLED ) && \
     (MEPSAN_RXBUFFERSIZE > MEPSAN_FIFO_LENGTH) )
    #define MEPSAN_RXBUFFER               MEPSAN_rxBuffer
    #define MEPSAN_RXBUFFERREAD           MEPSAN_rxBufferRead
    #define MEPSAN_RXBUFFERWRITE          MEPSAN_rxBufferWrite
    #define MEPSAN_RXBUFFERLOOPDETECT     MEPSAN_rxBufferLoopDetect
    #define MEPSAN_RXBUFFER_OVERFLOW      MEPSAN_rxBufferOverflow
#endif /* End MEPSAN_RX_ENABLED */

#ifdef MEPSAN_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define MEPSAN_CONTROL                MEPSAN_CONTROL_REG
#endif /* End MEPSAN_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(MEPSAN_TX_ENABLED)
    #define MEPSAN_TXDATA                 MEPSAN_TXDATA_REG
    #define MEPSAN_TXSTATUS               MEPSAN_TXSTATUS_REG
    #define MEPSAN_TXSTATUS_MASK          MEPSAN_TXSTATUS_MASK_REG
    #define MEPSAN_TXSTATUS_ACTL          MEPSAN_TXSTATUS_ACTL_REG
    /* DP clock */
    #if(MEPSAN_TXCLKGEN_DP)
        #define MEPSAN_TXBITCLKGEN_CTR        MEPSAN_TXBITCLKGEN_CTR_REG
        #define MEPSAN_TXBITCLKTX_COMPLETE    MEPSAN_TXBITCLKTX_COMPLETE_REG
    #else     /* Count7 clock*/
        #define MEPSAN_TXBITCTR_PERIOD        MEPSAN_TXBITCTR_PERIOD_REG
        #define MEPSAN_TXBITCTR_CONTROL       MEPSAN_TXBITCTR_CONTROL_REG
        #define MEPSAN_TXBITCTR_COUNTER       MEPSAN_TXBITCTR_COUNTER_REG
    #endif /* MEPSAN_TXCLKGEN_DP */
#endif /* End MEPSAN_TX_ENABLED */

#if(MEPSAN_HD_ENABLED)
    #define MEPSAN_TXDATA                 MEPSAN_TXDATA_REG
    #define MEPSAN_TXSTATUS               MEPSAN_TXSTATUS_REG
    #define MEPSAN_TXSTATUS_MASK          MEPSAN_TXSTATUS_MASK_REG
    #define MEPSAN_TXSTATUS_ACTL          MEPSAN_TXSTATUS_ACTL_REG
#endif /* End MEPSAN_HD_ENABLED */

#if( (MEPSAN_RX_ENABLED) || (MEPSAN_HD_ENABLED) )
    #define MEPSAN_RXDATA                 MEPSAN_RXDATA_REG
    #define MEPSAN_RXADDRESS1             MEPSAN_RXADDRESS1_REG
    #define MEPSAN_RXADDRESS2             MEPSAN_RXADDRESS2_REG
    #define MEPSAN_RXBITCTR_PERIOD        MEPSAN_RXBITCTR_PERIOD_REG
    #define MEPSAN_RXBITCTR_CONTROL       MEPSAN_RXBITCTR_CONTROL_REG
    #define MEPSAN_RXBITCTR_COUNTER       MEPSAN_RXBITCTR_COUNTER_REG
    #define MEPSAN_RXSTATUS               MEPSAN_RXSTATUS_REG
    #define MEPSAN_RXSTATUS_MASK          MEPSAN_RXSTATUS_MASK_REG
    #define MEPSAN_RXSTATUS_ACTL          MEPSAN_RXSTATUS_ACTL_REG
#endif /* End  (MEPSAN_RX_ENABLED) || (MEPSAN_HD_ENABLED) */

#if(MEPSAN_INTERNAL_CLOCK_USED)
    #define MEPSAN_INTCLOCK_CLKEN         MEPSAN_INTCLOCK_CLKEN_REG
#endif /* End MEPSAN_INTERNAL_CLOCK_USED */

#define MEPSAN_WAIT_FOR_COMLETE_REINIT    MEPSAN_WAIT_FOR_COMPLETE_REINIT

#endif  /* CY_UART_MEPSAN_H */


/* [] END OF FILE */
