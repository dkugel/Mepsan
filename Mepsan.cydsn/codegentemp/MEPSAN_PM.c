/*******************************************************************************
* File Name: MEPSAN_PM.c
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

#include "MEPSAN.h"


/***************************************
* Local data allocation
***************************************/

static MEPSAN_BACKUP_STRUCT  MEPSAN_backup =
{
    /* enableState - disabled */
    0u,
};



/*******************************************************************************
* Function Name: MEPSAN_SaveConfig
********************************************************************************
*
* Summary:
*  This function saves the component nonretention control register.
*  Does not save the FIFO which is a set of nonretention registers.
*  This function is called by the MEPSAN_Sleep() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  MEPSAN_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void MEPSAN_SaveConfig(void)
{
    #if(MEPSAN_CONTROL_REG_REMOVED == 0u)
        MEPSAN_backup.cr = MEPSAN_CONTROL_REG;
    #endif /* End MEPSAN_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: MEPSAN_RestoreConfig
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
*  MEPSAN_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
* Notes:
*  If this function is called without calling MEPSAN_SaveConfig() 
*  first, the data loaded may be incorrect.
*
*******************************************************************************/
void MEPSAN_RestoreConfig(void)
{
    #if(MEPSAN_CONTROL_REG_REMOVED == 0u)
        MEPSAN_CONTROL_REG = MEPSAN_backup.cr;
    #endif /* End MEPSAN_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: MEPSAN_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred API to prepare the component for sleep. 
*  The MEPSAN_Sleep() API saves the current component state. Then it
*  calls the MEPSAN_Stop() function and calls 
*  MEPSAN_SaveConfig() to save the hardware configuration.
*  Call the MEPSAN_Sleep() function before calling the CyPmSleep() 
*  or the CyPmHibernate() function. 
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  MEPSAN_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void MEPSAN_Sleep(void)
{
    #if(MEPSAN_RX_ENABLED || MEPSAN_HD_ENABLED)
        if((MEPSAN_RXSTATUS_ACTL_REG  & MEPSAN_INT_ENABLE) != 0u)
        {
            MEPSAN_backup.enableState = 1u;
        }
        else
        {
            MEPSAN_backup.enableState = 0u;
        }
    #else
        if((MEPSAN_TXSTATUS_ACTL_REG  & MEPSAN_INT_ENABLE) !=0u)
        {
            MEPSAN_backup.enableState = 1u;
        }
        else
        {
            MEPSAN_backup.enableState = 0u;
        }
    #endif /* End MEPSAN_RX_ENABLED || MEPSAN_HD_ENABLED*/

    MEPSAN_Stop();
    MEPSAN_SaveConfig();
}


/*******************************************************************************
* Function Name: MEPSAN_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred API to restore the component to the state when 
*  MEPSAN_Sleep() was called. The MEPSAN_Wakeup() function
*  calls the MEPSAN_RestoreConfig() function to restore the 
*  configuration. If the component was enabled before the 
*  MEPSAN_Sleep() function was called, the MEPSAN_Wakeup()
*  function will also re-enable the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  MEPSAN_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void MEPSAN_Wakeup(void)
{
    MEPSAN_RestoreConfig();
    #if( (MEPSAN_RX_ENABLED) || (MEPSAN_HD_ENABLED) )
        MEPSAN_ClearRxBuffer();
    #endif /* End (MEPSAN_RX_ENABLED) || (MEPSAN_HD_ENABLED) */
    #if(MEPSAN_TX_ENABLED || MEPSAN_HD_ENABLED)
        MEPSAN_ClearTxBuffer();
    #endif /* End MEPSAN_TX_ENABLED || MEPSAN_HD_ENABLED */

    if(MEPSAN_backup.enableState != 0u)
    {
        MEPSAN_Enable();
    }
}


/* [] END OF FILE */
