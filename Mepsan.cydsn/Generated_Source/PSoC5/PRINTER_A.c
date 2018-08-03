/*******************************************************************************
* File Name: PRINTER_A.c
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

#include "PRINTER_A.h"
#if (PRINTER_A_INTERNAL_CLOCK_USED)
    #include "PRINTER_A_IntClock.h"
#endif /* End PRINTER_A_INTERNAL_CLOCK_USED */


/***************************************
* Global data allocation
***************************************/

uint8 PRINTER_A_initVar = 0u;

#if (PRINTER_A_TX_INTERRUPT_ENABLED && PRINTER_A_TX_ENABLED)
    volatile uint8 PRINTER_A_txBuffer[PRINTER_A_TX_BUFFER_SIZE];
    volatile uint8 PRINTER_A_txBufferRead = 0u;
    uint8 PRINTER_A_txBufferWrite = 0u;
#endif /* (PRINTER_A_TX_INTERRUPT_ENABLED && PRINTER_A_TX_ENABLED) */

#if (PRINTER_A_RX_INTERRUPT_ENABLED && (PRINTER_A_RX_ENABLED || PRINTER_A_HD_ENABLED))
    uint8 PRINTER_A_errorStatus = 0u;
    volatile uint8 PRINTER_A_rxBuffer[PRINTER_A_RX_BUFFER_SIZE];
    volatile uint8 PRINTER_A_rxBufferRead  = 0u;
    volatile uint8 PRINTER_A_rxBufferWrite = 0u;
    volatile uint8 PRINTER_A_rxBufferLoopDetect = 0u;
    volatile uint8 PRINTER_A_rxBufferOverflow   = 0u;
    #if (PRINTER_A_RXHW_ADDRESS_ENABLED)
        volatile uint8 PRINTER_A_rxAddressMode = PRINTER_A_RX_ADDRESS_MODE;
        volatile uint8 PRINTER_A_rxAddressDetected = 0u;
    #endif /* (PRINTER_A_RXHW_ADDRESS_ENABLED) */
#endif /* (PRINTER_A_RX_INTERRUPT_ENABLED && (PRINTER_A_RX_ENABLED || PRINTER_A_HD_ENABLED)) */


/*******************************************************************************
* Function Name: PRINTER_A_Start
********************************************************************************
*
* Summary:
*  This is the preferred method to begin component operation.
*  PRINTER_A_Start() sets the initVar variable, calls the
*  PRINTER_A_Init() function, and then calls the
*  PRINTER_A_Enable() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  The PRINTER_A_intiVar variable is used to indicate initial
*  configuration of this component. The variable is initialized to zero (0u)
*  and set to one (1u) the first time PRINTER_A_Start() is called. This
*  allows for component initialization without re-initialization in all
*  subsequent calls to the PRINTER_A_Start() routine.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PRINTER_A_Start(void) 
{
    /* If not initialized then initialize all required hardware and software */
    if(PRINTER_A_initVar == 0u)
    {
        PRINTER_A_Init();
        PRINTER_A_initVar = 1u;
    }

    PRINTER_A_Enable();
}


/*******************************************************************************
* Function Name: PRINTER_A_Init
********************************************************************************
*
* Summary:
*  Initializes or restores the component according to the customizer Configure
*  dialog settings. It is not necessary to call PRINTER_A_Init() because
*  the PRINTER_A_Start() API calls this function and is the preferred
*  method to begin component operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void PRINTER_A_Init(void) 
{
    #if(PRINTER_A_RX_ENABLED || PRINTER_A_HD_ENABLED)

        #if (PRINTER_A_RX_INTERRUPT_ENABLED)
            /* Set RX interrupt vector and priority */
            (void) CyIntSetVector(PRINTER_A_RX_VECT_NUM, &PRINTER_A_RXISR);
            CyIntSetPriority(PRINTER_A_RX_VECT_NUM, PRINTER_A_RX_PRIOR_NUM);
            PRINTER_A_errorStatus = 0u;
        #endif /* (PRINTER_A_RX_INTERRUPT_ENABLED) */

        #if (PRINTER_A_RXHW_ADDRESS_ENABLED)
            PRINTER_A_SetRxAddressMode(PRINTER_A_RX_ADDRESS_MODE);
            PRINTER_A_SetRxAddress1(PRINTER_A_RX_HW_ADDRESS1);
            PRINTER_A_SetRxAddress2(PRINTER_A_RX_HW_ADDRESS2);
        #endif /* End PRINTER_A_RXHW_ADDRESS_ENABLED */

        /* Init Count7 period */
        PRINTER_A_RXBITCTR_PERIOD_REG = PRINTER_A_RXBITCTR_INIT;
        /* Configure the Initial RX interrupt mask */
        PRINTER_A_RXSTATUS_MASK_REG  = PRINTER_A_INIT_RX_INTERRUPTS_MASK;
    #endif /* End PRINTER_A_RX_ENABLED || PRINTER_A_HD_ENABLED*/

    #if(PRINTER_A_TX_ENABLED)
        #if (PRINTER_A_TX_INTERRUPT_ENABLED)
            /* Set TX interrupt vector and priority */
            (void) CyIntSetVector(PRINTER_A_TX_VECT_NUM, &PRINTER_A_TXISR);
            CyIntSetPriority(PRINTER_A_TX_VECT_NUM, PRINTER_A_TX_PRIOR_NUM);
        #endif /* (PRINTER_A_TX_INTERRUPT_ENABLED) */

        /* Write Counter Value for TX Bit Clk Generator*/
        #if (PRINTER_A_TXCLKGEN_DP)
            PRINTER_A_TXBITCLKGEN_CTR_REG = PRINTER_A_BIT_CENTER;
            PRINTER_A_TXBITCLKTX_COMPLETE_REG = ((PRINTER_A_NUMBER_OF_DATA_BITS +
                        PRINTER_A_NUMBER_OF_START_BIT) * PRINTER_A_OVER_SAMPLE_COUNT) - 1u;
        #else
            PRINTER_A_TXBITCTR_PERIOD_REG = ((PRINTER_A_NUMBER_OF_DATA_BITS +
                        PRINTER_A_NUMBER_OF_START_BIT) * PRINTER_A_OVER_SAMPLE_8) - 1u;
        #endif /* End PRINTER_A_TXCLKGEN_DP */

        /* Configure the Initial TX interrupt mask */
        #if (PRINTER_A_TX_INTERRUPT_ENABLED)
            PRINTER_A_TXSTATUS_MASK_REG = PRINTER_A_TX_STS_FIFO_EMPTY;
        #else
            PRINTER_A_TXSTATUS_MASK_REG = PRINTER_A_INIT_TX_INTERRUPTS_MASK;
        #endif /*End PRINTER_A_TX_INTERRUPT_ENABLED*/

    #endif /* End PRINTER_A_TX_ENABLED */

    #if(PRINTER_A_PARITY_TYPE_SW)  /* Write Parity to Control Register */
        PRINTER_A_WriteControlRegister( \
            (PRINTER_A_ReadControlRegister() & (uint8)~PRINTER_A_CTRL_PARITY_TYPE_MASK) | \
            (uint8)(PRINTER_A_PARITY_TYPE << PRINTER_A_CTRL_PARITY_TYPE0_SHIFT) );
    #endif /* End PRINTER_A_PARITY_TYPE_SW */
}


