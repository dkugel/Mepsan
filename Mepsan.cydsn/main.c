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
        side.a.states[0] = PumpState(side.a.dir);                                    
        //ReturnStatus(side.a.dir);        
    }
    if(Positions == 2){
        
        side.a.states[0] = PumpState(side.a.dir);
        side.b.states[0] = PumpState(side.b.dir);
                    
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
    uint8 x,screen_size;
    DecVol = 3;
    PWM_1_Start(); 
    CyGlobalIntEnable;
    UART_1_Start();
    UART_2_Start();
    UART_3_Start();
    screen_Start();
    Timer_1_Start();    
    side.a.ppuNozzle[0][0]=0x01;
    side.a.ppuNozzle[0][1]=0x04;
    side.a.ppuNozzle[0][2]=0x20;
    
    side.b.ppuNozzle[0][0]=0x00;
    side.b.ppuNozzle[0][1]=0x84;
    side.b.ppuNozzle[0][2]=0x55;
    
    side.a.ProcessedPPU[0][0] = (side.a.ppuNozzle[0][0] >> 4) + 0x30;
    side.a.ProcessedPPU[0][1] = (side.a.ppuNozzle[0][0] & 0x0F) + 0x30;
    side.a.ProcessedPPU[0][2] = (side.a.ppuNozzle[0][1] >> 4) + 0x30;
    side.a.ProcessedPPU[0][3] = (side.a.ppuNozzle[0][1] & 0x0F) + 0x30;
    side.a.ProcessedPPU[0][4] = (side.a.ppuNozzle[0][2] >> 4) + 0x30;
    side.a.ProcessedPPU[0][5] = (side.a.ppuNozzle[0][2] & 0x0F) + 0x30;
    
    side.b.ProcessedPPU[0][0] = (side.b.ppuNozzle[0][0] >> 4) + 0x30;
    side.b.ProcessedPPU[0][1] = (side.b.ppuNozzle[0][0] & 0x0F) + 0x30;
    side.b.ProcessedPPU[0][2] = (side.b.ppuNozzle[0][1] >> 4) + 0x30;
    side.b.ProcessedPPU[0][3] = (side.b.ppuNozzle[0][1] & 0x0F) + 0x30;
    side.b.ProcessedPPU[0][4] = (side.b.ppuNozzle[0][2] >> 4) + 0x30;
    side.b.ProcessedPPU[0][5] = (side.b.ppuNozzle[0][2] & 0x0F) + 0x30;
    
    //while(side.a.dir == 0xFF){
        for(uint8 y = 0; y < 16; y ++){
            PumpAddress[y] = GetAddress(y); //Posiciones activas      
        } 
    //}
    
    side.a.dir = PumpAddress[1];
    side.b.dir = PumpAddress[2];
    side.c.dir = PumpAddress[3];
    side.d.dir = PumpAddress[4]; 
    side.a.dir = 0x00;
    side.b.dir = 0x01;
    TotalRequestType = 0; 
    Receipt = 1;        
    for (;;)
    {
        screen_size = screen_GetRxBufferSize();
        if(screen_size >= 9 ){
            for(uint8 LCDRx = 0; LCDRx < screen_size; LCDRx++){
                touch1[LCDRx] = screen_ReadRxData();
            }
            screen_size = 0;
        }
        screen_PutChar(0x5A);
        if(touch1[8] == 0x30){
            PriceUpdate(side.a.dir,side.a.ppuNozzle[0]);
            Authorize(side.a.dir);
            for(uint8 LCDRx = 0; LCDRx < 20; LCDRx++){
                touch1[LCDRx] = 0x00;
            }
        }else{
            PollingPos();                       
        }                
        if(touch1[8] == 0x35){
            //PriceUpdate(0,side.a.ppuNozzle[0]);
            //ReturnStatus(0);
            PumpState(side.a.dir);            
            TotalRequest(side.a.dir, TotalRequestType, 1); //dir 0, volume, nozzle 1                           
            PumpState(side.a.dir); 
            for(uint8 LCDRx = 0; LCDRx < 20; LCDRx++){
                touch1[LCDRx] = 0x00;
            }            
            PrintReceipt(side.a.dir);                        
        }        
         if(touch1[8] == 0x36){
            //PriceUpdate(0,side.a.ppuNozzle[0]);
            //ReturnStatus(0);
            PumpState(side.b.dir);            
            TotalRequest(side.b.dir, TotalRequestType, 1); //dir 0, volume, nozzle 1                           
            PumpState(side.b.dir); 
            for(uint8 LCDRx = 0; LCDRx < 20; LCDRx++){
                touch1[LCDRx] = 0x00;
            }            
            PrintReceipt(side.b.dir);
                       
        }        
         if(touch1[8] == 0x37){
            //PriceUpdate(0,side.a.ppuNozzle[0]);
            //ReturnStatus(0);
            PumpState(side.a.dir);            
            TotalRequest(side.a.dir, TotalRequestType, 1); //dir 0, volume, nozzle 1                           
            PumpState(side.a.dir); 
            PumpState(side.b.dir);            
            TotalRequest(side.b.dir, TotalRequestType, 1); //dir 0, volume, nozzle 1                           
            PumpState(side.b.dir);
            for(uint8 LCDRx = 0; LCDRx < 20; LCDRx++){
                touch1[LCDRx] = 0x00;
            }                        
            PrintShift(side.a.dir);       
            PrintShift(side.b.dir);            
        }                      
    }
}





/* [] END OF FILE */
