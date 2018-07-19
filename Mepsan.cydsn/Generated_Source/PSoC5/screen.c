/*******************************************************************************
* File Name: screen.c
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

#include "screen.h"
#if (screen_INTERNAL_CLOCK_USED)
    #include "screen_IntClock.h"
#endif /* End screen_INTERNAL_CLOCK_USED */


/***************************************
* Global data allocation
***************************************/

uint8 screen_initVar = 0u;

#if (screen_TX_INTERRUPT_ENABLED && screen_TX_ENABLED)
    volatile uint8 screen_txBuffer[screen_TX_BUFFER_SIZE];
    volatile uint8 screen_txBufferRead = 0u;
    uint8 screen_txBufferWrite = 0u;
#endif /* (screen_TX_INTERRUPT_ENABLED && screen_TX_ENABLED) */

#if (screen_RX_INTERRUPT_ENABLED && (screen_RX_ENABLED || screen_HD_ENABLED))
    uint8 screen_errorStatus = 0u;
    volatile uint8 screen_rxBuffer[screen_RX_BUFFER_SIZE];
    volatile uint8 screen_rxBufferRead  = 0u;
    volatile uint8 screen_rxBufferWrite = 0u;
    volatile uint8 screen_rxBufferLoopDetect = 0u;
    volatile uint8 screen_rxBufferOverflow   = 0u;
    #if (screen_RXHW_ADDRESS_ENABLED)
        volatile uint8 screen_rxAddressMode = screen_RX_ADDRESS_MODE;
        volatile uint8 screen_rxAddressDetected = 0u;
    #endif /* (screen_RXHW_ADDRESS_ENABLED) */
#endif /* (screen_RX_INTERRUPT_ENABLED && (screen_RX_ENABLED || screen_HD_ENABLED)) */


/*******************************************************************************
* Function Name: screen_Start
********************************************************************************
*
* Summary:
*  This is the preferred method to begin component operation.
*  screen_Start() sets the initVar variable, calls the
*  screen_Init() function, and then calls the
*  screen_Enable() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  The screen_intiVar variable is used to indicate initial
*  configuration of this component. The variable is initialized to zero (0u)
*  and set to one (1u) the first time screen_Start() is called. This
*  allows for component initialization without re-initialization in all
*  subsequent calls to the screen_Start() routine.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void screen_Start(void) 
{
    /* If not initialized then initialize all required hardware and software */
    if(screen_initVar == 0u)
    {
        screen_Init();
        screen_initVar = 1u;
    }

    screen_Enable();
}


/*******************************************************************************
* Function Name: screen_Init
********************************************************************************
*
* Summary:
*  Initializes or restores the component according to the customizer Configure
*  dialog settings. It is not necessary to call screen_Init() because
*  the screen_Start() API calls this function and is the preferred
*  method to begin component operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void screen_Init(void) 
{
    #if(screen_RX_ENABLED || screen_HD_ENABLED)

        #if (screen_RX_INTERRUPT_ENABLED)
            /* Set RX interrupt vector and priority */
            (void) CyIntSetVector(screen_RX_VECT_NUM, &screen_RXISR);
            CyIntSetPriority(screen_RX_VECT_NUM, screen_RX_PRIOR_NUM);
            screen_errorStatus = 0u;
        #endif /* (screen_RX_INTERRUPT_ENABLED) */

        #if (screen_RXHW_ADDRESS_ENABLED)
            screen_SetRxAddressMode(screen_RX_ADDRESS_MODE);
            screen_SetRxAddress1(screen_RX_HW_ADDRESS1);
            screen_SetRxAddress2(screen_RX_HW_ADDRESS2);
        #endif /* End screen_RXHW_ADDRESS_ENABLED */

        /* Init Count7 period */
        screen_RXBITCTR_PERIOD_REG = screen_RXBITCTR_INIT;
        /* Configure the Initial RX interrupt mask */
        screen_RXSTATUS_MASK_REG  = screen_INIT_RX_INTERRUPTS_MASK;
    #endif /* End screen_RX_ENABLED || screen_HD_ENABLED*/

    #if(screen_TX_ENABLED)
        #if (screen_TX_INTERRUPT_ENABLED)
            /* Set TX interrupt vector and priority */
            (void) CyIntSetVector(screen_TX_VECT_NUM, &screen_TXISR);
            CyIntSetPriority(screen_TX_VECT_NUM, screen_TX_PRIOR_NUM);
        #endif /* (screen_TX_INTERRUPT_ENABLED) */

        /* Write Counter Value for TX Bit Clk Generator*/
        #if (screen_TXCLKGEN_DP)
            screen_TXBITCLKGEN_CTR_REG = screen_BIT_CENTER;
            screen_TXBITCLKTX_COMPLETE_REG = ((screen_NUMBER_OF_DATA_BITS +
                        screen_NUMBER_OF_START_BIT) * screen_OVER_SAMPLE_COUNT) - 1u;
        #else
            screen_TXBITCTR_PERIOD_REG = ((screen_NUMBER_OF_DATA_BITS +
                        screen_NUMBER_OF_START_BIT) * screen_OVER_SAMPLE_8) - 1u;
        #endif /* End screen_TXCLKGEN_DP */

        /* Configure the Initial TX interrupt mask */
        #if (screen_TX_INTERRUPT_ENABLED)
            screen_TXSTATUS_MASK_REG = screen_TX_STS_FIFO_EMPTY;
        #else
            screen_TXSTATUS_MASK_REG = screen_INIT_TX_INTERRUPTS_MASK;
        #endif /*End screen_TX_INTERRUPT_ENABLED*/

    #endif /* End screen_TX_ENABLED */

    #if(screen_PARITY_TYPE_SW)  /* Write Parity to Control Register */
        screen_WriteControlRegister( \
            (screen_ReadControlRegister() & (uint8)~screen_CTRL_PARITY_TYPE_MASK) | \
            (uint8)(screen_PARITY_TYPE << screen_CTRL_PARITY_TYPE0_SHIFT) );
    #endif /* End screen_PARITY_TYPE_SW */
}


