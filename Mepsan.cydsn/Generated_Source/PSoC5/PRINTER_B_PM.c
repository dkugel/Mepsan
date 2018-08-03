/*******************************************************************************
* File Name: PRINTER_B_PM.c
* Version 2.50
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "PRINTER_B.h"


/***************************************
* Local data allocation
***************************************/

static PRINTER_B_BACKUP_STRUCT  PRINTER_B_backup =
{
    /* enableState - disabled */
    0u,
};



/*******************************************************************************
* Function Name: PRINTER_B_SaveConfig
********************************************************************************
*
* Summary:
*  This function saves the component nonretention control register.
*  Does not save the FIFO which is a set of nonretention registers.
*  This function is called by the PRINTER_B_Sleep() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  PRINTER_B_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PRINTER_B_SaveConfig(void)
{
    #if(PRINTER_B_CONTROL_REG_REMOVED == 0u)
        PRINTER_B_backup.cr = PRINTER_B_CONTROL_REG;
    #endif /* End PRINTER_B_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: PRINTER_B_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the nonretention control register except FIFO.
*  Does not restore the FIFO which is a set of nonretention registers.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  PRINTER_B_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
* Notes:
*  If this function is called without calling PRINTER_B_SaveConfig() 
*  first, the data loaded may be incorrect.
*
*******************************************************************************/
void PRINTER_B_RestoreConfig(void)
{
    #if(PRINTER_B_CONTROL_REG_REMOVED == 0u)
        PRINTER_B_CONTROL_REG = PRINTER_B_backup.cr;
    #endif /* End PRINTER_B_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: PRINTER_B_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred API to prepare the component for sleep. 
*  The PRINTER_B_Sleep() API saves the current component state. Then it
*  calls the PRINTER_B_Stop() function and calls 
*  PRINTER_B_SaveConfig() to save the hardware configuration.
*  Call the PRINTER_B_Sleep() function before calling the CyPmSleep() 
*  or the CyPmHibernate() function. 
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  PRINTER_B_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PRINTER_B_Sleep(void)
{
    #if(PRINTER_B_RX_ENABLED || PRINTER_B_HD_ENABLED)
        if((PRINTER_B_RXSTATUS_ACTL_REG  & PRINTER_B_INT_ENABLE) != 0u)
        {
            PRINTER_B_backup.enableState = 1u;
        }
        else
        {
            PRINTER_B_backup.enableState = 0u;
        }
    #else
        if((PRINTER_B_TXSTATUS_ACTL_REG  & PRINTER_B_INT_ENABLE) !=0u)
        {
            PRINTER_B_backup.enableState = 1u;
        }
        else
        {
            PRINTER_B_backup.enableState = 0u;
        }
    #endif /* End PRINTER_B_RX_ENABLED || PRINTER_B_HD_ENABLED*/

    PRINTER_B_Stop();
    PRINTER_B_SaveConfig();
}


/*******************************************************************************
* Function Name: PRINTER_B_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred API to restore the component to the state when 
*  PRINTER_B_Sleep() was called. The PRINTER_B_Wakeup() function
*  calls the PRINTER_B_RestoreConfig() function to restore the 
*  configuration. If the component was enabled before the 
*  PRINTER_B_Sleep() function was called, the PRINTER_B_Wakeup()
*  function will also re-enable the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  PRINTER_B_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PRINTER_B_Wakeup(void)
{
    PRINTER_B_RestoreConfig();
    #if( (PRINTER_B_RX_ENABLED) || (PRINTER_B_HD_ENABLED) )
        PRINTER_B_ClearRxBuffer();
    #endif /* End (PRINTER_B_RX_ENABLED) || (PRINTER_B_HD_ENABLED) */
    #if(PRINTER_B_TX_ENABLED || PRINTER_B_HD_ENABLED)
        PRINTER_B_ClearTxBuffer();
    #endif /* End PRINTER_B_TX_ENABLED || PRINTER_B_HD_ENABLED */

    if(PRINTER_B_backup.enableState != 0u)
    {
        PRINTER_B_Enable();
    }
}


/* [] END OF FILE */
