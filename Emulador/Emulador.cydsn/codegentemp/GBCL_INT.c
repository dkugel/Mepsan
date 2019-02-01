/*******************************************************************************
* File Name: GBCLINT.c
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

#include "GBCL.h"
#include "cyapicallbacks.h"


/***************************************
* Custom Declarations
***************************************/
/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */

#if (GBCL_RX_INTERRUPT_ENABLED && (GBCL_RX_ENABLED || GBCL_HD_ENABLED))
    /*******************************************************************************
    * Function Name: GBCL_RXISR
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
    *  GBCL_rxBuffer - RAM buffer pointer for save received data.
    *  GBCL_rxBufferWrite - cyclic index for write to rxBuffer,
    *     increments after each byte saved to buffer.
    *  GBCL_rxBufferRead - cyclic index for read from rxBuffer,
    *     checked to detect overflow condition.
    *  GBCL_rxBufferOverflow - software overflow flag. Set to one
    *     when GBCL_rxBufferWrite index overtakes
    *     GBCL_rxBufferRead index.
    *  GBCL_rxBufferLoopDetect - additional variable to detect overflow.
    *     Set to one when GBCL_rxBufferWrite is equal to
    *    GBCL_rxBufferRead
    *  GBCL_rxAddressMode - this variable contains the Address mode,
    *     selected in customizer or set by UART_SetRxAddressMode() API.
    *  GBCL_rxAddressDetected - set to 1 when correct address received,
    *     and analysed to store following addressed data bytes to the buffer.
    *     When not correct address received, set to 0 to skip following data bytes.
    *
    *******************************************************************************/
    CY_ISR(GBCL_RXISR)
    {
        uint8 readData;
        uint8 readStatus;
        uint8 increment_pointer = 0u;

    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef GBCL_RXISR_ENTRY_CALLBACK
        GBCL_RXISR_EntryCallback();
    #endif /* GBCL_RXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START GBCL_RXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        do
        {
            /* Read receiver status register */
            readStatus = GBCL_RXSTATUS_REG;
            /* Copy the same status to readData variable for backward compatibility support 
            *  of the user code in GBCL_RXISR_ERROR` section. 
            */
            readData = readStatus;

            if((readStatus & (GBCL_RX_STS_BREAK | 
                            GBCL_RX_STS_PAR_ERROR |
                            GBCL_RX_STS_STOP_ERROR | 
                            GBCL_RX_STS_OVERRUN)) != 0u)
            {
                /* ERROR handling. */
                GBCL_errorStatus |= readStatus & ( GBCL_RX_STS_BREAK | 
                                                            GBCL_RX_STS_PAR_ERROR | 
                                                            GBCL_RX_STS_STOP_ERROR | 
                                                            GBCL_RX_STS_OVERRUN);
                /* `#START GBCL_RXISR_ERROR` */

                /* `#END` */
                
            #ifdef GBCL_RXISR_ERROR_CALLBACK
                GBCL_RXISR_ERROR_Callback();
            #endif /* GBCL_RXISR_ERROR_CALLBACK */
            }
            
            if((readStatus & GBCL_RX_STS_FIFO_NOTEMPTY) != 0u)
            {
                /* Read data from the RX data register */
                readData = GBCL_RXDATA_REG;
            #if (GBCL_RXHW_ADDRESS_ENABLED)
                if(GBCL_rxAddressMode == (uint8)GBCL__B_UART__AM_SW_DETECT_TO_BUFFER)
                {
                    if((readStatus & GBCL_RX_STS_MRKSPC) != 0u)
                    {
                        if ((readStatus & GBCL_RX_STS_ADDR_MATCH) != 0u)
                        {
                            GBCL_rxAddressDetected = 1u;
                        }
                        else
                        {
                            GBCL_rxAddressDetected = 0u;
                        }
                    }
                    if(GBCL_rxAddressDetected != 0u)
                    {   /* Store only addressed data */
                        GBCL_rxBuffer[GBCL_rxBufferWrite] = readData;
                        increment_pointer = 1u;
                    }
                }
                else /* Without software addressing */
                {
                    GBCL_rxBuffer[GBCL_rxBufferWrite] = readData;
                    increment_pointer = 1u;
                }
            #else  /* Without addressing */
                GBCL_rxBuffer[GBCL_rxBufferWrite] = readData;
                increment_pointer = 1u;
            #endif /* (GBCL_RXHW_ADDRESS_ENABLED) */

                /* Do not increment buffer pointer when skip not addressed data */
                if(increment_pointer != 0u)
                {
                    if(GBCL_rxBufferLoopDetect != 0u)
                    {   /* Set Software Buffer status Overflow */
                        GBCL_rxBufferOverflow = 1u;
                    }
                    /* Set next pointer. */
                    GBCL_rxBufferWrite++;

                    /* Check pointer for a loop condition */
                    if(GBCL_rxBufferWrite >= GBCL_RX_BUFFER_SIZE)
                    {
                        GBCL_rxBufferWrite = 0u;
                    }

                    /* Detect pre-overload condition and set flag */
                    if(GBCL_rxBufferWrite == GBCL_rxBufferRead)
                    {
                        GBCL_rxBufferLoopDetect = 1u;
                        /* When Hardware Flow Control selected */
                        #if (GBCL_FLOW_CONTROL != 0u)
                            /* Disable RX interrupt mask, it is enabled when user read data from the buffer using APIs */
                            GBCL_RXSTATUS_MASK_REG  &= (uint8)~GBCL_RX_STS_FIFO_NOTEMPTY;
                            CyIntClearPending(GBCL_RX_VECT_NUM);
                            break; /* Break the reading of the FIFO loop, leave the data there for generating RTS signal */
                        #endif /* (GBCL_FLOW_CONTROL != 0u) */
                    }
                }
            }
        }while((readStatus & GBCL_RX_STS_FIFO_NOTEMPTY) != 0u);

        /* User code required at end of ISR (Optional) */
        /* `#START GBCL_RXISR_END` */

        /* `#END` */

    #ifdef GBCL_RXISR_EXIT_CALLBACK
        GBCL_RXISR_ExitCallback();
    #endif /* GBCL_RXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
    }
    
