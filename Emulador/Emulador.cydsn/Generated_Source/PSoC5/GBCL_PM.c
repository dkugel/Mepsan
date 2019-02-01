/*******************************************************************************
* File Name: GBCL_PM.c
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

#include "GBCL.h"


/***************************************
* Local data allocation
***************************************/

static GBCL_BACKUP_STRUCT  GBCL_backup =
{
    /* enableState - disabled */
    0u,
};



/*******************************************************************************
* Function Name: GBCL_SaveConfig
********************************************************************************
*
* Summary:
*  This function saves the component nonretention control register.
*  Does not save the FIFO which is a set of nonretention registers.
*  This function is called by the GBCL_Sleep() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  GBCL_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void GBCL_SaveConfig(void)
{
    #if(GBCL_CONTROL_REG_REMOVED == 0u)
        GBCL_backup.cr = GBCL_CONTROL_REG;
    #endif /* End GBCL_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: GBCL_RestoreConfig
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
*  GBCL_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
* Notes:
*  If this function is called without calling GBCL_SaveConfig() 
*  first, the data loaded may be incorrect.
*
*******************************************************************************/
void GBCL_RestoreConfig(void)
{
    #if(GBCL_CONTROL_REG_REMOVED == 0u)
        GBCL_CONTROL_REG = GBCL_backup.cr;
    #endif /* End GBCL_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: GBCL_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred API to prepare the component for sleep. 
*  The GBCL_Sleep() API saves the current component state. Then it
*  calls the GBCL_Stop() function and calls 
*  GBCL_SaveConfig() to save the hardware configuration.
*  Call the GBCL_Sleep() function before calling the CyPmSleep() 
*  or the CyPmHibernate() function. 
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  GBCL_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void GBCL_Sleep(void)
{
    #if(GBCL_RX_ENABLED || GBCL_HD_ENABLED)
        if((GBCL_RXSTATUS_ACTL_REG  & GBCL_INT_ENABLE) != 0u)
        {
            GBCL_backup.enableState = 1u;
        }
        else
        {
            GBCL_backup.enableState = 0u;
        }
    #else
        if((GBCL_TXSTATUS_ACTL_REG  & GBCL_INT_ENABLE) !=0u)
        {
            GBCL_backup.enableState = 1u;
        }
        else
        {
            GBCL_backup.enableState = 0u;
        }
    #endif /* End GBCL_RX_ENABLED || GBCL_HD_ENABLED*/

    GBCL_Stop();
    GBCL_SaveConfig();
}


/*******************************************************************************
* Function Name: GBCL_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred API to restore the component to the state when 
*  GBCL_Sleep() was called. The GBCL_Wakeup() function
*  calls the GBCL_RestoreConfig() function to restore the 
*  configuration. If the component was enabled before the 
*  GBCL_Sleep() function was called, the GBCL_Wakeup()
*  function will also re-enable the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  GBCL_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void GBCL_Wakeup(void)
{
    GBCL_RestoreConfig();
    #if( (GBCL_RX_ENABLED) || (GBCL_HD_ENABLED) )
        GBCL_ClearRxBuffer();
    #endif /* End (GBCL_RX_ENABLED) || (GBCL_HD_ENABLED) */
    #if(GBCL_TX_ENABLED || GBCL_HD_ENABLED)
        GBCL_ClearTxBuffer();
    #endif /* End GBCL_TX_ENABLED || GBCL_HD_ENABLED */

    if(GBCL_backup.enableState != 0u)
    {
        GBCL_Enable();
    }
}


/* [] END OF FILE */