/*******************************************************************************
* Function Name: screen_Enable
********************************************************************************
*
* Summary:
*  Activates the hardware and begins component operation. It is not necessary
*  to call screen_Enable() because the screen_Start() API
*  calls this function, which is the preferred method to begin component
*  operation.

* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  screen_rxAddressDetected - set to initial state (0).
*
*******************************************************************************/
void screen_Enable(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    #if (screen_RX_ENABLED || screen_HD_ENABLED)
        /* RX Counter (Count7) Enable */
        screen_RXBITCTR_CONTROL_REG |= screen_CNTR_ENABLE;

        /* Enable the RX Interrupt */
        screen_RXSTATUS_ACTL_REG  |= screen_INT_ENABLE;

        #if (screen_RX_INTERRUPT_ENABLED)
            screen_EnableRxInt();

            #if (screen_RXHW_ADDRESS_ENABLED)
                screen_rxAddressDetected = 0u;
            #endif /* (screen_RXHW_ADDRESS_ENABLED) */
        #endif /* (screen_RX_INTERRUPT_ENABLED) */
    #endif /* (screen_RX_ENABLED || screen_HD_ENABLED) */

    #if(screen_TX_ENABLED)
        /* TX Counter (DP/Count7) Enable */
        #if(!screen_TXCLKGEN_DP)
            screen_TXBITCTR_CONTROL_REG |= screen_CNTR_ENABLE;
        #endif /* End screen_TXCLKGEN_DP */

        /* Enable the TX Interrupt */
        screen_TXSTATUS_ACTL_REG |= screen_INT_ENABLE;
        #if (screen_TX_INTERRUPT_ENABLED)
            screen_ClearPendingTxInt(); /* Clear history of TX_NOT_EMPTY */
            screen_EnableTxInt();
        #endif /* (screen_TX_INTERRUPT_ENABLED) */
     #endif /* (screen_TX_INTERRUPT_ENABLED) */

    #if (screen_INTERNAL_CLOCK_USED)
        screen_IntClock_Start();  /* Enable the clock */
    #endif /* (screen_INTERNAL_CLOCK_USED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: screen_Stop
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
void screen_Stop(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    /* Write Bit Counter Disable */
    #if (screen_RX_ENABLED || screen_HD_ENABLED)
        screen_RXBITCTR_CONTROL_REG &= (uint8) ~screen_CNTR_ENABLE;
    #endif /* (screen_RX_ENABLED || screen_HD_ENABLED) */

    #if (screen_TX_ENABLED)
        #if(!screen_TXCLKGEN_DP)
            screen_TXBITCTR_CONTROL_REG &= (uint8) ~screen_CNTR_ENABLE;
        #endif /* (!screen_TXCLKGEN_DP) */
    #endif /* (screen_TX_ENABLED) */

    #if (screen_INTERNAL_CLOCK_USED)
        screen_IntClock_Stop();   /* Disable the clock */
    #endif /* (screen_INTERNAL_CLOCK_USED) */

    /* Disable internal interrupt component */
    #if (screen_RX_ENABLED || screen_HD_ENABLED)
        screen_RXSTATUS_ACTL_REG  &= (uint8) ~screen_INT_ENABLE;

        #if (screen_RX_INTERRUPT_ENABLED)
            screen_DisableRxInt();
        #endif /* (screen_RX_INTERRUPT_ENABLED) */
    #endif /* (screen_RX_ENABLED || screen_HD_ENABLED) */

    #if (screen_TX_ENABLED)
        screen_TXSTATUS_ACTL_REG &= (uint8) ~screen_INT_ENABLE;

        #if (screen_TX_INTERRUPT_ENABLED)
            screen_DisableTxInt();
        #endif /* (screen_TX_INTERRUPT_ENABLED) */
    #endif /* (screen_TX_ENABLED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: screen_ReadControlRegister
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
uint8 screen_ReadControlRegister(void) 
{
    #if (screen_CONTROL_REG_REMOVED)
        return(0u);
    #else
        return(screen_CONTROL_REG);
    #endif /* (screen_CONTROL_REG_REMOVED) */
}


/*******************************************************************************
* Function Name: screen_WriteControlRegister
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
void  screen_WriteControlRegister(uint8 control) 
{
    #if (screen_CONTROL_REG_REMOVED)
        if(0u != control)
        {
            /* Suppress compiler warning */
        }
    #else
       screen_CONTROL_REG = control;
    #endif /* (screen_CONTROL_REG_REMOVED) */
}