#endif /* (GBCL_RX_INTERRUPT_ENABLED && (GBCL_RX_ENABLED || GBCL_HD_ENABLED)) */


#if (GBCL_TX_INTERRUPT_ENABLED && GBCL_TX_ENABLED)
    /*******************************************************************************
    * Function Name: GBCL_TXISR
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
    *  GBCL_txBuffer - RAM buffer pointer for transmit data from.
    *  GBCL_txBufferRead - cyclic index for read and transmit data
    *     from txBuffer, increments after each transmitted byte.
    *  GBCL_rxBufferWrite - cyclic index for write to txBuffer,
    *     checked to detect available for transmission bytes.
    *
    *******************************************************************************/
    CY_ISR(GBCL_TXISR)
    {
    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef GBCL_TXISR_ENTRY_CALLBACK
        GBCL_TXISR_EntryCallback();
    #endif /* GBCL_TXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START GBCL_TXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        while((GBCL_txBufferRead != GBCL_txBufferWrite) &&
             ((GBCL_TXSTATUS_REG & GBCL_TX_STS_FIFO_FULL) == 0u))
        {
            /* Check pointer wrap around */
            if(GBCL_txBufferRead >= GBCL_TX_BUFFER_SIZE)
            {
                GBCL_txBufferRead = 0u;
            }

            GBCL_TXDATA_REG = GBCL_txBuffer[GBCL_txBufferRead];

            /* Set next pointer */
            GBCL_txBufferRead++;
        }

        /* User code required at end of ISR (Optional) */
        /* `#START GBCL_TXISR_END` */

        /* `#END` */

    #ifdef GBCL_TXISR_EXIT_CALLBACK
        GBCL_TXISR_ExitCallback();
    #endif /* GBCL_TXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
   }
#endif /* (GBCL_TX_INTERRUPT_ENABLED && GBCL_TX_ENABLED) */


/* [] END OF FILE */
