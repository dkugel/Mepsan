/*******************************************************************************
* File Name: MEPSAN.c
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

#include "MEPSAN.h"
#if (MEPSAN_INTERNAL_CLOCK_USED)
    #include "MEPSAN_IntClock.h"
#endif /* End MEPSAN_INTERNAL_CLOCK_USED */


/***************************************
* Global data allocation
***************************************/

uint8 MEPSAN_initVar = 0u;

#if (MEPSAN_TX_INTERRUPT_ENABLED && MEPSAN_TX_ENABLED)
    volatile uint8 MEPSAN_txBuffer[MEPSAN_TX_BUFFER_SIZE];
    volatile uint8 MEPSAN_txBufferRead = 0u;
    uint8 MEPSAN_txBufferWrite = 0u;
#endif /* (MEPSAN_TX_INTERRUPT_ENABLED && MEPSAN_TX_ENABLED) */

#if (MEPSAN_RX_INTERRUPT_ENABLED && (MEPSAN_RX_ENABLED || MEPSAN_HD_ENABLED))
    uint8 MEPSAN_errorStatus = 0u;
    volatile uint8 MEPSAN_rxBuffer[MEPSAN_RX_BUFFER_SIZE];
    volatile uint8 MEPSAN_rxBufferRead  = 0u;
    volatile uint8 MEPSAN_rxBufferWrite = 0u;
    volatile uint8 MEPSAN_rxBufferLoopDetect = 0u;
    volatile uint8 MEPSAN_rxBufferOverflow   = 0u;
    #if (MEPSAN_RXHW_ADDRESS_ENABLED)
        volatile uint8 MEPSAN_rxAddressMode = MEPSAN_RX_ADDRESS_MODE;
        volatile uint8 MEPSAN_rxAddressDetected = 0u;
    #endif /* (MEPSAN_RXHW_ADDRESS_ENABLED) */
#endif /* (MEPSAN_RX_INTERRUPT_ENABLED && (MEPSAN_RX_ENABLED || MEPSAN_HD_ENABLED)) */


/*******************************************************************************
* Function Name: MEPSAN_Start
********************************************************************************
*
* Summary:
*  This is the preferred method to begin component operation.
*  MEPSAN_Start() sets the initVar variable, calls the
*  MEPSAN_Init() function, and then calls the
*  MEPSAN_Enable() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  The MEPSAN_intiVar variable is used to indicate initial
*  configuration of this component. The variable is initialized to zero (0u)
*  and set to one (1u) the first time MEPSAN_Start() is called. This
*  allows for component initialization without re-initialization in all
*  subsequent calls to the MEPSAN_Start() routine.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void MEPSAN_Start(void) 
{
    /* If not initialized then initialize all required hardware and software */
    if(MEPSAN_initVar == 0u)
    {
        MEPSAN_Init();
        MEPSAN_initVar = 1u;
    }

    MEPSAN_Enable();
}


/*******************************************************************************
* Function Name: MEPSAN_Init
********************************************************************************
*
* Summary:
*  Initializes or restores the component according to the customizer Configure
*  dialog settings. It is not necessary to call MEPSAN_Init() because
*  the MEPSAN_Start() API calls this function and is the preferred
*  method to begin component operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void MEPSAN_Init(void) 
{
    #if(MEPSAN_RX_ENABLED || MEPSAN_HD_ENABLED)

        #if (MEPSAN_RX_INTERRUPT_ENABLED)
            /* Set RX interrupt vector and priority */
            (void) CyIntSetVector(MEPSAN_RX_VECT_NUM, &MEPSAN_RXISR);
            CyIntSetPriority(MEPSAN_RX_VECT_NUM, MEPSAN_RX_PRIOR_NUM);
            MEPSAN_errorStatus = 0u;
        #endif /* (MEPSAN_RX_INTERRUPT_ENABLED) */

        #if (MEPSAN_RXHW_ADDRESS_ENABLED)
            MEPSAN_SetRxAddressMode(MEPSAN_RX_ADDRESS_MODE);
            MEPSAN_SetRxAddress1(MEPSAN_RX_HW_ADDRESS1);
            MEPSAN_SetRxAddress2(MEPSAN_RX_HW_ADDRESS2);
        #endif /* End MEPSAN_RXHW_ADDRESS_ENABLED */

        /* Init Count7 period */
        MEPSAN_RXBITCTR_PERIOD_REG = MEPSAN_RXBITCTR_INIT;
        /* Configure the Initial RX interrupt mask */
        MEPSAN_RXSTATUS_MASK_REG  = MEPSAN_INIT_RX_INTERRUPTS_MASK;
    #endif /* End MEPSAN_RX_ENABLED || MEPSAN_HD_ENABLED*/

    #if(MEPSAN_TX_ENABLED)
        #if (MEPSAN_TX_INTERRUPT_ENABLED)
            /* Set TX interrupt vector and priority */
            (void) CyIntSetVector(MEPSAN_TX_VECT_NUM, &MEPSAN_TXISR);
            CyIntSetPriority(MEPSAN_TX_VECT_NUM, MEPSAN_TX_PRIOR_NUM);
        #endif /* (MEPSAN_TX_INTERRUPT_ENABLED) */

        /* Write Counter Value for TX Bit Clk Generator*/
        #if (MEPSAN_TXCLKGEN_DP)
            MEPSAN_TXBITCLKGEN_CTR_REG = MEPSAN_BIT_CENTER;
            MEPSAN_TXBITCLKTX_COMPLETE_REG = ((MEPSAN_NUMBER_OF_DATA_BITS +
                        MEPSAN_NUMBER_OF_START_BIT) * MEPSAN_OVER_SAMPLE_COUNT) - 1u;
        #else
            MEPSAN_TXBITCTR_PERIOD_REG = ((MEPSAN_NUMBER_OF_DATA_BITS +
                        MEPSAN_NUMBER_OF_START_BIT) * MEPSAN_OVER_SAMPLE_8) - 1u;
        #endif /* End MEPSAN_TXCLKGEN_DP */

        /* Configure the Initial TX interrupt mask */
        #if (MEPSAN_TX_INTERRUPT_ENABLED)
            MEPSAN_TXSTATUS_MASK_REG = MEPSAN_TX_STS_FIFO_EMPTY;
        #else
            MEPSAN_TXSTATUS_MASK_REG = MEPSAN_INIT_TX_INTERRUPTS_MASK;
        #endif /*End MEPSAN_TX_INTERRUPT_ENABLED*/

    #endif /* End MEPSAN_TX_ENABLED */

    #if(MEPSAN_PARITY_TYPE_SW)  /* Write Parity to Control Register */
        MEPSAN_WriteControlRegister( \
            (MEPSAN_ReadControlRegister() & (uint8)~MEPSAN_CTRL_PARITY_TYPE_MASK) | \
            (uint8)(MEPSAN_PARITY_TYPE << MEPSAN_CTRL_PARITY_TYPE0_SHIFT) );
    #endif /* End MEPSAN_PARITY_TYPE_SW */
}