#if(screen_RX_ENABLED || screen_HD_ENABLED)
    /*******************************************************************************
    * Function Name: screen_SetRxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the RX interrupt sources enabled.
    *
    * Parameters:
    *  IntSrc:  Bit field containing the RX interrupts to enable. Based on the 
    *  bit-field arrangement of the status register. This value must be a 
    *  combination of status register bit-masks shown below:
    *      screen_RX_STS_FIFO_NOTEMPTY    Interrupt on byte received.
    *      screen_RX_STS_PAR_ERROR        Interrupt on parity error.
    *      screen_RX_STS_STOP_ERROR       Interrupt on stop error.
    *      screen_RX_STS_BREAK            Interrupt on break.
    *      screen_RX_STS_OVERRUN          Interrupt on overrun error.
    *      screen_RX_STS_ADDR_MATCH       Interrupt on address match.
    *      screen_RX_STS_MRKSPC           Interrupt on address detect.
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void screen_SetRxInterruptMode(uint8 intSrc) 
    {
        screen_RXSTATUS_MASK_REG  = intSrc;
    }


    /*******************************************************************************
    * Function Name: screen_ReadRxData
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
    *  screen_rxBuffer - RAM buffer pointer for save received data.
    *  screen_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  screen_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  screen_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 screen_ReadRxData(void) 
    {
        uint8 rxData;

    #if (screen_RX_INTERRUPT_ENABLED)

        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        screen_DisableRxInt();

        locRxBufferRead  = screen_rxBufferRead;
        locRxBufferWrite = screen_rxBufferWrite;

        if( (screen_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = screen_rxBuffer[locRxBufferRead];
            locRxBufferRead++;

            if(locRxBufferRead >= screen_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            screen_rxBufferRead = locRxBufferRead;

            if(screen_rxBufferLoopDetect != 0u)
            {
                screen_rxBufferLoopDetect = 0u;
                #if ((screen_RX_INTERRUPT_ENABLED) && (screen_FLOW_CONTROL != 0u))
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( screen_HD_ENABLED )
                        if((screen_CONTROL_REG & screen_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only in RX
                            *  configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            screen_RXSTATUS_MASK_REG  |= screen_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        screen_RXSTATUS_MASK_REG  |= screen_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end screen_HD_ENABLED */
                #endif /* ((screen_RX_INTERRUPT_ENABLED) && (screen_FLOW_CONTROL != 0u)) */
            }
        }
        else
        {   /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
            rxData = screen_RXDATA_REG;
        }

        screen_EnableRxInt();

    #else

        /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
        rxData = screen_RXDATA_REG;

    #endif /* (screen_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: screen_ReadRxStatus
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
    *  screen_RX_STS_FIFO_NOTEMPTY.
    *  screen_RX_STS_FIFO_NOTEMPTY clears immediately after RX data
    *  register read.
    *
    * Global Variables:
    *  screen_rxBufferOverflow - used to indicate overload condition.
    *   It set to one in RX interrupt when there isn't free space in
    *   screen_rxBufferRead to write new data. This condition returned
    *   and cleared to zero by this API as an
    *   screen_RX_STS_SOFT_BUFF_OVER bit along with RX Status register
    *   bits.
    *
    *******************************************************************************/
    uint8 screen_ReadRxStatus(void) 
    {
        uint8 status;

        status = screen_RXSTATUS_REG & screen_RX_HW_MASK;

    #if (screen_RX_INTERRUPT_ENABLED)
        if(screen_rxBufferOverflow != 0u)
        {
            status |= screen_RX_STS_SOFT_BUFF_OVER;
            screen_rxBufferOverflow = 0u;
        }
    #endif /* (screen_RX_INTERRUPT_ENABLED) */

        return(status);
    }


    /*******************************************************************************
    * Function Name: screen_GetChar
    ********************************************************************************
    *
    * Summary:
    *  Returns the last received byte of data. screen_GetChar() is
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
    *  screen_rxBuffer - RAM buffer pointer for save received data.
    *  screen_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  screen_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  screen_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 screen_GetChar(void) 
    {
        uint8 rxData = 0u;
        uint8 rxStatus;

    #if (screen_RX_INTERRUPT_ENABLED)
        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        screen_DisableRxInt();

        locRxBufferRead  = screen_rxBufferRead;
        locRxBufferWrite = screen_rxBufferWrite;

        if( (screen_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = screen_rxBuffer[locRxBufferRead];
            locRxBufferRead++;
            if(locRxBufferRead >= screen_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            screen_rxBufferRead = locRxBufferRead;

            if(screen_rxBufferLoopDetect != 0u)
            {
                screen_rxBufferLoopDetect = 0u;
                #if( (screen_RX_INTERRUPT_ENABLED) && (screen_FLOW_CONTROL != 0u) )
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( screen_HD_ENABLED )
                        if((screen_CONTROL_REG & screen_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only if
                            *  RX configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            screen_RXSTATUS_MASK_REG |= screen_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        screen_RXSTATUS_MASK_REG |= screen_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end screen_HD_ENABLED */
                #endif /* screen_RX_INTERRUPT_ENABLED and Hardware flow control*/
            }

        }
        else
        {   rxStatus = screen_RXSTATUS_REG;
            if((rxStatus & screen_RX_STS_FIFO_NOTEMPTY) != 0u)
            {   /* Read received data from FIFO */
                rxData = screen_RXDATA_REG;
                /*Check status on error*/
                if((rxStatus & (screen_RX_STS_BREAK | screen_RX_STS_PAR_ERROR |
                                screen_RX_STS_STOP_ERROR | screen_RX_STS_OVERRUN)) != 0u)
                {
                    rxData = 0u;
                }
            }
        }

        screen_EnableRxInt();

    #else

        rxStatus =screen_RXSTATUS_REG;
        if((rxStatus & screen_RX_STS_FIFO_NOTEMPTY) != 0u)
        {
            /* Read received data from FIFO */
            rxData = screen_RXDATA_REG;

            /*Check status on error*/
            if((rxStatus & (screen_RX_STS_BREAK | screen_RX_STS_PAR_ERROR |
                            screen_RX_STS_STOP_ERROR | screen_RX_STS_OVERRUN)) != 0u)
            {
                rxData = 0u;
            }
        }
    #endif /* (screen_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: screen_GetByte
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
    uint16 screen_GetByte(void) 
    {
        
    #if (screen_RX_INTERRUPT_ENABLED)
        uint16 locErrorStatus;
        /* Protect variables that could change on interrupt */
        screen_DisableRxInt();
        locErrorStatus = (uint16)screen_errorStatus;
        screen_errorStatus = 0u;
        screen_EnableRxInt();
        return ( (uint16)(locErrorStatus << 8u) | screen_ReadRxData() );
    #else
        return ( ((uint16)screen_ReadRxStatus() << 8u) | screen_ReadRxData() );
    #endif /* screen_RX_INTERRUPT_ENABLED */
        
    }


    /*******************************************************************************
    * Function Name: screen_GetRxBufferSize
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
    *  screen_rxBufferWrite - used to calculate left bytes.
    *  screen_rxBufferRead - used to calculate left bytes.
    *  screen_rxBufferLoopDetect - checked to decide left bytes amount.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the RX Buffer is.
    *
    *******************************************************************************/
    uint8 screen_GetRxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (screen_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt */
        screen_DisableRxInt();

        if(screen_rxBufferRead == screen_rxBufferWrite)
        {
            if(screen_rxBufferLoopDetect != 0u)
            {
                size = screen_RX_BUFFER_SIZE;
            }
            else
            {
                size = 0u;
            }
        }
        else if(screen_rxBufferRead < screen_rxBufferWrite)
        {
            size = (screen_rxBufferWrite - screen_rxBufferRead);
        }
        else
        {
            size = (screen_RX_BUFFER_SIZE - screen_rxBufferRead) + screen_rxBufferWrite;
        }

        screen_EnableRxInt();

    #else

        /* We can only know if there is data in the fifo. */
        size = ((screen_RXSTATUS_REG & screen_RX_STS_FIFO_NOTEMPTY) != 0u) ? 1u : 0u;

    #endif /* (screen_RX_INTERRUPT_ENABLED) */

        return(size);
    }


    /*******************************************************************************
    * Function Name: screen_ClearRxBuffer
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
    *  screen_rxBufferWrite - cleared to zero.
    *  screen_rxBufferRead - cleared to zero.
    *  screen_rxBufferLoopDetect - cleared to zero.
    *  screen_rxBufferOverflow - cleared to zero.
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
    void screen_ClearRxBuffer(void) 
    {
        uint8 enableInterrupts;

        /* Clear the HW FIFO */
        enableInterrupts = CyEnterCriticalSection();
        screen_RXDATA_AUX_CTL_REG |= (uint8)  screen_RX_FIFO_CLR;
        screen_RXDATA_AUX_CTL_REG &= (uint8) ~screen_RX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (screen_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        screen_DisableRxInt();

        screen_rxBufferRead = 0u;
        screen_rxBufferWrite = 0u;
        screen_rxBufferLoopDetect = 0u;
        screen_rxBufferOverflow = 0u;

        screen_EnableRxInt();

    #endif /* (screen_RX_INTERRUPT_ENABLED) */

    }


    /*******************************************************************************
    * Function Name: screen_SetRxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Sets the software controlled Addressing mode used by the RX portion of the
    *  UART.
    *
    * Parameters:
    *  addressMode: Enumerated value indicating the mode of RX addressing
    *  screen__B_UART__AM_SW_BYTE_BYTE -  Software Byte-by-Byte address
    *                                               detection
    *  screen__B_UART__AM_SW_DETECT_TO_BUFFER - Software Detect to Buffer
    *                                               address detection
    *  screen__B_UART__AM_HW_BYTE_BY_BYTE - Hardware Byte-by-Byte address
    *                                               detection
    *  screen__B_UART__AM_HW_DETECT_TO_BUFFER - Hardware Detect to Buffer
    *                                               address detection
    *  screen__B_UART__AM_NONE - No address detection
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  screen_rxAddressMode - the parameter stored in this variable for
    *   the farther usage in RX ISR.
    *  screen_rxAddressDetected - set to initial state (0).
    *
    *******************************************************************************/
    void screen_SetRxAddressMode(uint8 addressMode)
                                                        
    {
        #if(screen_RXHW_ADDRESS_ENABLED)
            #if(screen_CONTROL_REG_REMOVED)
                if(0u != addressMode)
                {
                    /* Suppress compiler warning */
                }
            #else /* screen_CONTROL_REG_REMOVED */
                uint8 tmpCtrl;
                tmpCtrl = screen_CONTROL_REG & (uint8)~screen_CTRL_RXADDR_MODE_MASK;
                tmpCtrl |= (uint8)(addressMode << screen_CTRL_RXADDR_MODE0_SHIFT);
                screen_CONTROL_REG = tmpCtrl;

                #if(screen_RX_INTERRUPT_ENABLED && \
                   (screen_RXBUFFERSIZE > screen_FIFO_LENGTH) )
                    screen_rxAddressMode = addressMode;
                    screen_rxAddressDetected = 0u;
                #endif /* End screen_RXBUFFERSIZE > screen_FIFO_LENGTH*/
            #endif /* End screen_CONTROL_REG_REMOVED */
        #else /* screen_RXHW_ADDRESS_ENABLED */
            if(0u != addressMode)
            {
                /* Suppress compiler warning */
            }
        #endif /* End screen_RXHW_ADDRESS_ENABLED */
    }


    /*******************************************************************************
    * Function Name: screen_SetRxAddress1
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
    void screen_SetRxAddress1(uint8 address) 
    {
        screen_RXADDRESS1_REG = address;
    }


    /*******************************************************************************
    * Function Name: screen_SetRxAddress2
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
    void screen_SetRxAddress2(uint8 address) 
    {
        screen_RXADDRESS2_REG = address;
    }

#endif  /* screen_RX_ENABLED || screen_HD_ENABLED*/


