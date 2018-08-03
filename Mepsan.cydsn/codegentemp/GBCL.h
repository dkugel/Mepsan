/*******************************************************************************
* File Name: GBCL.h
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


#if !defined(CY_UART_GBCL_H)
#define CY_UART_GBCL_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */


/***************************************
* Conditional Compilation Parameters
***************************************/

#define GBCL_RX_ENABLED                     (1u)
#define GBCL_TX_ENABLED                     (1u)
#define GBCL_HD_ENABLED                     (0u)
#define GBCL_RX_INTERRUPT_ENABLED           (1u)
#define GBCL_TX_INTERRUPT_ENABLED           (0u)
#define GBCL_INTERNAL_CLOCK_USED            (0u)
#define GBCL_RXHW_ADDRESS_ENABLED           (0u)
#define GBCL_OVER_SAMPLE_COUNT              (8u)
#define GBCL_PARITY_TYPE                    (1u)
#define GBCL_PARITY_TYPE_SW                 (0u)
#define GBCL_BREAK_DETECT                   (0u)
#define GBCL_BREAK_BITS_TX                  (13u)
#define GBCL_BREAK_BITS_RX                  (13u)
#define GBCL_TXCLKGEN_DP                    (1u)
#define GBCL_USE23POLLING                   (1u)
#define GBCL_FLOW_CONTROL                   (0u)
#define GBCL_CLK_FREQ                       (0u)
#define GBCL_TX_BUFFER_SIZE                 (4u)
#define GBCL_RX_BUFFER_SIZE                 (128u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component UART_v2_50 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#if defined(GBCL_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG)
    #define GBCL_CONTROL_REG_REMOVED            (0u)
#else
    #define GBCL_CONTROL_REG_REMOVED            (1u)
#endif /* End GBCL_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */


/***************************************
*      Data Structure Definition
***************************************/

/* Sleep Mode API Support */
typedef struct GBCL_backupStruct_
{
    uint8 enableState;

    #if(GBCL_CONTROL_REG_REMOVED == 0u)
        uint8 cr;
    #endif /* End GBCL_CONTROL_REG_REMOVED */

} GBCL_BACKUP_STRUCT;


/***************************************
*       Function Prototypes
***************************************/

void GBCL_Start(void) ;
void GBCL_Stop(void) ;
uint8 GBCL_ReadControlRegister(void) ;
void GBCL_WriteControlRegister(uint8 control) ;

void GBCL_Init(void) ;
void GBCL_Enable(void) ;
void GBCL_SaveConfig(void) ;
void GBCL_RestoreConfig(void) ;
void GBCL_Sleep(void) ;
void GBCL_Wakeup(void) ;

/* Only if RX is enabled */
#if( (GBCL_RX_ENABLED) || (GBCL_HD_ENABLED) )

    #if (GBCL_RX_INTERRUPT_ENABLED)
        #define GBCL_EnableRxInt()  CyIntEnable (GBCL_RX_VECT_NUM)
        #define GBCL_DisableRxInt() CyIntDisable(GBCL_RX_VECT_NUM)
        CY_ISR_PROTO(GBCL_RXISR);
    #endif /* GBCL_RX_INTERRUPT_ENABLED */

    void GBCL_SetRxAddressMode(uint8 addressMode)
                                                           ;
    void GBCL_SetRxAddress1(uint8 address) ;
    void GBCL_SetRxAddress2(uint8 address) ;

    void  GBCL_SetRxInterruptMode(uint8 intSrc) ;
    uint8 GBCL_ReadRxData(void) ;
    uint8 GBCL_ReadRxStatus(void) ;
    uint8 GBCL_GetChar(void) ;
    uint16 GBCL_GetByte(void) ;
    uint8 GBCL_GetRxBufferSize(void)
                                                            ;
    void GBCL_ClearRxBuffer(void) ;

    /* Obsolete functions, defines for backward compatible */
    #define GBCL_GetRxInterruptSource   GBCL_ReadRxStatus

#endif /* End (GBCL_RX_ENABLED) || (GBCL_HD_ENABLED) */

