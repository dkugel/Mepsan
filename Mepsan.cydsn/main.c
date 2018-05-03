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
#include <Gbr.h>
#include <printer.h>

CY_ISR(polling);




CY_ISR(polling){
    Timer_1_ReadStatusRegister();    											 
    PollCounter++; //Incrementa el contador    
}


void PollingPos(void){
    uint8 x;
    if(side.a.dir != 0xFF && side.b.dir == 0xFF){
        Positions = 1;
    }
    if(side.a.dir != 0xFF && side.b.dir != 0xFF){
        Positions = 2;
    }
    if(side.c.dir != 0xFF && side.d.dir == 0xFF){
        Positions = 3;
    }
    if(side.c.dir != 0xFF && side.d.dir != 0xFF){
        Positions = 4;
    }
    if(Positions == 1){
        if(x == 5)
            x = 0;
        side.a.states[x] = PumpState(side.a.dir);                
        x++;     
        ReturnStatus(side.a.dir);
        TotalRequest(side.a.dir, 0, 1); // dir, volume, nozzle
    }
    if(Positions == 2){
        for(x = 0; x < 5; x++){
            side.a.states[x] = PumpState(side.a.dir);
            side.b.states[x] = PumpState(side.b.dir);
        }            
    }
    if(Positions == 3){
        for(x = 0; x < 5; x++){
            side.a.states[x] = PumpState(side.a.dir);
            side.b.states[x] = PumpState(side.b.dir);
            side.c.states[x] = PumpState(side.c.dir);
        }
        
    }
    if(Positions == 4){
        for(x = 0; x < 5; x++){
            side.a.states[x] = PumpState(side.a.dir);
            side.b.states[x] = PumpState(side.b.dir);
            side.c.states[x] = PumpState(side.c.dir);
            side.d.states[x] = PumpState(side.d.dir);
        }            
    } 

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
    uint8 x;
    PWM_1_Start(); 
    CyGlobalIntEnable;
    UART_1_Start();
    UART_2_Start();
    UART_3_Start();
    Timer_1_Start();    
    side.a.ppuNozzle[0][0]=0x01;
    side.a.ppuNozzle[0][1]=0x05;
    side.a.ppuNozzle[0][2]=0x30;    
    for(uint8 y = 0; y < 16; y ++){
        PumpAddress[y] = GetAddress(y); //Posiciones activas      
    }    
    side.a.dir = PumpAddress[1];
    side.b.dir = PumpAddress[2];
    side.c.dir = PumpAddress[3];
    side.d.dir = PumpAddress[4];  
            
    for (;;)
    {
        if(EnablePin_1_Read() == 1u){
            PriceUpdate(0,side.a.ppuNozzle[0]);
            Authorize(side.a.dir);
            PrintReceipt();
        }else{
            PollingPos();
        }                
        if(Kill_Switch_Read() == 0u){
            //PriceUpdate(0,side.a.ppuNozzle[0]);
            //ReturnStatus(0);
            PumpState(0);
            TotalRequest(0, 0, 1); //dir 0, volume, nozzle 1
            ProccessResponse(0);
            PumpState(0);
//            TotalRequest(0, 1, 1); //dir 0, volume, nozzle 1
//            ProccessResponse();
        }
        
    }
}





/* [] END OF FILE */
