/*******************************************************************************
* File Name: PRINTER_BINT.c
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

#include "PRINTER_B.h"
#include "cyapicallbacks.h"


/***************************************
* Custom Declarations
***************************************/
/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */

#if (PRINTER_B_RX_INTERRUPT_ENABLED && (PRINTER_B_RX_ENABLED || PRINTER_B_HD_ENABLED))
    /*******************************************************************************
    * Function Name: PRINTER_B_RXISR
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
    *  PRINTER_B_rxBuffer - RAM buffer pointer for save received data.
    *  PRINTER_B_rxBufferWrite - cyclic index for write to rxBuffer,
    *     increments after each byte saved to buffer.
    *  PRINTER_B_rxBufferRead - cyclic index for read from rxBuffer,
    *     checked to detect overflow condition.
    *  PRINTER_B_rxBufferOverflow - software overflow flag. Set to one
    *     when PRINTER_B_rxBufferWrite index overtakes
    *     PRINTER_B_rxBufferRead index.
    *  PRINTER_B_rxBufferLoopDetect - additional variable to detect overflow.
    *     Set to one when PRINTER_B_rxBufferWrite is equal to
    *    PRINTER_B_rxBufferRead
    *  PRINTER_B_rxAddressMode - this variable contains the Address mode,
    *     selected in customizer or set by UART_SetRxAddressMode() API.
    *  PRINTER_B_rxAddressDetected - set to 1 when correct address received,
    *     and analysed to store following addressed data bytes to the buffer.
    *     When not correct address received, set to 0 to skip following data bytes.
    *
    *******************************************************************************/
    CY_ISR(PRINTER_B_RXISR)
    {
        uint8 readData;
        uint8 readStatus;
        uint8 increment_pointer = 0u;

    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef PRINTER_B_RXISR_ENTRY_CALLBACK
        PRINTER_B_RXISR_EntryCallback();
    #endif /* PRINTER_B_RXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START PRINTER_B_RXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        do
        {
            /* Read receiver status register */
            readStatus = PRINTER_B_RXSTATUS_REG;
            /* Copy the same status to readData variable for backward compatibility support 
            *  of the user code in PRINTER_B_RXISR_ERROR` section. 
            */
            readData = readStatus;

            if((readStatus & (PRINTER_B_RX_STS_BREAK | 
                            PRINTER_B_RX_STS_PAR_ERROR |
                            PRINTER_B_RX_STS_STOP_ERROR | 
                            PRINTER_B_RX_STS_OVERRUN)) != 0u)
            {
                /* ERROR handling. */
                PRINTER_B_errorStatus |= readStatus & ( PRINTER_B_RX_STS_BREAK | 
                                                            PRINTER_B_RX_STS_PAR_ERROR | 
                                                            PRINTER_B_RX_STS_STOP_ERROR | 
                                                            PRINTER_B_RX_STS_OVERRUN);
                /* `#START PRINTER_B_RXISR_ERROR` */

                /* `#END` */
                
            #ifdef PRINTER_B_RXISR_ERROR_CALLBACK
                PRINTER_B_RXISR_ERROR_Callback();
            #endif /* PRINTER_B_RXISR_ERROR_CALLBACK */
            }
            
            if((readStatus & PRINTER_B_RX_STS_FIFO_NOTEMPTY) != 0u)
            {
                /* Read data from the RX data register */
                readData = PRINTER_B_RXDATA_REG;
            #if (PRINTER_B_RXHW_ADDRESS_ENABLED)
                if(PRINTER_B_rxAddressMode == (uint8)PRINTER_B__B_UART__AM_SW_DETECT_TO_BUFFER)
                {
                    if((readStatus & PRINTER_B_RX_STS_MRKSPC) != 0u)
                    {
                        if ((readStatus & PRINTER_B_RX_STS_ADDR_MATCH) != 0u)
                        {
                            PRINTER_B_rxAddressDetected = 1u;
                        }
                        else
                        {
                            PRINTER_B_rxAddressDetected = 0u;
                        }
                    }
                    if(PRINTER_B_rxAddressDetected != 0u)
                    {   /* Store only addressed data */
                        PRINTER_B_rxBuffer[PRINTER_B_rxBufferWrite] = readData;
                        increment_pointer = 1u;
                    }
                }
                else /* Without software addressing */
                {
                    PRINTER_B_rxBuffer[PRINTER_B_rxBufferWrite] = readData;
                    increment_pointer = 1u;
                }
            #else  /* Without addressing */
                PRINTER_B_rxBuffer[PRINTER_B_rxBufferWrite] = readData;
                increment_pointer = 1u;
            #endif /* (PRINTER_B_RXHW_ADDRESS_ENABLED) */

                /* Do not increment buffer pointer when skip not addressed data */
                if(increment_pointer != 0u)
                {
                    if(PRINTER_B_rxBufferLoopDetect != 0u)
                    {   /* Set Software Buffer status Overflow */
                        PRINTER_B_rxBufferOverflow = 1u;
                    }
                    /* Set next pointer. */
                    PRINTER_B_rxBufferWrite++;

                    /* Check pointer for a loop condition */
                    if(PRINTER_B_rxBufferWrite >= PRINTER_B_RX_BUFFER_SIZE)
                    {
                        PRINTER_B_rxBufferWrite = 0u;
                    }

                    /* Detect pre-overload condition and set flag */
                    if(PRINTER_B_rxBufferWrite == PRINTER_B_rxBufferRead)
                    {
                        PRINTER_B_rxBufferLoopDetect = 1u;
                        /* When Hardware Flow Control selected */
                        #if (PRINTER_B_FLOW_CONTROL != 0u)
                            /* Disable RX interrupt mask, it is enabled when user read data from the buffer using APIs */
                            PRINTER_B_RXSTATUS_MASK_REG  &= (uint8)~PRINTER_B_RX_STS_FIFO_NOTEMPTY;
                            CyIntClearPending(PRINTER_B_RX_VECT_NUM);
                            break; /* Break the reading of the FIFO loop, leave the data there for generating RTS signal */
                        #endif /* (PRINTER_B_FLOW_CONTROL != 0u) */
                    }
                }
            }
        }while((readStatus & PRINTER_B_RX_STS_FIFO_NOTEMPTY) != 0u);

        /* User code required at end of ISR (Optional) */
        /* `#START PRINTER_B_RXISR_END` */

        /* `#END` */

    #ifdef PRINTER_B_RXISR_EXIT_CALLBACK
        PRINTER_B_RXISR_ExitCallback();
    #endif /* PRINTER_B_RXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
    }
    