/*******************************************************************************
* Function Name: MEPSAN_Enable
********************************************************************************
*
* Summary:
*  Activates the hardware and begins component operation. It is not necessary
*  to call MEPSAN_Enable() because the MEPSAN_Start() API
*  calls this function, which is the preferred method to begin component
*  operation.

* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  MEPSAN_rxAddressDetected - set to initial state (0).
*
*******************************************************************************/
void MEPSAN_Enable(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    #if (MEPSAN_RX_ENABLED || MEPSAN_HD_ENABLED)
        /* RX Counter (Count7) Enable */
        MEPSAN_RXBITCTR_CONTROL_REG |= MEPSAN_CNTR_ENABLE;

        /* Enable the RX Interrupt */
        MEPSAN_RXSTATUS_ACTL_REG  |= MEPSAN_INT_ENABLE;

        #if (MEPSAN_RX_INTERRUPT_ENABLED)
            MEPSAN_EnableRxInt();

            #if (MEPSAN_RXHW_ADDRESS_ENABLED)
                MEPSAN_rxAddressDetected = 0u;
            #endif /* (MEPSAN_RXHW_ADDRESS_ENABLED) */
        #endif /* (MEPSAN_RX_INTERRUPT_ENABLED) */
    #endif /* (MEPSAN_RX_ENABLED || MEPSAN_HD_ENABLED) */

    #if(MEPSAN_TX_ENABLED)
        /* TX Counter (DP/Count7) Enable */
        #if(!MEPSAN_TXCLKGEN_DP)
            MEPSAN_TXBITCTR_CONTROL_REG |= MEPSAN_CNTR_ENABLE;
        #endif /* End MEPSAN_TXCLKGEN_DP */

        /* Enable the TX Interrupt */
        MEPSAN_TXSTATUS_ACTL_REG |= MEPSAN_INT_ENABLE;
        #if (MEPSAN_TX_INTERRUPT_ENABLED)
            MEPSAN_ClearPendingTxInt(); /* Clear history of TX_NOT_EMPTY */
            MEPSAN_EnableTxInt();
        #endif /* (MEPSAN_TX_INTERRUPT_ENABLED) */
     #endif /* (MEPSAN_TX_INTERRUPT_ENABLED) */

    #if (MEPSAN_INTERNAL_CLOCK_USED)
        MEPSAN_IntClock_Start();  /* Enable the clock */
    #endif /* (MEPSAN_INTERNAL_CLOCK_USED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: MEPSAN_Stop
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
void MEPSAN_Stop(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    /* Write Bit Counter Disable */
    #if (MEPSAN_RX_ENABLED || MEPSAN_HD_ENABLED)
        MEPSAN_RXBITCTR_CONTROL_REG &= (uint8) ~MEPSAN_CNTR_ENABLE;
    #endif /* (MEPSAN_RX_ENABLED || MEPSAN_HD_ENABLED) */

    #if (MEPSAN_TX_ENABLED)
        #if(!MEPSAN_TXCLKGEN_DP)
            MEPSAN_TXBITCTR_CONTROL_REG &= (uint8) ~MEPSAN_CNTR_ENABLE;
        #endif /* (!MEPSAN_TXCLKGEN_DP) */
    #endif /* (MEPSAN_TX_ENABLED) */

    #if (MEPSAN_INTERNAL_CLOCK_USED)
        MEPSAN_IntClock_Stop();   /* Disable the clock */
    #endif /* (MEPSAN_INTERNAL_CLOCK_USED) */

    /* Disable internal interrupt component */
    #if (MEPSAN_RX_ENABLED || MEPSAN_HD_ENABLED)
        MEPSAN_RXSTATUS_ACTL_REG  &= (uint8) ~MEPSAN_INT_ENABLE;

        #if (MEPSAN_RX_INTERRUPT_ENABLED)
            MEPSAN_DisableRxInt();
        #endif /* (MEPSAN_RX_INTERRUPT_ENABLED) */
    #endif /* (MEPSAN_RX_ENABLED || MEPSAN_HD_ENABLED) */

    #if (MEPSAN_TX_ENABLED)
        MEPSAN_TXSTATUS_ACTL_REG &= (uint8) ~MEPSAN_INT_ENABLE;

        #if (MEPSAN_TX_INTERRUPT_ENABLED)
            MEPSAN_DisableTxInt();
        #endif /* (MEPSAN_TX_INTERRUPT_ENABLED) */
    #endif /* (MEPSAN_TX_ENABLED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: MEPSAN_ReadControlRegister
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
uint8 MEPSAN_ReadControlRegister(void) 
{
    #if (MEPSAN_CONTROL_REG_REMOVED)
        return(0u);
    #else
        return(MEPSAN_CONTROL_REG);
    #endif /* (MEPSAN_CONTROL_REG_REMOVED) */
}


/*******************************************************************************
* Function Name: MEPSAN_WriteControlRegister
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
void  MEPSAN_WriteControlRegister(uint8 control) 
{
    #if (MEPSAN_CONTROL_REG_REMOVED)
        if(0u != control)
        {
            /* Suppress compiler warning */
        }
    #else
       MEPSAN_CONTROL_REG = control;
    #endif /* (MEPSAN_CONTROL_REG_REMOVED) */
}


