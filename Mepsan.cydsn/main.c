/******************************************************************************
* File Name: main.c
*
* Version: 1.10
*
* Description: This is the source code for the Blinking LED code example.
*
* Related Document: CE195352_PSoC_5LP_Blinking_LED.pdf
*
* Hardware Dependency: See CE195352_PSoC_5LP_Blinking_LED.pdf
*
*******************************************************************************
* Copyright (2018), Cypress Semiconductor Corporation. All rights reserved.
*******************************************************************************
* This software, including source code, documentation and related materials
* (“Software”), is owned by Cypress Semiconductor Corporation or one of its
* subsidiaries (“Cypress”) and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software (“EULA”).
*
* If no EULA applies, Cypress hereby grants you a personal, nonexclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Cypress’s integrated circuit products.
* Any reproduction, modification, translation, compilation, or representation
* of this Software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, 
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress 
* reserves the right to make changes to the Software without notice. Cypress 
* does not assume any liability arising out of the application or use of the 
* Software or any product or circuit described in the Software. Cypress does 
* not authorize its products for use in any products where a malfunction or 
* failure of the Cypress product may reasonably be expected to result in 
* significant property damage, injury or death (“High Risk Product”). By 
* including Cypress’s product in a High Risk Product, the manufacturer of such 
* system or application assumes all risk of such use and in doing so agrees to 
* indemnify Cypress against all liability.
*******************************************************************************/

#include <project.h>
#include <GVar.h>
#include <PumpCom.h>


CY_ISR(polling);
uint8 Positions;



CY_ISR(polling){
    Timer_1_ReadStatusRegister();    											 
    PollCounter++; //Incrementa el contador    
}

/*******************************************************************************
* Function Name: main
********************************************************************************
*
*
*******************************************************************************/

int main()
{
    /* Prepare components */
    PWM_1_Start(); 
    CyGlobalIntEnable;
    UART_1_Start();
    Timer_1_Start();
    Positions = GetAddress();
    PollCounter = 0;
    
    for (;;)
    {
        if(PollCounter == 50){
            if(Positions == 2){
                PollPump(side.a.dir);
                PollPump(side.b.dir);
                PollCounter = 0;
            }
            if(Positions == 4){
                PollPump(side.a.dir);
                PollPump(side.b.dir);
                PollPump(side.c.dir);
                PollPump(side.d.dir);
                PollCounter = 0;
            }
        }
    }
}


/* [] END OF FILE */
