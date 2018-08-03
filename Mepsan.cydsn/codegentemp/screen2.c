/*******************************************************************************
* File Name: screen2.c
* Version 2.50
*
* Description:
*  This file provides all API functionality of the UART component
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "screen2.h"
#if (screen2_INTERNAL_CLOCK_USED)
    #include "screen2_IntClock.h"
#endif /* End screen2_INTERNAL_CLOCK_USED */


/***************************************
* Global data allocation
***************************************/

uint8 screen2_initVar = 0u;

#if (screen2_TX_INTERRUPT_ENABLED && screen2_TX_ENABLED)
    volatile uint8 screen2_txBuffer[screen2_TX_BUFFER_SIZE];
    volatile uint8 screen2_txBufferRead = 0u;
    uint8 screen2_txBufferWrite = 0u;
#endif /* (screen2_TX_INTERRUPT_ENABLED && screen2_TX_ENABLED) */

#if (screen2_RX_INTERRUPT_ENABLED && (screen2_RX_ENABLED || screen2_HD_ENABLED))
    uint8 screen2_errorStatus = 0u;
    volatile uint8 screen2_rxBuffer[screen2_RX_BUFFER_SIZE];
    volatile uint8 screen2_rxBufferRead  = 0u;
    volatile uint8 screen2_rxBufferWrite = 0u;
    volatile uint8 screen2_rxBufferLoopDetect = 0u;
    volatile uint8 screen2_rxBufferOverflow   = 0u;
    #if (screen2_RXHW_ADDRESS_ENABLED)
        volatile uint8 screen2_rxAddressMode = screen2_RX_ADDRESS_MODE;
        volatile uint8 screen2_rxAddressDetected = 0u;
    #endif /* (screen2_RXHW_ADDRESS_ENABLED) */
#endif /* (screen2_RX_INTERRUPT_ENABLED && (screen2_RX_ENABLED || screen2_HD_ENABLED)) */


/*******************************************************************************
* Function Name: screen2_Start
********************************************************************************
*
* Summary:
*  This is the preferred method to begin component operation.
*  screen2_Start() sets the initVar variable, calls the
*  screen2_Init() function, and then calls the
*  screen2_Enable() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  The screen2_intiVar variable is used to indicate initial
*  configuration of this component. The variable is initialized to zero (0u)
*  and set to one (1u) the first time screen2_Start() is called. This
*  allows for component initialization without re-initialization in all
*  subsequent calls to the screen2_Start() routine.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void screen2_Start(void) 
{
    /* If not initialized then initialize all required hardware and software */
    if(screen2_initVar == 0u)
    {
        screen2_Init();
        screen2_initVar = 1u;
    }

    screen2_Enable();
}


/*******************************************************************************
* Function Name: screen2_Init
********************************************************************************
*
* Summary:
*  Initializes or restores the component according to the customizer Configure
*  dialog settings. It is not necessary to call screen2_Init() because
*  the screen2_Start() API calls this function and is the preferred
*  method to begin component operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void screen2_Init(void) 
{
    #if(screen2_RX_ENABLED || screen2_HD_ENABLED)

        #if (screen2_RX_INTERRUPT_ENABLED)
            /* Set RX interrupt vector and priority */
            (void) CyIntSetVector(screen2_RX_VECT_NUM, &screen2_RXISR);
            CyIntSetPriority(screen2_RX_VECT_NUM, screen2_RX_PRIOR_NUM);
            screen2_errorStatus = 0u;
        #endif /* (screen2_RX_INTERRUPT_ENABLED) */

        #if (screen2_RXHW_ADDRESS_ENABLED)
            screen2_SetRxAddressMode(screen2_RX_ADDRESS_MODE);
            screen2_SetRxAddress1(screen2_RX_HW_ADDRESS1);
            screen2_SetRxAddress2(screen2_RX_HW_ADDRESS2);
        #endif /* End screen2_RXHW_ADDRESS_ENABLED */

        /* Init Count7 period */
        screen2_RXBITCTR_PERIOD_REG = screen2_RXBITCTR_INIT;
        /* Configure the Initial RX interrupt mask */
        screen2_RXSTATUS_MASK_REG  = screen2_INIT_RX_INTERRUPTS_MASK;
    #endif /* End screen2_RX_ENABLED || screen2_HD_ENABLED*/

    #if(screen2_TX_ENABLED)
        #if (screen2_TX_INTERRUPT_ENABLED)
            /* Set TX interrupt vector and priority */
            (void) CyIntSetVector(screen2_TX_VECT_NUM, &screen2_TXISR);
            CyIntSetPriority(screen2_TX_VECT_NUM, screen2_TX_PRIOR_NUM);
        #endif /* (screen2_TX_INTERRUPT_ENABLED) */

        /* Write Counter Value for TX Bit Clk Generator*/
        #if (screen2_TXCLKGEN_DP)
            screen2_TXBITCLKGEN_CTR_REG = screen2_BIT_CENTER;
            screen2_TXBITCLKTX_COMPLETE_REG = ((screen2_NUMBER_OF_DATA_BITS +
                        screen2_NUMBER_OF_START_BIT) * screen2_OVER_SAMPLE_COUNT) - 1u;
        #else
            screen2_TXBITCTR_PERIOD_REG = ((screen2_NUMBER_OF_DATA_BITS +
                        screen2_NUMBER_OF_START_BIT) * screen2_OVER_SAMPLE_8) - 1u;
        #endif /* End screen2_TXCLKGEN_DP */

        /* Configure the Initial TX interrupt mask */
        #if (screen2_TX_INTERRUPT_ENABLED)
            screen2_TXSTATUS_MASK_REG = screen2_TX_STS_FIFO_EMPTY;
        #else
            screen2_TXSTATUS_MASK_REG = screen2_INIT_TX_INTERRUPTS_MASK;
        #endif /*End screen2_TX_INTERRUPT_ENABLED*/

    #endif /* End screen2_TX_ENABLED */

    #if(screen2_PARITY_TYPE_SW)  /* Write Parity to Control Register */
        screen2_WriteControlRegister( \
            (screen2_ReadControlRegister() & (uint8)~screen2_CTRL_PARITY_TYPE_MASK) | \
            (uint8)(screen2_PARITY_TYPE << screen2_CTRL_PARITY_TYPE0_SHIFT) );
    #endif /* End screen2_PARITY_TYPE_SW */
}