/* Only if TX is enabled */
#if(GBCL_TX_ENABLED || GBCL_HD_ENABLED)

    #if(GBCL_TX_INTERRUPT_ENABLED)
        #define GBCL_EnableTxInt()  CyIntEnable (GBCL_TX_VECT_NUM)
        #define GBCL_DisableTxInt() CyIntDisable(GBCL_TX_VECT_NUM)
        #define GBCL_SetPendingTxInt() CyIntSetPending(GBCL_TX_VECT_NUM)
        #define GBCL_ClearPendingTxInt() CyIntClearPending(GBCL_TX_VECT_NUM)
        CY_ISR_PROTO(GBCL_TXISR);
    #endif /* GBCL_TX_INTERRUPT_ENABLED */

    void GBCL_SetTxInterruptMode(uint8 intSrc) ;
    void GBCL_WriteTxData(uint8 txDataByte) ;
    uint8 GBCL_ReadTxStatus(void) ;
    void GBCL_PutChar(uint8 txDataByte) ;
    void GBCL_PutString(const char8 string[]) ;
    void GBCL_PutArray(const uint8 string[], uint8 byteCount)
                                                            ;
    void GBCL_PutCRLF(uint8 txDataByte) ;
    void GBCL_ClearTxBuffer(void) ;
    void GBCL_SetTxAddressMode(uint8 addressMode) ;
    void GBCL_SendBreak(uint8 retMode) ;
    uint8 GBCL_GetTxBufferSize(void)
                                                            ;
    /* Obsolete functions, defines for backward compatible */
    #define GBCL_PutStringConst         GBCL_PutString
    #define GBCL_PutArrayConst          GBCL_PutArray
    #define GBCL_GetTxInterruptSource   GBCL_ReadTxStatus

#endif /* End GBCL_TX_ENABLED || GBCL_HD_ENABLED */

#if(GBCL_HD_ENABLED)
    void GBCL_LoadRxConfig(void) ;
    void GBCL_LoadTxConfig(void) ;
#endif /* End GBCL_HD_ENABLED */


/* Communication bootloader APIs */
#if defined(CYDEV_BOOTLOADER_IO_COMP) && ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_GBCL) || \
                                          (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))
    /* Physical layer functions */
    void    GBCL_CyBtldrCommStart(void) CYSMALL ;
    void    GBCL_CyBtldrCommStop(void) CYSMALL ;
    void    GBCL_CyBtldrCommReset(void) CYSMALL ;
    cystatus GBCL_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;
    cystatus GBCL_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;

    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_GBCL)
        #define CyBtldrCommStart    GBCL_CyBtldrCommStart
        #define CyBtldrCommStop     GBCL_CyBtldrCommStop
        #define CyBtldrCommReset    GBCL_CyBtldrCommReset
        #define CyBtldrCommWrite    GBCL_CyBtldrCommWrite
        #define CyBtldrCommRead     GBCL_CyBtldrCommRead
    #endif  /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_GBCL) */

    /* Byte to Byte time out for detecting end of block data from host */
    #define GBCL_BYTE2BYTE_TIME_OUT (25u)
    #define GBCL_PACKET_EOP         (0x17u) /* End of packet defined by bootloader */
    #define GBCL_WAIT_EOP_DELAY     (5u)    /* Additional 5ms to wait for End of packet */
    #define GBCL_BL_CHK_DELAY_MS    (1u)    /* Time Out quantity equal 1mS */

#endif /* CYDEV_BOOTLOADER_IO_COMP */


/***************************************
*          API Constants
***************************************/
/* Parameters for SetTxAddressMode API*/
#define GBCL_SET_SPACE      (0x00u)
#define GBCL_SET_MARK       (0x01u)

/* Status Register definitions */
#if( (GBCL_TX_ENABLED) || (GBCL_HD_ENABLED) )
    #if(GBCL_TX_INTERRUPT_ENABLED)
        #define GBCL_TX_VECT_NUM            (uint8)GBCL_TXInternalInterrupt__INTC_NUMBER
        #define GBCL_TX_PRIOR_NUM           (uint8)GBCL_TXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* GBCL_TX_INTERRUPT_ENABLED */

    #define GBCL_TX_STS_COMPLETE_SHIFT          (0x00u)
    #define GBCL_TX_STS_FIFO_EMPTY_SHIFT        (0x01u)
    #define GBCL_TX_STS_FIFO_NOT_FULL_SHIFT     (0x03u)
    #if(GBCL_TX_ENABLED)
        #define GBCL_TX_STS_FIFO_FULL_SHIFT     (0x02u)
    #else /* (GBCL_HD_ENABLED) */
        #define GBCL_TX_STS_FIFO_FULL_SHIFT     (0x05u)  /* Needs MD=0 */
    #endif /* (GBCL_TX_ENABLED) */

    #define GBCL_TX_STS_COMPLETE            (uint8)(0x01u << GBCL_TX_STS_COMPLETE_SHIFT)
    #define GBCL_TX_STS_FIFO_EMPTY          (uint8)(0x01u << GBCL_TX_STS_FIFO_EMPTY_SHIFT)
    #define GBCL_TX_STS_FIFO_FULL           (uint8)(0x01u << GBCL_TX_STS_FIFO_FULL_SHIFT)
    #define GBCL_TX_STS_FIFO_NOT_FULL       (uint8)(0x01u << GBCL_TX_STS_FIFO_NOT_FULL_SHIFT)
