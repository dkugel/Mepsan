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
#include "project.h"

int main(void)
{
    uint8 buffer_size;
    uint8 SPLL;
    uint8 bufferPOLL[100];
    uint8 Response;
    GBCL_Start();
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        /* Place your application code here. */
        buffer_size = GBCL_GetRxBufferSize();
        if(buffer_size >= 1 ){
            for(uint8 GBRx = 0; GBRx < buffer_size; GBRx++){
                bufferPOLL[GBRx] = GBCL_ReadRxData();
            }            
        }
        
        switch(bufferPOLL[0]){
            case 0x01:
                Response = 0x61;
            break;
        }
        
        CyDelay(50);
        GBCL_PutChar(Response);
        
    }
}

/* [] END OF FILE */