/*******************************************************************************
* Function Name: PRINTER_A_Enable
********************************************************************************
*
* Summary:
*  Activates the hardware and begins component operation. It is not necessary
*  to call PRINTER_A_Enable() because the PRINTER_A_Start() API
*  calls this function, which is the preferred method to begin component
*  operation.

* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  PRINTER_A_rxAddressDetected - set to initial state (0).
*
*******************************************************************************/
void PRINTER_A_Enable(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    #if (PRINTER_A_RX_ENABLED || PRINTER_A_HD_ENABLED)
        /* RX Counter (Count7) Enable */
        PRINTER_A_RXBITCTR_CONTROL_REG |= PRINTER_A_CNTR_ENABLE;

        /* Enable the RX Interrupt */
        PRINTER_A_RXSTATUS_ACTL_REG  |= PRINTER_A_INT_ENABLE;

        #if (PRINTER_A_RX_INTERRUPT_ENABLED)
            PRINTER_A_EnableRxInt();

            #if (PRINTER_A_RXHW_ADDRESS_ENABLED)
                PRINTER_A_rxAddressDetected = 0u;
            #endif /* (PRINTER_A_RXHW_ADDRESS_ENABLED) */
        #endif /* (PRINTER_A_RX_INTERRUPT_ENABLED) */
    #endif /* (PRINTER_A_RX_ENABLED || PRINTER_A_HD_ENABLED) */

    #if(PRINTER_A_TX_ENABLED)
        /* TX Counter (DP/Count7) Enable */
        #if(!PRINTER_A_TXCLKGEN_DP)
            PRINTER_A_TXBITCTR_CONTROL_REG |= PRINTER_A_CNTR_ENABLE;
        #endif /* End PRINTER_A_TXCLKGEN_DP */

        /* Enable the TX Interrupt */
        PRINTER_A_TXSTATUS_ACTL_REG |= PRINTER_A_INT_ENABLE;
        #if (PRINTER_A_TX_INTERRUPT_ENABLED)
            PRINTER_A_ClearPendingTxInt(); /* Clear history of TX_NOT_EMPTY */
            PRINTER_A_EnableTxInt();
        #endif /* (PRINTER_A_TX_INTERRUPT_ENABLED) */
     #endif /* (PRINTER_A_TX_INTERRUPT_ENABLED) */

    #if (PRINTER_A_INTERNAL_CLOCK_USED)
        PRINTER_A_IntClock_Start();  /* Enable the clock */
    #endif /* (PRINTER_A_INTERNAL_CLOCK_USED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: PRINTER_A_Stop
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
void PRINTER_A_Stop(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    /* Write Bit Counter Disable */
    #if (PRINTER_A_RX_ENABLED || PRINTER_A_HD_ENABLED)
        PRINTER_A_RXBITCTR_CONTROL_REG &= (uint8) ~PRINTER_A_CNTR_ENABLE;
    #endif /* (PRINTER_A_RX_ENABLED || PRINTER_A_HD_ENABLED) */

    #if (PRINTER_A_TX_ENABLED)
        #if(!PRINTER_A_TXCLKGEN_DP)
            PRINTER_A_TXBITCTR_CONTROL_REG &= (uint8) ~PRINTER_A_CNTR_ENABLE;
        #endif /* (!PRINTER_A_TXCLKGEN_DP) */
    #endif /* (PRINTER_A_TX_ENABLED) */

    #if (PRINTER_A_INTERNAL_CLOCK_USED)
        PRINTER_A_IntClock_Stop();   /* Disable the clock */
    #endif /* (PRINTER_A_INTERNAL_CLOCK_USED) */

    /* Disable internal interrupt component */
    #if (PRINTER_A_RX_ENABLED || PRINTER_A_HD_ENABLED)
        PRINTER_A_RXSTATUS_ACTL_REG  &= (uint8) ~PRINTER_A_INT_ENABLE;

        #if (PRINTER_A_RX_INTERRUPT_ENABLED)
            PRINTER_A_DisableRxInt();
        #endif /* (PRINTER_A_RX_INTERRUPT_ENABLED) */
    #endif /* (PRINTER_A_RX_ENABLED || PRINTER_A_HD_ENABLED) */

    #if (PRINTER_A_TX_ENABLED)
        PRINTER_A_TXSTATUS_ACTL_REG &= (uint8) ~PRINTER_A_INT_ENABLE;

        #if (PRINTER_A_TX_INTERRUPT_ENABLED)
            PRINTER_A_DisableTxInt();
        #endif /* (PRINTER_A_TX_INTERRUPT_ENABLED) */
    #endif /* (PRINTER_A_TX_ENABLED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: PRINTER_A_ReadControlRegister
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
uint8 PRINTER_A_ReadControlRegister(void) 
{
    #if (PRINTER_A_CONTROL_REG_REMOVED)
        return(0u);
    #else
        return(PRINTER_A_CONTROL_REG);
    #endif /* (PRINTER_A_CONTROL_REG_REMOVED) */
}


/*******************************************************************************
* Function Name: PRINTER_A_WriteControlRegister
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
void  PRINTER_A_WriteControlRegister(uint8 control) 
{
    #if (PRINTER_A_CONTROL_REG_REMOVED)
        if(0u != control)
        {
            /* Suppress compiler warning */
        }
    #else
       PRINTER_A_CONTROL_REG = control;
    #endif /* (PRINTER_A_CONTROL_REG_REMOVED) */
}