#if(MEPSAN_RX_ENABLED || MEPSAN_HD_ENABLED)
    /*******************************************************************************
    * Function Name: MEPSAN_SetRxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the RX interrupt sources enabled.
    *
    * Parameters:
    *  IntSrc:  Bit field containing the RX interrupts to enable. Based on the 
    *  bit-field arrangement of the status register. This value must be a 
    *  combination of status register bit-masks shown below:
    *      MEPSAN_RX_STS_FIFO_NOTEMPTY    Interrupt on byte received.
    *      MEPSAN_RX_STS_PAR_ERROR        Interrupt on parity error.
    *      MEPSAN_RX_STS_STOP_ERROR       Interrupt on stop error.
    *      MEPSAN_RX_STS_BREAK            Interrupt on break.
    *      MEPSAN_RX_STS_OVERRUN          Interrupt on overrun error.
    *      MEPSAN_RX_STS_ADDR_MATCH       Interrupt on address match.
    *      MEPSAN_RX_STS_MRKSPC           Interrupt on address detect.
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void MEPSAN_SetRxInterruptMode(uint8 intSrc) 
    {
        MEPSAN_RXSTATUS_MASK_REG  = intSrc;
    }


    /*******************************************************************************
    * Function Name: MEPSAN_ReadRxData
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
    *  MEPSAN_rxBuffer - RAM buffer pointer for save received data.
    *  MEPSAN_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  MEPSAN_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  MEPSAN_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 MEPSAN_ReadRxData(void) 
    {
        uint8 rxData;

    #if (MEPSAN_RX_INTERRUPT_ENABLED)

        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        MEPSAN_DisableRxInt();

        locRxBufferRead  = MEPSAN_rxBufferRead;
        locRxBufferWrite = MEPSAN_rxBufferWrite;

        if( (MEPSAN_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = MEPSAN_rxBuffer[locRxBufferRead];
            locRxBufferRead++;

            if(locRxBufferRead >= MEPSAN_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            MEPSAN_rxBufferRead = locRxBufferRead;

            if(MEPSAN_rxBufferLoopDetect != 0u)
            {
                MEPSAN_rxBufferLoopDetect = 0u;
                #if ((MEPSAN_RX_INTERRUPT_ENABLED) && (MEPSAN_FLOW_CONTROL != 0u))
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( MEPSAN_HD_ENABLED )
                        if((MEPSAN_CONTROL_REG & MEPSAN_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only in RX
                            *  configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            MEPSAN_RXSTATUS_MASK_REG  |= MEPSAN_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        MEPSAN_RXSTATUS_MASK_REG  |= MEPSAN_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end MEPSAN_HD_ENABLED */
                #endif /* ((MEPSAN_RX_INTERRUPT_ENABLED) && (MEPSAN_FLOW_CONTROL != 0u)) */
            }
        }
        else
        {   /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
            rxData = MEPSAN_RXDATA_REG;
        }

        MEPSAN_EnableRxInt();

    #else

        /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
        rxData = MEPSAN_RXDATA_REG;

    #endif /* (MEPSAN_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: MEPSAN_ReadRxStatus
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
    *  MEPSAN_RX_STS_FIFO_NOTEMPTY.
    *  MEPSAN_RX_STS_FIFO_NOTEMPTY clears immediately after RX data
    *  register read.
    *
    * Global Variables:
    *  MEPSAN_rxBufferOverflow - used to indicate overload condition.
    *   It set to one in RX interrupt when there isn't free space in
    *   MEPSAN_rxBufferRead to write new data. This condition returned
    *   and cleared to zero by this API as an
    *   MEPSAN_RX_STS_SOFT_BUFF_OVER bit along with RX Status register
    *   bits.
    *
    *******************************************************************************/
    uint8 MEPSAN_ReadRxStatus(void) 
    {
        uint8 status;

        status = MEPSAN_RXSTATUS_REG & MEPSAN_RX_HW_MASK;

    #if (MEPSAN_RX_INTERRUPT_ENABLED)
        if(MEPSAN_rxBufferOverflow != 0u)
        {
            status |= MEPSAN_RX_STS_SOFT_BUFF_OVER;
            MEPSAN_rxBufferOverflow = 0u;
        }
    #endif /* (MEPSAN_RX_INTERRUPT_ENABLED) */

        return(status);
    }


    /*******************************************************************************
    * Function Name: MEPSAN_GetChar
    ********************************************************************************
    *
    * Summary:
    *  Returns the last received byte of data. MEPSAN_GetChar() is
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
    *  MEPSAN_rxBuffer - RAM buffer pointer for save received data.
    *  MEPSAN_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  MEPSAN_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  MEPSAN_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 MEPSAN_GetChar(void) 
    {
        uint8 rxData = 0u;
        uint8 rxStatus;

    #if (MEPSAN_RX_INTERRUPT_ENABLED)
        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        MEPSAN_DisableRxInt();

        locRxBufferRead  = MEPSAN_rxBufferRead;
        locRxBufferWrite = MEPSAN_rxBufferWrite;

        if( (MEPSAN_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = MEPSAN_rxBuffer[locRxBufferRead];
            locRxBufferRead++;
            if(locRxBufferRead >= MEPSAN_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            MEPSAN_rxBufferRead = locRxBufferRead;

            if(MEPSAN_rxBufferLoopDetect != 0u)
            {
                MEPSAN_rxBufferLoopDetect = 0u;
                #if( (MEPSAN_RX_INTERRUPT_ENABLED) && (MEPSAN_FLOW_CONTROL != 0u) )
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( MEPSAN_HD_ENABLED )
                        if((MEPSAN_CONTROL_REG & MEPSAN_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only if
                            *  RX configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            MEPSAN_RXSTATUS_MASK_REG |= MEPSAN_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        MEPSAN_RXSTATUS_MASK_REG |= MEPSAN_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end MEPSAN_HD_ENABLED */
                #endif /* MEPSAN_RX_INTERRUPT_ENABLED and Hardware flow control*/
            }

        }
        else
        {   rxStatus = MEPSAN_RXSTATUS_REG;
            if((rxStatus & MEPSAN_RX_STS_FIFO_NOTEMPTY) != 0u)
            {   /* Read received data from FIFO */
                rxData = MEPSAN_RXDATA_REG;
                /*Check status on error*/
                if((rxStatus & (MEPSAN_RX_STS_BREAK | MEPSAN_RX_STS_PAR_ERROR |
                                MEPSAN_RX_STS_STOP_ERROR | MEPSAN_RX_STS_OVERRUN)) != 0u)
                {
                    rxData = 0u;
                }
            }
        }

        MEPSAN_EnableRxInt();

    #else

        rxStatus =MEPSAN_RXSTATUS_REG;
        if((rxStatus & MEPSAN_RX_STS_FIFO_NOTEMPTY) != 0u)
        {
            /* Read received data from FIFO */
            rxData = MEPSAN_RXDATA_REG;

            /*Check status on error*/
            if((rxStatus & (MEPSAN_RX_STS_BREAK | MEPSAN_RX_STS_PAR_ERROR |
                            MEPSAN_RX_STS_STOP_ERROR | MEPSAN_RX_STS_OVERRUN)) != 0u)
            {
                rxData = 0u;
            }
        }
    #endif /* (MEPSAN_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: MEPSAN_GetByte
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
    uint16 MEPSAN_GetByte(void) 
    {
        
    #if (MEPSAN_RX_INTERRUPT_ENABLED)
        uint16 locErrorStatus;
        /* Protect variables that could change on interrupt */
        MEPSAN_DisableRxInt();
        locErrorStatus = (uint16)MEPSAN_errorStatus;
        MEPSAN_errorStatus = 0u;
        MEPSAN_EnableRxInt();
        return ( (uint16)(locErrorStatus << 8u) | MEPSAN_ReadRxData() );
    #else
        return ( ((uint16)MEPSAN_ReadRxStatus() << 8u) | MEPSAN_ReadRxData() );
    #endif /* MEPSAN_RX_INTERRUPT_ENABLED */
        
    }


    /*******************************************************************************
    * Function Name: MEPSAN_GetRxBufferSize
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
    *  MEPSAN_rxBufferWrite - used to calculate left bytes.
    *  MEPSAN_rxBufferRead - used to calculate left bytes.
    *  MEPSAN_rxBufferLoopDetect - checked to decide left bytes amount.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the RX Buffer is.
    *
    *******************************************************************************/
    uint8 MEPSAN_GetRxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (MEPSAN_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt */
        MEPSAN_DisableRxInt();

        if(MEPSAN_rxBufferRead == MEPSAN_rxBufferWrite)
        {
            if(MEPSAN_rxBufferLoopDetect != 0u)
            {
                size = MEPSAN_RX_BUFFER_SIZE;
            }
            else
            {
                size = 0u;
            }
        }
        else if(MEPSAN_rxBufferRead < MEPSAN_rxBufferWrite)
        {
            size = (MEPSAN_rxBufferWrite - MEPSAN_rxBufferRead);
        }
        else
        {
            size = (MEPSAN_RX_BUFFER_SIZE - MEPSAN_rxBufferRead) + MEPSAN_rxBufferWrite;
        }

        MEPSAN_EnableRxInt();

    #else

        /* We can only know if there is data in the fifo. */
        size = ((MEPSAN_RXSTATUS_REG & MEPSAN_RX_STS_FIFO_NOTEMPTY) != 0u) ? 1u : 0u;

    #endif /* (MEPSAN_RX_INTERRUPT_ENABLED) */

        return(size);
    }


    /*******************************************************************************
    * Function Name: MEPSAN_ClearRxBuffer
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
    *  MEPSAN_rxBufferWrite - cleared to zero.
    *  MEPSAN_rxBufferRead - cleared to zero.
    *  MEPSAN_rxBufferLoopDetect - cleared to zero.
    *  MEPSAN_rxBufferOverflow - cleared to zero.
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
    void MEPSAN_ClearRxBuffer(void) 
    {
        uint8 enableInterrupts;

        /* Clear the HW FIFO */
        enableInterrupts = CyEnterCriticalSection();
        MEPSAN_RXDATA_AUX_CTL_REG |= (uint8)  MEPSAN_RX_FIFO_CLR;
        MEPSAN_RXDATA_AUX_CTL_REG &= (uint8) ~MEPSAN_RX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (MEPSAN_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        MEPSAN_DisableRxInt();

        MEPSAN_rxBufferRead = 0u;
        MEPSAN_rxBufferWrite = 0u;
        MEPSAN_rxBufferLoopDetect = 0u;
        MEPSAN_rxBufferOverflow = 0u;

        MEPSAN_EnableRxInt();

    #endif /* (MEPSAN_RX_INTERRUPT_ENABLED) */

    }


    /*******************************************************************************
    * Function Name: MEPSAN_SetRxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Sets the software controlled Addressing mode used by the RX portion of the
    *  UART.
    *
    * Parameters:
    *  addressMode: Enumerated value indicating the mode of RX addressing
    *  MEPSAN__B_UART__AM_SW_BYTE_BYTE -  Software Byte-by-Byte address
    *                                               detection
    *  MEPSAN__B_UART__AM_SW_DETECT_TO_BUFFER - Software Detect to Buffer
    *                                               address detection
    *  MEPSAN__B_UART__AM_HW_BYTE_BY_BYTE - Hardware Byte-by-Byte address
    *                                               detection
    *  MEPSAN__B_UART__AM_HW_DETECT_TO_BUFFER - Hardware Detect to Buffer
    *                                               address detection
    *  MEPSAN__B_UART__AM_NONE - No address detection
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  MEPSAN_rxAddressMode - the parameter stored in this variable for
    *   the farther usage in RX ISR.
    *  MEPSAN_rxAddressDetected - set to initial state (0).
    *
    *******************************************************************************/
    void MEPSAN_SetRxAddressMode(uint8 addressMode)
                                                        
    {
        #if(MEPSAN_RXHW_ADDRESS_ENABLED)
            #if(MEPSAN_CONTROL_REG_REMOVED)
                if(0u != addressMode)
                {
                    /* Suppress compiler warning */
                }
            #else /* MEPSAN_CONTROL_REG_REMOVED */
                uint8 tmpCtrl;
                tmpCtrl = MEPSAN_CONTROL_REG & (uint8)~MEPSAN_CTRL_RXADDR_MODE_MASK;
                tmpCtrl |= (uint8)(addressMode << MEPSAN_CTRL_RXADDR_MODE0_SHIFT);
                MEPSAN_CONTROL_REG = tmpCtrl;

                #if(MEPSAN_RX_INTERRUPT_ENABLED && \
                   (MEPSAN_RXBUFFERSIZE > MEPSAN_FIFO_LENGTH) )
                    MEPSAN_rxAddressMode = addressMode;
                    MEPSAN_rxAddressDetected = 0u;
                #endif /* End MEPSAN_RXBUFFERSIZE > MEPSAN_FIFO_LENGTH*/
            #endif /* End MEPSAN_CONTROL_REG_REMOVED */
        #else /* MEPSAN_RXHW_ADDRESS_ENABLED */
            if(0u != addressMode)
            {
                /* Suppress compiler warning */
            }
        #endif /* End MEPSAN_RXHW_ADDRESS_ENABLED */
    }


    /*******************************************************************************
    * Function Name: MEPSAN_SetRxAddress1
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
    void MEPSAN_SetRxAddress1(uint8 address) 
    {
        MEPSAN_RXADDRESS1_REG = address;
    }


    /*******************************************************************************
    * Function Name: MEPSAN_SetRxAddress2
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
    void MEPSAN_SetRxAddress2(uint8 address) 
    {
        MEPSAN_RXADDRESS2_REG = address;
    }

#endif  /* MEPSAN_RX_ENABLED || MEPSAN_HD_ENABLED*/


