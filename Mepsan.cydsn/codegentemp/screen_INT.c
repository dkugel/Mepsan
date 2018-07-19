/*******************************************************************************
* File Name: screenINT.c
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

#include "screen.h"
#include "cyapicallbacks.h"


/***************************************
* Custom Declarations
***************************************/
/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */

#if (screen_RX_INTERRUPT_ENABLED && (screen_RX_ENABLED || screen_HD_ENABLED))
    /*******************************************************************************
    * Function Name: screen_RXISR
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
    *  screen_rxBuffer - RAM buffer pointer for save received data.
    *  screen_rxBufferWrite - cyclic index for write to rxBuffer,
    *     increments after each byte saved to buffer.
    *  screen_rxBufferRead - cyclic index for read from rxBuffer,
    *     checked to detect overflow condition.
    *  screen_rxBufferOverflow - software overflow flag. Set to one
    *     when screen_rxBufferWrite index overtakes
    *     screen_rxBufferRead index.
    *  screen_rxBufferLoopDetect - additional variable to detect overflow.
    *     Set to one when screen_rxBufferWrite is equal to
    *    screen_rxBufferRead
    *  screen_rxAddressMode - this variable contains the Address mode,
    *     selected in customizer or set by UART_SetRxAddressMode() API.
    *  screen_rxAddressDetected - set to 1 when correct address received,
    *     and analysed to store following addressed data bytes to the buffer.
    *     When not correct address received, set to 0 to skip following data bytes.
    *
    *******************************************************************************/
    CY_ISR(screen_RXISR)
    {
        uint8 readData;
        uint8 readStatus;
        uint8 increment_pointer = 0u;

    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef screen_RXISR_ENTRY_CALLBACK
        screen_RXISR_EntryCallback();
    #endif /* screen_RXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START screen_RXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        do
        {
            /* Read receiver status register */
            readStatus = screen_RXSTATUS_REG;
            /* Copy the same status to readData variable for backward compatibility support 
            *  of the user code in screen_RXISR_ERROR` section. 
            */
            readData = readStatus;

            if((readStatus & (screen_RX_STS_BREAK | 
                            screen_RX_STS_PAR_ERROR |
                            screen_RX_STS_STOP_ERROR | 
                            screen_RX_STS_OVERRUN)) != 0u)
            {
                /* ERROR handling. */
                screen_errorStatus |= readStatus & ( screen_RX_STS_BREAK | 
                                                            screen_RX_STS_PAR_ERROR | 
                                                            screen_RX_STS_STOP_ERROR | 
                                                            screen_RX_STS_OVERRUN);
                /* `#START screen_RXISR_ERROR` */

                /* `#END` */
                
            #ifdef screen_RXISR_ERROR_CALLBACK
                screen_RXISR_ERROR_Callback();
            #endif /* screen_RXISR_ERROR_CALLBACK */
            }
            
            if((readStatus & screen_RX_STS_FIFO_NOTEMPTY) != 0u)
            {
                /* Read data from the RX data register */
                readData = screen_RXDATA_REG;
            #if (screen_RXHW_ADDRESS_ENABLED)
                if(screen_rxAddressMode == (uint8)screen__B_UART__AM_SW_DETECT_TO_BUFFER)
                {
                    if((readStatus & screen_RX_STS_MRKSPC) != 0u)
                    {
                        if ((readStatus & screen_RX_STS_ADDR_MATCH) != 0u)
                        {
                            screen_rxAddressDetected = 1u;
                        }
                        else
                        {
                            screen_rxAddressDetected = 0u;
                        }
                    }
                    if(screen_rxAddressDetected != 0u)
                    {   /* Store only addressed data */
                        screen_rxBuffer[screen_rxBufferWrite] = readData;
                        increment_pointer = 1u;
                    }
                }
                else /* Without software addressing */
                {
                    screen_rxBuffer[screen_rxBufferWrite] = readData;
                    increment_pointer = 1u;
                }
            #else  /* Without addressing */
                screen_rxBuffer[screen_rxBufferWrite] = readData;
                increment_pointer = 1u;
            #endif /* (screen_RXHW_ADDRESS_ENABLED) */

                /* Do not increment buffer pointer when skip not addressed data */
                if(increment_pointer != 0u)
                {
                    if(screen_rxBufferLoopDetect != 0u)
                    {   /* Set Software Buffer status Overflow */
                        screen_rxBufferOverflow = 1u;
                    }
                    /* Set next pointer. */
                    screen_rxBufferWrite++;

                    /* Check pointer for a loop condition */
                    if(screen_rxBufferWrite >= screen_RX_BUFFER_SIZE)
                    {
                        screen_rxBufferWrite = 0u;
                    }

                    /* Detect pre-overload condition and set flag */
                    if(screen_rxBufferWrite == screen_rxBufferRead)
                    {
                        screen_rxBufferLoopDetect = 1u;
                        /* When Hardware Flow Control selected */
                        #if (screen_FLOW_CONTROL != 0u)
                            /* Disable RX interrupt mask, it is enabled when user read data from the buffer using APIs */
                            screen_RXSTATUS_MASK_REG  &= (uint8)~screen_RX_STS_FIFO_NOTEMPTY;
                            CyIntClearPending(screen_RX_VECT_NUM);
                            break; /* Break the reading of the FIFO loop, leave the data there for generating RTS signal */
                        #endif /* (screen_FLOW_CONTROL != 0u) */
                    }
                }
            }
        }while((readStatus & screen_RX_STS_FIFO_NOTEMPTY) != 0u);

        /* User code required at end of ISR (Optional) */
        /* `#START screen_RXISR_END` */

        /* `#END` */

    #ifdef screen_RXISR_EXIT_CALLBACK
        screen_RXISR_ExitCallback();
    #endif /* screen_RXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
    }
    
#endif /* (screen_RX_INTERRUPT_ENABLED && (screen_RX_ENABLED || screen_HD_ENABLED)) */


#if (screen_TX_INTERRUPT_ENABLED && screen_TX_ENABLED)
    /*******************************************************************************
    * Function Name: screen_TXISR
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
    *  screen_txBuffer - RAM buffer pointer for transmit data from.
    *  screen_txBufferRead - cyclic index for read and transmit data
    *     from txBuffer, increments after each transmitted byte.
    *  screen_rxBufferWrite - cyclic index for write to txBuffer,
    *     checked to detect available for transmission bytes.
    *
    *******************************************************************************/
    CY_ISR(screen_TXISR)
    {
    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef screen_TXISR_ENTRY_CALLBACK
        screen_TXISR_EntryCallback();
    #endif /* screen_TXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START screen_TXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        while((screen_txBufferRead != screen_txBufferWrite) &&
             ((screen_TXSTATUS_REG & screen_TX_STS_FIFO_FULL) == 0u))
        {
            /* Check pointer wrap around */
            if(screen_txBufferRead >= screen_TX_BUFFER_SIZE)
            {
                screen_txBufferRead = 0u;
            }

            screen_TXDATA_REG = screen_txBuffer[screen_txBufferRead];

            /* Set next pointer */
            screen_txBufferRead++;
        }

        /* User code required at end of ISR (Optional) */
        /* `#START screen_TXISR_END` */

        /* `#END` */

    #ifdef screen_TXISR_EXIT_CALLBACK
        screen_TXISR_ExitCallback();
    #endif /* screen_TXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
   }
#endif /* (screen_TX_INTERRUPT_ENABLED && screen_TX_ENABLED) */


/* [] END OF FILE */
