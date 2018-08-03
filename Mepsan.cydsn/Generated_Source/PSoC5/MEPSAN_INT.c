/*******************************************************************************
* File Name: MEPSANINT.c
* Version 2.50
*
* Description:
*  This file provides all Interrupt Service functionality of the UART component
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "MEPSAN.h"
#include "cyapicallbacks.h"


/***************************************
* Custom Declarations
***************************************/
/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */

#if (MEPSAN_RX_INTERRUPT_ENABLED && (MEPSAN_RX_ENABLED || MEPSAN_HD_ENABLED))
    /*******************************************************************************
    * Function Name: MEPSAN_RXISR
    ********************************************************************************
    *
    * Summary:
    *  Interrupt Service Routine for RX portion of the UART
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  MEPSAN_rxBuffer - RAM buffer pointer for save received data.
    *  MEPSAN_rxBufferWrite - cyclic index for write to rxBuffer,
    *     increments after each byte saved to buffer.
    *  MEPSAN_rxBufferRead - cyclic index for read from rxBuffer,
    *     checked to detect overflow condition.
    *  MEPSAN_rxBufferOverflow - software overflow flag. Set to one
    *     when MEPSAN_rxBufferWrite index overtakes
    *     MEPSAN_rxBufferRead index.
    *  MEPSAN_rxBufferLoopDetect - additional variable to detect overflow.
    *     Set to one when MEPSAN_rxBufferWrite is equal to
    *    MEPSAN_rxBufferRead
    *  MEPSAN_rxAddressMode - this variable contains the Address mode,
    *     selected in customizer or set by UART_SetRxAddressMode() API.
    *  MEPSAN_rxAddressDetected - set to 1 when correct address received,
    *     and analysed to store following addressed data bytes to the buffer.
    *     When not correct address received, set to 0 to skip following data bytes.
    *
    *******************************************************************************/
    CY_ISR(MEPSAN_RXISR)
    {
        uint8 readData;
        uint8 readStatus;
        uint8 increment_pointer = 0u;

    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef MEPSAN_RXISR_ENTRY_CALLBACK
        MEPSAN_RXISR_EntryCallback();
    #endif /* MEPSAN_RXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START MEPSAN_RXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        do
        {
            /* Read receiver status register */
            readStatus = MEPSAN_RXSTATUS_REG;
            /* Copy the same status to readData variable for backward compatibility support 
            *  of the user code in MEPSAN_RXISR_ERROR` section. 
            */
            readData = readStatus;

            if((readStatus & (MEPSAN_RX_STS_BREAK | 
                            MEPSAN_RX_STS_PAR_ERROR |
                            MEPSAN_RX_STS_STOP_ERROR | 
                            MEPSAN_RX_STS_OVERRUN)) != 0u)
            {
                /* ERROR handling. */
                MEPSAN_errorStatus |= readStatus & ( MEPSAN_RX_STS_BREAK | 
                                                            MEPSAN_RX_STS_PAR_ERROR | 
                                                            MEPSAN_RX_STS_STOP_ERROR | 
                                                            MEPSAN_RX_STS_OVERRUN);
                /* `#START MEPSAN_RXISR_ERROR` */

                /* `#END` */
                
            #ifdef MEPSAN_RXISR_ERROR_CALLBACK
                MEPSAN_RXISR_ERROR_Callback();
            #endif /* MEPSAN_RXISR_ERROR_CALLBACK */
            }
            
            if((readStatus & MEPSAN_RX_STS_FIFO_NOTEMPTY) != 0u)
            {
                /* Read data from the RX data register */
                readData = MEPSAN_RXDATA_REG;
            #if (MEPSAN_RXHW_ADDRESS_ENABLED)
                if(MEPSAN_rxAddressMode == (uint8)MEPSAN__B_UART__AM_SW_DETECT_TO_BUFFER)
                {
                    if((readStatus & MEPSAN_RX_STS_MRKSPC) != 0u)
                    {
                        if ((readStatus & MEPSAN_RX_STS_ADDR_MATCH) != 0u)
                        {
                            MEPSAN_rxAddressDetected = 1u;
                        }
                        else
                        {
                            MEPSAN_rxAddressDetected = 0u;
                        }
                    }
                    if(MEPSAN_rxAddressDetected != 0u)
                    {   /* Store only addressed data */
                        MEPSAN_rxBuffer[MEPSAN_rxBufferWrite] = readData;
                        increment_pointer = 1u;
                    }
                }
                else /* Without software addressing */
                {
                    MEPSAN_rxBuffer[MEPSAN_rxBufferWrite] = readData;
                    increment_pointer = 1u;
                }
            #else  /* Without addressing */
                MEPSAN_rxBuffer[MEPSAN_rxBufferWrite] = readData;
                increment_pointer = 1u;
            #endif /* (MEPSAN_RXHW_ADDRESS_ENABLED) */

                /* Do not increment buffer pointer when skip not addressed data */
                if(increment_pointer != 0u)
                {
                    if(MEPSAN_rxBufferLoopDetect != 0u)
                    {   /* Set Software Buffer status Overflow */
                        MEPSAN_rxBufferOverflow = 1u;
                    }
                    /* Set next pointer. */
                    MEPSAN_rxBufferWrite++;

                    /* Check pointer for a loop condition */
                    if(MEPSAN_rxBufferWrite >= MEPSAN_RX_BUFFER_SIZE)
                    {
                        MEPSAN_rxBufferWrite = 0u;
                    }

                    /* Detect pre-overload condition and set flag */
                    if(MEPSAN_rxBufferWrite == MEPSAN_rxBufferRead)
                    {
                        MEPSAN_rxBufferLoopDetect = 1u;
                        /* When Hardware Flow Control selected */
                        #if (MEPSAN_FLOW_CONTROL != 0u)
                            /* Disable RX interrupt mask, it is enabled when user read data from the buffer using APIs */
                            MEPSAN_RXSTATUS_MASK_REG  &= (uint8)~MEPSAN_RX_STS_FIFO_NOTEMPTY;
                            CyIntClearPending(MEPSAN_RX_VECT_NUM);
                            break; /* Break the reading of the FIFO loop, leave the data there for generating RTS signal */
                        #endif /* (MEPSAN_FLOW_CONTROL != 0u) */
                    }
                }
            }
        }while((readStatus & MEPSAN_RX_STS_FIFO_NOTEMPTY) != 0u);

        /* User code required at end of ISR (Optional) */
        /* `#START MEPSAN_RXISR_END` */

        /* `#END` */

    #ifdef MEPSAN_RXISR_EXIT_CALLBACK
        MEPSAN_RXISR_ExitCallback();
    #endif /* MEPSAN_RXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
    }
    