#if(PRINTER_A_RX_ENABLED || PRINTER_A_HD_ENABLED)
    /*******************************************************************************
    * Function Name: PRINTER_A_SetRxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the RX interrupt sources enabled.
    *
    * Parameters:
    *  IntSrc:  Bit field containing the RX interrupts to enable. Based on the 
    *  bit-field arrangement of the status register. This value must be a 
    *  combination of status register bit-masks shown below:
    *      PRINTER_A_RX_STS_FIFO_NOTEMPTY    Interrupt on byte received.
    *      PRINTER_A_RX_STS_PAR_ERROR        Interrupt on parity error.
    *      PRINTER_A_RX_STS_STOP_ERROR       Interrupt on stop error.
    *      PRINTER_A_RX_STS_BREAK            Interrupt on break.
    *      PRINTER_A_RX_STS_OVERRUN          Interrupt on overrun error.
    *      PRINTER_A_RX_STS_ADDR_MATCH       Interrupt on address match.
    *      PRINTER_A_RX_STS_MRKSPC           Interrupt on address detect.
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void PRINTER_A_SetRxInterruptMode(uint8 intSrc) 
    {
        PRINTER_A_RXSTATUS_MASK_REG  = intSrc;
    }


    /*******************************************************************************
    * Function Name: PRINTER_A_ReadRxData
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
    *  PRINTER_A_rxBuffer - RAM buffer pointer for save received data.
    *  PRINTER_A_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  PRINTER_A_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  PRINTER_A_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 PRINTER_A_ReadRxData(void) 
    {
        uint8 rxData;

    #if (PRINTER_A_RX_INTERRUPT_ENABLED)

        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        PRINTER_A_DisableRxInt();

        locRxBufferRead  = PRINTER_A_rxBufferRead;
        locRxBufferWrite = PRINTER_A_rxBufferWrite;

        if( (PRINTER_A_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = PRINTER_A_rxBuffer[locRxBufferRead];
            locRxBufferRead++;

            if(locRxBufferRead >= PRINTER_A_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            PRINTER_A_rxBufferRead = locRxBufferRead;

            if(PRINTER_A_rxBufferLoopDetect != 0u)
            {
                PRINTER_A_rxBufferLoopDetect = 0u;
                #if ((PRINTER_A_RX_INTERRUPT_ENABLED) && (PRINTER_A_FLOW_CONTROL != 0u))
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( PRINTER_A_HD_ENABLED )
                        if((PRINTER_A_CONTROL_REG & PRINTER_A_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only in RX
                            *  configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            PRINTER_A_RXSTATUS_MASK_REG  |= PRINTER_A_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        PRINTER_A_RXSTATUS_MASK_REG  |= PRINTER_A_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end PRINTER_A_HD_ENABLED */
                #endif /* ((PRINTER_A_RX_INTERRUPT_ENABLED) && (PRINTER_A_FLOW_CONTROL != 0u)) */
            }
        }
        else
        {   /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
            rxData = PRINTER_A_RXDATA_REG;
        }

        PRINTER_A_EnableRxInt();

    #else

        /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
        rxData = PRINTER_A_RXDATA_REG;

    #endif /* (PRINTER_A_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: PRINTER_A_ReadRxStatus
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
    *  PRINTER_A_RX_STS_FIFO_NOTEMPTY.
    *  PRINTER_A_RX_STS_FIFO_NOTEMPTY clears immediately after RX data
    *  register read.
    *
    * Global Variables:
    *  PRINTER_A_rxBufferOverflow - used to indicate overload condition.
    *   It set to one in RX interrupt when there isn't free space in
    *   PRINTER_A_rxBufferRead to write new data. This condition returned
    *   and cleared to zero by this API as an
    *   PRINTER_A_RX_STS_SOFT_BUFF_OVER bit along with RX Status register
    *   bits.
    *
    *******************************************************************************/
    uint8 PRINTER_A_ReadRxStatus(void) 
    {
        uint8 status;

        status = PRINTER_A_RXSTATUS_REG & PRINTER_A_RX_HW_MASK;

    #if (PRINTER_A_RX_INTERRUPT_ENABLED)
        if(PRINTER_A_rxBufferOverflow != 0u)
        {
            status |= PRINTER_A_RX_STS_SOFT_BUFF_OVER;
            PRINTER_A_rxBufferOverflow = 0u;
        }
    #endif /* (PRINTER_A_RX_INTERRUPT_ENABLED) */

        return(status);
    }


    /*******************************************************************************
    * Function Name: PRINTER_A_GetChar
    ********************************************************************************
    *
    * Summary:
    *  Returns the last received byte of data. PRINTER_A_GetChar() is
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
    *  PRINTER_A_rxBuffer - RAM buffer pointer for save received data.
    *  PRINTER_A_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  PRINTER_A_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  PRINTER_A_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 PRINTER_A_GetChar(void) 
    {
        uint8 rxData = 0u;
        uint8 rxStatus;

    #if (PRINTER_A_RX_INTERRUPT_ENABLED)
        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        PRINTER_A_DisableRxInt();

        locRxBufferRead  = PRINTER_A_rxBufferRead;
        locRxBufferWrite = PRINTER_A_rxBufferWrite;

        if( (PRINTER_A_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = PRINTER_A_rxBuffer[locRxBufferRead];
            locRxBufferRead++;
            if(locRxBufferRead >= PRINTER_A_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            PRINTER_A_rxBufferRead = locRxBufferRead;

            if(PRINTER_A_rxBufferLoopDetect != 0u)
            {
                PRINTER_A_rxBufferLoopDetect = 0u;
                #if( (PRINTER_A_RX_INTERRUPT_ENABLED) && (PRINTER_A_FLOW_CONTROL != 0u) )
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( PRINTER_A_HD_ENABLED )
                        if((PRINTER_A_CONTROL_REG & PRINTER_A_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only if
                            *  RX configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            PRINTER_A_RXSTATUS_MASK_REG |= PRINTER_A_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        PRINTER_A_RXSTATUS_MASK_REG |= PRINTER_A_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end PRINTER_A_HD_ENABLED */
                #endif /* PRINTER_A_RX_INTERRUPT_ENABLED and Hardware flow control*/
            }

        }
        else
        {   rxStatus = PRINTER_A_RXSTATUS_REG;
            if((rxStatus & PRINTER_A_RX_STS_FIFO_NOTEMPTY) != 0u)
            {   /* Read received data from FIFO */
                rxData = PRINTER_A_RXDATA_REG;
                /*Check status on error*/
                if((rxStatus & (PRINTER_A_RX_STS_BREAK | PRINTER_A_RX_STS_PAR_ERROR |
                                PRINTER_A_RX_STS_STOP_ERROR | PRINTER_A_RX_STS_OVERRUN)) != 0u)
                {
                    rxData = 0u;
                }
            }
        }

        PRINTER_A_EnableRxInt();

    #else

        rxStatus =PRINTER_A_RXSTATUS_REG;
        if((rxStatus & PRINTER_A_RX_STS_FIFO_NOTEMPTY) != 0u)
        {
            /* Read received data from FIFO */
            rxData = PRINTER_A_RXDATA_REG;

            /*Check status on error*/
            if((rxStatus & (PRINTER_A_RX_STS_BREAK | PRINTER_A_RX_STS_PAR_ERROR |
                            PRINTER_A_RX_STS_STOP_ERROR | PRINTER_A_RX_STS_OVERRUN)) != 0u)
            {
                rxData = 0u;
            }
        }
    #endif /* (PRINTER_A_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: PRINTER_A_GetByte
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
    uint16 PRINTER_A_GetByte(void) 
    {
        
    #if (PRINTER_A_RX_INTERRUPT_ENABLED)
        uint16 locErrorStatus;
        /* Protect variables that could change on interrupt */
        PRINTER_A_DisableRxInt();
        locErrorStatus = (uint16)PRINTER_A_errorStatus;
        PRINTER_A_errorStatus = 0u;
        PRINTER_A_EnableRxInt();
        return ( (uint16)(locErrorStatus << 8u) | PRINTER_A_ReadRxData() );
    #else
        return ( ((uint16)PRINTER_A_ReadRxStatus() << 8u) | PRINTER_A_ReadRxData() );
    #endif /* PRINTER_A_RX_INTERRUPT_ENABLED */
        
    }


    /*******************************************************************************
    * Function Name: PRINTER_A_GetRxBufferSize
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
    *  PRINTER_A_rxBufferWrite - used to calculate left bytes.
    *  PRINTER_A_rxBufferRead - used to calculate left bytes.
    *  PRINTER_A_rxBufferLoopDetect - checked to decide left bytes amount.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the RX Buffer is.
    *
    *******************************************************************************/
    uint8 PRINTER_A_GetRxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (PRINTER_A_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt */
        PRINTER_A_DisableRxInt();

        if(PRINTER_A_rxBufferRead == PRINTER_A_rxBufferWrite)
        {
            if(PRINTER_A_rxBufferLoopDetect != 0u)
            {
                size = PRINTER_A_RX_BUFFER_SIZE;
            }
            else
            {
                size = 0u;
            }
        }
        else if(PRINTER_A_rxBufferRead < PRINTER_A_rxBufferWrite)
        {
            size = (PRINTER_A_rxBufferWrite - PRINTER_A_rxBufferRead);
        }
        else
        {
            size = (PRINTER_A_RX_BUFFER_SIZE - PRINTER_A_rxBufferRead) + PRINTER_A_rxBufferWrite;
        }

        PRINTER_A_EnableRxInt();

    #else

        /* We can only know if there is data in the fifo. */
        size = ((PRINTER_A_RXSTATUS_REG & PRINTER_A_RX_STS_FIFO_NOTEMPTY) != 0u) ? 1u : 0u;

    #endif /* (PRINTER_A_RX_INTERRUPT_ENABLED) */

        return(size);
    }


    /*******************************************************************************
    * Function Name: PRINTER_A_ClearRxBuffer
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
    *  PRINTER_A_rxBufferWrite - cleared to zero.
    *  PRINTER_A_rxBufferRead - cleared to zero.
    *  PRINTER_A_rxBufferLoopDetect - cleared to zero.
    *  PRINTER_A_rxBufferOverflow - cleared to zero.
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
    void PRINTER_A_ClearRxBuffer(void) 
    {
        uint8 enableInterrupts;

        /* Clear the HW FIFO */
        enableInterrupts = CyEnterCriticalSection();
        PRINTER_A_RXDATA_AUX_CTL_REG |= (uint8)  PRINTER_A_RX_FIFO_CLR;
        PRINTER_A_RXDATA_AUX_CTL_REG &= (uint8) ~PRINTER_A_RX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (PRINTER_A_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        PRINTER_A_DisableRxInt();

        PRINTER_A_rxBufferRead = 0u;
        PRINTER_A_rxBufferWrite = 0u;
        PRINTER_A_rxBufferLoopDetect = 0u;
        PRINTER_A_rxBufferOverflow = 0u;

        PRINTER_A_EnableRxInt();

    #endif /* (PRINTER_A_RX_INTERRUPT_ENABLED) */

    }


    /*******************************************************************************
    * Function Name: PRINTER_A_SetRxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Sets the software controlled Addressing mode used by the RX portion of the
    *  UART.
    *
    * Parameters:
    *  addressMode: Enumerated value indicating the mode of RX addressing
    *  PRINTER_A__B_UART__AM_SW_BYTE_BYTE -  Software Byte-by-Byte address
    *                                               detection
    *  PRINTER_A__B_UART__AM_SW_DETECT_TO_BUFFER - Software Detect to Buffer
    *                                               address detection
    *  PRINTER_A__B_UART__AM_HW_BYTE_BY_BYTE - Hardware Byte-by-Byte address
    *                                               detection
    *  PRINTER_A__B_UART__AM_HW_DETECT_TO_BUFFER - Hardware Detect to Buffer
    *                                               address detection
    *  PRINTER_A__B_UART__AM_NONE - No address detection
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  PRINTER_A_rxAddressMode - the parameter stored in this variable for
    *   the farther usage in RX ISR.
    *  PRINTER_A_rxAddressDetected - set to initial state (0).
    *
    *******************************************************************************/
    void PRINTER_A_SetRxAddressMode(uint8 addressMode)
                                                        
    {
        #if(PRINTER_A_RXHW_ADDRESS_ENABLED)
            #if(PRINTER_A_CONTROL_REG_REMOVED)
                if(0u != addressMode)
                {
                    /* Suppress compiler warning */
                }
            #else /* PRINTER_A_CONTROL_REG_REMOVED */
                uint8 tmpCtrl;
                tmpCtrl = PRINTER_A_CONTROL_REG & (uint8)~PRINTER_A_CTRL_RXADDR_MODE_MASK;
                tmpCtrl |= (uint8)(addressMode << PRINTER_A_CTRL_RXADDR_MODE0_SHIFT);
                PRINTER_A_CONTROL_REG = tmpCtrl;

                #if(PRINTER_A_RX_INTERRUPT_ENABLED && \
                   (PRINTER_A_RXBUFFERSIZE > PRINTER_A_FIFO_LENGTH) )
                    PRINTER_A_rxAddressMode = addressMode;
                    PRINTER_A_rxAddressDetected = 0u;
                #endif /* End PRINTER_A_RXBUFFERSIZE > PRINTER_A_FIFO_LENGTH*/
            #endif /* End PRINTER_A_CONTROL_REG_REMOVED */
        #else /* PRINTER_A_RXHW_ADDRESS_ENABLED */
            if(0u != addressMode)
            {
                /* Suppress compiler warning */
            }
        #endif /* End PRINTER_A_RXHW_ADDRESS_ENABLED */
    }


    /*******************************************************************************
    * Function Name: PRINTER_A_SetRxAddress1
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
    void PRINTER_A_SetRxAddress1(uint8 address) 
    {
        PRINTER_A_RXADDRESS1_REG = address;
    }


    /*******************************************************************************
    * Function Name: PRINTER_A_SetRxAddress2
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
    void PRINTER_A_SetRxAddress2(uint8 address) 
    {
        PRINTER_A_RXADDRESS2_REG = address;
    }

#endif  /* PRINTER_A_RX_ENABLED || PRINTER_A_HD_ENABLED*/