/*******************************************************************************
* Function Name: screen2_Enable
********************************************************************************
*
* Summary:
*  Activates the hardware and begins component operation. It is not necessary
*  to call screen2_Enable() because the screen2_Start() API
*  calls this function, which is the preferred method to begin component
*  operation.

* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  screen2_rxAddressDetected - set to initial state (0).
*
*******************************************************************************/
void screen2_Enable(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    #if (screen2_RX_ENABLED || screen2_HD_ENABLED)
        /* RX Counter (Count7) Enable */
        screen2_RXBITCTR_CONTROL_REG |= screen2_CNTR_ENABLE;

        /* Enable the RX Interrupt */
        screen2_RXSTATUS_ACTL_REG  |= screen2_INT_ENABLE;

        #if (screen2_RX_INTERRUPT_ENABLED)
            screen2_EnableRxInt();

            #if (screen2_RXHW_ADDRESS_ENABLED)
                screen2_rxAddressDetected = 0u;
            #endif /* (screen2_RXHW_ADDRESS_ENABLED) */
        #endif /* (screen2_RX_INTERRUPT_ENABLED) */
    #endif /* (screen2_RX_ENABLED || screen2_HD_ENABLED) */

    #if(screen2_TX_ENABLED)
        /* TX Counter (DP/Count7) Enable */
        #if(!screen2_TXCLKGEN_DP)
            screen2_TXBITCTR_CONTROL_REG |= screen2_CNTR_ENABLE;
        #endif /* End screen2_TXCLKGEN_DP */

        /* Enable the TX Interrupt */
        screen2_TXSTATUS_ACTL_REG |= screen2_INT_ENABLE;
        #if (screen2_TX_INTERRUPT_ENABLED)
            screen2_ClearPendingTxInt(); /* Clear history of TX_NOT_EMPTY */
            screen2_EnableTxInt();
        #endif /* (screen2_TX_INTERRUPT_ENABLED) */
     #endif /* (screen2_TX_INTERRUPT_ENABLED) */

    #if (screen2_INTERNAL_CLOCK_USED)
        screen2_IntClock_Start();  /* Enable the clock */
    #endif /* (screen2_INTERNAL_CLOCK_USED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: screen2_Stop
********************************************************************************
*
* Summary:
*  Disables the UART operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void screen2_Stop(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    /* Write Bit Counter Disable */
    #if (screen2_RX_ENABLED || screen2_HD_ENABLED)
        screen2_RXBITCTR_CONTROL_REG &= (uint8) ~screen2_CNTR_ENABLE;
    #endif /* (screen2_RX_ENABLED || screen2_HD_ENABLED) */

    #if (screen2_TX_ENABLED)
        #if(!screen2_TXCLKGEN_DP)
            screen2_TXBITCTR_CONTROL_REG &= (uint8) ~screen2_CNTR_ENABLE;
        #endif /* (!screen2_TXCLKGEN_DP) */
    #endif /* (screen2_TX_ENABLED) */

    #if (screen2_INTERNAL_CLOCK_USED)
        screen2_IntClock_Stop();   /* Disable the clock */
    #endif /* (screen2_INTERNAL_CLOCK_USED) */

    /* Disable internal interrupt component */
    #if (screen2_RX_ENABLED || screen2_HD_ENABLED)
        screen2_RXSTATUS_ACTL_REG  &= (uint8) ~screen2_INT_ENABLE;

        #if (screen2_RX_INTERRUPT_ENABLED)
            screen2_DisableRxInt();
        #endif /* (screen2_RX_INTERRUPT_ENABLED) */
    #endif /* (screen2_RX_ENABLED || screen2_HD_ENABLED) */

    #if (screen2_TX_ENABLED)
        screen2_TXSTATUS_ACTL_REG &= (uint8) ~screen2_INT_ENABLE;

        #if (screen2_TX_INTERRUPT_ENABLED)
            screen2_DisableTxInt();
        #endif /* (screen2_TX_INTERRUPT_ENABLED) */
    #endif /* (screen2_TX_ENABLED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: screen2_ReadControlRegister
********************************************************************************
*
* Summary:
*  Returns the current value of the control register.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the control register.
*
*******************************************************************************/
uint8 screen2_ReadControlRegister(void) 
{
    #if (screen2_CONTROL_REG_REMOVED)
        return(0u);
    #else
        return(screen2_CONTROL_REG);
    #endif /* (screen2_CONTROL_REG_REMOVED) */
}


/*******************************************************************************
* Function Name: screen2_WriteControlRegister
********************************************************************************
*
* Summary:
*  Writes an 8-bit value into the control register
*
* Parameters:
*  control:  control register value
*
* Return:
*  None.
*
*******************************************************************************/
void  screen2_WriteControlRegister(uint8 control) 
{
    #if (screen2_CONTROL_REG_REMOVED)
        if(0u != control)
        {
            /* Suppress compiler warning */
        }
    #else
       screen2_CONTROL_REG = control;
    #endif /* (screen2_CONTROL_REG_REMOVED) */
}


#if(screen2_RX_ENABLED || screen2_HD_ENABLED)
    /*******************************************************************************
    * Function Name: screen2_SetRxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the RX interrupt sources enabled.
    *
    * Parameters:
    *  IntSrc:  Bit field containing the RX interrupts to enable. Based on the 
    *  bit-field arrangement of the status register. This value must be a 
    *  combination of status register bit-masks shown below:
    *      screen2_RX_STS_FIFO_NOTEMPTY    Interrupt on byte received.
    *      screen2_RX_STS_PAR_ERROR        Interrupt on parity error.
    *      screen2_RX_STS_STOP_ERROR       Interrupt on stop error.
    *      screen2_RX_STS_BREAK            Interrupt on break.
    *      screen2_RX_STS_OVERRUN          Interrupt on overrun error.
    *      screen2_RX_STS_ADDR_MATCH       Interrupt on address match.
    *      screen2_RX_STS_MRKSPC           Interrupt on address detect.
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void screen2_SetRxInterruptMode(uint8 intSrc) 
    {
        screen2_RXSTATUS_MASK_REG  = intSrc;
    }


    /*******************************************************************************
    * Function Name: screen2_ReadRxData
    ********************************************************************************
    *
    * Summary:
    *  Returns the next byte of received data. This function returns data without
    *  checking the status. You must check the status separately.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Received data from RX register
    *
    * Global Variables:
    *  screen2_rxBuffer - RAM buffer pointer for save received data.
    *  screen2_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  screen2_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  screen2_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 screen2_ReadRxData(void) 
    {
        uint8 rxData;

    #if (screen2_RX_INTERRUPT_ENABLED)

        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        screen2_DisableRxInt();

        locRxBufferRead  = screen2_rxBufferRead;
        locRxBufferWrite = screen2_rxBufferWrite;

        if( (screen2_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = screen2_rxBuffer[locRxBufferRead];
            locRxBufferRead++;

            if(locRxBufferRead >= screen2_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            screen2_rxBufferRead = locRxBufferRead;

            if(screen2_rxBufferLoopDetect != 0u)
            {
                screen2_rxBufferLoopDetect = 0u;
                #if ((screen2_RX_INTERRUPT_ENABLED) && (screen2_FLOW_CONTROL != 0u))
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( screen2_HD_ENABLED )
                        if((screen2_CONTROL_REG & screen2_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only in RX
                            *  configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            screen2_RXSTATUS_MASK_REG  |= screen2_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        screen2_RXSTATUS_MASK_REG  |= screen2_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end screen2_HD_ENABLED */
                #endif /* ((screen2_RX_INTERRUPT_ENABLED) && (screen2_FLOW_CONTROL != 0u)) */
            }
        }
        else
        {   /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
            rxData = screen2_RXDATA_REG;
        }

        screen2_EnableRxInt();

    #else

        /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
        rxData = screen2_RXDATA_REG;

    #endif /* (screen2_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: screen2_ReadRxStatus
    ********************************************************************************
    *
    * Summary:
    *  Returns the current state of the receiver status register and the software
    *  buffer overflow status.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Current state of the status register.
    *
    * Side Effect:
    *  All status register bits are clear-on-read except
    *  screen2_RX_STS_FIFO_NOTEMPTY.
    *  screen2_RX_STS_FIFO_NOTEMPTY clears immediately after RX data
    *  register read.
    *
    * Global Variables:
    *  screen2_rxBufferOverflow - used to indicate overload condition.
    *   It set to one in RX interrupt when there isn't free space in
    *   screen2_rxBufferRead to write new data. This condition returned
    *   and cleared to zero by this API as an
    *   screen2_RX_STS_SOFT_BUFF_OVER bit along with RX Status register
    *   bits.
    *
    *******************************************************************************/
    uint8 screen2_ReadRxStatus(void) 
    {
        uint8 status;

        status = screen2_RXSTATUS_REG & screen2_RX_HW_MASK;

    #if (screen2_RX_INTERRUPT_ENABLED)
        if(screen2_rxBufferOverflow != 0u)
        {
            status |= screen2_RX_STS_SOFT_BUFF_OVER;
            screen2_rxBufferOverflow = 0u;
        }
    #endif /* (screen2_RX_INTERRUPT_ENABLED) */

        return(status);
    }


    /*******************************************************************************
    * Function Name: screen2_GetChar
    ********************************************************************************
    *
    * Summary:
    *  Returns the last received byte of data. screen2_GetChar() is
    *  designed for ASCII characters and returns a uint8 where 1 to 255 are values
    *  for valid characters and 0 indicates an error occurred or no data is present.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Character read from UART RX buffer. ASCII characters from 1 to 255 are valid.
    *  A returned zero signifies an error condition or no data available.
    *
    * Global Variables:
    *  screen2_rxBuffer - RAM buffer pointer for save received data.
    *  screen2_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  screen2_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  screen2_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 screen2_GetChar(void) 
    {
        uint8 rxData = 0u;
        uint8 rxStatus;

    #if (screen2_RX_INTERRUPT_ENABLED)
        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        screen2_DisableRxInt();

        locRxBufferRead  = screen2_rxBufferRead;
        locRxBufferWrite = screen2_rxBufferWrite;

        if( (screen2_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = screen2_rxBuffer[locRxBufferRead];
            locRxBufferRead++;
            if(locRxBufferRead >= screen2_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            screen2_rxBufferRead = locRxBufferRead;

            if(screen2_rxBufferLoopDetect != 0u)
            {
                screen2_rxBufferLoopDetect = 0u;
                #if( (screen2_RX_INTERRUPT_ENABLED) && (screen2_FLOW_CONTROL != 0u) )
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( screen2_HD_ENABLED )
                        if((screen2_CONTROL_REG & screen2_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only if
                            *  RX configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            screen2_RXSTATUS_MASK_REG |= screen2_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        screen2_RXSTATUS_MASK_REG |= screen2_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end screen2_HD_ENABLED */
                #endif /* screen2_RX_INTERRUPT_ENABLED and Hardware flow control*/
            }

        }
        else
        {   rxStatus = screen2_RXSTATUS_REG;
            if((rxStatus & screen2_RX_STS_FIFO_NOTEMPTY) != 0u)
            {   /* Read received data from FIFO */
                rxData = screen2_RXDATA_REG;
                /*Check status on error*/
                if((rxStatus & (screen2_RX_STS_BREAK | screen2_RX_STS_PAR_ERROR |
                                screen2_RX_STS_STOP_ERROR | screen2_RX_STS_OVERRUN)) != 0u)
                {
                    rxData = 0u;
                }
            }
        }

        screen2_EnableRxInt();

    #else

        rxStatus =screen2_RXSTATUS_REG;
        if((rxStatus & screen2_RX_STS_FIFO_NOTEMPTY) != 0u)
        {
            /* Read received data from FIFO */
            rxData = screen2_RXDATA_REG;

            /*Check status on error*/
            if((rxStatus & (screen2_RX_STS_BREAK | screen2_RX_STS_PAR_ERROR |
                            screen2_RX_STS_STOP_ERROR | screen2_RX_STS_OVERRUN)) != 0u)
            {
                rxData = 0u;
            }
        }
    #endif /* (screen2_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: screen2_GetByte
    ********************************************************************************
    *
    * Summary:
    *  Reads UART RX buffer immediately, returns received character and error
    *  condition.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  MSB contains status and LSB contains UART RX data. If the MSB is nonzero,
    *  an error has occurred.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint16 screen2_GetByte(void) 
    {
        
    #if (screen2_RX_INTERRUPT_ENABLED)
        uint16 locErrorStatus;
        /* Protect variables that could change on interrupt */
        screen2_DisableRxInt();
        locErrorStatus = (uint16)screen2_errorStatus;
        screen2_errorStatus = 0u;
        screen2_EnableRxInt();
        return ( (uint16)(locErrorStatus << 8u) | screen2_ReadRxData() );
    #else
        return ( ((uint16)screen2_ReadRxStatus() << 8u) | screen2_ReadRxData() );
    #endif /* screen2_RX_INTERRUPT_ENABLED */
        
    }


    /*******************************************************************************
    * Function Name: screen2_GetRxBufferSize
    ********************************************************************************
    *
    * Summary:
    *  Returns the number of received bytes available in the RX buffer.
    *  * RX software buffer is disabled (RX Buffer Size parameter is equal to 4): 
    *    returns 0 for empty RX FIFO or 1 for not empty RX FIFO.
    *  * RX software buffer is enabled: returns the number of bytes available in 
    *    the RX software buffer. Bytes available in the RX FIFO do not take to 
    *    account.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  uint8: Number of bytes in the RX buffer. 
    *    Return value type depends on RX Buffer Size parameter.
    *
    * Global Variables:
    *  screen2_rxBufferWrite - used to calculate left bytes.
    *  screen2_rxBufferRead - used to calculate left bytes.
    *  screen2_rxBufferLoopDetect - checked to decide left bytes amount.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the RX Buffer is.
    *
    *******************************************************************************/
    uint8 screen2_GetRxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (screen2_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt */
        screen2_DisableRxInt();

        if(screen2_rxBufferRead == screen2_rxBufferWrite)
        {
            if(screen2_rxBufferLoopDetect != 0u)
            {
                size = screen2_RX_BUFFER_SIZE;
            }
            else
            {
                size = 0u;
            }
        }
        else if(screen2_rxBufferRead < screen2_rxBufferWrite)
        {
            size = (screen2_rxBufferWrite - screen2_rxBufferRead);
        }
        else
        {
            size = (screen2_RX_BUFFER_SIZE - screen2_rxBufferRead) + screen2_rxBufferWrite;
        }

        screen2_EnableRxInt();

    #else

        /* We can only know if there is data in the fifo. */
        size = ((screen2_RXSTATUS_REG & screen2_RX_STS_FIFO_NOTEMPTY) != 0u) ? 1u : 0u;

    #endif /* (screen2_RX_INTERRUPT_ENABLED) */

        return(size);
    }


    /*******************************************************************************
    * Function Name: screen2_ClearRxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Clears the receiver memory buffer and hardware RX FIFO of all received data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  screen2_rxBufferWrite - cleared to zero.
    *  screen2_rxBufferRead - cleared to zero.
    *  screen2_rxBufferLoopDetect - cleared to zero.
    *  screen2_rxBufferOverflow - cleared to zero.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Setting the pointers to zero makes the system believe there is no data to
    *  read and writing will resume at address 0 overwriting any data that may
    *  have remained in the RAM.
    *
    * Side Effects:
    *  Any received data not read from the RAM or FIFO buffer will be lost.
    *
    *******************************************************************************/
    void screen2_ClearRxBuffer(void) 
    {
        uint8 enableInterrupts;

        /* Clear the HW FIFO */
        enableInterrupts = CyEnterCriticalSection();
        screen2_RXDATA_AUX_CTL_REG |= (uint8)  screen2_RX_FIFO_CLR;
        screen2_RXDATA_AUX_CTL_REG &= (uint8) ~screen2_RX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (screen2_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        screen2_DisableRxInt();

        screen2_rxBufferRead = 0u;
        screen2_rxBufferWrite = 0u;
        screen2_rxBufferLoopDetect = 0u;
        screen2_rxBufferOverflow = 0u;

        screen2_EnableRxInt();

    #endif /* (screen2_RX_INTERRUPT_ENABLED) */

    }


    /*******************************************************************************
    * Function Name: screen2_SetRxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Sets the software controlled Addressing mode used by the RX portion of the
    *  UART.
    *
    * Parameters:
    *  addressMode: Enumerated value indicating the mode of RX addressing
    *  screen2__B_UART__AM_SW_BYTE_BYTE -  Software Byte-by-Byte address
    *                                               detection
    *  screen2__B_UART__AM_SW_DETECT_TO_BUFFER - Software Detect to Buffer
    *                                               address detection
    *  screen2__B_UART__AM_HW_BYTE_BY_BYTE - Hardware Byte-by-Byte address
    *                                               detection
    *  screen2__B_UART__AM_HW_DETECT_TO_BUFFER - Hardware Detect to Buffer
    *                                               address detection
    *  screen2__B_UART__AM_NONE - No address detection
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  screen2_rxAddressMode - the parameter stored in this variable for
    *   the farther usage in RX ISR.
    *  screen2_rxAddressDetected - set to initial state (0).
    *
    *******************************************************************************/
    void screen2_SetRxAddressMode(uint8 addressMode)
                                                        
    {
        #if(screen2_RXHW_ADDRESS_ENABLED)
            #if(screen2_CONTROL_REG_REMOVED)
                if(0u != addressMode)
                {
                    /* Suppress compiler warning */
                }
            #else /* screen2_CONTROL_REG_REMOVED */
                uint8 tmpCtrl;
                tmpCtrl = screen2_CONTROL_REG & (uint8)~screen2_CTRL_RXADDR_MODE_MASK;
                tmpCtrl |= (uint8)(addressMode << screen2_CTRL_RXADDR_MODE0_SHIFT);
                screen2_CONTROL_REG = tmpCtrl;

                #if(screen2_RX_INTERRUPT_ENABLED && \
                   (screen2_RXBUFFERSIZE > screen2_FIFO_LENGTH) )
                    screen2_rxAddressMode = addressMode;
                    screen2_rxAddressDetected = 0u;
                #endif /* End screen2_RXBUFFERSIZE > screen2_FIFO_LENGTH*/
            #endif /* End screen2_CONTROL_REG_REMOVED */
        #else /* screen2_RXHW_ADDRESS_ENABLED */
            if(0u != addressMode)
            {
                /* Suppress compiler warning */
            }
        #endif /* End screen2_RXHW_ADDRESS_ENABLED */
    }


    /*******************************************************************************
    * Function Name: screen2_SetRxAddress1
    ********************************************************************************
    *
    * Summary:
    *  Sets the first of two hardware-detectable receiver addresses.
    *
    * Parameters:
    *  address: Address #1 for hardware address detection.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void screen2_SetRxAddress1(uint8 address) 
    {
        screen2_RXADDRESS1_REG = address;
    }


    /*******************************************************************************
    * Function Name: screen2_SetRxAddress2
    ********************************************************************************
    *
    * Summary:
    *  Sets the second of two hardware-detectable receiver addresses.
    *
    * Parameters:
    *  address: Address #2 for hardware address detection.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void screen2_SetRxAddress2(uint8 address) 
    {
        screen2_RXADDRESS2_REG = address;
    }

#endif  /* screen2_RX_ENABLED || screen2_HD_ENABLED*/