#endif /* (MEPSAN_RX_INTERRUPT_ENABLED && (MEPSAN_RX_ENABLED || MEPSAN_HD_ENABLED)) */


#if (MEPSAN_TX_INTERRUPT_ENABLED && MEPSAN_TX_ENABLED)
    /*******************************************************************************
    * Function Name: MEPSAN_TXISR
    ********************************************************************************
    *
    * Summary:
    * Interrupt Service Routine for the TX portion of the UART
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  MEPSAN_txBuffer - RAM buffer pointer for transmit data from.
    *  MEPSAN_txBufferRead - cyclic index for read and transmit data
    *     from txBuffer, increments after each transmitted byte.
    *  MEPSAN_rxBufferWrite - cyclic index for write to txBuffer,
    *     checked to detect available for transmission bytes.
    *
    *******************************************************************************/
    CY_ISR(MEPSAN_TXISR)
    {
    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef MEPSAN_TXISR_ENTRY_CALLBACK
        MEPSAN_TXISR_EntryCallback();
    #endif /* MEPSAN_TXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START MEPSAN_TXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        while((MEPSAN_txBufferRead != MEPSAN_txBufferWrite) &&
             ((MEPSAN_TXSTATUS_REG & MEPSAN_TX_STS_FIFO_FULL) == 0u))
        {
            /* Check pointer wrap around */
            if(MEPSAN_txBufferRead >= MEPSAN_TX_BUFFER_SIZE)
            {
                MEPSAN_txBufferRead = 0u;
            }

            MEPSAN_TXDATA_REG = MEPSAN_txBuffer[MEPSAN_txBufferRead];

            /* Set next pointer */
            MEPSAN_txBufferRead++;
        }

        /* User code required at end of ISR (Optional) */
        /* `#START MEPSAN_TXISR_END` */

        /* `#END` */

    #ifdef MEPSAN_TXISR_EXIT_CALLBACK
        MEPSAN_TXISR_ExitCallback();
    #endif /* MEPSAN_TXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
   }
#endif /* (MEPSAN_TX_INTERRUPT_ENABLED && MEPSAN_TX_ENABLED) */


/* [] END OF FILE */