#endif /* (PRINTER_B_RX_INTERRUPT_ENABLED && (PRINTER_B_RX_ENABLED || PRINTER_B_HD_ENABLED)) */


#if (PRINTER_B_TX_INTERRUPT_ENABLED && PRINTER_B_TX_ENABLED)
    /*******************************************************************************
    * Function Name: PRINTER_B_TXISR
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
    *  PRINTER_B_txBuffer - RAM buffer pointer for transmit data from.
    *  PRINTER_B_txBufferRead - cyclic index for read and transmit data
    *     from txBuffer, increments after each transmitted byte.
    *  PRINTER_B_rxBufferWrite - cyclic index for write to txBuffer,
    *     checked to detect available for transmission bytes.
    *
    *******************************************************************************/
    CY_ISR(PRINTER_B_TXISR)
    {
    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef PRINTER_B_TXISR_ENTRY_CALLBACK
        PRINTER_B_TXISR_EntryCallback();
    #endif /* PRINTER_B_TXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START PRINTER_B_TXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        while((PRINTER_B_txBufferRead != PRINTER_B_txBufferWrite) &&
             ((PRINTER_B_TXSTATUS_REG & PRINTER_B_TX_STS_FIFO_FULL) == 0u))
        {
            /* Check pointer wrap around */
            if(PRINTER_B_txBufferRead >= PRINTER_B_TX_BUFFER_SIZE)
            {
                PRINTER_B_txBufferRead = 0u;
            }

            PRINTER_B_TXDATA_REG = PRINTER_B_txBuffer[PRINTER_B_txBufferRead];

            /* Set next pointer */
            PRINTER_B_txBufferRead++;
        }

        /* User code required at end of ISR (Optional) */
        /* `#START PRINTER_B_TXISR_END` */

        /* `#END` */

    #ifdef PRINTER_B_TXISR_EXIT_CALLBACK
        PRINTER_B_TXISR_ExitCallback();
    #endif /* PRINTER_B_TXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
   }
#endif /* (PRINTER_B_TX_INTERRUPT_ENABLED && PRINTER_B_TX_ENABLED) */


/* [] END OF FILE */
