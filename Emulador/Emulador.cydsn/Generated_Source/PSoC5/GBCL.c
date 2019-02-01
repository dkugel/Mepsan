/*******************************************************************************
* File Name: GBCL.c
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

#include "GBCL.h"
#if (GBCL_INTERNAL_CLOCK_USED)
    #include "GBCL_IntClock.h"
#endif /* End GBCL_INTERNAL_CLOCK_USED */


/***************************************
* Global data allocation
***************************************/

uint8 GBCL_initVar = 0u;

#if (GBCL_TX_INTERRUPT_ENABLED && GBCL_TX_ENABLED)
    volatile uint8 GBCL_txBuffer[GBCL_TX_BUFFER_SIZE];
    volatile uint8 GBCL_txBufferRead = 0u;
    uint8 GBCL_txBufferWrite = 0u;
#endif /* (GBCL_TX_INTERRUPT_ENABLED && GBCL_TX_ENABLED) */

#if (GBCL_RX_INTERRUPT_ENABLED && (GBCL_RX_ENABLED || GBCL_HD_ENABLED))
    uint8 GBCL_errorStatus = 0u;
    volatile uint8 GBCL_rxBuffer[GBCL_RX_BUFFER_SIZE];
    volatile uint8 GBCL_rxBufferRead  = 0u;
    volatile uint8 GBCL_rxBufferWrite = 0u;
    volatile uint8 GBCL_rxBufferLoopDetect = 0u;
    volatile uint8 GBCL_rxBufferOverflow   = 0u;
    #if (GBCL_RXHW_ADDRESS_ENABLED)
        volatile uint8 GBCL_rxAddressMode = GBCL_RX_ADDRESS_MODE;
        volatile uint8 GBCL_rxAddressDetected = 0u;
    #endif /* (GBCL_RXHW_ADDRESS_ENABLED) */
#endif /* (GBCL_RX_INTERRUPT_ENABLED && (GBCL_RX_ENABLED || GBCL_HD_ENABLED)) */


/*******************************************************************************
* Function Name: GBCL_Start
********************************************************************************
*
* Summary:
*  This is the preferred method to begin component operation.
*  GBCL_Start() sets the initVar variable, calls the
*  GBCL_Init() function, and then calls the
*  GBCL_Enable() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  The GBCL_intiVar variable is used to indicate initial
*  configuration of this component. The variable is initialized to zero (0u)
*  and set to one (1u) the first time GBCL_Start() is called. This
*  allows for component initialization without re-initialization in all
*  subsequent calls to the GBCL_Start() routine.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void GBCL_Start(void) 
{
    /* If not initialized then initialize all required hardware and software */
    if(GBCL_initVar == 0u)
    {
        GBCL_Init();
        GBCL_initVar = 1u;
    }

    GBCL_Enable();
}


/*******************************************************************************
* Function Name: GBCL_Init
********************************************************************************
*
* Summary:
*  Initializes or restores the component according to the customizer Configure
*  dialog settings. It is not necessary to call GBCL_Init() because
*  the GBCL_Start() API calls this function and is the preferred
*  method to begin component operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void GBCL_Init(void) 
{
    #if(GBCL_RX_ENABLED || GBCL_HD_ENABLED)

        #if (GBCL_RX_INTERRUPT_ENABLED)
            /* Set RX interrupt vector and priority */
            (void) CyIntSetVector(GBCL_RX_VECT_NUM, &GBCL_RXISR);
            CyIntSetPriority(GBCL_RX_VECT_NUM, GBCL_RX_PRIOR_NUM);
            GBCL_errorStatus = 0u;
        #endif /* (GBCL_RX_INTERRUPT_ENABLED) */

        #if (GBCL_RXHW_ADDRESS_ENABLED)
            GBCL_SetRxAddressMode(GBCL_RX_ADDRESS_MODE);
            GBCL_SetRxAddress1(GBCL_RX_HW_ADDRESS1);
            GBCL_SetRxAddress2(GBCL_RX_HW_ADDRESS2);
        #endif /* End GBCL_RXHW_ADDRESS_ENABLED */

        /* Init Count7 period */
        GBCL_RXBITCTR_PERIOD_REG = GBCL_RXBITCTR_INIT;
        /* Configure the Initial RX interrupt mask */
        GBCL_RXSTATUS_MASK_REG  = GBCL_INIT_RX_INTERRUPTS_MASK;
    #endif /* End GBCL_RX_ENABLED || GBCL_HD_ENABLED*/

    #if(GBCL_TX_ENABLED)
        #if (GBCL_TX_INTERRUPT_ENABLED)
            /* Set TX interrupt vector and priority */
            (void) CyIntSetVector(GBCL_TX_VECT_NUM, &GBCL_TXISR);
            CyIntSetPriority(GBCL_TX_VECT_NUM, GBCL_TX_PRIOR_NUM);
        #endif /* (GBCL_TX_INTERRUPT_ENABLED) */

        /* Write Counter Value for TX Bit Clk Generator*/
        #if (GBCL_TXCLKGEN_DP)
            GBCL_TXBITCLKGEN_CTR_REG = GBCL_BIT_CENTER;
            GBCL_TXBITCLKTX_COMPLETE_REG = ((GBCL_NUMBER_OF_DATA_BITS +
                        GBCL_NUMBER_OF_START_BIT) * GBCL_OVER_SAMPLE_COUNT) - 1u;
        #else
            GBCL_TXBITCTR_PERIOD_REG = ((GBCL_NUMBER_OF_DATA_BITS +
                        GBCL_NUMBER_OF_START_BIT) * GBCL_OVER_SAMPLE_8) - 1u;
        #endif /* End GBCL_TXCLKGEN_DP */

        /* Configure the Initial TX interrupt mask */
        #if (GBCL_TX_INTERRUPT_ENABLED)
            GBCL_TXSTATUS_MASK_REG = GBCL_TX_STS_FIFO_EMPTY;
        #else
            GBCL_TXSTATUS_MASK_REG = GBCL_INIT_TX_INTERRUPTS_MASK;
        #endif /*End GBCL_TX_INTERRUPT_ENABLED*/

    #endif /* End GBCL_TX_ENABLED */

    #if(GBCL_PARITY_TYPE_SW)  /* Write Parity to Control Register */
        GBCL_WriteControlRegister( \
            (GBCL_ReadControlRegister() & (uint8)~GBCL_CTRL_PARITY_TYPE_MASK) | \
            (uint8)(GBCL_PARITY_TYPE << GBCL_CTRL_PARITY_TYPE0_SHIFT) );
    #endif /* End GBCL_PARITY_TYPE_SW */
}


