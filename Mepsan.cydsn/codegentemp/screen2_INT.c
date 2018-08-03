/*******************************************************************************
* File Name: screen2INT.c
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

#include "screen2.h"
#include "cyapicallbacks.h"


/***************************************
* Custom Declarations
***************************************/
/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */

#if (screen2_RX_INTERRUPT_ENABLED && (screen2_RX_ENABLED || screen2_HD_ENABLED))
    /*******************************************************************************
    * Function Name: screen2_RXISR
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
    *  screen2_rxBuffer - RAM buffer pointer for save received data.
    *  screen2_rxBufferWrite - cyclic index for write to rxBuffer,
    *     increments after each byte saved to buffer.
    *  screen2_rxBufferRead - cyclic index for read from rxBuffer,
    *     checked to detect overflow condition.
    *  screen2_rxBufferOverflow - software overflow flag. Set to one
    *     when screen2_rxBufferWrite index overtakes
    *     screen2_rxBufferRead index.
    *  screen2_rxBufferLoopDetect - additional variable to detect overflow.
    *     Set to one when screen2_rxBufferWrite is equal to
    *    screen2_rxBufferRead
    *  screen2_rxAddressMode - this variable contains the Address mode,
    *     selected in customizer or set by UART_SetRxAddressMode() API.
    *  screen2_rxAddressDetected - set to 1 when correct address received,
    *     and analysed to store following addressed data bytes to the buffer.
    *     When not correct address received, set to 0 to skip following data bytes.
    *
    *******************************************************************************/
    CY_ISR(screen2_RXISR)
    {
        uint8 readData;
        uint8 readStatus;
        uint8 increment_pointer = 0u;

    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef screen2_RXISR_ENTRY_CALLBACK
        screen2_RXISR_EntryCallback();
    #endif /* screen2_RXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START screen2_RXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        do
        {
            /* Read receiver status register */
            readStatus = screen2_RXSTATUS_REG;
            /* Copy the same status to readData variable for backward compatibility support 
            *  of the user code in screen2_RXISR_ERROR` section. 
            */
            readData = readStatus;

            if((readStatus & (screen2_RX_STS_BREAK | 
                            screen2_RX_STS_PAR_ERROR |
                            screen2_RX_STS_STOP_ERROR | 
                            screen2_RX_STS_OVERRUN)) != 0u)
            {
                /* ERROR handling. */
                screen2_errorStatus |= readStatus & ( screen2_RX_STS_BREAK | 
                                                            screen2_RX_STS_PAR_ERROR | 
                                                            screen2_RX_STS_STOP_ERROR | 
                                                            screen2_RX_STS_OVERRUN);
                /* `#START screen2_RXISR_ERROR` */

                /* `#END` */
                
            #ifdef screen2_RXISR_ERROR_CALLBACK
                screen2_RXISR_ERROR_Callback();
            #endif /* screen2_RXISR_ERROR_CALLBACK */
            }
            
            if((readStatus & screen2_RX_STS_FIFO_NOTEMPTY) != 0u)
            {
                /* Read data from the RX data register */
                readData = screen2_RXDATA_REG;
            #if (screen2_RXHW_ADDRESS_ENABLED)
                if(screen2_rxAddressMode == (uint8)screen2__B_UART__AM_SW_DETECT_TO_BUFFER)
                {
                    if((readStatus & screen2_RX_STS_MRKSPC) != 0u)
                    {
                        if ((readStatus & screen2_RX_STS_ADDR_MATCH) != 0u)
                        {
                            screen2_rxAddressDetected = 1u;
                        }
                        else
                        {
                            screen2_rxAddressDetected = 0u;
                        }
                    }
                    if(screen2_rxAddressDetected != 0u)
                    {   /* Store only addressed data */
                        screen2_rxBuffer[screen2_rxBufferWrite] = readData;
                        increment_pointer = 1u;
                    }
                }
                else /* Without software addressing */
                {
                    screen2_rxBuffer[screen2_rxBufferWrite] = readData;
                    increment_pointer = 1u;
                }
            #else  /* Without addressing */
                screen2_rxBuffer[screen2_rxBufferWrite] = readData;
                increment_pointer = 1u;
            #endif /* (screen2_RXHW_ADDRESS_ENABLED) */

                /* Do not increment buffer pointer when skip not addressed data */
                if(increment_pointer != 0u)
                {
                    if(screen2_rxBufferLoopDetect != 0u)
                    {   /* Set Software Buffer status Overflow */
                        screen2_rxBufferOverflow = 1u;
                    }
                    /* Set next pointer. */
                    screen2_rxBufferWrite++;

                    /* Check pointer for a loop condition */
                    if(screen2_rxBufferWrite >= screen2_RX_BUFFER_SIZE)
                    {
                        screen2_rxBufferWrite = 0u;
                    }

                    /* Detect pre-overload condition and set flag */
                    if(screen2_rxBufferWrite == screen2_rxBufferRead)
                    {
                        screen2_rxBufferLoopDetect = 1u;
                        /* When Hardware Flow Control selected */
                        #if (screen2_FLOW_CONTROL != 0u)
                            /* Disable RX interrupt mask, it is enabled when user read data from the buffer using APIs */
                            screen2_RXSTATUS_MASK_REG  &= (uint8)~screen2_RX_STS_FIFO_NOTEMPTY;
                            CyIntClearPending(screen2_RX_VECT_NUM);
                            break; /* Break the reading of the FIFO loop, leave the data there for generating RTS signal */
                        #endif /* (screen2_FLOW_CONTROL != 0u) */
                    }
                }
            }
        }while((readStatus & screen2_RX_STS_FIFO_NOTEMPTY) != 0u);

        /* User code required at end of ISR (Optional) */
        /* `#START screen2_RXISR_END` */

        /* `#END` */

    #ifdef screen2_RXISR_EXIT_CALLBACK
        screen2_RXISR_ExitCallback();
    #endif /* screen2_RXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
    }
    