#endif /* End (GBCL_TX_ENABLED) || (GBCL_HD_ENABLED)*/

#if( (GBCL_RX_ENABLED) || (GBCL_HD_ENABLED) )
    #if(GBCL_RX_INTERRUPT_ENABLED)
        #define GBCL_RX_VECT_NUM            (uint8)GBCL_RXInternalInterrupt__INTC_NUMBER
        #define GBCL_RX_PRIOR_NUM           (uint8)GBCL_RXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* GBCL_RX_INTERRUPT_ENABLED */
    #define GBCL_RX_STS_MRKSPC_SHIFT            (0x00u)
    #define GBCL_RX_STS_BREAK_SHIFT             (0x01u)
    #define GBCL_RX_STS_PAR_ERROR_SHIFT         (0x02u)
    #define GBCL_RX_STS_STOP_ERROR_SHIFT        (0x03u)
    #define GBCL_RX_STS_OVERRUN_SHIFT           (0x04u)
    #define GBCL_RX_STS_FIFO_NOTEMPTY_SHIFT     (0x05u)
    #define GBCL_RX_STS_ADDR_MATCH_SHIFT        (0x06u)
    #define GBCL_RX_STS_SOFT_BUFF_OVER_SHIFT    (0x07u)

    #define GBCL_RX_STS_MRKSPC           (uint8)(0x01u << GBCL_RX_STS_MRKSPC_SHIFT)
    #define GBCL_RX_STS_BREAK            (uint8)(0x01u << GBCL_RX_STS_BREAK_SHIFT)
    #define GBCL_RX_STS_PAR_ERROR        (uint8)(0x01u << GBCL_RX_STS_PAR_ERROR_SHIFT)
    #define GBCL_RX_STS_STOP_ERROR       (uint8)(0x01u << GBCL_RX_STS_STOP_ERROR_SHIFT)
    #define GBCL_RX_STS_OVERRUN          (uint8)(0x01u << GBCL_RX_STS_OVERRUN_SHIFT)
    #define GBCL_RX_STS_FIFO_NOTEMPTY    (uint8)(0x01u << GBCL_RX_STS_FIFO_NOTEMPTY_SHIFT)
    #define GBCL_RX_STS_ADDR_MATCH       (uint8)(0x01u << GBCL_RX_STS_ADDR_MATCH_SHIFT)
    #define GBCL_RX_STS_SOFT_BUFF_OVER   (uint8)(0x01u << GBCL_RX_STS_SOFT_BUFF_OVER_SHIFT)
    #define GBCL_RX_HW_MASK                     (0x7Fu)
#endif /* End (GBCL_RX_ENABLED) || (GBCL_HD_ENABLED) */

/* Control Register definitions */
#define GBCL_CTRL_HD_SEND_SHIFT                 (0x00u) /* 1 enable TX part in Half Duplex mode */
#define GBCL_CTRL_HD_SEND_BREAK_SHIFT           (0x01u) /* 1 send BREAK signal in Half Duplez mode */
#define GBCL_CTRL_MARK_SHIFT                    (0x02u) /* 1 sets mark, 0 sets space */
#define GBCL_CTRL_PARITY_TYPE0_SHIFT            (0x03u) /* Defines the type of parity implemented */
#define GBCL_CTRL_PARITY_TYPE1_SHIFT            (0x04u) /* Defines the type of parity implemented */
#define GBCL_CTRL_RXADDR_MODE0_SHIFT            (0x05u)
#define GBCL_CTRL_RXADDR_MODE1_SHIFT            (0x06u)
#define GBCL_CTRL_RXADDR_MODE2_SHIFT            (0x07u)

