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
        if(side.a.MepRequest == MEPSAN_AUTHORIZE){
            Authorize(side.a.dir);
            side.a.Copy = 0;
        }
        if(side.b.MepRequest == MEPSAN_AUTHORIZE){
            Authorize(side.b.dir);
            side.b.Copy = 0;
        }
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
    uint8 x,screen_size,W_autorize,y,temp;
    uint8 passw[4]={'1','2','3','4'};
    uint8 change_pic[]={0x5A,0xA5,0x04,0x80,0x03,0x00,0x06};
    uint8 change_pic2[]={0x5A,0xA5,0x04,0x80,0x03,0x00,0x09};
    uint8 price_change;
    uint8 auxPrice[5];
    DecVol = 3;
    PWM_1_Start(); 
    CyGlobalIntEnable;
    MEPSAN_Start();
    GBCL_Start();
    PRINTER_A_Start();
    screen_Start();
    EEPROM_1_Start();
    side.a.Copy = 0;
    side.b.Copy = 0;
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
    
        
//    for(x = 0; x < 3; x ++){
//        side.a.ppuNozzle[0][x] = EEPROM_1_ReadByte(12 + x);
//        side.a.ppuNozzle[1][x] = EEPROM_1_ReadByte(15 + x);
//        side.a.ppuNozzle[2][x] = EEPROM_1_ReadByte(18 + x);
//        side.a.ppuNozzle[3][x] = EEPROM_1_ReadByte(21 + x);
//        side.b.ppuNozzle[0][x] = EEPROM_1_ReadByte(24 + x);
//        side.b.ppuNozzle[1][x] = EEPROM_1_ReadByte(27 + x);
//        side.b.ppuNozzle[2][x] = EEPROM_1_ReadByte(30 + x);
//        side.b.ppuNozzle[3][x] = EEPROM_1_ReadByte(33 + x);
//    }
//    
//    side.a.ProcessedPPU[0][0] = (side.a.ppuNozzle[0][0] >> 4) + 0x30;
//    side.a.ProcessedPPU[0][1] = (side.a.ppuNozzle[0][0] & 0x0F) + 0x30;
//    side.a.ProcessedPPU[0][2] = (side.a.ppuNozzle[0][1] >> 4) + 0x30;
//    side.a.ProcessedPPU[0][3] = (side.a.ppuNozzle[0][1] & 0x0F) + 0x30;
//    side.a.ProcessedPPU[0][4] = (side.a.ppuNozzle[0][2] >> 4) + 0x30;
//    side.a.ProcessedPPU[0][5] = (side.a.ppuNozzle[0][2] & 0x0F) + 0x30;
//    
//    side.b.ProcessedPPU[0][0] = (side.b.ppuNozzle[0][0] >> 4) + 0x30;
//    side.b.ProcessedPPU[0][1] = (side.b.ppuNozzle[0][0] & 0x0F) + 0x30;
//    side.b.ProcessedPPU[0][2] = (side.b.ppuNozzle[0][1] >> 4) + 0x30;
//    side.b.ProcessedPPU[0][3] = (side.b.ppuNozzle[0][1] & 0x0F) + 0x30;
//    side.b.ProcessedPPU[0][4] = (side.b.ppuNozzle[0][2] >> 4) + 0x30;
//    side.b.ProcessedPPU[0][5] = (side.b.ppuNozzle[0][2] & 0x0F) + 0x30;
    
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
    LCDhose = 0;
    for (;;)
    {
        PollingPos(); 
        //screen_PutChar(0x5A);
        screen_size = screen_GetRxBufferSize();
        if(screen_size >= 5 ){
            for(uint8 LCDRx = 0; LCDRx < screen_size; LCDRx++){
                touch1[LCDRx] = screen_ReadRxData();
            }            
        }
        CyDelay(4);
        switch(touch1[8]){
        	case 0xAA:
        		W_autorize = 1;
        		for(uint8 LCDRx = 0; LCDRx < 20; LCDRx++){
        			touch1[LCDRx] = 0x00;
        		}
        	break;
        	case 0x1C:
        		W_autorize = 'k';
        		for(uint8 LCDRx = 0; LCDRx < 20; LCDRx++){
        			touch1[LCDRx] = 0x00;
        		}		
        	break;
        	case 0xCA:
        		W_autorize = 3;
        		for(uint8 LCDRx = 0; LCDRx < 20; LCDRx++){
        			touch1[LCDRx] = 0x00;
        		}		
        	break;
        	case 0x0A:
        		if(W_autorize == 1){
        			W_autorize = 0;                    
                    //PriceUpdate(side.a.dir,side.a.ppuNozzle[0]);
                    Authorize(side.a.dir);
                    for(uint8 LCDRx = 0; LCDRx < 20; LCDRx++){
                        touch1[LCDRx] = 0x00;
                    }
        		}
        		if(W_autorize == 2){ 
                    W_autorize = 0x0A;
                    PumpState(side.a.dir);            
                    TotalRequest(side.a.dir, TotalRequestType, 1); //dir 0, volume, nozzle 1                           
                    PumpState(side.a.dir);
                    for(uint8 LCDRx = 0; LCDRx < 20; LCDRx++){
                        touch1[LCDRx] = 0x00;
                    }                    
        		}
                if(W_autorize == 'k'){                    
                    W_autorize = 0x1A; 
                    PumpState(side.a.dir);            
                    TotalRequest(side.a.dir, TotalRequestType, 1); //dir 0, volume, nozzle 1                           
                    PumpState(side.a.dir);
                    for(uint8 LCDRx = 0; LCDRx < 20; LCDRx++){
                        touch1[LCDRx] = 0x00;
                    }
                    for(uint8 LCDRx = 0; LCDRx < 8; LCDRx++){
                        side.a.msn_plate[LCDRx] = 0x20;
                    }
                    for(uint8 LCDRx = 0; LCDRx < 6; LCDRx++){
                        side.a.km[LCDRx] = 0x20;
                    }                                        
        		}
        	break;            
        	case 0x0B:
        		if(W_autorize == 1){
        			W_autorize = 0;
                    //PriceUpdate(side.b.dir,side.b.ppuNozzle[0]);
                    Authorize(side.b.dir);
                    for(uint8 LCDRx = 0; LCDRx < 20; LCDRx++){
                        touch1[LCDRx] = 0x00;
                    }
        		}
        		if(W_autorize == 2){
                    W_autorize = 0x0B;
                    PumpState(side.b.dir);            
                    TotalRequest(side.b.dir, TotalRequestType, 1); //dir 0, volume, nozzle 1                           
                    PumpState(side.b.dir);                     
                }                
                if(W_autorize == 'k'){                    
                    W_autorize = 0x1B; 
                    PumpState(side.b.dir);            
                    TotalRequest(side.b.dir, TotalRequestType, 1); //dir 0, volume, nozzle 1                           
                    PumpState(side.b.dir);
                    for(uint8 LCDRx = 0; LCDRx < 20; LCDRx++){
                        touch1[LCDRx] = 0x00;
                    }
                    for(uint8 LCDRx = 0; LCDRx < 8; LCDRx++){
                        side.b.msn_plate[LCDRx] = 0x20;
                    }
                    for(uint8 LCDRx = 0; LCDRx < 6; LCDRx++){
                        side.b.km[LCDRx] = 0x20;
                    }                                        
        		}
        	break;
        	case 0xCC:
        		PumpState(side.b.dir);            
        		TotalRequest(side.b.dir, TotalRequestType, 1); //dir 0, volume, nozzle 1      
                CyDelay(10);
        		PumpState(side.b.dir); 
        		PumpState(side.a.dir);            
        		TotalRequest(side.a.dir, TotalRequestType, 1); //dir 0, volume, nozzle 1   
                CyDelay(10);
        		PumpState(side.a.dir);
                PumpState(side.b.dir);
                PumpState(side.a.dir);
        		for(uint8 LCDRx = 0; LCDRx < 20; LCDRx++){
        			touch1[LCDRx] = 0x00;
        		}                        
        		PrintShift(); 
        	break;
            case 0x53:
                if(W_autorize == 0xAC){
                    side.a.Copy = 1;
                    PrintReceipt(side.a.dir);
                }
                if(W_autorize == 0xBC){
                    side.b.Copy = 1;
                    PrintReceipt(side.b.dir);
                }                
            break;
            case 0x4E:
                if(W_autorize == 0xAC){
                    W_autorize = 0;
                    side.a.Copy = 0;                    
                }
                if(W_autorize == 0xBC){
                    W_autorize = 0;
                    side.b.Copy = 0;                                        
                }                
            break;
        	default:
                if(touch1[3] == 0x83 && price_change == 0x0A && touch1[8] != 0xE1){
                    for(uint8 LCDRx = 0; LCDRx < 6; LCDRx++){                        
                        side.a.ProcessedPPU[0][LCDRx] = 0x30;
            		}
                    y = 0;
                    LCDhose = 0;
                    price_change = 0;
                    for(uint8 LCDRx = 7; LCDRx < 12; LCDRx++){
                        if((touch1[LCDRx] == 0x00) ||(touch1[LCDRx] == 0xFF))
                            break;
                        auxPrice[LCDRx-7] = touch1[LCDRx];
                        y++;
            		}
                    for (x = 6; x >0; x--){
                        side.a.ProcessedPPU[0][x] = auxPrice[y];                        
                        if(y == 0 )
                            break;
                        y--;
                    }
                    
                    side.a.ppuNozzle[0][0] = ((side.a.ProcessedPPU[0][0] - 0x30)<<4) |((side.a.ProcessedPPU[0][1] - 0x30));
                    side.a.ppuNozzle[0][1] = ((side.a.ProcessedPPU[0][2] - 0x30)<<4) |((side.a.ProcessedPPU[0][3] - 0x30));
                    side.a.ppuNozzle[0][2] = ((side.a.ProcessedPPU[0][4] - 0x30)<<4) |((side.a.ProcessedPPU[0][5] - 0x30));
                    for(x = 0; x < 3; x++){
                        EEPROM_1_WriteByte(side.a.ppuNozzle[0][x],12+x);
                    }                                                            
                    for(uint8 LCDRx = 0; LCDRx < 20; LCDRx++){
            			touch1[LCDRx] = 0x00;
            		}
                }
                if(touch1[3] == 0x83 && price_change == 1 ){
                    LCDhose = touch1[8];
                    price_change = 0x0A;
                }
        		if(touch1[3] == 0x83 && W_autorize == 3){             
                    W_autorize = 0;
                    if((touch1[7] == passw[0])&&(touch1[8] == passw[1])&&(touch1[9] == passw[2]) && (touch1[10] == passw[3]) &&(touch1[11] == 0xFF) ){
                        for(uint8 LCDRx = 0; LCDRx < 6; LCDRx++){
                            screen_PutChar(change_pic[LCDRx]);
                        }
                        CyDelay(4);
                    }
                    if(touch1[8] == 0x01)
                        price_change = 1;
                    if(touch1[8] == 0x02)
                        price_change = 2;
                    for(uint8 LCDRx = 0; LCDRx < 20; LCDRx++){
            			touch1[LCDRx] = 0x00;
            		}
                } 
                if(touch1[3] == 0x83 && W_autorize == 0x1A){                                 
                    for(uint8 LCDRx = 7; LCDRx < 15; LCDRx++){
                        if((touch1[LCDRx] == 0x00) ||(touch1[LCDRx] == 0xFF))
                            break;
                        side.a.km[LCDRx-7] = touch1[LCDRx];
            		}
                    for(uint8 LCDRx = 0; LCDRx < 20; LCDRx++){
            			touch1[LCDRx] = 0x00;
            		}                    
                    W_autorize = 0x0A;
                }
                if(touch1[3] == 0x83 && W_autorize == 0x0A){             
                    W_autorize = 0;
                    for(uint8 LCDRx = 7; LCDRx < 15; LCDRx++){
                        if((touch1[LCDRx] == 0x00) ||(touch1[LCDRx] == 0xFF))
                            break;
                        if(touch1[LCDRx] > 96 && touch1[LCDRx] < 123){
                            side.a.msn_plate[LCDRx-7] = touch1[LCDRx]-0x20;                            
                        }else{
                            side.a.msn_plate[LCDRx-7] = touch1[LCDRx];
                        }
            		}
                    for(uint8 LCDRx = 0; LCDRx < 20; LCDRx++){
            			touch1[LCDRx] = 0x00;
            		}
                    PrintReceipt(side.a.dir);
                    side.a.Copy = 1;
                    W_autorize =0xAC;
                } 
                if(touch1[3] == 0x83 && W_autorize == 0x1B){                                 
                    for(uint8 LCDRx = 7; LCDRx < 15; LCDRx++){
                        if((touch1[LCDRx] == 0x00) ||(touch1[LCDRx] == 0xFF))
                            break;
                        side.b.km[LCDRx-7] = touch1[LCDRx];
            		}
                    for(uint8 LCDRx = 0; LCDRx < 20; LCDRx++){
            			touch1[LCDRx] = 0x00;
            		}                    
                    W_autorize = 0x0B;
                }
                if(touch1[3] == 0x83 && W_autorize == 0x0B){             
                    W_autorize = 0;
                    for(uint8 LCDRx = 7; LCDRx < 15; LCDRx++){
                        if((touch1[LCDRx] == 0x00) ||(touch1[LCDRx] == 0xFF))
                            break;
                        if(touch1[LCDRx] > 96 && touch1[LCDRx] < 123){
                            side.b.msn_plate[LCDRx-7] = touch1[LCDRx]-0x20;                            
                        }else{
                            side.b.msn_plate[LCDRx-7] = touch1[LCDRx];
                        }
            		}
                    for(uint8 LCDRx = 0; LCDRx < 20; LCDRx++){
            			touch1[LCDRx] = 0x00;
            		}
                    PrintReceipt(side.b.dir);
                    side.b.Copy = 1;
                    W_autorize  = 0xBC;
                }
                
        	break;
        }
        screen_size = 0;
    }
}





/* [] END OF FILE */