#if( (MEPSAN_TX_ENABLED) || (MEPSAN_HD_ENABLED) )
    /*******************************************************************************
    * Function Name: MEPSAN_SetTxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the TX interrupt sources to be enabled, but does not enable the
    *  interrupt.
    *
    * Parameters:
    *  intSrc: Bit field containing the TX interrupt sources to enable
    *   MEPSAN_TX_STS_COMPLETE        Interrupt on TX byte complete
    *   MEPSAN_TX_STS_FIFO_EMPTY      Interrupt when TX FIFO is empty
    *   MEPSAN_TX_STS_FIFO_FULL       Interrupt when TX FIFO is full
    *   MEPSAN_TX_STS_FIFO_NOT_FULL   Interrupt when TX FIFO is not full
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void MEPSAN_SetTxInterruptMode(uint8 intSrc) 
    {
        MEPSAN_TXSTATUS_MASK_REG = intSrc;
    }


    /*******************************************************************************
    * Function Name: MEPSAN_WriteTxData
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
    *  MEPSAN_txBuffer - RAM buffer pointer for save data for transmission
    *  MEPSAN_txBufferWrite - cyclic index for write to txBuffer,
    *    incremented after each byte saved to buffer.
    *  MEPSAN_txBufferRead - cyclic index for read from txBuffer,
    *    checked to identify the condition to write to FIFO directly or to TX buffer
    *  MEPSAN_initVar - checked to identify that the component has been
    *    initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void MEPSAN_WriteTxData(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function*/
        if(MEPSAN_initVar != 0u)
        {
        #if (MEPSAN_TX_INTERRUPT_ENABLED)

            /* Protect variables that could change on interrupt. */
            MEPSAN_DisableTxInt();

            if( (MEPSAN_txBufferRead == MEPSAN_txBufferWrite) &&
                ((MEPSAN_TXSTATUS_REG & MEPSAN_TX_STS_FIFO_FULL) == 0u) )
            {
                /* Add directly to the FIFO. */
                MEPSAN_TXDATA_REG = txDataByte;
            }
            else
            {
                if(MEPSAN_txBufferWrite >= MEPSAN_TX_BUFFER_SIZE)
                {
                    MEPSAN_txBufferWrite = 0u;
                }

                MEPSAN_txBuffer[MEPSAN_txBufferWrite] = txDataByte;

                /* Add to the software buffer. */
                MEPSAN_txBufferWrite++;
            }

            MEPSAN_EnableTxInt();

        #else

            /* Add directly to the FIFO. */
            MEPSAN_TXDATA_REG = txDataByte;

        #endif /*(MEPSAN_TX_INTERRUPT_ENABLED) */
        }
    }


    /*******************************************************************************
    * Function Name: MEPSAN_ReadTxStatus
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
    uint8 MEPSAN_ReadTxStatus(void) 
    {
        return(MEPSAN_TXSTATUS_REG);
    }


    /*******************************************************************************
    * Function Name: MEPSAN_PutChar
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
    *  MEPSAN_txBuffer - RAM buffer pointer for save data for transmission
    *  MEPSAN_txBufferWrite - cyclic index for write to txBuffer,
    *     checked to identify free space in txBuffer and incremented after each byte
    *     saved to buffer.
    *  MEPSAN_txBufferRead - cyclic index for read from txBuffer,
    *     checked to identify free space in txBuffer.
    *  MEPSAN_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to transmit any byte of data in a single transfer
    *
    *******************************************************************************/
    void MEPSAN_PutChar(uint8 txDataByte) 
    {
    #if (MEPSAN_TX_INTERRUPT_ENABLED)
        /* The temporary output pointer is used since it takes two instructions
        *  to increment with a wrap, and we can't risk doing that with the real
        *  pointer and getting an interrupt in between instructions.
        */
        uint8 locTxBufferWrite;
        uint8 locTxBufferRead;

        do
        { /* Block if software buffer is full, so we don't overwrite. */

        #if ((MEPSAN_TX_BUFFER_SIZE > MEPSAN_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Disable TX interrupt to protect variables from modification */
            MEPSAN_DisableTxInt();
        #endif /* (MEPSAN_TX_BUFFER_SIZE > MEPSAN_MAX_BYTE_VALUE) && (CY_PSOC3) */

            locTxBufferWrite = MEPSAN_txBufferWrite;
            locTxBufferRead  = MEPSAN_txBufferRead;

        #if ((MEPSAN_TX_BUFFER_SIZE > MEPSAN_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Enable interrupt to continue transmission */
            MEPSAN_EnableTxInt();
        #endif /* (MEPSAN_TX_BUFFER_SIZE > MEPSAN_MAX_BYTE_VALUE) && (CY_PSOC3) */
        }
        while( (locTxBufferWrite < locTxBufferRead) ? (locTxBufferWrite == (locTxBufferRead - 1u)) :
                                ((locTxBufferWrite - locTxBufferRead) ==
                                (uint8)(MEPSAN_TX_BUFFER_SIZE - 1u)) );

        if( (locTxBufferRead == locTxBufferWrite) &&
            ((MEPSAN_TXSTATUS_REG & MEPSAN_TX_STS_FIFO_FULL) == 0u) )
        {
            /* Add directly to the FIFO */
            MEPSAN_TXDATA_REG = txDataByte;
        }
        else
        {
            if(locTxBufferWrite >= MEPSAN_TX_BUFFER_SIZE)
            {
                locTxBufferWrite = 0u;
            }
            /* Add to the software buffer. */
            MEPSAN_txBuffer[locTxBufferWrite] = txDataByte;
            locTxBufferWrite++;

            /* Finally, update the real output pointer */
        #if ((MEPSAN_TX_BUFFER_SIZE > MEPSAN_MAX_BYTE_VALUE) && (CY_PSOC3))
            MEPSAN_DisableTxInt();
        #endif /* (MEPSAN_TX_BUFFER_SIZE > MEPSAN_MAX_BYTE_VALUE) && (CY_PSOC3) */

            MEPSAN_txBufferWrite = locTxBufferWrite;

        #if ((MEPSAN_TX_BUFFER_SIZE > MEPSAN_MAX_BYTE_VALUE) && (CY_PSOC3))
            MEPSAN_EnableTxInt();
        #endif /* (MEPSAN_TX_BUFFER_SIZE > MEPSAN_MAX_BYTE_VALUE) && (CY_PSOC3) */

            if(0u != (MEPSAN_TXSTATUS_REG & MEPSAN_TX_STS_FIFO_EMPTY))
            {
                /* Trigger TX interrupt to send software buffer */
                MEPSAN_SetPendingTxInt();
            }
        }

    #else

        while((MEPSAN_TXSTATUS_REG & MEPSAN_TX_STS_FIFO_FULL) != 0u)
        {
            /* Wait for room in the FIFO */
        }

        /* Add directly to the FIFO */
        MEPSAN_TXDATA_REG = txDataByte;

    #endif /* MEPSAN_TX_INTERRUPT_ENABLED */
    }


    /*******************************************************************************
    * Function Name: MEPSAN_PutString
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
    *  MEPSAN_initVar - checked to identify that the component has been
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
    void MEPSAN_PutString(const char8 string[]) 
    {
        uint16 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(MEPSAN_initVar != 0u)
        {
            /* This is a blocking function, it will not exit until all data is sent */
            while(string[bufIndex] != (char8) 0)
            {
                MEPSAN_PutChar((uint8)string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: MEPSAN_PutArray
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
    *  MEPSAN_initVar - checked to identify that the component has been
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
    void MEPSAN_PutArray(const uint8 string[], uint8 byteCount)
                                                                    
    {
        uint8 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(MEPSAN_initVar != 0u)
        {
            while(bufIndex < byteCount)
            {
                MEPSAN_PutChar(string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: MEPSAN_PutCRLF
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
    *  MEPSAN_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void MEPSAN_PutCRLF(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function */
        if(MEPSAN_initVar != 0u)
        {
            MEPSAN_PutChar(txDataByte);
            MEPSAN_PutChar(0x0Du);
            MEPSAN_PutChar(0x0Au);
        }
    }


    /*******************************************************************************
    * Function Name: MEPSAN_GetTxBufferSize
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
    *  MEPSAN_txBufferWrite - used to calculate left space.
    *  MEPSAN_txBufferRead - used to calculate left space.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the TX Buffer is.
    *
    *******************************************************************************/
    uint8 MEPSAN_GetTxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (MEPSAN_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        MEPSAN_DisableTxInt();

        if(MEPSAN_txBufferRead == MEPSAN_txBufferWrite)
        {
            size = 0u;
        }
        else if(MEPSAN_txBufferRead < MEPSAN_txBufferWrite)
        {
            size = (MEPSAN_txBufferWrite - MEPSAN_txBufferRead);
        }
        else
        {
            size = (MEPSAN_TX_BUFFER_SIZE - MEPSAN_txBufferRead) +
                    MEPSAN_txBufferWrite;
        }

        MEPSAN_EnableTxInt();

    #else

        size = MEPSAN_TXSTATUS_REG;

        /* Is the fifo is full. */
        if((size & MEPSAN_TX_STS_FIFO_FULL) != 0u)
        {
            size = MEPSAN_FIFO_LENGTH;
        }
        else if((size & MEPSAN_TX_STS_FIFO_EMPTY) != 0u)
        {
            size = 0u;
        }
        else
        {
            /* We only know there is data in the fifo. */
            size = 1u;
        }

    #endif /* (MEPSAN_TX_INTERRUPT_ENABLED) */

    return(size);
    }


    /*******************************************************************************
    * Function Name: MEPSAN_ClearTxBuffer
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
    *  MEPSAN_txBufferWrite - cleared to zero.
    *  MEPSAN_txBufferRead - cleared to zero.
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
    void MEPSAN_ClearTxBuffer(void) 
    {
        uint8 enableInterrupts;

        enableInterrupts = CyEnterCriticalSection();
        /* Clear the HW FIFO */
        MEPSAN_TXDATA_AUX_CTL_REG |= (uint8)  MEPSAN_TX_FIFO_CLR;
        MEPSAN_TXDATA_AUX_CTL_REG &= (uint8) ~MEPSAN_TX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (MEPSAN_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        MEPSAN_DisableTxInt();

        MEPSAN_txBufferRead = 0u;
        MEPSAN_txBufferWrite = 0u;

        /* Enable Tx interrupt. */
        MEPSAN_EnableTxInt();

    #endif /* (MEPSAN_TX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: MEPSAN_SendBreak
    ********************************************************************************
    *
    * Summary:
    *  Transmits a break signal on the bus.
    *
    * Parameters:
    *  uint8 retMode:  Send Break return mode. See the following table for options.
    *   MEPSAN_SEND_BREAK - Initialize registers for break, send the Break
    *       signal and return immediately.
    *   MEPSAN_WAIT_FOR_COMPLETE_REINIT - Wait until break transmission is
    *       complete, reinitialize registers to normal transmission mode then return
    *   MEPSAN_REINIT - Reinitialize registers to normal transmission mode
    *       then return.
    *   MEPSAN_SEND_WAIT_REINIT - Performs both options: 
    *      MEPSAN_SEND_BREAK and MEPSAN_WAIT_FOR_COMPLETE_REINIT.
    *      This option is recommended for most cases.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  MEPSAN_initVar - checked to identify that the component has been
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
    *     When interrupt appear with MEPSAN_TX_STS_COMPLETE status:
    *     SendBreak(2);     - complete Break operation
    *
    * Side Effects:
    *  The MEPSAN_SendBreak() function initializes registers to send a
    *  break signal.
    *  Break signal length depends on the break signal bits configuration.
    *  The register configuration should be reinitialized before normal 8-bit
    *  communication can continue.
    *
    *******************************************************************************/
    void MEPSAN_SendBreak(uint8 retMode) 
    {

        /* If not Initialized then skip this function*/
        if(MEPSAN_initVar != 0u)
        {
            /* Set the Counter to 13-bits and transmit a 00 byte */
            /* When that is done then reset the counter value back */
            uint8 tmpStat;

        #if(MEPSAN_HD_ENABLED) /* Half Duplex mode*/

            if( (retMode == MEPSAN_SEND_BREAK) ||
                (retMode == MEPSAN_SEND_WAIT_REINIT ) )
            {
                /* CTRL_HD_SEND_BREAK - sends break bits in HD mode */
                MEPSAN_WriteControlRegister(MEPSAN_ReadControlRegister() |
                                                      MEPSAN_CTRL_HD_SEND_BREAK);
                /* Send zeros */
                MEPSAN_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = MEPSAN_TXSTATUS_REG;
                }
                while((tmpStat & MEPSAN_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == MEPSAN_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == MEPSAN_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = MEPSAN_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & MEPSAN_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == MEPSAN_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == MEPSAN_REINIT) ||
                (retMode == MEPSAN_SEND_WAIT_REINIT) )
            {
                MEPSAN_WriteControlRegister(MEPSAN_ReadControlRegister() &
                                              (uint8)~MEPSAN_CTRL_HD_SEND_BREAK);
            }

        #else /* MEPSAN_HD_ENABLED Full Duplex mode */

            static uint8 txPeriod;

            if( (retMode == MEPSAN_SEND_BREAK) ||
                (retMode == MEPSAN_SEND_WAIT_REINIT) )
            {
                /* CTRL_HD_SEND_BREAK - skip to send parity bit at Break signal in Full Duplex mode */
                #if( (MEPSAN_PARITY_TYPE != MEPSAN__B_UART__NONE_REVB) || \
                                    (MEPSAN_PARITY_TYPE_SW != 0u) )
                    MEPSAN_WriteControlRegister(MEPSAN_ReadControlRegister() |
                                                          MEPSAN_CTRL_HD_SEND_BREAK);
                #endif /* End MEPSAN_PARITY_TYPE != MEPSAN__B_UART__NONE_REVB  */

                #if(MEPSAN_TXCLKGEN_DP)
                    txPeriod = MEPSAN_TXBITCLKTX_COMPLETE_REG;
                    MEPSAN_TXBITCLKTX_COMPLETE_REG = MEPSAN_TXBITCTR_BREAKBITS;
                #else
                    txPeriod = MEPSAN_TXBITCTR_PERIOD_REG;
                    MEPSAN_TXBITCTR_PERIOD_REG = MEPSAN_TXBITCTR_BREAKBITS8X;
                #endif /* End MEPSAN_TXCLKGEN_DP */

                /* Send zeros */
                MEPSAN_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = MEPSAN_TXSTATUS_REG;
                }
                while((tmpStat & MEPSAN_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == MEPSAN_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == MEPSAN_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = MEPSAN_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & MEPSAN_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == MEPSAN_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == MEPSAN_REINIT) ||
                (retMode == MEPSAN_SEND_WAIT_REINIT) )
            {

            #if(MEPSAN_TXCLKGEN_DP)
                MEPSAN_TXBITCLKTX_COMPLETE_REG = txPeriod;
            #else
                MEPSAN_TXBITCTR_PERIOD_REG = txPeriod;
            #endif /* End MEPSAN_TXCLKGEN_DP */

            #if( (MEPSAN_PARITY_TYPE != MEPSAN__B_UART__NONE_REVB) || \
                 (MEPSAN_PARITY_TYPE_SW != 0u) )
                MEPSAN_WriteControlRegister(MEPSAN_ReadControlRegister() &
                                                      (uint8) ~MEPSAN_CTRL_HD_SEND_BREAK);
            #endif /* End MEPSAN_PARITY_TYPE != NONE */
            }
        #endif    /* End MEPSAN_HD_ENABLED */
        }
    }


    /*******************************************************************************
    * Function Name: MEPSAN_SetTxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the transmitter to signal the next bytes is address or data.
    *
    * Parameters:
    *  addressMode: 
    *       MEPSAN_SET_SPACE - Configure the transmitter to send the next
    *                                    byte as a data.
    *       MEPSAN_SET_MARK  - Configure the transmitter to send the next
    *                                    byte as an address.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  This function sets and clears MEPSAN_CTRL_MARK bit in the Control
    *  register.
    *
    *******************************************************************************/
    void MEPSAN_SetTxAddressMode(uint8 addressMode) 
    {
        /* Mark/Space sending enable */
        if(addressMode != 0u)
        {
        #if( MEPSAN_CONTROL_REG_REMOVED == 0u )
            MEPSAN_WriteControlRegister(MEPSAN_ReadControlRegister() |
                                                  MEPSAN_CTRL_MARK);
        #endif /* End MEPSAN_CONTROL_REG_REMOVED == 0u */
        }
        else
        {
        #if( MEPSAN_CONTROL_REG_REMOVED == 0u )
            MEPSAN_WriteControlRegister(MEPSAN_ReadControlRegister() &
                                                  (uint8) ~MEPSAN_CTRL_MARK);
        #endif /* End MEPSAN_CONTROL_REG_REMOVED == 0u */
        }
    }

#endif  /* EndMEPSAN_TX_ENABLED */

#if(MEPSAN_HD_ENABLED)


    /*******************************************************************************
    * Function Name: MEPSAN_LoadRxConfig
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
    void MEPSAN_LoadRxConfig(void) 
    {
        MEPSAN_WriteControlRegister(MEPSAN_ReadControlRegister() &
                                                (uint8)~MEPSAN_CTRL_HD_SEND);
        MEPSAN_RXBITCTR_PERIOD_REG = MEPSAN_HD_RXBITCTR_INIT;

    #if (MEPSAN_RX_INTERRUPT_ENABLED)
        /* Enable RX interrupt after set RX configuration */
        MEPSAN_SetRxInterruptMode(MEPSAN_INIT_RX_INTERRUPTS_MASK);
    #endif /* (MEPSAN_RX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: MEPSAN_LoadTxConfig
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
    void MEPSAN_LoadTxConfig(void) 
    {
    #if (MEPSAN_RX_INTERRUPT_ENABLED)
        /* Disable RX interrupts before set TX configuration */
        MEPSAN_SetRxInterruptMode(0u);
    #endif /* (MEPSAN_RX_INTERRUPT_ENABLED) */

        MEPSAN_WriteControlRegister(MEPSAN_ReadControlRegister() | MEPSAN_CTRL_HD_SEND);
        MEPSAN_RXBITCTR_PERIOD_REG = MEPSAN_HD_TXBITCTR_INIT;
    }

#endif  /* MEPSAN_HD_ENABLED */


/* [] END OF FILE */