#define GBCL_CTRL_HD_SEND               (uint8)(0x01u << GBCL_CTRL_HD_SEND_SHIFT)
#define GBCL_CTRL_HD_SEND_BREAK         (uint8)(0x01u << GBCL_CTRL_HD_SEND_BREAK_SHIFT)
#define GBCL_CTRL_MARK                  (uint8)(0x01u << GBCL_CTRL_MARK_SHIFT)
#define GBCL_CTRL_PARITY_TYPE_MASK      (uint8)(0x03u << GBCL_CTRL_PARITY_TYPE0_SHIFT)
#define GBCL_CTRL_RXADDR_MODE_MASK      (uint8)(0x07u << GBCL_CTRL_RXADDR_MODE0_SHIFT)

/* StatusI Register Interrupt Enable Control Bits. As defined by the Register map for the AUX Control Register */
#define GBCL_INT_ENABLE                         (0x10u)

/* Bit Counter (7-bit) Control Register Bit Definitions. As defined by the Register map for the AUX Control Register */
#define GBCL_CNTR_ENABLE                        (0x20u)

/*   Constants for SendBreak() "retMode" parameter  */
#define GBCL_SEND_BREAK                         (0x00u)
#define GBCL_WAIT_FOR_COMPLETE_REINIT           (0x01u)
#define GBCL_REINIT                             (0x02u)
#define GBCL_SEND_WAIT_REINIT                   (0x03u)

#define GBCL_OVER_SAMPLE_8                      (8u)
#define GBCL_OVER_SAMPLE_16                     (16u)

#define GBCL_BIT_CENTER                         (GBCL_OVER_SAMPLE_COUNT - 2u)

#define GBCL_FIFO_LENGTH                        (4u)
#define GBCL_NUMBER_OF_START_BIT                (1u)
#define GBCL_MAX_BYTE_VALUE                     (0xFFu)

/* 8X always for count7 implementation */
#define GBCL_TXBITCTR_BREAKBITS8X   ((GBCL_BREAK_BITS_TX * GBCL_OVER_SAMPLE_8) - 1u)
/* 8X or 16X for DP implementation */
#define GBCL_TXBITCTR_BREAKBITS ((GBCL_BREAK_BITS_TX * GBCL_OVER_SAMPLE_COUNT) - 1u)

#define GBCL_HALF_BIT_COUNT   \
                            (((GBCL_OVER_SAMPLE_COUNT / 2u) + (GBCL_USE23POLLING * 1u)) - 2u)
#if (GBCL_OVER_SAMPLE_COUNT == GBCL_OVER_SAMPLE_8)
    #define GBCL_HD_TXBITCTR_INIT   (((GBCL_BREAK_BITS_TX + \
                            GBCL_NUMBER_OF_START_BIT) * GBCL_OVER_SAMPLE_COUNT) - 1u)

    /* This parameter is increased on the 2 in 2 out of 3 mode to sample voting in the middle */
    #define GBCL_RXBITCTR_INIT  ((((GBCL_BREAK_BITS_RX + GBCL_NUMBER_OF_START_BIT) \
                            * GBCL_OVER_SAMPLE_COUNT) + GBCL_HALF_BIT_COUNT) - 1u)

#else /* GBCL_OVER_SAMPLE_COUNT == GBCL_OVER_SAMPLE_16 */
    #define GBCL_HD_TXBITCTR_INIT   ((8u * GBCL_OVER_SAMPLE_COUNT) - 1u)
    /* 7bit counter need one more bit for OverSampleCount = 16 */
    #define GBCL_RXBITCTR_INIT      (((7u * GBCL_OVER_SAMPLE_COUNT) - 1u) + \
                                                      GBCL_HALF_BIT_COUNT)
#endif /* End GBCL_OVER_SAMPLE_COUNT */

#define GBCL_HD_RXBITCTR_INIT                   GBCL_RXBITCTR_INIT


/***************************************
* Global variables external identifier
***************************************/

extern uint8 GBCL_initVar;
#if (GBCL_TX_INTERRUPT_ENABLED && GBCL_TX_ENABLED)
    extern volatile uint8 GBCL_txBuffer[GBCL_TX_BUFFER_SIZE];
    extern volatile uint8 GBCL_txBufferRead;
    extern uint8 GBCL_txBufferWrite;