#if( (PRINTER_A_TX_ENABLED) || (PRINTER_A_HD_ENABLED) )
    /*******************************************************************************
    * Function Name: PRINTER_A_SetTxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the TX interrupt sources to be enabled, but does not enable the
    *  interrupt.
    *
    * Parameters:
    *  intSrc: Bit field containing the TX interrupt sources to enable
    *   PRINTER_A_TX_STS_COMPLETE        Interrupt on TX byte complete
    *   PRINTER_A_TX_STS_FIFO_EMPTY      Interrupt when TX FIFO is empty
    *   PRINTER_A_TX_STS_FIFO_FULL       Interrupt when TX FIFO is full
    *   PRINTER_A_TX_STS_FIFO_NOT_FULL   Interrupt when TX FIFO is not full
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void PRINTER_A_SetTxInterruptMode(uint8 intSrc) 
    {
        PRINTER_A_TXSTATUS_MASK_REG = intSrc;
    }


    /*******************************************************************************
    * Function Name: PRINTER_A_WriteTxData
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
    *  PRINTER_A_txBuffer - RAM buffer pointer for save data for transmission
    *  PRINTER_A_txBufferWrite - cyclic index for write to txBuffer,
    *    incremented after each byte saved to buffer.
    *  PRINTER_A_txBufferRead - cyclic index for read from txBuffer,
    *    checked to identify the condition to write to FIFO directly or to TX buffer
    *  PRINTER_A_initVar - checked to identify that the component has been
    *    initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void PRINTER_A_WriteTxData(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function*/
        if(PRINTER_A_initVar != 0u)
        {
        #if (PRINTER_A_TX_INTERRUPT_ENABLED)

            /* Protect variables that could change on interrupt. */
            PRINTER_A_DisableTxInt();

            if( (PRINTER_A_txBufferRead == PRINTER_A_txBufferWrite) &&
                ((PRINTER_A_TXSTATUS_REG & PRINTER_A_TX_STS_FIFO_FULL) == 0u) )
            {
                /* Add directly to the FIFO. */
                PRINTER_A_TXDATA_REG = txDataByte;
            }
            else
            {
                if(PRINTER_A_txBufferWrite >= PRINTER_A_TX_BUFFER_SIZE)
                {
                    PRINTER_A_txBufferWrite = 0u;
                }

                PRINTER_A_txBuffer[PRINTER_A_txBufferWrite] = txDataByte;

                /* Add to the software buffer. */
                PRINTER_A_txBufferWrite++;
            }

            PRINTER_A_EnableTxInt();

        #else

            /* Add directly to the FIFO. */
            PRINTER_A_TXDATA_REG = txDataByte;

        #endif /*(PRINTER_A_TX_INTERRUPT_ENABLED) */
        }
    }


    /*******************************************************************************
    * Function Name: PRINTER_A_ReadTxStatus
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
    uint8 PRINTER_A_ReadTxStatus(void) 
    {
        return(PRINTER_A_TXSTATUS_REG);
    }


    /*******************************************************************************
    * Function Name: PRINTER_A_PutChar
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
    *  PRINTER_A_txBuffer - RAM buffer pointer for save data for transmission
    *  PRINTER_A_txBufferWrite - cyclic index for write to txBuffer,
    *     checked to identify free space in txBuffer and incremented after each byte
    *     saved to buffer.
    *  PRINTER_A_txBufferRead - cyclic index for read from txBuffer,
    *     checked to identify free space in txBuffer.
    *  PRINTER_A_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to transmit any byte of data in a single transfer
    *
    *******************************************************************************/
    void PRINTER_A_PutChar(uint8 txDataByte) 
    {
    #if (PRINTER_A_TX_INTERRUPT_ENABLED)
        /* The temporary output pointer is used since it takes two instructions
        *  to increment with a wrap, and we can't risk doing that with the real
        *  pointer and getting an interrupt in between instructions.
        */
        uint8 locTxBufferWrite;
        uint8 locTxBufferRead;

        do
        { /* Block if software buffer is full, so we don't overwrite. */

        #if ((PRINTER_A_TX_BUFFER_SIZE > PRINTER_A_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Disable TX interrupt to protect variables from modification */
            PRINTER_A_DisableTxInt();
        #endif /* (PRINTER_A_TX_BUFFER_SIZE > PRINTER_A_MAX_BYTE_VALUE) && (CY_PSOC3) */

            locTxBufferWrite = PRINTER_A_txBufferWrite;
            locTxBufferRead  = PRINTER_A_txBufferRead;

        #if ((PRINTER_A_TX_BUFFER_SIZE > PRINTER_A_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Enable interrupt to continue transmission */
            PRINTER_A_EnableTxInt();
        #endif /* (PRINTER_A_TX_BUFFER_SIZE > PRINTER_A_MAX_BYTE_VALUE) && (CY_PSOC3) */
        }
        while( (locTxBufferWrite < locTxBufferRead) ? (locTxBufferWrite == (locTxBufferRead - 1u)) :
                                ((locTxBufferWrite - locTxBufferRead) ==
                                (uint8)(PRINTER_A_TX_BUFFER_SIZE - 1u)) );

        if( (locTxBufferRead == locTxBufferWrite) &&
            ((PRINTER_A_TXSTATUS_REG & PRINTER_A_TX_STS_FIFO_FULL) == 0u) )
        {
            /* Add directly to the FIFO */
            PRINTER_A_TXDATA_REG = txDataByte;
        }
        else
        {
            if(locTxBufferWrite >= PRINTER_A_TX_BUFFER_SIZE)
            {
                locTxBufferWrite = 0u;
            }
            /* Add to the software buffer. */
            PRINTER_A_txBuffer[locTxBufferWrite] = txDataByte;
            locTxBufferWrite++;

            /* Finally, update the real output pointer */
        #if ((PRINTER_A_TX_BUFFER_SIZE > PRINTER_A_MAX_BYTE_VALUE) && (CY_PSOC3))
            PRINTER_A_DisableTxInt();
        #endif /* (PRINTER_A_TX_BUFFER_SIZE > PRINTER_A_MAX_BYTE_VALUE) && (CY_PSOC3) */

            PRINTER_A_txBufferWrite = locTxBufferWrite;

        #if ((PRINTER_A_TX_BUFFER_SIZE > PRINTER_A_MAX_BYTE_VALUE) && (CY_PSOC3))
            PRINTER_A_EnableTxInt();
        #endif /* (PRINTER_A_TX_BUFFER_SIZE > PRINTER_A_MAX_BYTE_VALUE) && (CY_PSOC3) */

            if(0u != (PRINTER_A_TXSTATUS_REG & PRINTER_A_TX_STS_FIFO_EMPTY))
            {
                /* Trigger TX interrupt to send software buffer */
                PRINTER_A_SetPendingTxInt();
            }
        }

    #else

        while((PRINTER_A_TXSTATUS_REG & PRINTER_A_TX_STS_FIFO_FULL) != 0u)
        {
            /* Wait for room in the FIFO */
        }

        /* Add directly to the FIFO */
        PRINTER_A_TXDATA_REG = txDataByte;

    #endif /* PRINTER_A_TX_INTERRUPT_ENABLED */
    }


    /*******************************************************************************
    * Function Name: PRINTER_A_PutString
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
    *  PRINTER_A_initVar - checked to identify that the component has been
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
    void PRINTER_A_PutString(const char8 string[]) 
    {
        uint16 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(PRINTER_A_initVar != 0u)
        {
            /* This is a blocking function, it will not exit until all data is sent */
            while(string[bufIndex] != (char8) 0)
            {
                PRINTER_A_PutChar((uint8)string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: PRINTER_A_PutArray
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
    *  PRINTER_A_initVar - checked to identify that the component has been
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
    void PRINTER_A_PutArray(const uint8 string[], uint8 byteCount)
                                                                    
    {
        uint8 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(PRINTER_A_initVar != 0u)
        {
            while(bufIndex < byteCount)
            {
                PRINTER_A_PutChar(string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: PRINTER_A_PutCRLF
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
    *  PRINTER_A_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void PRINTER_A_PutCRLF(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function */
        if(PRINTER_A_initVar != 0u)
        {
            PRINTER_A_PutChar(txDataByte);
            PRINTER_A_PutChar(0x0Du);
            PRINTER_A_PutChar(0x0Au);
        }
    }


    /*******************************************************************************
    * Function Name: PRINTER_A_GetTxBufferSize
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
    *  PRINTER_A_txBufferWrite - used to calculate left space.
    *  PRINTER_A_txBufferRead - used to calculate left space.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the TX Buffer is.
    *
    *******************************************************************************/
    uint8 PRINTER_A_GetTxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (PRINTER_A_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        PRINTER_A_DisableTxInt();

        if(PRINTER_A_txBufferRead == PRINTER_A_txBufferWrite)
        {
            size = 0u;
        }
        else if(PRINTER_A_txBufferRead < PRINTER_A_txBufferWrite)
        {
            size = (PRINTER_A_txBufferWrite - PRINTER_A_txBufferRead);
        }
        else
        {
            size = (PRINTER_A_TX_BUFFER_SIZE - PRINTER_A_txBufferRead) +
                    PRINTER_A_txBufferWrite;
        }

        PRINTER_A_EnableTxInt();

    #else

        size = PRINTER_A_TXSTATUS_REG;

        /* Is the fifo is full. */
        if((size & PRINTER_A_TX_STS_FIFO_FULL) != 0u)
        {
            size = PRINTER_A_FIFO_LENGTH;
        }
        else if((size & PRINTER_A_TX_STS_FIFO_EMPTY) != 0u)
        {
            size = 0u;
        }
        else
        {
            /* We only know there is data in the fifo. */
            size = 1u;
        }

    #endif /* (PRINTER_A_TX_INTERRUPT_ENABLED) */

    return(size);
    }


    /*******************************************************************************
    * Function Name: PRINTER_A_ClearTxBuffer
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
    *  PRINTER_A_txBufferWrite - cleared to zero.
    *  PRINTER_A_txBufferRead - cleared to zero.
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
    void PRINTER_A_ClearTxBuffer(void) 
    {
        uint8 enableInterrupts;

        enableInterrupts = CyEnterCriticalSection();
        /* Clear the HW FIFO */
        PRINTER_A_TXDATA_AUX_CTL_REG |= (uint8)  PRINTER_A_TX_FIFO_CLR;
        PRINTER_A_TXDATA_AUX_CTL_REG &= (uint8) ~PRINTER_A_TX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (PRINTER_A_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        PRINTER_A_DisableTxInt();

        PRINTER_A_txBufferRead = 0u;
        PRINTER_A_txBufferWrite = 0u;

        /* Enable Tx interrupt. */
        PRINTER_A_EnableTxInt();

    #endif /* (PRINTER_A_TX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: PRINTER_A_SendBreak
    ********************************************************************************
    *
    * Summary:
    *  Transmits a break signal on the bus.
    *
    * Parameters:
    *  uint8 retMode:  Send Break return mode. See the following table for options.
    *   PRINTER_A_SEND_BREAK - Initialize registers for break, send the Break
    *       signal and return immediately.
    *   PRINTER_A_WAIT_FOR_COMPLETE_REINIT - Wait until break transmission is
    *       complete, reinitialize registers to normal transmission mode then return
    *   PRINTER_A_REINIT - Reinitialize registers to normal transmission mode
    *       then return.
    *   PRINTER_A_SEND_WAIT_REINIT - Performs both options: 
    *      PRINTER_A_SEND_BREAK and PRINTER_A_WAIT_FOR_COMPLETE_REINIT.
    *      This option is recommended for most cases.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  PRINTER_A_initVar - checked to identify that the component has been
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
    *     When interrupt appear with PRINTER_A_TX_STS_COMPLETE status:
    *     SendBreak(2);     - complete Break operation
    *
    * Side Effects:
    *  The PRINTER_A_SendBreak() function initializes registers to send a
    *  break signal.
    *  Break signal length depends on the break signal bits configuration.
    *  The register configuration should be reinitialized before normal 8-bit
    *  communication can continue.
    *
    *******************************************************************************/
    void PRINTER_A_SendBreak(uint8 retMode) 
    {

        /* If not Initialized then skip this function*/
        if(PRINTER_A_initVar != 0u)
        {
            /* Set the Counter to 13-bits and transmit a 00 byte */
            /* When that is done then reset the counter value back */
            uint8 tmpStat;

        #if(PRINTER_A_HD_ENABLED) /* Half Duplex mode*/

            if( (retMode == PRINTER_A_SEND_BREAK) ||
                (retMode == PRINTER_A_SEND_WAIT_REINIT ) )
            {
                /* CTRL_HD_SEND_BREAK - sends break bits in HD mode */
                PRINTER_A_WriteControlRegister(PRINTER_A_ReadControlRegister() |
                                                      PRINTER_A_CTRL_HD_SEND_BREAK);
                /* Send zeros */
                PRINTER_A_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = PRINTER_A_TXSTATUS_REG;
                }
                while((tmpStat & PRINTER_A_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == PRINTER_A_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == PRINTER_A_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = PRINTER_A_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & PRINTER_A_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == PRINTER_A_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == PRINTER_A_REINIT) ||
                (retMode == PRINTER_A_SEND_WAIT_REINIT) )
            {
                PRINTER_A_WriteControlRegister(PRINTER_A_ReadControlRegister() &
                                              (uint8)~PRINTER_A_CTRL_HD_SEND_BREAK);
            }

        #else /* PRINTER_A_HD_ENABLED Full Duplex mode */

            static uint8 txPeriod;

            if( (retMode == PRINTER_A_SEND_BREAK) ||
                (retMode == PRINTER_A_SEND_WAIT_REINIT) )
            {
                /* CTRL_HD_SEND_BREAK - skip to send parity bit at Break signal in Full Duplex mode */
                #if( (PRINTER_A_PARITY_TYPE != PRINTER_A__B_UART__NONE_REVB) || \
                                    (PRINTER_A_PARITY_TYPE_SW != 0u) )
                    PRINTER_A_WriteControlRegister(PRINTER_A_ReadControlRegister() |
                                                          PRINTER_A_CTRL_HD_SEND_BREAK);
                #endif /* End PRINTER_A_PARITY_TYPE != PRINTER_A__B_UART__NONE_REVB  */

                #if(PRINTER_A_TXCLKGEN_DP)
                    txPeriod = PRINTER_A_TXBITCLKTX_COMPLETE_REG;
                    PRINTER_A_TXBITCLKTX_COMPLETE_REG = PRINTER_A_TXBITCTR_BREAKBITS;
                #else
                    txPeriod = PRINTER_A_TXBITCTR_PERIOD_REG;
                    PRINTER_A_TXBITCTR_PERIOD_REG = PRINTER_A_TXBITCTR_BREAKBITS8X;
                #endif /* End PRINTER_A_TXCLKGEN_DP */

                /* Send zeros */
                PRINTER_A_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = PRINTER_A_TXSTATUS_REG;
                }
                while((tmpStat & PRINTER_A_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == PRINTER_A_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == PRINTER_A_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = PRINTER_A_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & PRINTER_A_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == PRINTER_A_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == PRINTER_A_REINIT) ||
                (retMode == PRINTER_A_SEND_WAIT_REINIT) )
            {

            #if(PRINTER_A_TXCLKGEN_DP)
                PRINTER_A_TXBITCLKTX_COMPLETE_REG = txPeriod;
            #else
                PRINTER_A_TXBITCTR_PERIOD_REG = txPeriod;
            #endif /* End PRINTER_A_TXCLKGEN_DP */

            #if( (PRINTER_A_PARITY_TYPE != PRINTER_A__B_UART__NONE_REVB) || \
                 (PRINTER_A_PARITY_TYPE_SW != 0u) )
                PRINTER_A_WriteControlRegister(PRINTER_A_ReadControlRegister() &
                                                      (uint8) ~PRINTER_A_CTRL_HD_SEND_BREAK);
            #endif /* End PRINTER_A_PARITY_TYPE != NONE */
            }
        #endif    /* End PRINTER_A_HD_ENABLED */
        }
    }


    /*******************************************************************************
    * Function Name: PRINTER_A_SetTxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the transmitter to signal the next bytes is address or data.
    *
    * Parameters:
    *  addressMode: 
    *       PRINTER_A_SET_SPACE - Configure the transmitter to send the next
    *                                    byte as a data.
    *       PRINTER_A_SET_MARK  - Configure the transmitter to send the next
    *                                    byte as an address.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  This function sets and clears PRINTER_A_CTRL_MARK bit in the Control
    *  register.
    *
    *******************************************************************************/
    void PRINTER_A_SetTxAddressMode(uint8 addressMode) 
    {
        /* Mark/Space sending enable */
        if(addressMode != 0u)
        {
        #if( PRINTER_A_CONTROL_REG_REMOVED == 0u )
            PRINTER_A_WriteControlRegister(PRINTER_A_ReadControlRegister() |
                                                  PRINTER_A_CTRL_MARK);
        #endif /* End PRINTER_A_CONTROL_REG_REMOVED == 0u */
        }
        else
        {
        #if( PRINTER_A_CONTROL_REG_REMOVED == 0u )
            PRINTER_A_WriteControlRegister(PRINTER_A_ReadControlRegister() &
                                                  (uint8) ~PRINTER_A_CTRL_MARK);
        #endif /* End PRINTER_A_CONTROL_REG_REMOVED == 0u */
        }
    }

#endif  /* EndPRINTER_A_TX_ENABLED */

#if(PRINTER_A_HD_ENABLED)


    /*******************************************************************************
    * Function Name: PRINTER_A_LoadRxConfig
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
    void PRINTER_A_LoadRxConfig(void) 
    {
        PRINTER_A_WriteControlRegister(PRINTER_A_ReadControlRegister() &
                                                (uint8)~PRINTER_A_CTRL_HD_SEND);
        PRINTER_A_RXBITCTR_PERIOD_REG = PRINTER_A_HD_RXBITCTR_INIT;

    #if (PRINTER_A_RX_INTERRUPT_ENABLED)
        /* Enable RX interrupt after set RX configuration */
        PRINTER_A_SetRxInterruptMode(PRINTER_A_INIT_RX_INTERRUPTS_MASK);
    #endif /* (PRINTER_A_RX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: PRINTER_A_LoadTxConfig
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
    void PRINTER_A_LoadTxConfig(void) 
    {
    #if (PRINTER_A_RX_INTERRUPT_ENABLED)
        /* Disable RX interrupts before set TX configuration */
        PRINTER_A_SetRxInterruptMode(0u);
    #endif /* (PRINTER_A_RX_INTERRUPT_ENABLED) */

        PRINTER_A_WriteControlRegister(PRINTER_A_ReadControlRegister() | PRINTER_A_CTRL_HD_SEND);
        PRINTER_A_RXBITCTR_PERIOD_REG = PRINTER_A_HD_TXBITCTR_INIT;
    }

#endif  /* PRINTER_A_HD_ENABLED */


/* [] END OF FILE */
