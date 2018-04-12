/*******************************************************************************
* File Name: EnablePin.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_EnablePin_H) /* Pins EnablePin_H */
#define CY_PINS_EnablePin_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "EnablePin_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 EnablePin__PORT == 15 && ((EnablePin__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    EnablePin_Write(uint8 value);
void    EnablePin_SetDriveMode(uint8 mode);
uint8   EnablePin_ReadDataReg(void);
uint8   EnablePin_Read(void);
void    EnablePin_SetInterruptMode(uint16 position, uint16 mode);
uint8   EnablePin_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the EnablePin_SetDriveMode() function.
     *  @{
     */
        #define EnablePin_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define EnablePin_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define EnablePin_DM_RES_UP          PIN_DM_RES_UP
        #define EnablePin_DM_RES_DWN         PIN_DM_RES_DWN
        #define EnablePin_DM_OD_LO           PIN_DM_OD_LO
        #define EnablePin_DM_OD_HI           PIN_DM_OD_HI
        #define EnablePin_DM_STRONG          PIN_DM_STRONG
        #define EnablePin_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define EnablePin_MASK               EnablePin__MASK
#define EnablePin_SHIFT              EnablePin__SHIFT
#define EnablePin_WIDTH              1u

/* Interrupt constants */
#if defined(EnablePin__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in EnablePin_SetInterruptMode() function.
     *  @{
     */
        #define EnablePin_INTR_NONE      (uint16)(0x0000u)
        #define EnablePin_INTR_RISING    (uint16)(0x0001u)
        #define EnablePin_INTR_FALLING   (uint16)(0x0002u)
        #define EnablePin_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define EnablePin_INTR_MASK      (0x01u) 
#endif /* (EnablePin__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define EnablePin_PS                     (* (reg8 *) EnablePin__PS)
/* Data Register */
#define EnablePin_DR                     (* (reg8 *) EnablePin__DR)
/* Port Number */
#define EnablePin_PRT_NUM                (* (reg8 *) EnablePin__PRT) 
/* Connect to Analog Globals */                                                  
#define EnablePin_AG                     (* (reg8 *) EnablePin__AG)                       
/* Analog MUX bux enable */
#define EnablePin_AMUX                   (* (reg8 *) EnablePin__AMUX) 
/* Bidirectional Enable */                                                        
#define EnablePin_BIE                    (* (reg8 *) EnablePin__BIE)
/* Bit-mask for Aliased Register Access */
#define EnablePin_BIT_MASK               (* (reg8 *) EnablePin__BIT_MASK)
/* Bypass Enable */
#define EnablePin_BYP                    (* (reg8 *) EnablePin__BYP)
/* Port wide control signals */                                                   
#define EnablePin_CTL                    (* (reg8 *) EnablePin__CTL)
/* Drive Modes */
#define EnablePin_DM0                    (* (reg8 *) EnablePin__DM0) 
#define EnablePin_DM1                    (* (reg8 *) EnablePin__DM1)
#define EnablePin_DM2                    (* (reg8 *) EnablePin__DM2) 
/* Input Buffer Disable Override */
#define EnablePin_INP_DIS                (* (reg8 *) EnablePin__INP_DIS)
/* LCD Common or Segment Drive */
#define EnablePin_LCD_COM_SEG            (* (reg8 *) EnablePin__LCD_COM_SEG)
/* Enable Segment LCD */
#define EnablePin_LCD_EN                 (* (reg8 *) EnablePin__LCD_EN)
/* Slew Rate Control */
#define EnablePin_SLW                    (* (reg8 *) EnablePin__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define EnablePin_PRTDSI__CAPS_SEL       (* (reg8 *) EnablePin__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define EnablePin_PRTDSI__DBL_SYNC_IN    (* (reg8 *) EnablePin__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define EnablePin_PRTDSI__OE_SEL0        (* (reg8 *) EnablePin__PRTDSI__OE_SEL0) 
#define EnablePin_PRTDSI__OE_SEL1        (* (reg8 *) EnablePin__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define EnablePin_PRTDSI__OUT_SEL0       (* (reg8 *) EnablePin__PRTDSI__OUT_SEL0) 
#define EnablePin_PRTDSI__OUT_SEL1       (* (reg8 *) EnablePin__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define EnablePin_PRTDSI__SYNC_OUT       (* (reg8 *) EnablePin__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(EnablePin__SIO_CFG)
    #define EnablePin_SIO_HYST_EN        (* (reg8 *) EnablePin__SIO_HYST_EN)
    #define EnablePin_SIO_REG_HIFREQ     (* (reg8 *) EnablePin__SIO_REG_HIFREQ)
    #define EnablePin_SIO_CFG            (* (reg8 *) EnablePin__SIO_CFG)
    #define EnablePin_SIO_DIFF           (* (reg8 *) EnablePin__SIO_DIFF)
#endif /* (EnablePin__SIO_CFG) */

/* Interrupt Registers */
#if defined(EnablePin__INTSTAT)
    #define EnablePin_INTSTAT            (* (reg8 *) EnablePin__INTSTAT)
    #define EnablePin_SNAP               (* (reg8 *) EnablePin__SNAP)
    
	#define EnablePin_0_INTTYPE_REG 		(* (reg8 *) EnablePin__0__INTTYPE)
#endif /* (EnablePin__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_EnablePin_H */


/* [] END OF FILE */
