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


void PollingGB(void){
    uint8 size,GBRx, GBRxData;
    GBRxData = GBCL_GetChar(); 
    if(GBRxData == 0x00){
        GBCL_PutChar(0x60);
        
    }
    if(GBRxData == 0x01){
        GBCL_PutChar(0x61);
        
    }

}

int main()
{
    /* Prepare components */
   
    CyGlobalIntEnable;
    GBCL_Start();

   
    for (;;)
    {
        //PollingPos(); 
        PollingGB();
        CyDelay(50);
       
    }
}


/* [] END OF FILE */
