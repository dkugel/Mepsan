/**
 * @file PumpCom.c
 * @author Kugel Electronics
 * @date 10/04/2018
 * @brief Comunicación con el dispensador.
 *
 * Here typically goes a more extensive explanation of what the header
 * defines. Doxygens tags are words preceeded by either a backslash @\
 * or by an at symbol @@.
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 */

/* [] END OF FILE */

#include <GVar.h>



/*******************************************************************************
* Function Name: PollPump
********************************************************************************
*
*
*******************************************************************************/
uint8 GetAddress (void){
    uint8 x;
    uint8 response;
    UART_1_ClearRxBuffer();
    side.a.dir = 0xFF;
    side.b.dir = 0xFF;
    side.c.dir = 0xFF;
    side.d.dir = 0xFF;
    EnablePin_Write (1u);
    for(x=1;x<=15;x++){
        UART_1_PutChar(x);
        CyDelay(50);
        EnablePin_Write (0u);
        if((UART_1_GetRxBufferSize()>=1)&&(side.a.dir==0xff)){  
           response = UART_1_ReadRxData();
           side.a.dir=x;	
           UART_1_ClearRxBuffer();
        }
        if((UART_1_GetRxBufferSize()>=1)&&(x!=side.a.dir)){
           side.b.dir=x;
           UART_1_ClearRxBuffer();
        }
        if((UART_1_GetRxBufferSize()>=1)&&(x!=side.a.dir)&&(x!=side.b.dir)){
           side.c.dir=x;
           UART_1_ClearRxBuffer();
        }
        if((UART_1_GetRxBufferSize()>=1)&&(x!=side.a.dir)&&(x!=side.b.dir)&&(x!=side.c.dir)){
           side.d.dir=x;
           UART_1_ClearRxBuffer();
        }
    }
    UART_1_ClearRxBuffer();
    UART_1_ClearTxBuffer();
    if((side.a.dir!=0xff)&&(side.b.dir!=0xff)){
        return 2;
    }
    if((side.a.dir!=0xff)&&(side.b.dir!=0xff)&&(side.c.dir!=0xff)&&(side.d.dir!=0xff)){
        return 4;
    }else{
        if((side.a.dir!=0xff)||(side.b.dir!=0xff)){
            return 1;
        }else{
            return 0;
        }
    }
    EnablePin_Write (0u);
}




/*******************************************************************************
* Function Name: PollPump
********************************************************************************
*
*
*******************************************************************************/

uint8 PumpState(uint8 address)
{    
    UART_1_PutChar((0x50+address));
    UART_1_PutChar(0x20);
    UART_1_PutChar(0xFA);
    CyDelay(50);
    if(UART_1_GetRxBufferSize()>=1){
       CurrentState=(UART_1_ReadRxData()&0xF0)>>4;
       UART_1_ClearRxBuffer();
       return CurrentState;
    }
    else{
        return 0;
    }
}