#endif /* (GBCL_TX_INTERRUPT_ENABLED && GBCL_TX_ENABLED) */
#if (GBCL_RX_INTERRUPT_ENABLED && (GBCL_RX_ENABLED || GBCL_HD_ENABLED))
    extern uint8 GBCL_errorStatus;
    extern volatile uint8 GBCL_rxBuffer[GBCL_RX_BUFFER_SIZE];
    extern volatile uint8 GBCL_rxBufferRead;
    extern volatile uint8 GBCL_rxBufferWrite;
    extern volatile uint8 GBCL_rxBufferLoopDetect;
    extern volatile uint8 GBCL_rxBufferOverflow;
    #if (GBCL_RXHW_ADDRESS_ENABLED)
        extern volatile uint8 GBCL_rxAddressMode;
        extern volatile uint8 GBCL_rxAddressDetected;
    #endif /* (GBCL_RXHW_ADDRESS_ENABLED) */
#endif /* (GBCL_RX_INTERRUPT_ENABLED && (GBCL_RX_ENABLED || GBCL_HD_ENABLED)) */


/***************************************
* Enumerated Types and Parameters
***************************************/

#define GBCL__B_UART__AM_SW_BYTE_BYTE 1
#define GBCL__B_UART__AM_SW_DETECT_TO_BUFFER 2
#define GBCL__B_UART__AM_HW_BYTE_BY_BYTE 3
#define GBCL__B_UART__AM_HW_DETECT_TO_BUFFER 4
#define GBCL__B_UART__AM_NONE 0

#define GBCL__B_UART__NONE_REVB 0
#define GBCL__B_UART__EVEN_REVB 1
#define GBCL__B_UART__ODD_REVB 2
#define GBCL__B_UART__MARK_SPACE_REVB 3



/***************************************
*    Initial Parameter Constants
***************************************/

/* UART shifts max 8 bits, Mark/Space functionality working if 9 selected */
#define GBCL_NUMBER_OF_DATA_BITS    ((8u > 8u) ? 8u : 8u)
#define GBCL_NUMBER_OF_STOP_BITS    (1u)

#if (GBCL_RXHW_ADDRESS_ENABLED)
    #define GBCL_RX_ADDRESS_MODE    (0u)
    #define GBCL_RX_HW_ADDRESS1     (0u)
    #define GBCL_RX_HW_ADDRESS2     (0u)
#endif /* (GBCL_RXHW_ADDRESS_ENABLED) */

#define GBCL_INIT_RX_INTERRUPTS_MASK \
                                  (uint8)((1 << GBCL_RX_STS_FIFO_NOTEMPTY_SHIFT) \
                                        | (0 << GBCL_RX_STS_MRKSPC_SHIFT) \
                                        | (0 << GBCL_RX_STS_ADDR_MATCH_SHIFT) \
                                        | (0 << GBCL_RX_STS_PAR_ERROR_SHIFT) \
                                        | (0 << GBCL_RX_STS_STOP_ERROR_SHIFT) \
                                        | (0 << GBCL_RX_STS_BREAK_SHIFT) \
                                        | (0 << GBCL_RX_STS_OVERRUN_SHIFT))

#define GBCL_INIT_TX_INTERRUPTS_MASK \
                                  (uint8)((0 << GBCL_TX_STS_COMPLETE_SHIFT) \
                                        | (0 << GBCL_TX_STS_FIFO_EMPTY_SHIFT) \
                                        | (0 << GBCL_TX_STS_FIFO_FULL_SHIFT) \
                                        | (0 << GBCL_TX_STS_FIFO_NOT_FULL_SHIFT))


/***************************************
*              Registers
***************************************/

