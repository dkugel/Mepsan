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
    uint8 x,screen_size,W_autorize;
    uint8 passw[4]={'1','2','3','4'};
    uint8 change_pic[7]={0x5A,0xA5,0x04,0x80,0x03,0x00,0x06};
    uint8 change_pic2[7]={0x5A,0xA5,0x04,0x80,0x03,0x00,0x09};
    DecVol = 3;
    PWM_1_Start(); 
    CyGlobalIntEnable;
    UART_1_Start();
    UART_2_Start();
    UART_3_Start();
    screen_Start();
    EEPROM_1_Start();
    if(EEPROM_1_ReadByte(0) == 0x5A){
        for(x=0;x<6;x++){
    		sale_number[x]=EEPROM_1_ReadByte(x);
    	}
    }else{
        EEPROM_1_WriteByte(0x5A,0);
        for(x=1;x<6;x++){
    		sale_number[x]='0';
    	}
    }
    int_salenumber=((sale_number[5]&0x0F)*10000)+((sale_number[4]&0x0F)*1000)+((sale_number[3]&0x0F)*100)+((sale_number[2]&0x0F)*10)+((sale_number[1]&0x0F));
    if(EEPROM_1_ReadByte(6) == 0x5A){
        for(x=0;x<6;x++){
    		shift_number[x+6]=EEPROM_1_ReadByte(x+6);
    	}
    }else{
        EEPROM_1_WriteByte(0x5A,6);
        for(x=1;x<6;x++){
    		shift_number[x+6]='0';
    	}
    }
    int_shiftnumber=((shift_number[5]&0x0F)*10000)+((shift_number[4]&0x0F)*1000)+((shift_number[3]&0x0F)*100)+((shift_number[2]&0x0F)*10)+((shift_number[1]&0x0F));
    
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
    W_autorize = 0;        
    
    for (;;)
    {
        PollingPos(); 
        screen_PutChar(0x5A);
        screen_size = screen_GetRxBufferSize();
        if(screen_size >= 9 ){
            for(uint8 LCDRx = 0; LCDRx < screen_size; LCDRx++){
                touch1[LCDRx] = screen_ReadRxData();
            }
            screen_size = 0;
        }
        if(touch1[8] == 0xAA){
            W_autorize = 1;
            for(uint8 LCDRx = 0; LCDRx < 20; LCDRx++){
                touch1[LCDRx] = 0x00;
            }
        }
        if(touch1[8] == 0x1C){
            W_autorize = 2;
            for(uint8 LCDRx = 0; LCDRx < 20; LCDRx++){
                touch1[LCDRx] = 0x00;
            }
        }
        if(touch1[8] == 0xCA){
            W_autorize = 3;
            for(uint8 LCDRx = 0; LCDRx < 20; LCDRx++){
                touch1[LCDRx] = 0x00;
            }
        }
        
        if(touch1[8] == 0x0A && W_autorize == 1){
            W_autorize = 0;
            PriceUpdate(side.a.dir,side.a.ppuNozzle[0]);
            Authorize(side.a.dir);
            for(uint8 LCDRx = 0; LCDRx < 20; LCDRx++){
                touch1[LCDRx] = 0x00;
            }
        }
        if(touch1[8] == 0x0B && W_autorize == 1){
            W_autorize = 0;
            PriceUpdate(side.b.dir,side.b.ppuNozzle[0]);
            Authorize(side.b.dir);
            for(uint8 LCDRx = 0; LCDRx < 20; LCDRx++){
                touch1[LCDRx] = 0x00;
            }
        }                                            
        if(touch1[8] == 0x0A && W_autorize == 2){
            W_autorize = 0;
            PumpState(side.a.dir);            
            TotalRequest(side.a.dir, TotalRequestType, 1); //dir 0, volume, nozzle 1                           
            PumpState(side.a.dir); 
            for(uint8 LCDRx = 0; LCDRx < 20; LCDRx++){
                touch1[LCDRx] = 0x00;
            }            
            PrintReceipt(side.a.dir);                        
        }        
         if(touch1[8] == 0x0B && W_autorize == 2){
            W_autorize = 0;
            PumpState(side.b.dir);            
            TotalRequest(side.b.dir, TotalRequestType, 1); //dir 0, volume, nozzle 1                           
            PumpState(side.b.dir); 
            for(uint8 LCDRx = 0; LCDRx < 20; LCDRx++){
                touch1[LCDRx] = 0x00;
            }            
            PrintReceipt(side.b.dir);
                       
        }        
        if(touch1[8] == 0xCC){
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
            PrintShift();                  
        }
        if(touch1[3] == 0x83 && W_autorize == 3 ){
             
            W_autorize = 0;
            if((touch1[7] == passw[0])&&(touch1[8] == passw[1])&&(touch1[9] == passw[2]) && (touch1[10] == passw[3]) &&(touch1[11] == 0xFF) ){
                for(uint8 LCDRx = 0; LCDRx < 6; LCDRx++){
                    screen_PutChar(change_pic[LCDRx]);
                }
            }else{                
                for(uint8 LCDRx = 0; LCDRx < 6; LCDRx++){
                    screen_PutChar(change_pic2[LCDRx]);
                }
            }
        }
    }
}





/* [] END OF FILE */