/*******************************************************************************
* Function Name: GBCL_Enable
********************************************************************************
*
* Summary:
*  Activates the hardware and begins component operation. It is not necessary
*  to call GBCL_Enable() because the GBCL_Start() API
*  calls this function, which is the preferred method to begin component
*  operation.

* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  GBCL_rxAddressDetected - set to initial state (0).
*
*******************************************************************************/
void GBCL_Enable(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    #if (GBCL_RX_ENABLED || GBCL_HD_ENABLED)
        /* RX Counter (Count7) Enable */
        GBCL_RXBITCTR_CONTROL_REG |= GBCL_CNTR_ENABLE;

        /* Enable the RX Interrupt */
        GBCL_RXSTATUS_ACTL_REG  |= GBCL_INT_ENABLE;

        #if (GBCL_RX_INTERRUPT_ENABLED)
            GBCL_EnableRxInt();

            #if (GBCL_RXHW_ADDRESS_ENABLED)
                GBCL_rxAddressDetected = 0u;
            #endif /* (GBCL_RXHW_ADDRESS_ENABLED) */
        #endif /* (GBCL_RX_INTERRUPT_ENABLED) */
    #endif /* (GBCL_RX_ENABLED || GBCL_HD_ENABLED) */

    #if(GBCL_TX_ENABLED)
        /* TX Counter (DP/Count7) Enable */
        #if(!GBCL_TXCLKGEN_DP)
            GBCL_TXBITCTR_CONTROL_REG |= GBCL_CNTR_ENABLE;
        #endif /* End GBCL_TXCLKGEN_DP */

        /* Enable the TX Interrupt */
        GBCL_TXSTATUS_ACTL_REG |= GBCL_INT_ENABLE;
        #if (GBCL_TX_INTERRUPT_ENABLED)
            GBCL_ClearPendingTxInt(); /* Clear history of TX_NOT_EMPTY */
            GBCL_EnableTxInt();
        #endif /* (GBCL_TX_INTERRUPT_ENABLED) */
     #endif /* (GBCL_TX_INTERRUPT_ENABLED) */

    #if (GBCL_INTERNAL_CLOCK_USED)
        GBCL_IntClock_Start();  /* Enable the clock */
    #endif /* (GBCL_INTERNAL_CLOCK_USED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: GBCL_Stop
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
void GBCL_Stop(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    /* Write Bit Counter Disable */
    #if (GBCL_RX_ENABLED || GBCL_HD_ENABLED)
        GBCL_RXBITCTR_CONTROL_REG &= (uint8) ~GBCL_CNTR_ENABLE;
    #endif /* (GBCL_RX_ENABLED || GBCL_HD_ENABLED) */

    #if (GBCL_TX_ENABLED)
        #if(!GBCL_TXCLKGEN_DP)
            GBCL_TXBITCTR_CONTROL_REG &= (uint8) ~GBCL_CNTR_ENABLE;
        #endif /* (!GBCL_TXCLKGEN_DP) */
    #endif /* (GBCL_TX_ENABLED) */

    #if (GBCL_INTERNAL_CLOCK_USED)
        GBCL_IntClock_Stop();   /* Disable the clock */
    #endif /* (GBCL_INTERNAL_CLOCK_USED) */

    /* Disable internal interrupt component */
    #if (GBCL_RX_ENABLED || GBCL_HD_ENABLED)
        GBCL_RXSTATUS_ACTL_REG  &= (uint8) ~GBCL_INT_ENABLE;

        #if (GBCL_RX_INTERRUPT_ENABLED)
            GBCL_DisableRxInt();
        #endif /* (GBCL_RX_INTERRUPT_ENABLED) */
    #endif /* (GBCL_RX_ENABLED || GBCL_HD_ENABLED) */

    #if (GBCL_TX_ENABLED)
        GBCL_TXSTATUS_ACTL_REG &= (uint8) ~GBCL_INT_ENABLE;

        #if (GBCL_TX_INTERRUPT_ENABLED)
            GBCL_DisableTxInt();
        #endif /* (GBCL_TX_INTERRUPT_ENABLED) */
    #endif /* (GBCL_TX_ENABLED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: GBCL_ReadControlRegister
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
uint8 GBCL_ReadControlRegister(void) 
{
    #if (GBCL_CONTROL_REG_REMOVED)
        return(0u);
    #else
        return(GBCL_CONTROL_REG);
    #endif /* (GBCL_CONTROL_REG_REMOVED) */
}


/*******************************************************************************
* Function Name: GBCL_WriteControlRegister
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
void  GBCL_WriteControlRegister(uint8 control) 
{
    #if (GBCL_CONTROL_REG_REMOVED)
        if(0u != control)
        {
            /* Suppress compiler warning */
        }
    #else
       GBCL_CONTROL_REG = control;
    #endif /* (GBCL_CONTROL_REG_REMOVED) */
}


