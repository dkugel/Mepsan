/*******************************************************************************
* File Name: screen2_PM.c
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

#include "screen2.h"


/***************************************
* Local data allocation
***************************************/

static screen2_BACKUP_STRUCT  screen2_backup =
{
    /* enableState - disabled */
    0u,
};



/*******************************************************************************
* Function Name: screen2_SaveConfig
********************************************************************************
*
* Summary:
*  This function saves the component nonretention control register.
*  Does not save the FIFO which is a set of nonretention registers.
*  This function is called by the screen2_Sleep() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  screen2_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void screen2_SaveConfig(void)
{
    #if(screen2_CONTROL_REG_REMOVED == 0u)
        screen2_backup.cr = screen2_CONTROL_REG;
    #endif /* End screen2_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: screen2_RestoreConfig
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
*  screen2_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
* Notes:
*  If this function is called without calling screen2_SaveConfig() 
*  first, the data loaded may be incorrect.
*
*******************************************************************************/
void screen2_RestoreConfig(void)
{
    #if(screen2_CONTROL_REG_REMOVED == 0u)
        screen2_CONTROL_REG = screen2_backup.cr;
    #endif /* End screen2_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: screen2_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred API to prepare the component for sleep. 
*  The screen2_Sleep() API saves the current component state. Then it
*  calls the screen2_Stop() function and calls 
*  screen2_SaveConfig() to save the hardware configuration.
*  Call the screen2_Sleep() function before calling the CyPmSleep() 
*  or the CyPmHibernate() function. 
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  screen2_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void screen2_Sleep(void)
{
    #if(screen2_RX_ENABLED || screen2_HD_ENABLED)
        if((screen2_RXSTATUS_ACTL_REG  & screen2_INT_ENABLE) != 0u)
        {
            screen2_backup.enableState = 1u;
        }
        else
        {
            screen2_backup.enableState = 0u;
        }
    #else
        if((screen2_TXSTATUS_ACTL_REG  & screen2_INT_ENABLE) !=0u)
        {
            screen2_backup.enableState = 1u;
        }
        else
        {
            screen2_backup.enableState = 0u;
        }
    #endif /* End screen2_RX_ENABLED || screen2_HD_ENABLED*/

    screen2_Stop();
    screen2_SaveConfig();
}


/*******************************************************************************
* Function Name: screen2_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred API to restore the component to the state when 
*  screen2_Sleep() was called. The screen2_Wakeup() function
*  calls the screen2_RestoreConfig() function to restore the 
*  configuration. If the component was enabled before the 
*  screen2_Sleep() function was called, the screen2_Wakeup()
*  function will also re-enable the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  screen2_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void screen2_Wakeup(void)
{
    screen2_RestoreConfig();
    #if( (screen2_RX_ENABLED) || (screen2_HD_ENABLED) )
        screen2_ClearRxBuffer();
    #endif /* End (screen2_RX_ENABLED) || (screen2_HD_ENABLED) */
    #if(screen2_TX_ENABLED || screen2_HD_ENABLED)
        screen2_ClearTxBuffer();
    #endif /* End screen2_TX_ENABLED || screen2_HD_ENABLED */

    if(screen2_backup.enableState != 0u)
    {
        screen2_Enable();
    }
}


/* [] END OF FILE */