#ifdef GBCL_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define GBCL_CONTROL_REG \
                            (* (reg8 *) GBCL_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
    #define GBCL_CONTROL_PTR \
                            (  (reg8 *) GBCL_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
#endif /* End GBCL_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(GBCL_TX_ENABLED)
    #define GBCL_TXDATA_REG          (* (reg8 *) GBCL_BUART_sTX_TxShifter_u0__F0_REG)
    #define GBCL_TXDATA_PTR          (  (reg8 *) GBCL_BUART_sTX_TxShifter_u0__F0_REG)
    #define GBCL_TXDATA_AUX_CTL_REG  (* (reg8 *) GBCL_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define GBCL_TXDATA_AUX_CTL_PTR  (  (reg8 *) GBCL_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define GBCL_TXSTATUS_REG        (* (reg8 *) GBCL_BUART_sTX_TxSts__STATUS_REG)
    #define GBCL_TXSTATUS_PTR        (  (reg8 *) GBCL_BUART_sTX_TxSts__STATUS_REG)
    #define GBCL_TXSTATUS_MASK_REG   (* (reg8 *) GBCL_BUART_sTX_TxSts__MASK_REG)
    #define GBCL_TXSTATUS_MASK_PTR   (  (reg8 *) GBCL_BUART_sTX_TxSts__MASK_REG)
    #define GBCL_TXSTATUS_ACTL_REG   (* (reg8 *) GBCL_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)
    #define GBCL_TXSTATUS_ACTL_PTR   (  (reg8 *) GBCL_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)

    /* DP clock */
    #if(GBCL_TXCLKGEN_DP)
        #define GBCL_TXBITCLKGEN_CTR_REG        \
                                        (* (reg8 *) GBCL_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define GBCL_TXBITCLKGEN_CTR_PTR        \
                                        (  (reg8 *) GBCL_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define GBCL_TXBITCLKTX_COMPLETE_REG    \
                                        (* (reg8 *) GBCL_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
        #define GBCL_TXBITCLKTX_COMPLETE_PTR    \
                                        (  (reg8 *) GBCL_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
    #else     /* Count7 clock*/
        #define GBCL_TXBITCTR_PERIOD_REG    \
                                        (* (reg8 *) GBCL_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define GBCL_TXBITCTR_PERIOD_PTR    \
                                        (  (reg8 *) GBCL_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define GBCL_TXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) GBCL_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define GBCL_TXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) GBCL_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define GBCL_TXBITCTR_COUNTER_REG   \
                                        (* (reg8 *) GBCL_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
        #define GBCL_TXBITCTR_COUNTER_PTR   \
                                        (  (reg8 *) GBCL_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
    #endif /* GBCL_TXCLKGEN_DP */

#endif /* End GBCL_TX_ENABLED */

#if(GBCL_HD_ENABLED)

    #define GBCL_TXDATA_REG             (* (reg8 *) GBCL_BUART_sRX_RxShifter_u0__F1_REG )
    #define GBCL_TXDATA_PTR             (  (reg8 *) GBCL_BUART_sRX_RxShifter_u0__F1_REG )
    #define GBCL_TXDATA_AUX_CTL_REG     (* (reg8 *) GBCL_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)
    #define GBCL_TXDATA_AUX_CTL_PTR     (  (reg8 *) GBCL_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define GBCL_TXSTATUS_REG           (* (reg8 *) GBCL_BUART_sRX_RxSts__STATUS_REG )
    #define GBCL_TXSTATUS_PTR           (  (reg8 *) GBCL_BUART_sRX_RxSts__STATUS_REG )
    #define GBCL_TXSTATUS_MASK_REG      (* (reg8 *) GBCL_BUART_sRX_RxSts__MASK_REG )
    #define GBCL_TXSTATUS_MASK_PTR      (  (reg8 *) GBCL_BUART_sRX_RxSts__MASK_REG )
    #define GBCL_TXSTATUS_ACTL_REG      (* (reg8 *) GBCL_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define GBCL_TXSTATUS_ACTL_PTR      (  (reg8 *) GBCL_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End GBCL_HD_ENABLED */

#if( (GBCL_RX_ENABLED) || (GBCL_HD_ENABLED) )
    #define GBCL_RXDATA_REG             (* (reg8 *) GBCL_BUART_sRX_RxShifter_u0__F0_REG )
    #define GBCL_RXDATA_PTR             (  (reg8 *) GBCL_BUART_sRX_RxShifter_u0__F0_REG )
    #define GBCL_RXADDRESS1_REG         (* (reg8 *) GBCL_BUART_sRX_RxShifter_u0__D0_REG )
    #define GBCL_RXADDRESS1_PTR         (  (reg8 *) GBCL_BUART_sRX_RxShifter_u0__D0_REG )
    #define GBCL_RXADDRESS2_REG         (* (reg8 *) GBCL_BUART_sRX_RxShifter_u0__D1_REG )
    #define GBCL_RXADDRESS2_PTR         (  (reg8 *) GBCL_BUART_sRX_RxShifter_u0__D1_REG )
    #define GBCL_RXDATA_AUX_CTL_REG     (* (reg8 *) GBCL_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define GBCL_RXBITCTR_PERIOD_REG    (* (reg8 *) GBCL_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define GBCL_RXBITCTR_PERIOD_PTR    (  (reg8 *) GBCL_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define GBCL_RXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) GBCL_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define GBCL_RXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) GBCL_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define GBCL_RXBITCTR_COUNTER_REG   (* (reg8 *) GBCL_BUART_sRX_RxBitCounter__COUNT_REG )
    #define GBCL_RXBITCTR_COUNTER_PTR   (  (reg8 *) GBCL_BUART_sRX_RxBitCounter__COUNT_REG )

    #define GBCL_RXSTATUS_REG           (* (reg8 *) GBCL_BUART_sRX_RxSts__STATUS_REG )
    #define GBCL_RXSTATUS_PTR           (  (reg8 *) GBCL_BUART_sRX_RxSts__STATUS_REG )
    #define GBCL_RXSTATUS_MASK_REG      (* (reg8 *) GBCL_BUART_sRX_RxSts__MASK_REG )
    #define GBCL_RXSTATUS_MASK_PTR      (  (reg8 *) GBCL_BUART_sRX_RxSts__MASK_REG )
    #define GBCL_RXSTATUS_ACTL_REG      (* (reg8 *) GBCL_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define GBCL_RXSTATUS_ACTL_PTR      (  (reg8 *) GBCL_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End  (GBCL_RX_ENABLED) || (GBCL_HD_ENABLED) */

#if(GBCL_INTERNAL_CLOCK_USED)
    /* Register to enable or disable the digital clocks */
    #define GBCL_INTCLOCK_CLKEN_REG     (* (reg8 *) GBCL_IntClock__PM_ACT_CFG)
    #define GBCL_INTCLOCK_CLKEN_PTR     (  (reg8 *) GBCL_IntClock__PM_ACT_CFG)

    /* Clock mask for this clock. */
    #define GBCL_INTCLOCK_CLKEN_MASK    GBCL_IntClock__PM_ACT_MSK
#endif /* End GBCL_INTERNAL_CLOCK_USED */


/***************************************
*       Register Constants
***************************************/

#if(GBCL_TX_ENABLED)
    #define GBCL_TX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End GBCL_TX_ENABLED */

#if(GBCL_HD_ENABLED)
    #define GBCL_TX_FIFO_CLR            (0x02u) /* FIFO1 CLR */
#endif /* End GBCL_HD_ENABLED */

#if( (GBCL_RX_ENABLED) || (GBCL_HD_ENABLED) )
    #define GBCL_RX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End  (GBCL_RX_ENABLED) || (GBCL_HD_ENABLED) */


/***************************************
* The following code is DEPRECATED and
* should not be used in new projects.
***************************************/

/* UART v2_40 obsolete definitions */
#define GBCL_WAIT_1_MS      GBCL_BL_CHK_DELAY_MS   

#define GBCL_TXBUFFERSIZE   GBCL_TX_BUFFER_SIZE
#define GBCL_RXBUFFERSIZE   GBCL_RX_BUFFER_SIZE

#if (GBCL_RXHW_ADDRESS_ENABLED)
    #define GBCL_RXADDRESSMODE  GBCL_RX_ADDRESS_MODE
    #define GBCL_RXHWADDRESS1   GBCL_RX_HW_ADDRESS1
    #define GBCL_RXHWADDRESS2   GBCL_RX_HW_ADDRESS2
    /* Backward compatible define */
    #define GBCL_RXAddressMode  GBCL_RXADDRESSMODE
#endif /* (GBCL_RXHW_ADDRESS_ENABLED) */

/* UART v2_30 obsolete definitions */
#define GBCL_initvar                    GBCL_initVar

#define GBCL_RX_Enabled                 GBCL_RX_ENABLED
#define GBCL_TX_Enabled                 GBCL_TX_ENABLED
#define GBCL_HD_Enabled                 GBCL_HD_ENABLED
#define GBCL_RX_IntInterruptEnabled     GBCL_RX_INTERRUPT_ENABLED
#define GBCL_TX_IntInterruptEnabled     GBCL_TX_INTERRUPT_ENABLED
#define GBCL_InternalClockUsed          GBCL_INTERNAL_CLOCK_USED
#define GBCL_RXHW_Address_Enabled       GBCL_RXHW_ADDRESS_ENABLED
#define GBCL_OverSampleCount            GBCL_OVER_SAMPLE_COUNT
#define GBCL_ParityType                 GBCL_PARITY_TYPE

#if( GBCL_TX_ENABLED && (GBCL_TXBUFFERSIZE > GBCL_FIFO_LENGTH))
    #define GBCL_TXBUFFER               GBCL_txBuffer
    #define GBCL_TXBUFFERREAD           GBCL_txBufferRead
    #define GBCL_TXBUFFERWRITE          GBCL_txBufferWrite
#endif /* End GBCL_TX_ENABLED */
#if( ( GBCL_RX_ENABLED || GBCL_HD_ENABLED ) && \
     (GBCL_RXBUFFERSIZE > GBCL_FIFO_LENGTH) )
    #define GBCL_RXBUFFER               GBCL_rxBuffer
    #define GBCL_RXBUFFERREAD           GBCL_rxBufferRead
    #define GBCL_RXBUFFERWRITE          GBCL_rxBufferWrite
    #define GBCL_RXBUFFERLOOPDETECT     GBCL_rxBufferLoopDetect
    #define GBCL_RXBUFFER_OVERFLOW      GBCL_rxBufferOverflow
#endif /* End GBCL_RX_ENABLED */

#ifdef GBCL_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define GBCL_CONTROL                GBCL_CONTROL_REG
#endif /* End GBCL_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(GBCL_TX_ENABLED)
    #define GBCL_TXDATA                 GBCL_TXDATA_REG
    #define GBCL_TXSTATUS               GBCL_TXSTATUS_REG
    #define GBCL_TXSTATUS_MASK          GBCL_TXSTATUS_MASK_REG
    #define GBCL_TXSTATUS_ACTL          GBCL_TXSTATUS_ACTL_REG
    /* DP clock */
    #if(GBCL_TXCLKGEN_DP)
        #define GBCL_TXBITCLKGEN_CTR        GBCL_TXBITCLKGEN_CTR_REG
        #define GBCL_TXBITCLKTX_COMPLETE    GBCL_TXBITCLKTX_COMPLETE_REG
    #else     /* Count7 clock*/
        #define GBCL_TXBITCTR_PERIOD        GBCL_TXBITCTR_PERIOD_REG
        #define GBCL_TXBITCTR_CONTROL       GBCL_TXBITCTR_CONTROL_REG
        #define GBCL_TXBITCTR_COUNTER       GBCL_TXBITCTR_COUNTER_REG
    #endif /* GBCL_TXCLKGEN_DP */
#endif /* End GBCL_TX_ENABLED */

#if(GBCL_HD_ENABLED)
    #define GBCL_TXDATA                 GBCL_TXDATA_REG
    #define GBCL_TXSTATUS               GBCL_TXSTATUS_REG
    #define GBCL_TXSTATUS_MASK          GBCL_TXSTATUS_MASK_REG
    #define GBCL_TXSTATUS_ACTL          GBCL_TXSTATUS_ACTL_REG
#endif /* End GBCL_HD_ENABLED */

#if( (GBCL_RX_ENABLED) || (GBCL_HD_ENABLED) )
    #define GBCL_RXDATA                 GBCL_RXDATA_REG
    #define GBCL_RXADDRESS1             GBCL_RXADDRESS1_REG
    #define GBCL_RXADDRESS2             GBCL_RXADDRESS2_REG
    #define GBCL_RXBITCTR_PERIOD        GBCL_RXBITCTR_PERIOD_REG
    #define GBCL_RXBITCTR_CONTROL       GBCL_RXBITCTR_CONTROL_REG
    #define GBCL_RXBITCTR_COUNTER       GBCL_RXBITCTR_COUNTER_REG
    #define GBCL_RXSTATUS               GBCL_RXSTATUS_REG
    #define GBCL_RXSTATUS_MASK          GBCL_RXSTATUS_MASK_REG
    #define GBCL_RXSTATUS_ACTL          GBCL_RXSTATUS_ACTL_REG
#endif /* End  (GBCL_RX_ENABLED) || (GBCL_HD_ENABLED) */

#if(GBCL_INTERNAL_CLOCK_USED)
    #define GBCL_INTCLOCK_CLKEN         GBCL_INTCLOCK_CLKEN_REG
#endif /* End GBCL_INTERNAL_CLOCK_USED */

#define GBCL_WAIT_FOR_COMLETE_REINIT    GBCL_WAIT_FOR_COMPLETE_REINIT

#endif  /* CY_UART_GBCL_H */


/* [] END OF FILE */