#if(GBCL_RX_ENABLED || GBCL_HD_ENABLED)
    /*******************************************************************************
    * Function Name: GBCL_SetRxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the RX interrupt sources enabled.
    *
    * Parameters:
    *  IntSrc:  Bit field containing the RX interrupts to enable. Based on the 
    *  bit-field arrangement of the status register. This value must be a 
    *  combination of status register bit-masks shown below:
    *      GBCL_RX_STS_FIFO_NOTEMPTY    Interrupt on byte received.
    *      GBCL_RX_STS_PAR_ERROR        Interrupt on parity error.
    *      GBCL_RX_STS_STOP_ERROR       Interrupt on stop error.
    *      GBCL_RX_STS_BREAK            Interrupt on break.
    *      GBCL_RX_STS_OVERRUN          Interrupt on overrun error.
    *      GBCL_RX_STS_ADDR_MATCH       Interrupt on address match.
    *      GBCL_RX_STS_MRKSPC           Interrupt on address detect.
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void GBCL_SetRxInterruptMode(uint8 intSrc) 
    {
        GBCL_RXSTATUS_MASK_REG  = intSrc;
    }


    /*******************************************************************************
    * Function Name: GBCL_ReadRxData
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
    *  GBCL_rxBuffer - RAM buffer pointer for save received data.
    *  GBCL_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  GBCL_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  GBCL_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 GBCL_ReadRxData(void) 
    {
        uint8 rxData;

    #if (GBCL_RX_INTERRUPT_ENABLED)

        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        GBCL_DisableRxInt();

        locRxBufferRead  = GBCL_rxBufferRead;
        locRxBufferWrite = GBCL_rxBufferWrite;

        if( (GBCL_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = GBCL_rxBuffer[locRxBufferRead];
            locRxBufferRead++;

            if(locRxBufferRead >= GBCL_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            GBCL_rxBufferRead = locRxBufferRead;

            if(GBCL_rxBufferLoopDetect != 0u)
            {
                GBCL_rxBufferLoopDetect = 0u;
                #if ((GBCL_RX_INTERRUPT_ENABLED) && (GBCL_FLOW_CONTROL != 0u))
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( GBCL_HD_ENABLED )
                        if((GBCL_CONTROL_REG & GBCL_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only in RX
                            *  configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            GBCL_RXSTATUS_MASK_REG  |= GBCL_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        GBCL_RXSTATUS_MASK_REG  |= GBCL_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end GBCL_HD_ENABLED */
                #endif /* ((GBCL_RX_INTERRUPT_ENABLED) && (GBCL_FLOW_CONTROL != 0u)) */
            }
        }
        else
        {   /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
            rxData = GBCL_RXDATA_REG;
        }

        GBCL_EnableRxInt();

    #else

        /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
        rxData = GBCL_RXDATA_REG;

    #endif /* (GBCL_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: GBCL_ReadRxStatus
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
    *  GBCL_RX_STS_FIFO_NOTEMPTY.
    *  GBCL_RX_STS_FIFO_NOTEMPTY clears immediately after RX data
    *  register read.
    *
    * Global Variables:
    *  GBCL_rxBufferOverflow - used to indicate overload condition.
    *   It set to one in RX interrupt when there isn't free space in
    *   GBCL_rxBufferRead to write new data. This condition returned
    *   and cleared to zero by this API as an
    *   GBCL_RX_STS_SOFT_BUFF_OVER bit along with RX Status register
    *   bits.
    *
    *******************************************************************************/
    uint8 GBCL_ReadRxStatus(void) 
    {
        uint8 status;

        status = GBCL_RXSTATUS_REG & GBCL_RX_HW_MASK;

    #if (GBCL_RX_INTERRUPT_ENABLED)
        if(GBCL_rxBufferOverflow != 0u)
        {
            status |= GBCL_RX_STS_SOFT_BUFF_OVER;
            GBCL_rxBufferOverflow = 0u;
        }
    #endif /* (GBCL_RX_INTERRUPT_ENABLED) */

        return(status);
    }


    /*******************************************************************************
    * Function Name: GBCL_GetChar
    ********************************************************************************
    *
    * Summary:
    *  Returns the last received byte of data. GBCL_GetChar() is
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
    *  GBCL_rxBuffer - RAM buffer pointer for save received data.
    *  GBCL_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  GBCL_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  GBCL_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 GBCL_GetChar(void) 
    {
        uint8 rxData = 0u;
        uint8 rxStatus;

    #if (GBCL_RX_INTERRUPT_ENABLED)
        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        GBCL_DisableRxInt();

        locRxBufferRead  = GBCL_rxBufferRead;
        locRxBufferWrite = GBCL_rxBufferWrite;

        if( (GBCL_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = GBCL_rxBuffer[locRxBufferRead];
            locRxBufferRead++;
            if(locRxBufferRead >= GBCL_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            GBCL_rxBufferRead = locRxBufferRead;

            if(GBCL_rxBufferLoopDetect != 0u)
            {
                GBCL_rxBufferLoopDetect = 0u;
                #if( (GBCL_RX_INTERRUPT_ENABLED) && (GBCL_FLOW_CONTROL != 0u) )
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( GBCL_HD_ENABLED )
                        if((GBCL_CONTROL_REG & GBCL_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only if
                            *  RX configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            GBCL_RXSTATUS_MASK_REG |= GBCL_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        GBCL_RXSTATUS_MASK_REG |= GBCL_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end GBCL_HD_ENABLED */
                #endif /* GBCL_RX_INTERRUPT_ENABLED and Hardware flow control*/
            }

        }
        else
        {   rxStatus = GBCL_RXSTATUS_REG;
            if((rxStatus & GBCL_RX_STS_FIFO_NOTEMPTY) != 0u)
            {   /* Read received data from FIFO */
                rxData = GBCL_RXDATA_REG;
                /*Check status on error*/
                if((rxStatus & (GBCL_RX_STS_BREAK | GBCL_RX_STS_PAR_ERROR |
                                GBCL_RX_STS_STOP_ERROR | GBCL_RX_STS_OVERRUN)) != 0u)
                {
                    rxData = 0u;
                }
            }
        }

        GBCL_EnableRxInt();

    #else

        rxStatus =GBCL_RXSTATUS_REG;
        if((rxStatus & GBCL_RX_STS_FIFO_NOTEMPTY) != 0u)
        {
            /* Read received data from FIFO */
            rxData = GBCL_RXDATA_REG;

            /*Check status on error*/
            if((rxStatus & (GBCL_RX_STS_BREAK | GBCL_RX_STS_PAR_ERROR |
                            GBCL_RX_STS_STOP_ERROR | GBCL_RX_STS_OVERRUN)) != 0u)
            {
                rxData = 0u;
            }
        }
    #endif /* (GBCL_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: GBCL_GetByte
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
    uint16 GBCL_GetByte(void) 
    {
        
    #if (GBCL_RX_INTERRUPT_ENABLED)
        uint16 locErrorStatus;
        /* Protect variables that could change on interrupt */
        GBCL_DisableRxInt();
        locErrorStatus = (uint16)GBCL_errorStatus;
        GBCL_errorStatus = 0u;
        GBCL_EnableRxInt();
        return ( (uint16)(locErrorStatus << 8u) | GBCL_ReadRxData() );
    #else
        return ( ((uint16)GBCL_ReadRxStatus() << 8u) | GBCL_ReadRxData() );
    #endif /* GBCL_RX_INTERRUPT_ENABLED */
        
    }


    /*******************************************************************************
    * Function Name: GBCL_GetRxBufferSize
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
    *  GBCL_rxBufferWrite - used to calculate left bytes.
    *  GBCL_rxBufferRead - used to calculate left bytes.
    *  GBCL_rxBufferLoopDetect - checked to decide left bytes amount.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the RX Buffer is.
    *
    *******************************************************************************/
    uint8 GBCL_GetRxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (GBCL_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt */
        GBCL_DisableRxInt();

        if(GBCL_rxBufferRead == GBCL_rxBufferWrite)
        {
            if(GBCL_rxBufferLoopDetect != 0u)
            {
                size = GBCL_RX_BUFFER_SIZE;
            }
            else
            {
                size = 0u;
            }
        }
        else if(GBCL_rxBufferRead < GBCL_rxBufferWrite)
        {
            size = (GBCL_rxBufferWrite - GBCL_rxBufferRead);
        }
        else
        {
            size = (GBCL_RX_BUFFER_SIZE - GBCL_rxBufferRead) + GBCL_rxBufferWrite;
        }

        GBCL_EnableRxInt();

    #else

        /* We can only know if there is data in the fifo. */
        size = ((GBCL_RXSTATUS_REG & GBCL_RX_STS_FIFO_NOTEMPTY) != 0u) ? 1u : 0u;

    #endif /* (GBCL_RX_INTERRUPT_ENABLED) */

        return(size);
    }


    /*******************************************************************************
    * Function Name: GBCL_ClearRxBuffer
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
    *  GBCL_rxBufferWrite - cleared to zero.
    *  GBCL_rxBufferRead - cleared to zero.
    *  GBCL_rxBufferLoopDetect - cleared to zero.
    *  GBCL_rxBufferOverflow - cleared to zero.
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
    void GBCL_ClearRxBuffer(void) 
    {
        uint8 enableInterrupts;

        /* Clear the HW FIFO */
        enableInterrupts = CyEnterCriticalSection();
        GBCL_RXDATA_AUX_CTL_REG |= (uint8)  GBCL_RX_FIFO_CLR;
        GBCL_RXDATA_AUX_CTL_REG &= (uint8) ~GBCL_RX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (GBCL_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        GBCL_DisableRxInt();

        GBCL_rxBufferRead = 0u;
        GBCL_rxBufferWrite = 0u;
        GBCL_rxBufferLoopDetect = 0u;
        GBCL_rxBufferOverflow = 0u;

        GBCL_EnableRxInt();

    #endif /* (GBCL_RX_INTERRUPT_ENABLED) */

    }


    /*******************************************************************************
    * Function Name: GBCL_SetRxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Sets the software controlled Addressing mode used by the RX portion of the
    *  UART.
    *
    * Parameters:
    *  addressMode: Enumerated value indicating the mode of RX addressing
    *  GBCL__B_UART__AM_SW_BYTE_BYTE -  Software Byte-by-Byte address
    *                                               detection
    *  GBCL__B_UART__AM_SW_DETECT_TO_BUFFER - Software Detect to Buffer
    *                                               address detection
    *  GBCL__B_UART__AM_HW_BYTE_BY_BYTE - Hardware Byte-by-Byte address
    *                                               detection
    *  GBCL__B_UART__AM_HW_DETECT_TO_BUFFER - Hardware Detect to Buffer
    *                                               address detection
    *  GBCL__B_UART__AM_NONE - No address detection
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  GBCL_rxAddressMode - the parameter stored in this variable for
    *   the farther usage in RX ISR.
    *  GBCL_rxAddressDetected - set to initial state (0).
    *
    *******************************************************************************/
    void GBCL_SetRxAddressMode(uint8 addressMode)
                                                        
    {
        #if(GBCL_RXHW_ADDRESS_ENABLED)
            #if(GBCL_CONTROL_REG_REMOVED)
                if(0u != addressMode)
                {
                    /* Suppress compiler warning */
                }
            #else /* GBCL_CONTROL_REG_REMOVED */
                uint8 tmpCtrl;
                tmpCtrl = GBCL_CONTROL_REG & (uint8)~GBCL_CTRL_RXADDR_MODE_MASK;
                tmpCtrl |= (uint8)(addressMode << GBCL_CTRL_RXADDR_MODE0_SHIFT);
                GBCL_CONTROL_REG = tmpCtrl;

                #if(GBCL_RX_INTERRUPT_ENABLED && \
                   (GBCL_RXBUFFERSIZE > GBCL_FIFO_LENGTH) )
                    GBCL_rxAddressMode = addressMode;
                    GBCL_rxAddressDetected = 0u;
                #endif /* End GBCL_RXBUFFERSIZE > GBCL_FIFO_LENGTH*/
            #endif /* End GBCL_CONTROL_REG_REMOVED */
        #else /* GBCL_RXHW_ADDRESS_ENABLED */
            if(0u != addressMode)
            {
                /* Suppress compiler warning */
            }
        #endif /* End GBCL_RXHW_ADDRESS_ENABLED */
    }


    /*******************************************************************************
    * Function Name: GBCL_SetRxAddress1
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
    void GBCL_SetRxAddress1(uint8 address) 
    {
        GBCL_RXADDRESS1_REG = address;
    }


    /*******************************************************************************
    * Function Name: GBCL_SetRxAddress2
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
    void GBCL_SetRxAddress2(uint8 address) 
    {
        GBCL_RXADDRESS2_REG = address;
    }

#endif  /* GBCL_RX_ENABLED || GBCL_HD_ENABLED*/


