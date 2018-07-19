/*******************************************************************************
* File Name: screen_PM.c
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

#include "screen.h"


/***************************************
* Local data allocation
***************************************/

static screen_BACKUP_STRUCT  screen_backup =
{
    /* enableState - disabled */
    0u,
};



/*******************************************************************************
* Function Name: screen_SaveConfig
********************************************************************************
*
* Summary:
*  This function saves the component nonretention control register.
*  Does not save the FIFO which is a set of nonretention registers.
*  This function is called by the screen_Sleep() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  screen_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void screen_SaveConfig(void)
{
    #if(screen_CONTROL_REG_REMOVED == 0u)
        screen_backup.cr = screen_CONTROL_REG;
    #endif /* End screen_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: screen_RestoreConfig
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
*  screen_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
* Notes:
*  If this function is called without calling screen_SaveConfig() 
*  first, the data loaded may be incorrect.
*
*******************************************************************************/
void screen_RestoreConfig(void)
{
    #if(screen_CONTROL_REG_REMOVED == 0u)
        screen_CONTROL_REG = screen_backup.cr;
    #endif /* End screen_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: screen_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred API to prepare the component for sleep. 
*  The screen_Sleep() API saves the current component state. Then it
*  calls the screen_Stop() function and calls 
*  screen_SaveConfig() to save the hardware configuration.
*  Call the screen_Sleep() function before calling the CyPmSleep() 
*  or the CyPmHibernate() function. 
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  screen_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void screen_Sleep(void)
{
    #if(screen_RX_ENABLED || screen_HD_ENABLED)
        if((screen_RXSTATUS_ACTL_REG  & screen_INT_ENABLE) != 0u)
        {
            screen_backup.enableState = 1u;
        }
        else
        {
            screen_backup.enableState = 0u;
        }
    #else
        if((screen_TXSTATUS_ACTL_REG  & screen_INT_ENABLE) !=0u)
        {
            screen_backup.enableState = 1u;
        }
        else
        {
            screen_backup.enableState = 0u;
        }
    #endif /* End screen_RX_ENABLED || screen_HD_ENABLED*/

    screen_Stop();
    screen_SaveConfig();
}


/*******************************************************************************
* Function Name: screen_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred API to restore the component to the state when 
*  screen_Sleep() was called. The screen_Wakeup() function
*  calls the screen_RestoreConfig() function to restore the 
*  configuration. If the component was enabled before the 
*  screen_Sleep() function was called, the screen_Wakeup()
*  function will also re-enable the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  screen_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void screen_Wakeup(void)
{
    screen_RestoreConfig();
    #if( (screen_RX_ENABLED) || (screen_HD_ENABLED) )
        screen_ClearRxBuffer();
    #endif /* End (screen_RX_ENABLED) || (screen_HD_ENABLED) */
    #if(screen_TX_ENABLED || screen_HD_ENABLED)
        screen_ClearTxBuffer();
    #endif /* End screen_TX_ENABLED || screen_HD_ENABLED */

    if(screen_backup.enableState != 0u)
    {
        screen_Enable();
    }
}


/* [] END OF FILE */
