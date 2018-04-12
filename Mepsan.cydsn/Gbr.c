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

/* [] END OF FILE */
#include <GVar.h>

void StateTransform(){
    //if(dirGbr == side.a.dir)
    UART_2_PutChar(GILB_IDLE);
    
}

void SystemQ(){
    uint8 QSystem;
    uint8 size;
    size = UART_2_GetRxBufferSize();
    dirGbr = UART_2_ReadRxData()&0x0F;
    if(size== 1){
        QSystem = UART_2_ReadRxData();
        switch(QSystem){
            case 0:
                StateTransform();
            break;
                
            case 2:
                
            break;
            
            case 4:
                
            break;
            
            case 5:
                
            break;
                
            case 6:
                
            break;
        }
    }
    if(size > 1){
    }
}