#if( (screen_TX_ENABLED) || (screen_HD_ENABLED) )
    /*******************************************************************************
    * Function Name: screen_SetTxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the TX interrupt sources to be enabled, but does not enable the
    *  interrupt.
    *
    * Parameters:
    *  intSrc: Bit field containing the TX interrupt sources to enable
    *   screen_TX_STS_COMPLETE        Interrupt on TX byte complete
    *   screen_TX_STS_FIFO_EMPTY      Interrupt when TX FIFO is empty
    *   screen_TX_STS_FIFO_FULL       Interrupt when TX FIFO is full
    *   screen_TX_STS_FIFO_NOT_FULL   Interrupt when TX FIFO is not full
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void screen_SetTxInterruptMode(uint8 intSrc) 
    {
        screen_TXSTATUS_MASK_REG = intSrc;
    }


    /*******************************************************************************
    * Function Name: screen_WriteTxData
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
    *  screen_txBuffer - RAM buffer pointer for save data for transmission
    *  screen_txBufferWrite - cyclic index for write to txBuffer,
    *    incremented after each byte saved to buffer.
    *  screen_txBufferRead - cyclic index for read from txBuffer,
    *    checked to identify the condition to write to FIFO directly or to TX buffer
    *  screen_initVar - checked to identify that the component has been
    *    initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void screen_WriteTxData(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function*/
        if(screen_initVar != 0u)
        {
        #if (screen_TX_INTERRUPT_ENABLED)

            /* Protect variables that could change on interrupt. */
            screen_DisableTxInt();

            if( (screen_txBufferRead == screen_txBufferWrite) &&
                ((screen_TXSTATUS_REG & screen_TX_STS_FIFO_FULL) == 0u) )
            {
                /* Add directly to the FIFO. */
                screen_TXDATA_REG = txDataByte;
            }
            else
            {
                if(screen_txBufferWrite >= screen_TX_BUFFER_SIZE)
                {
                    screen_txBufferWrite = 0u;
                }

                screen_txBuffer[screen_txBufferWrite] = txDataByte;

                /* Add to the software buffer. */
                screen_txBufferWrite++;
            }

            screen_EnableTxInt();

        #else

            /* Add directly to the FIFO. */
            screen_TXDATA_REG = txDataByte;

        #endif /*(screen_TX_INTERRUPT_ENABLED) */
        }
    }


    /*******************************************************************************
    * Function Name: screen_ReadTxStatus
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
    uint8 screen_ReadTxStatus(void) 
    {
        return(screen_TXSTATUS_REG);
    }


    /*******************************************************************************
    * Function Name: screen_PutChar
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
    *  screen_txBuffer - RAM buffer pointer for save data for transmission
    *  screen_txBufferWrite - cyclic index for write to txBuffer,
    *     checked to identify free space in txBuffer and incremented after each byte
    *     saved to buffer.
    *  screen_txBufferRead - cyclic index for read from txBuffer,
    *     checked to identify free space in txBuffer.
    *  screen_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to transmit any byte of data in a single transfer
    *
    *******************************************************************************/
    void screen_PutChar(uint8 txDataByte) 
    {
    #if (screen_TX_INTERRUPT_ENABLED)
        /* The temporary output pointer is used since it takes two instructions
        *  to increment with a wrap, and we can't risk doing that with the real
        *  pointer and getting an interrupt in between instructions.
        */
        uint8 locTxBufferWrite;
        uint8 locTxBufferRead;

        do
        { /* Block if software buffer is full, so we don't overwrite. */

        #if ((screen_TX_BUFFER_SIZE > screen_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Disable TX interrupt to protect variables from modification */
            screen_DisableTxInt();
        #endif /* (screen_TX_BUFFER_SIZE > screen_MAX_BYTE_VALUE) && (CY_PSOC3) */

            locTxBufferWrite = screen_txBufferWrite;
            locTxBufferRead  = screen_txBufferRead;

        #if ((screen_TX_BUFFER_SIZE > screen_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Enable interrupt to continue transmission */
            screen_EnableTxInt();
        #endif /* (screen_TX_BUFFER_SIZE > screen_MAX_BYTE_VALUE) && (CY_PSOC3) */
        }
        while( (locTxBufferWrite < locTxBufferRead) ? (locTxBufferWrite == (locTxBufferRead - 1u)) :
                                ((locTxBufferWrite - locTxBufferRead) ==
                                (uint8)(screen_TX_BUFFER_SIZE - 1u)) );

        if( (locTxBufferRead == locTxBufferWrite) &&
            ((screen_TXSTATUS_REG & screen_TX_STS_FIFO_FULL) == 0u) )
        {
            /* Add directly to the FIFO */
            screen_TXDATA_REG = txDataByte;
        }
        else
        {
            if(locTxBufferWrite >= screen_TX_BUFFER_SIZE)
            {
                locTxBufferWrite = 0u;
            }
            /* Add to the software buffer. */
            screen_txBuffer[locTxBufferWrite] = txDataByte;
            locTxBufferWrite++;

            /* Finally, update the real output pointer */
        #if ((screen_TX_BUFFER_SIZE > screen_MAX_BYTE_VALUE) && (CY_PSOC3))
            screen_DisableTxInt();
        #endif /* (screen_TX_BUFFER_SIZE > screen_MAX_BYTE_VALUE) && (CY_PSOC3) */

            screen_txBufferWrite = locTxBufferWrite;

        #if ((screen_TX_BUFFER_SIZE > screen_MAX_BYTE_VALUE) && (CY_PSOC3))
            screen_EnableTxInt();
        #endif /* (screen_TX_BUFFER_SIZE > screen_MAX_BYTE_VALUE) && (CY_PSOC3) */

            if(0u != (screen_TXSTATUS_REG & screen_TX_STS_FIFO_EMPTY))
            {
                /* Trigger TX interrupt to send software buffer */
                screen_SetPendingTxInt();
            }
        }

    #else

        while((screen_TXSTATUS_REG & screen_TX_STS_FIFO_FULL) != 0u)
        {
            /* Wait for room in the FIFO */
        }

        /* Add directly to the FIFO */
        screen_TXDATA_REG = txDataByte;

    #endif /* screen_TX_INTERRUPT_ENABLED */
    }


    /*******************************************************************************
    * Function Name: screen_PutString
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
    *  screen_initVar - checked to identify that the component has been
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
    void screen_PutString(const char8 string[]) 
    {
        uint16 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(screen_initVar != 0u)
        {
            /* This is a blocking function, it will not exit until all data is sent */
            while(string[bufIndex] != (char8) 0)
            {
                screen_PutChar((uint8)string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: screen_PutArray
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
    *  screen_initVar - checked to identify that the component has been
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
    void screen_PutArray(const uint8 string[], uint8 byteCount)
                                                                    
    {
        uint8 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(screen_initVar != 0u)
        {
            while(bufIndex < byteCount)
            {
                screen_PutChar(string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: screen_PutCRLF
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
    *  screen_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void screen_PutCRLF(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function */
        if(screen_initVar != 0u)
        {
            screen_PutChar(txDataByte);
            screen_PutChar(0x0Du);
            screen_PutChar(0x0Au);
        }
    }


    /*******************************************************************************
    * Function Name: screen_GetTxBufferSize
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
    *  screen_txBufferWrite - used to calculate left space.
    *  screen_txBufferRead - used to calculate left space.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the TX Buffer is.
    *
    *******************************************************************************/
    uint8 screen_GetTxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (screen_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        screen_DisableTxInt();

        if(screen_txBufferRead == screen_txBufferWrite)
        {
            size = 0u;
        }
        else if(screen_txBufferRead < screen_txBufferWrite)
        {
            size = (screen_txBufferWrite - screen_txBufferRead);
        }
        else
        {
            size = (screen_TX_BUFFER_SIZE - screen_txBufferRead) +
                    screen_txBufferWrite;
        }

        screen_EnableTxInt();

    #else

        size = screen_TXSTATUS_REG;

        /* Is the fifo is full. */
        if((size & screen_TX_STS_FIFO_FULL) != 0u)
        {
            size = screen_FIFO_LENGTH;
        }
        else if((size & screen_TX_STS_FIFO_EMPTY) != 0u)
        {
            size = 0u;
        }
        else
        {
            /* We only know there is data in the fifo. */
            size = 1u;
        }

    #endif /* (screen_TX_INTERRUPT_ENABLED) */

    return(size);
    }


    /*******************************************************************************
    * Function Name: screen_ClearTxBuffer
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
    *  screen_txBufferWrite - cleared to zero.
    *  screen_txBufferRead - cleared to zero.
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
    void screen_ClearTxBuffer(void) 
    {
        uint8 enableInterrupts;

        enableInterrupts = CyEnterCriticalSection();
        /* Clear the HW FIFO */
        screen_TXDATA_AUX_CTL_REG |= (uint8)  screen_TX_FIFO_CLR;
        screen_TXDATA_AUX_CTL_REG &= (uint8) ~screen_TX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (screen_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        screen_DisableTxInt();

        screen_txBufferRead = 0u;
        screen_txBufferWrite = 0u;

        /* Enable Tx interrupt. */
        screen_EnableTxInt();

    #endif /* (screen_TX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: screen_SendBreak
    ********************************************************************************
    *
    * Summary:
    *  Transmits a break signal on the bus.
    *
    * Parameters:
    *  uint8 retMode:  Send Break return mode. See the following table for options.
    *   screen_SEND_BREAK - Initialize registers for break, send the Break
    *       signal and return immediately.
    *   screen_WAIT_FOR_COMPLETE_REINIT - Wait until break transmission is
    *       complete, reinitialize registers to normal transmission mode then return
    *   screen_REINIT - Reinitialize registers to normal transmission mode
    *       then return.
    *   screen_SEND_WAIT_REINIT - Performs both options: 
    *      screen_SEND_BREAK and screen_WAIT_FOR_COMPLETE_REINIT.
    *      This option is recommended for most cases.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  screen_initVar - checked to identify that the component has been
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
    *     When interrupt appear with screen_TX_STS_COMPLETE status:
    *     SendBreak(2);     - complete Break operation
    *
    * Side Effects:
    *  The screen_SendBreak() function initializes registers to send a
    *  break signal.
    *  Break signal length depends on the break signal bits configuration.
    *  The register configuration should be reinitialized before normal 8-bit
    *  communication can continue.
    *
    *******************************************************************************/
    void screen_SendBreak(uint8 retMode) 
    {

        /* If not Initialized then skip this function*/
        if(screen_initVar != 0u)
        {
            /* Set the Counter to 13-bits and transmit a 00 byte */
            /* When that is done then reset the counter value back */
            uint8 tmpStat;

        #if(screen_HD_ENABLED) /* Half Duplex mode*/

            if( (retMode == screen_SEND_BREAK) ||
                (retMode == screen_SEND_WAIT_REINIT ) )
            {
                /* CTRL_HD_SEND_BREAK - sends break bits in HD mode */
                screen_WriteControlRegister(screen_ReadControlRegister() |
                                                      screen_CTRL_HD_SEND_BREAK);
                /* Send zeros */
                screen_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = screen_TXSTATUS_REG;
                }
                while((tmpStat & screen_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == screen_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == screen_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = screen_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & screen_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == screen_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == screen_REINIT) ||
                (retMode == screen_SEND_WAIT_REINIT) )
            {
                screen_WriteControlRegister(screen_ReadControlRegister() &
                                              (uint8)~screen_CTRL_HD_SEND_BREAK);
            }

        #else /* screen_HD_ENABLED Full Duplex mode */

            static uint8 txPeriod;

            if( (retMode == screen_SEND_BREAK) ||
                (retMode == screen_SEND_WAIT_REINIT) )
            {
                /* CTRL_HD_SEND_BREAK - skip to send parity bit at Break signal in Full Duplex mode */
                #if( (screen_PARITY_TYPE != screen__B_UART__NONE_REVB) || \
                                    (screen_PARITY_TYPE_SW != 0u) )
                    screen_WriteControlRegister(screen_ReadControlRegister() |
                                                          screen_CTRL_HD_SEND_BREAK);
                #endif /* End screen_PARITY_TYPE != screen__B_UART__NONE_REVB  */

                #if(screen_TXCLKGEN_DP)
                    txPeriod = screen_TXBITCLKTX_COMPLETE_REG;
                    screen_TXBITCLKTX_COMPLETE_REG = screen_TXBITCTR_BREAKBITS;
                #else
                    txPeriod = screen_TXBITCTR_PERIOD_REG;
                    screen_TXBITCTR_PERIOD_REG = screen_TXBITCTR_BREAKBITS8X;
                #endif /* End screen_TXCLKGEN_DP */

                /* Send zeros */
                screen_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = screen_TXSTATUS_REG;
                }
                while((tmpStat & screen_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == screen_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == screen_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = screen_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & screen_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == screen_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == screen_REINIT) ||
                (retMode == screen_SEND_WAIT_REINIT) )
            {

            #if(screen_TXCLKGEN_DP)
                screen_TXBITCLKTX_COMPLETE_REG = txPeriod;
            #else
                screen_TXBITCTR_PERIOD_REG = txPeriod;
            #endif /* End screen_TXCLKGEN_DP */

            #if( (screen_PARITY_TYPE != screen__B_UART__NONE_REVB) || \
                 (screen_PARITY_TYPE_SW != 0u) )
                screen_WriteControlRegister(screen_ReadControlRegister() &
                                                      (uint8) ~screen_CTRL_HD_SEND_BREAK);
            #endif /* End screen_PARITY_TYPE != NONE */
            }
        #endif    /* End screen_HD_ENABLED */
        }
    }


    /*******************************************************************************
    * Function Name: screen_SetTxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the transmitter to signal the next bytes is address or data.
    *
    * Parameters:
    *  addressMode: 
    *       screen_SET_SPACE - Configure the transmitter to send the next
    *                                    byte as a data.
    *       screen_SET_MARK  - Configure the transmitter to send the next
    *                                    byte as an address.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  This function sets and clears screen_CTRL_MARK bit in the Control
    *  register.
    *
    *******************************************************************************/
    void screen_SetTxAddressMode(uint8 addressMode) 
    {
        /* Mark/Space sending enable */
        if(addressMode != 0u)
        {
        #if( screen_CONTROL_REG_REMOVED == 0u )
            screen_WriteControlRegister(screen_ReadControlRegister() |
                                                  screen_CTRL_MARK);
        #endif /* End screen_CONTROL_REG_REMOVED == 0u */
        }
        else
        {
        #if( screen_CONTROL_REG_REMOVED == 0u )
            screen_WriteControlRegister(screen_ReadControlRegister() &
                                                  (uint8) ~screen_CTRL_MARK);
        #endif /* End screen_CONTROL_REG_REMOVED == 0u */
        }
    }

#endif  /* Endscreen_TX_ENABLED */

#if(screen_HD_ENABLED)


    /*******************************************************************************
    * Function Name: screen_LoadRxConfig
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
    void screen_LoadRxConfig(void) 
    {
        screen_WriteControlRegister(screen_ReadControlRegister() &
                                                (uint8)~screen_CTRL_HD_SEND);
        screen_RXBITCTR_PERIOD_REG = screen_HD_RXBITCTR_INIT;

    #if (screen_RX_INTERRUPT_ENABLED)
        /* Enable RX interrupt after set RX configuration */
        screen_SetRxInterruptMode(screen_INIT_RX_INTERRUPTS_MASK);
    #endif /* (screen_RX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: screen_LoadTxConfig
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
    void screen_LoadTxConfig(void) 
    {
    #if (screen_RX_INTERRUPT_ENABLED)
        /* Disable RX interrupts before set TX configuration */
        screen_SetRxInterruptMode(0u);
    #endif /* (screen_RX_INTERRUPT_ENABLED) */

        screen_WriteControlRegister(screen_ReadControlRegister() | screen_CTRL_HD_SEND);
        screen_RXBITCTR_PERIOD_REG = screen_HD_TXBITCTR_INIT;
    }

#endif  /* screen_HD_ENABLED */


/* [] END OF FILE */