#if( (screen2_TX_ENABLED) || (screen2_HD_ENABLED) )
    /*******************************************************************************
    * Function Name: screen2_SetTxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the TX interrupt sources to be enabled, but does not enable the
    *  interrupt.
    *
    * Parameters:
    *  intSrc: Bit field containing the TX interrupt sources to enable
    *   screen2_TX_STS_COMPLETE        Interrupt on TX byte complete
    *   screen2_TX_STS_FIFO_EMPTY      Interrupt when TX FIFO is empty
    *   screen2_TX_STS_FIFO_FULL       Interrupt when TX FIFO is full
    *   screen2_TX_STS_FIFO_NOT_FULL   Interrupt when TX FIFO is not full
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void screen2_SetTxInterruptMode(uint8 intSrc) 
    {
        screen2_TXSTATUS_MASK_REG = intSrc;
    }


    /*******************************************************************************
    * Function Name: screen2_WriteTxData
    ********************************************************************************
    *
    * Summary:
    *  Places a byte of data into the transmit buffer to be sent when the bus is
    *  available without checking the TX status register. You must check status
    *  separately.
    *
    * Parameters:
    *  txDataByte: data byte
    *
    * Return:
    * None.
    *
    * Global Variables:
    *  screen2_txBuffer - RAM buffer pointer for save data for transmission
    *  screen2_txBufferWrite - cyclic index for write to txBuffer,
    *    incremented after each byte saved to buffer.
    *  screen2_txBufferRead - cyclic index for read from txBuffer,
    *    checked to identify the condition to write to FIFO directly or to TX buffer
    *  screen2_initVar - checked to identify that the component has been
    *    initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void screen2_WriteTxData(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function*/
        if(screen2_initVar != 0u)
        {
        #if (screen2_TX_INTERRUPT_ENABLED)

            /* Protect variables that could change on interrupt. */
            screen2_DisableTxInt();

            if( (screen2_txBufferRead == screen2_txBufferWrite) &&
                ((screen2_TXSTATUS_REG & screen2_TX_STS_FIFO_FULL) == 0u) )
            {
                /* Add directly to the FIFO. */
                screen2_TXDATA_REG = txDataByte;
            }
            else
            {
                if(screen2_txBufferWrite >= screen2_TX_BUFFER_SIZE)
                {
                    screen2_txBufferWrite = 0u;
                }

                screen2_txBuffer[screen2_txBufferWrite] = txDataByte;

                /* Add to the software buffer. */
                screen2_txBufferWrite++;
            }

            screen2_EnableTxInt();

        #else

            /* Add directly to the FIFO. */
            screen2_TXDATA_REG = txDataByte;

        #endif /*(screen2_TX_INTERRUPT_ENABLED) */
        }
    }


    /*******************************************************************************
    * Function Name: screen2_ReadTxStatus
    ********************************************************************************
    *
    * Summary:
    *  Reads the status register for the TX portion of the UART.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Contents of the status register
    *
    * Theory:
    *  This function reads the TX status register, which is cleared on read.
    *  It is up to the user to handle all bits in this return value accordingly,
    *  even if the bit was not enabled as an interrupt source the event happened
    *  and must be handled accordingly.
    *
    *******************************************************************************/
    uint8 screen2_ReadTxStatus(void) 
    {
        return(screen2_TXSTATUS_REG);
    }


    /*******************************************************************************
    * Function Name: screen2_PutChar
    ********************************************************************************
    *
    * Summary:
    *  Puts a byte of data into the transmit buffer to be sent when the bus is
    *  available. This is a blocking API that waits until the TX buffer has room to
    *  hold the data.
    *
    * Parameters:
    *  txDataByte: Byte containing the data to transmit
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  screen2_txBuffer - RAM buffer pointer for save data for transmission
    *  screen2_txBufferWrite - cyclic index for write to txBuffer,
    *     checked to identify free space in txBuffer and incremented after each byte
    *     saved to buffer.
    *  screen2_txBufferRead - cyclic index for read from txBuffer,
    *     checked to identify free space in txBuffer.
    *  screen2_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to transmit any byte of data in a single transfer
    *
    *******************************************************************************/
    void screen2_PutChar(uint8 txDataByte) 
    {
    #if (screen2_TX_INTERRUPT_ENABLED)
        /* The temporary output pointer is used since it takes two instructions
        *  to increment with a wrap, and we can't risk doing that with the real
        *  pointer and getting an interrupt in between instructions.
        */
        uint8 locTxBufferWrite;
        uint8 locTxBufferRead;

        do
        { /* Block if software buffer is full, so we don't overwrite. */

        #if ((screen2_TX_BUFFER_SIZE > screen2_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Disable TX interrupt to protect variables from modification */
            screen2_DisableTxInt();
        #endif /* (screen2_TX_BUFFER_SIZE > screen2_MAX_BYTE_VALUE) && (CY_PSOC3) */

            locTxBufferWrite = screen2_txBufferWrite;
            locTxBufferRead  = screen2_txBufferRead;

        #if ((screen2_TX_BUFFER_SIZE > screen2_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Enable interrupt to continue transmission */
            screen2_EnableTxInt();
        #endif /* (screen2_TX_BUFFER_SIZE > screen2_MAX_BYTE_VALUE) && (CY_PSOC3) */
        }
        while( (locTxBufferWrite < locTxBufferRead) ? (locTxBufferWrite == (locTxBufferRead - 1u)) :
                                ((locTxBufferWrite - locTxBufferRead) ==
                                (uint8)(screen2_TX_BUFFER_SIZE - 1u)) );

        if( (locTxBufferRead == locTxBufferWrite) &&
            ((screen2_TXSTATUS_REG & screen2_TX_STS_FIFO_FULL) == 0u) )
        {
            /* Add directly to the FIFO */
            screen2_TXDATA_REG = txDataByte;
        }
        else
        {
            if(locTxBufferWrite >= screen2_TX_BUFFER_SIZE)
            {
                locTxBufferWrite = 0u;
            }
            /* Add to the software buffer. */
            screen2_txBuffer[locTxBufferWrite] = txDataByte;
            locTxBufferWrite++;

            /* Finally, update the real output pointer */
        #if ((screen2_TX_BUFFER_SIZE > screen2_MAX_BYTE_VALUE) && (CY_PSOC3))
            screen2_DisableTxInt();
        #endif /* (screen2_TX_BUFFER_SIZE > screen2_MAX_BYTE_VALUE) && (CY_PSOC3) */

            screen2_txBufferWrite = locTxBufferWrite;

        #if ((screen2_TX_BUFFER_SIZE > screen2_MAX_BYTE_VALUE) && (CY_PSOC3))
            screen2_EnableTxInt();
        #endif /* (screen2_TX_BUFFER_SIZE > screen2_MAX_BYTE_VALUE) && (CY_PSOC3) */

            if(0u != (screen2_TXSTATUS_REG & screen2_TX_STS_FIFO_EMPTY))
            {
                /* Trigger TX interrupt to send software buffer */
                screen2_SetPendingTxInt();
            }
        }

    #else

        while((screen2_TXSTATUS_REG & screen2_TX_STS_FIFO_FULL) != 0u)
        {
            /* Wait for room in the FIFO */
        }

        /* Add directly to the FIFO */
        screen2_TXDATA_REG = txDataByte;

    #endif /* screen2_TX_INTERRUPT_ENABLED */
    }


    /*******************************************************************************
    * Function Name: screen2_PutString
    ********************************************************************************
    *
    * Summary:
    *  Sends a NULL terminated string to the TX buffer for transmission.
    *
    * Parameters:
    *  string[]: Pointer to the null terminated string array residing in RAM or ROM
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  screen2_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  If there is not enough memory in the TX buffer for the entire string, this
    *  function blocks until the last character of the string is loaded into the
    *  TX buffer.
    *
    *******************************************************************************/
    void screen2_PutString(const char8 string[]) 
    {
        uint16 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(screen2_initVar != 0u)
        {
            /* This is a blocking function, it will not exit until all data is sent */
            while(string[bufIndex] != (char8) 0)
            {
                screen2_PutChar((uint8)string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: screen2_PutArray
    ********************************************************************************
    *
    * Summary:
    *  Places N bytes of data from a memory array into the TX buffer for
    *  transmission.
    *
    * Parameters:
    *  string[]: Address of the memory array residing in RAM or ROM.
    *  byteCount: Number of bytes to be transmitted. The type depends on TX Buffer
    *             Size parameter.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  screen2_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  If there is not enough memory in the TX buffer for the entire string, this
    *  function blocks until the last character of the string is loaded into the
    *  TX buffer.
    *
    *******************************************************************************/
    void screen2_PutArray(const uint8 string[], uint8 byteCount)
                                                                    
    {
        uint8 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(screen2_initVar != 0u)
        {
            while(bufIndex < byteCount)
            {
                screen2_PutChar(string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: screen2_PutCRLF
    ********************************************************************************
    *
    * Summary:
    *  Writes a byte of data followed by a carriage return (0x0D) and line feed
    *  (0x0A) to the transmit buffer.
    *
    * Parameters:
    *  txDataByte: Data byte to transmit before the carriage return and line feed.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  screen2_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void screen2_PutCRLF(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function */
        if(screen2_initVar != 0u)
        {
            screen2_PutChar(txDataByte);
            screen2_PutChar(0x0Du);
            screen2_PutChar(0x0Au);
        }
    }


    /*******************************************************************************
    * Function Name: screen2_GetTxBufferSize
    ********************************************************************************
    *
    * Summary:
    *  Returns the number of bytes in the TX buffer which are waiting to be 
    *  transmitted.
    *  * TX software buffer is disabled (TX Buffer Size parameter is equal to 4): 
    *    returns 0 for empty TX FIFO, 1 for not full TX FIFO or 4 for full TX FIFO.
    *  * TX software buffer is enabled: returns the number of bytes in the TX 
    *    software buffer which are waiting to be transmitted. Bytes available in the
    *    TX FIFO do not count.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Number of bytes used in the TX buffer. Return value type depends on the TX 
    *  Buffer Size parameter.
    *
    * Global Variables:
    *  screen2_txBufferWrite - used to calculate left space.
    *  screen2_txBufferRead - used to calculate left space.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the TX Buffer is.
    *
    *******************************************************************************/
    uint8 screen2_GetTxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (screen2_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        screen2_DisableTxInt();

        if(screen2_txBufferRead == screen2_txBufferWrite)
        {
            size = 0u;
        }
        else if(screen2_txBufferRead < screen2_txBufferWrite)
        {
            size = (screen2_txBufferWrite - screen2_txBufferRead);
        }
        else
        {
            size = (screen2_TX_BUFFER_SIZE - screen2_txBufferRead) +
                    screen2_txBufferWrite;
        }

        screen2_EnableTxInt();

    #else

        size = screen2_TXSTATUS_REG;

        /* Is the fifo is full. */
        if((size & screen2_TX_STS_FIFO_FULL) != 0u)
        {
            size = screen2_FIFO_LENGTH;
        }
        else if((size & screen2_TX_STS_FIFO_EMPTY) != 0u)
        {
            size = 0u;
        }
        else
        {
            /* We only know there is data in the fifo. */
            size = 1u;
        }

    #endif /* (screen2_TX_INTERRUPT_ENABLED) */

    return(size);
    }


    /*******************************************************************************
    * Function Name: screen2_ClearTxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Clears all data from the TX buffer and hardware TX FIFO.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  screen2_txBufferWrite - cleared to zero.
    *  screen2_txBufferRead - cleared to zero.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Setting the pointers to zero makes the system believe there is no data to
    *  read and writing will resume at address 0 overwriting any data that may have
    *  remained in the RAM.
    *
    * Side Effects:
    *  Data waiting in the transmit buffer is not sent; a byte that is currently
    *  transmitting finishes transmitting.
    *
    *******************************************************************************/
    void screen2_ClearTxBuffer(void) 
    {
        uint8 enableInterrupts;

        enableInterrupts = CyEnterCriticalSection();
        /* Clear the HW FIFO */
        screen2_TXDATA_AUX_CTL_REG |= (uint8)  screen2_TX_FIFO_CLR;
        screen2_TXDATA_AUX_CTL_REG &= (uint8) ~screen2_TX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (screen2_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        screen2_DisableTxInt();

        screen2_txBufferRead = 0u;
        screen2_txBufferWrite = 0u;

        /* Enable Tx interrupt. */
        screen2_EnableTxInt();

    #endif /* (screen2_TX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: screen2_SendBreak
    ********************************************************************************
    *
    * Summary:
    *  Transmits a break signal on the bus.
    *
    * Parameters:
    *  uint8 retMode:  Send Break return mode. See the following table for options.
    *   screen2_SEND_BREAK - Initialize registers for break, send the Break
    *       signal and return immediately.
    *   screen2_WAIT_FOR_COMPLETE_REINIT - Wait until break transmission is
    *       complete, reinitialize registers to normal transmission mode then return
    *   screen2_REINIT - Reinitialize registers to normal transmission mode
    *       then return.
    *   screen2_SEND_WAIT_REINIT - Performs both options: 
    *      screen2_SEND_BREAK and screen2_WAIT_FOR_COMPLETE_REINIT.
    *      This option is recommended for most cases.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  screen2_initVar - checked to identify that the component has been
    *     initialized.
    *  txPeriod - static variable, used for keeping TX period configuration.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  SendBreak function initializes registers to send 13-bit break signal. It is
    *  important to return the registers configuration to normal for continue 8-bit
    *  operation.
    *  There are 3 variants for this API usage:
    *  1) SendBreak(3) - function will send the Break signal and take care on the
    *     configuration returning. Function will block CPU until transmission
    *     complete.
    *  2) User may want to use blocking time if UART configured to the low speed
    *     operation
    *     Example for this case:
    *     SendBreak(0);     - initialize Break signal transmission
    *         Add your code here to use CPU time
    *     SendBreak(1);     - complete Break operation
    *  3) Same to 2) but user may want to initialize and use the interrupt to
    *     complete break operation.
    *     Example for this case:
    *     Initialize TX interrupt with "TX - On TX Complete" parameter
    *     SendBreak(0);     - initialize Break signal transmission
    *         Add your code here to use CPU time
    *     When interrupt appear with screen2_TX_STS_COMPLETE status:
    *     SendBreak(2);     - complete Break operation
    *
    * Side Effects:
    *  The screen2_SendBreak() function initializes registers to send a
    *  break signal.
    *  Break signal length depends on the break signal bits configuration.
    *  The register configuration should be reinitialized before normal 8-bit
    *  communication can continue.
    *
    *******************************************************************************/
    void screen2_SendBreak(uint8 retMode) 
    {

        /* If not Initialized then skip this function*/
        if(screen2_initVar != 0u)
        {
            /* Set the Counter to 13-bits and transmit a 00 byte */
            /* When that is done then reset the counter value back */
            uint8 tmpStat;

        #if(screen2_HD_ENABLED) /* Half Duplex mode*/

            if( (retMode == screen2_SEND_BREAK) ||
                (retMode == screen2_SEND_WAIT_REINIT ) )
            {
                /* CTRL_HD_SEND_BREAK - sends break bits in HD mode */
                screen2_WriteControlRegister(screen2_ReadControlRegister() |
                                                      screen2_CTRL_HD_SEND_BREAK);
                /* Send zeros */
                screen2_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = screen2_TXSTATUS_REG;
                }
                while((tmpStat & screen2_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == screen2_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == screen2_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = screen2_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & screen2_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == screen2_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == screen2_REINIT) ||
                (retMode == screen2_SEND_WAIT_REINIT) )
            {
                screen2_WriteControlRegister(screen2_ReadControlRegister() &
                                              (uint8)~screen2_CTRL_HD_SEND_BREAK);
            }

        #else /* screen2_HD_ENABLED Full Duplex mode */

            static uint8 txPeriod;

            if( (retMode == screen2_SEND_BREAK) ||
                (retMode == screen2_SEND_WAIT_REINIT) )
            {
                /* CTRL_HD_SEND_BREAK - skip to send parity bit at Break signal in Full Duplex mode */
                #if( (screen2_PARITY_TYPE != screen2__B_UART__NONE_REVB) || \
                                    (screen2_PARITY_TYPE_SW != 0u) )
                    screen2_WriteControlRegister(screen2_ReadControlRegister() |
                                                          screen2_CTRL_HD_SEND_BREAK);
                #endif /* End screen2_PARITY_TYPE != screen2__B_UART__NONE_REVB  */

                #if(screen2_TXCLKGEN_DP)
                    txPeriod = screen2_TXBITCLKTX_COMPLETE_REG;
                    screen2_TXBITCLKTX_COMPLETE_REG = screen2_TXBITCTR_BREAKBITS;
                #else
                    txPeriod = screen2_TXBITCTR_PERIOD_REG;
                    screen2_TXBITCTR_PERIOD_REG = screen2_TXBITCTR_BREAKBITS8X;
                #endif /* End screen2_TXCLKGEN_DP */

                /* Send zeros */
                screen2_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = screen2_TXSTATUS_REG;
                }
                while((tmpStat & screen2_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == screen2_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == screen2_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = screen2_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & screen2_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == screen2_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == screen2_REINIT) ||
                (retMode == screen2_SEND_WAIT_REINIT) )
            {

            #if(screen2_TXCLKGEN_DP)
                screen2_TXBITCLKTX_COMPLETE_REG = txPeriod;
            #else
                screen2_TXBITCTR_PERIOD_REG = txPeriod;
            #endif /* End screen2_TXCLKGEN_DP */

            #if( (screen2_PARITY_TYPE != screen2__B_UART__NONE_REVB) || \
                 (screen2_PARITY_TYPE_SW != 0u) )
                screen2_WriteControlRegister(screen2_ReadControlRegister() &
                                                      (uint8) ~screen2_CTRL_HD_SEND_BREAK);
            #endif /* End screen2_PARITY_TYPE != NONE */
            }
        #endif    /* End screen2_HD_ENABLED */
        }
    }


    /*******************************************************************************
    * Function Name: screen2_SetTxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the transmitter to signal the next bytes is address or data.
    *
    * Parameters:
    *  addressMode: 
    *       screen2_SET_SPACE - Configure the transmitter to send the next
    *                                    byte as a data.
    *       screen2_SET_MARK  - Configure the transmitter to send the next
    *                                    byte as an address.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  This function sets and clears screen2_CTRL_MARK bit in the Control
    *  register.
    *
    *******************************************************************************/
    void screen2_SetTxAddressMode(uint8 addressMode) 
    {
        /* Mark/Space sending enable */
        if(addressMode != 0u)
        {
        #if( screen2_CONTROL_REG_REMOVED == 0u )
            screen2_WriteControlRegister(screen2_ReadControlRegister() |
                                                  screen2_CTRL_MARK);
        #endif /* End screen2_CONTROL_REG_REMOVED == 0u */
        }
        else
        {
        #if( screen2_CONTROL_REG_REMOVED == 0u )
            screen2_WriteControlRegister(screen2_ReadControlRegister() &
                                                  (uint8) ~screen2_CTRL_MARK);
        #endif /* End screen2_CONTROL_REG_REMOVED == 0u */
        }
    }

#endif  /* Endscreen2_TX_ENABLED */

#if(screen2_HD_ENABLED)


    /*******************************************************************************
    * Function Name: screen2_LoadRxConfig
    ********************************************************************************
    *
    * Summary:
    *  Loads the receiver configuration in half duplex mode. After calling this
    *  function, the UART is ready to receive data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  Valid only in half duplex mode. You must make sure that the previous
    *  transaction is complete and it is safe to unload the transmitter
    *  configuration.
    *
    *******************************************************************************/
    void screen2_LoadRxConfig(void) 
    {
        screen2_WriteControlRegister(screen2_ReadControlRegister() &
                                                (uint8)~screen2_CTRL_HD_SEND);
        screen2_RXBITCTR_PERIOD_REG = screen2_HD_RXBITCTR_INIT;

    #if (screen2_RX_INTERRUPT_ENABLED)
        /* Enable RX interrupt after set RX configuration */
        screen2_SetRxInterruptMode(screen2_INIT_RX_INTERRUPTS_MASK);
    #endif /* (screen2_RX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: screen2_LoadTxConfig
    ********************************************************************************
    *
    * Summary:
    *  Loads the transmitter configuration in half duplex mode. After calling this
    *  function, the UART is ready to transmit data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  Valid only in half duplex mode. You must make sure that the previous
    *  transaction is complete and it is safe to unload the receiver configuration.
    *
    *******************************************************************************/
    void screen2_LoadTxConfig(void) 
    {
    #if (screen2_RX_INTERRUPT_ENABLED)
        /* Disable RX interrupts before set TX configuration */
        screen2_SetRxInterruptMode(0u);
    #endif /* (screen2_RX_INTERRUPT_ENABLED) */

        screen2_WriteControlRegister(screen2_ReadControlRegister() | screen2_CTRL_HD_SEND);
        screen2_RXBITCTR_PERIOD_REG = screen2_HD_TXBITCTR_INIT;
    }

#endif  /* screen2_HD_ENABLED */


/* [] END OF FILE */
