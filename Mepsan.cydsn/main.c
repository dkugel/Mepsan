/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>
#include <GVar.h>
#include <PumpCom.h>
#include <Gbr.h>
#include <printer.h>

uint8 GBRx;

void PollingGB(void){
    uint8 size, GBRxData,GBTxData;
    GBRxData = GBCL_GetChar(); 
    
    if(GBRxData == 0x00 || GBRxData == 0x01 ){
        PumpState(GBRxData);
        GBTxData = GBRxData|GILB_Send;
        GBCL_PutChar(GBTxData);        
        if(side.a.MepRequest == MEPSAN_AUTHORIZE){
            Authorize(side.a.dir);
            side.a.Copy = 0;
        }
        if(side.b.MepRequest == MEPSAN_AUTHORIZE){
            Authorize(side.a.dir);
            side.a.Copy = 0;
        }
        //return GBRx;
    }  
     
}



int main()
{
    /* Prepare components */
    uint8 posGB;
    CyGlobalIntEnable;
    GBCL_Start();
    MEPSAN_Start();
    side.a.dir = 0x00;
    side.b.dir = 0x01;
    Positions = 2;
    posGB = 0;
    GBRx = 0;
    
    
    for (;;)
    {
        //PollingPos(); 
        //while(posGB < 2){
        PollingGB();
        //PollingPos();
        //posGB = PollingGB();        
        
        //}               
        
       
    }
}


/* [] END OF FILE */