#if( (GBCL_TX_ENABLED) || (GBCL_HD_ENABLED) )
    /*******************************************************************************
    * Function Name: GBCL_SetTxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the TX interrupt sources to be enabled, but does not enable the
    *  interrupt.
    *
    * Parameters:
    *  intSrc: Bit field containing the TX interrupt sources to enable
    *   GBCL_TX_STS_COMPLETE        Interrupt on TX byte complete
    *   GBCL_TX_STS_FIFO_EMPTY      Interrupt when TX FIFO is empty
    *   GBCL_TX_STS_FIFO_FULL       Interrupt when TX FIFO is full
    *   GBCL_TX_STS_FIFO_NOT_FULL   Interrupt when TX FIFO is not full
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void GBCL_SetTxInterruptMode(uint8 intSrc) 
    {
        GBCL_TXSTATUS_MASK_REG = intSrc;
    }


    /*******************************************************************************
    * Function Name: GBCL_WriteTxData
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
    *  GBCL_txBuffer - RAM buffer pointer for save data for transmission
    *  GBCL_txBufferWrite - cyclic index for write to txBuffer,
    *    incremented after each byte saved to buffer.
    *  GBCL_txBufferRead - cyclic index for read from txBuffer,
    *    checked to identify the condition to write to FIFO directly or to TX buffer
    *  GBCL_initVar - checked to identify that the component has been
    *    initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void GBCL_WriteTxData(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function*/
        if(GBCL_initVar != 0u)
        {
        #if (GBCL_TX_INTERRUPT_ENABLED)

            /* Protect variables that could change on interrupt. */
            GBCL_DisableTxInt();

            if( (GBCL_txBufferRead == GBCL_txBufferWrite) &&
                ((GBCL_TXSTATUS_REG & GBCL_TX_STS_FIFO_FULL) == 0u) )
            {
                /* Add directly to the FIFO. */
                GBCL_TXDATA_REG = txDataByte;
            }
            else
            {
                if(GBCL_txBufferWrite >= GBCL_TX_BUFFER_SIZE)
                {
                    GBCL_txBufferWrite = 0u;
                }

                GBCL_txBuffer[GBCL_txBufferWrite] = txDataByte;

                /* Add to the software buffer. */
                GBCL_txBufferWrite++;
            }

            GBCL_EnableTxInt();

        #else

            /* Add directly to the FIFO. */
            GBCL_TXDATA_REG = txDataByte;

        #endif /*(GBCL_TX_INTERRUPT_ENABLED) */
        }
    }


    /*******************************************************************************
    * Function Name: GBCL_ReadTxStatus
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
    uint8 GBCL_ReadTxStatus(void) 
    {
        return(GBCL_TXSTATUS_REG);
    }


    /*******************************************************************************
    * Function Name: GBCL_PutChar
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
    *  GBCL_txBuffer - RAM buffer pointer for save data for transmission
    *  GBCL_txBufferWrite - cyclic index for write to txBuffer,
    *     checked to identify free space in txBuffer and incremented after each byte
    *     saved to buffer.
    *  GBCL_txBufferRead - cyclic index for read from txBuffer,
    *     checked to identify free space in txBuffer.
    *  GBCL_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to transmit any byte of data in a single transfer
    *
    *******************************************************************************/
    void GBCL_PutChar(uint8 txDataByte) 
    {
    #if (GBCL_TX_INTERRUPT_ENABLED)
        /* The temporary output pointer is used since it takes two instructions
        *  to increment with a wrap, and we can't risk doing that with the real
        *  pointer and getting an interrupt in between instructions.
        */
        uint8 locTxBufferWrite;
        uint8 locTxBufferRead;

        do
        { /* Block if software buffer is full, so we don't overwrite. */

        #if ((GBCL_TX_BUFFER_SIZE > GBCL_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Disable TX interrupt to protect variables from modification */
            GBCL_DisableTxInt();
        #endif /* (GBCL_TX_BUFFER_SIZE > GBCL_MAX_BYTE_VALUE) && (CY_PSOC3) */

            locTxBufferWrite = GBCL_txBufferWrite;
            locTxBufferRead  = GBCL_txBufferRead;

        #if ((GBCL_TX_BUFFER_SIZE > GBCL_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Enable interrupt to continue transmission */
            GBCL_EnableTxInt();
        #endif /* (GBCL_TX_BUFFER_SIZE > GBCL_MAX_BYTE_VALUE) && (CY_PSOC3) */
        }
        while( (locTxBufferWrite < locTxBufferRead) ? (locTxBufferWrite == (locTxBufferRead - 1u)) :
                                ((locTxBufferWrite - locTxBufferRead) ==
                                (uint8)(GBCL_TX_BUFFER_SIZE - 1u)) );

        if( (locTxBufferRead == locTxBufferWrite) &&
            ((GBCL_TXSTATUS_REG & GBCL_TX_STS_FIFO_FULL) == 0u) )
        {
            /* Add directly to the FIFO */
            GBCL_TXDATA_REG = txDataByte;
        }
        else
        {
            if(locTxBufferWrite >= GBCL_TX_BUFFER_SIZE)
            {
                locTxBufferWrite = 0u;
            }
            /* Add to the software buffer. */
            GBCL_txBuffer[locTxBufferWrite] = txDataByte;
            locTxBufferWrite++;

            /* Finally, update the real output pointer */
        #if ((GBCL_TX_BUFFER_SIZE > GBCL_MAX_BYTE_VALUE) && (CY_PSOC3))
            GBCL_DisableTxInt();
        #endif /* (GBCL_TX_BUFFER_SIZE > GBCL_MAX_BYTE_VALUE) && (CY_PSOC3) */

            GBCL_txBufferWrite = locTxBufferWrite;

        #if ((GBCL_TX_BUFFER_SIZE > GBCL_MAX_BYTE_VALUE) && (CY_PSOC3))
            GBCL_EnableTxInt();
        #endif /* (GBCL_TX_BUFFER_SIZE > GBCL_MAX_BYTE_VALUE) && (CY_PSOC3) */

            if(0u != (GBCL_TXSTATUS_REG & GBCL_TX_STS_FIFO_EMPTY))
            {
                /* Trigger TX interrupt to send software buffer */
                GBCL_SetPendingTxInt();
            }
        }

    #else

        while((GBCL_TXSTATUS_REG & GBCL_TX_STS_FIFO_FULL) != 0u)
        {
            /* Wait for room in the FIFO */
        }

        /* Add directly to the FIFO */
        GBCL_TXDATA_REG = txDataByte;

    #endif /* GBCL_TX_INTERRUPT_ENABLED */
    }


    /*******************************************************************************
    * Function Name: GBCL_PutString
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
    *  GBCL_initVar - checked to identify that the component has been
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
    void GBCL_PutString(const char8 string[]) 
    {
        uint16 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(GBCL_initVar != 0u)
        {
            /* This is a blocking function, it will not exit until all data is sent */
            while(string[bufIndex] != (char8) 0)
            {
                GBCL_PutChar((uint8)string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: GBCL_PutArray
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
    *  GBCL_initVar - checked to identify that the component has been
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
    void GBCL_PutArray(const uint8 string[], uint8 byteCount)
                                                                    
    {
        uint8 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(GBCL_initVar != 0u)
        {
            while(bufIndex < byteCount)
            {
                GBCL_PutChar(string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: GBCL_PutCRLF
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
    *  GBCL_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void GBCL_PutCRLF(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function */
        if(GBCL_initVar != 0u)
        {
            GBCL_PutChar(txDataByte);
            GBCL_PutChar(0x0Du);
            GBCL_PutChar(0x0Au);
        }
    }


    /*******************************************************************************
    * Function Name: GBCL_GetTxBufferSize
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
    *  GBCL_txBufferWrite - used to calculate left space.
    *  GBCL_txBufferRead - used to calculate left space.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the TX Buffer is.
    *
    *******************************************************************************/
    uint8 GBCL_GetTxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (GBCL_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        GBCL_DisableTxInt();

        if(GBCL_txBufferRead == GBCL_txBufferWrite)
        {
            size = 0u;
        }
        else if(GBCL_txBufferRead < GBCL_txBufferWrite)
        {
            size = (GBCL_txBufferWrite - GBCL_txBufferRead);
        }
        else
        {
            size = (GBCL_TX_BUFFER_SIZE - GBCL_txBufferRead) +
                    GBCL_txBufferWrite;
        }

        GBCL_EnableTxInt();

    #else

        size = GBCL_TXSTATUS_REG;

        /* Is the fifo is full. */
        if((size & GBCL_TX_STS_FIFO_FULL) != 0u)
        {
            size = GBCL_FIFO_LENGTH;
        }
        else if((size & GBCL_TX_STS_FIFO_EMPTY) != 0u)
        {
            size = 0u;
        }
        else
        {
            /* We only know there is data in the fifo. */
            size = 1u;
        }

    #endif /* (GBCL_TX_INTERRUPT_ENABLED) */

    return(size);
    }


    /*******************************************************************************
    * Function Name: GBCL_ClearTxBuffer
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
    *  GBCL_txBufferWrite - cleared to zero.
    *  GBCL_txBufferRead - cleared to zero.
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
    void GBCL_ClearTxBuffer(void) 
    {
        uint8 enableInterrupts;

        enableInterrupts = CyEnterCriticalSection();
        /* Clear the HW FIFO */
        GBCL_TXDATA_AUX_CTL_REG |= (uint8)  GBCL_TX_FIFO_CLR;
        GBCL_TXDATA_AUX_CTL_REG &= (uint8) ~GBCL_TX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (GBCL_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        GBCL_DisableTxInt();

        GBCL_txBufferRead = 0u;
        GBCL_txBufferWrite = 0u;

        /* Enable Tx interrupt. */
        GBCL_EnableTxInt();

    #endif /* (GBCL_TX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: GBCL_SendBreak
    ********************************************************************************
    *
    * Summary:
    *  Transmits a break signal on the bus.
    *
    * Parameters:
    *  uint8 retMode:  Send Break return mode. See the following table for options.
    *   GBCL_SEND_BREAK - Initialize registers for break, send the Break
    *       signal and return immediately.
    *   GBCL_WAIT_FOR_COMPLETE_REINIT - Wait until break transmission is
    *       complete, reinitialize registers to normal transmission mode then return
    *   GBCL_REINIT - Reinitialize registers to normal transmission mode
    *       then return.
    *   GBCL_SEND_WAIT_REINIT - Performs both options: 
    *      GBCL_SEND_BREAK and GBCL_WAIT_FOR_COMPLETE_REINIT.
    *      This option is recommended for most cases.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  GBCL_initVar - checked to identify that the component has been
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
    *     When interrupt appear with GBCL_TX_STS_COMPLETE status:
    *     SendBreak(2);     - complete Break operation
    *
    * Side Effects:
    *  The GBCL_SendBreak() function initializes registers to send a
    *  break signal.
    *  Break signal length depends on the break signal bits configuration.
    *  The register configuration should be reinitialized before normal 8-bit
    *  communication can continue.
    *
    *******************************************************************************/
    void GBCL_SendBreak(uint8 retMode) 
    {

        /* If not Initialized then skip this function*/
        if(GBCL_initVar != 0u)
        {
            /* Set the Counter to 13-bits and transmit a 00 byte */
            /* When that is done then reset the counter value back */
            uint8 tmpStat;

        #if(GBCL_HD_ENABLED) /* Half Duplex mode*/

            if( (retMode == GBCL_SEND_BREAK) ||
                (retMode == GBCL_SEND_WAIT_REINIT ) )
            {
                /* CTRL_HD_SEND_BREAK - sends break bits in HD mode */
                GBCL_WriteControlRegister(GBCL_ReadControlRegister() |
                                                      GBCL_CTRL_HD_SEND_BREAK);
                /* Send zeros */
                GBCL_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = GBCL_TXSTATUS_REG;
                }
                while((tmpStat & GBCL_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == GBCL_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == GBCL_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = GBCL_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & GBCL_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == GBCL_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == GBCL_REINIT) ||
                (retMode == GBCL_SEND_WAIT_REINIT) )
            {
                GBCL_WriteControlRegister(GBCL_ReadControlRegister() &
                                              (uint8)~GBCL_CTRL_HD_SEND_BREAK);
            }

        #else /* GBCL_HD_ENABLED Full Duplex mode */

            static uint8 txPeriod;

            if( (retMode == GBCL_SEND_BREAK) ||
                (retMode == GBCL_SEND_WAIT_REINIT) )
            {
                /* CTRL_HD_SEND_BREAK - skip to send parity bit at Break signal in Full Duplex mode */
                #if( (GBCL_PARITY_TYPE != GBCL__B_UART__NONE_REVB) || \
                                    (GBCL_PARITY_TYPE_SW != 0u) )
                    GBCL_WriteControlRegister(GBCL_ReadControlRegister() |
                                                          GBCL_CTRL_HD_SEND_BREAK);
                #endif /* End GBCL_PARITY_TYPE != GBCL__B_UART__NONE_REVB  */

                #if(GBCL_TXCLKGEN_DP)
                    txPeriod = GBCL_TXBITCLKTX_COMPLETE_REG;
                    GBCL_TXBITCLKTX_COMPLETE_REG = GBCL_TXBITCTR_BREAKBITS;
                #else
                    txPeriod = GBCL_TXBITCTR_PERIOD_REG;
                    GBCL_TXBITCTR_PERIOD_REG = GBCL_TXBITCTR_BREAKBITS8X;
                #endif /* End GBCL_TXCLKGEN_DP */

                /* Send zeros */
                GBCL_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = GBCL_TXSTATUS_REG;
                }
                while((tmpStat & GBCL_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == GBCL_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == GBCL_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = GBCL_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & GBCL_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == GBCL_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == GBCL_REINIT) ||
                (retMode == GBCL_SEND_WAIT_REINIT) )
            {

            #if(GBCL_TXCLKGEN_DP)
                GBCL_TXBITCLKTX_COMPLETE_REG = txPeriod;
            #else
                GBCL_TXBITCTR_PERIOD_REG = txPeriod;
            #endif /* End GBCL_TXCLKGEN_DP */

            #if( (GBCL_PARITY_TYPE != GBCL__B_UART__NONE_REVB) || \
                 (GBCL_PARITY_TYPE_SW != 0u) )
                GBCL_WriteControlRegister(GBCL_ReadControlRegister() &
                                                      (uint8) ~GBCL_CTRL_HD_SEND_BREAK);
            #endif /* End GBCL_PARITY_TYPE != NONE */
            }
        #endif    /* End GBCL_HD_ENABLED */
        }
    }


    /*******************************************************************************
    * Function Name: GBCL_SetTxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the transmitter to signal the next bytes is address or data.
    *
    * Parameters:
    *  addressMode: 
    *       GBCL_SET_SPACE - Configure the transmitter to send the next
    *                                    byte as a data.
    *       GBCL_SET_MARK  - Configure the transmitter to send the next
    *                                    byte as an address.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  This function sets and clears GBCL_CTRL_MARK bit in the Control
    *  register.
    *
    *******************************************************************************/
    void GBCL_SetTxAddressMode(uint8 addressMode) 
    {
        /* Mark/Space sending enable */
        if(addressMode != 0u)
        {
        #if( GBCL_CONTROL_REG_REMOVED == 0u )
            GBCL_WriteControlRegister(GBCL_ReadControlRegister() |
                                                  GBCL_CTRL_MARK);
        #endif /* End GBCL_CONTROL_REG_REMOVED == 0u */
        }
        else
        {
        #if( GBCL_CONTROL_REG_REMOVED == 0u )
            GBCL_WriteControlRegister(GBCL_ReadControlRegister() &
                                                  (uint8) ~GBCL_CTRL_MARK);
        #endif /* End GBCL_CONTROL_REG_REMOVED == 0u */
        }
    }

#endif  /* EndGBCL_TX_ENABLED */

#if(GBCL_HD_ENABLED)


    /*******************************************************************************
    * Function Name: GBCL_LoadRxConfig
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
    void GBCL_LoadRxConfig(void) 
    {
        GBCL_WriteControlRegister(GBCL_ReadControlRegister() &
                                                (uint8)~GBCL_CTRL_HD_SEND);
        GBCL_RXBITCTR_PERIOD_REG = GBCL_HD_RXBITCTR_INIT;

    #if (GBCL_RX_INTERRUPT_ENABLED)
        /* Enable RX interrupt after set RX configuration */
        GBCL_SetRxInterruptMode(GBCL_INIT_RX_INTERRUPTS_MASK);
    #endif /* (GBCL_RX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: GBCL_LoadTxConfig
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
    void GBCL_LoadTxConfig(void) 
    {
    #if (GBCL_RX_INTERRUPT_ENABLED)
        /* Disable RX interrupts before set TX configuration */
        GBCL_SetRxInterruptMode(0u);
    #endif /* (GBCL_RX_INTERRUPT_ENABLED) */

        GBCL_WriteControlRegister(GBCL_ReadControlRegister() | GBCL_CTRL_HD_SEND);
        GBCL_RXBITCTR_PERIOD_REG = GBCL_HD_TXBITCTR_INIT;
    }

#endif  /* GBCL_HD_ENABLED */


/* [] END OF FILE */