#endif /* (screen2_RX_INTERRUPT_ENABLED && (screen2_RX_ENABLED || screen2_HD_ENABLED)) */


#if (screen2_TX_INTERRUPT_ENABLED && screen2_TX_ENABLED)
    /*******************************************************************************
    * Function Name: screen2_TXISR
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
    *  screen2_txBuffer - RAM buffer pointer for transmit data from.
    *  screen2_txBufferRead - cyclic index for read and transmit data
    *     from txBuffer, increments after each transmitted byte.
    *  screen2_rxBufferWrite - cyclic index for write to txBuffer,
    *     checked to detect available for transmission bytes.
    *
    *******************************************************************************/
    CY_ISR(screen2_TXISR)
    {
    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef screen2_TXISR_ENTRY_CALLBACK
        screen2_TXISR_EntryCallback();
    #endif /* screen2_TXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START screen2_TXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        while((screen2_txBufferRead != screen2_txBufferWrite) &&
             ((screen2_TXSTATUS_REG & screen2_TX_STS_FIFO_FULL) == 0u))
        {
            /* Check pointer wrap around */
            if(screen2_txBufferRead >= screen2_TX_BUFFER_SIZE)
            {
                screen2_txBufferRead = 0u;
            }

            screen2_TXDATA_REG = screen2_txBuffer[screen2_txBufferRead];

            /* Set next pointer */
            screen2_txBufferRead++;
        }

        /* User code required at end of ISR (Optional) */
        /* `#START screen2_TXISR_END` */

        /* `#END` */

    #ifdef screen2_TXISR_EXIT_CALLBACK
        screen2_TXISR_ExitCallback();
    #endif /* screen2_TXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
   }
#endif /* (screen2_TX_INTERRUPT_ENABLED && screen2_TX_ENABLED) */


/* [] END OF FILE */
