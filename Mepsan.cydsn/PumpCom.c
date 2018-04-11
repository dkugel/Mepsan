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
    side.a.dir = 0xFF;
    side.b.dir = 0xFF;
    side.c.dir = 0xFF;
    side.d.dir = 0xFF;
    for(x=0;x<=15;x++){
        UART_1_PutChar(x);
        CyDelay(50);
        if((UART_1_GetRxBufferSize()>=1)&&(side.a.dir==0xff)){
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
}
//
//lado.a.dir=0xff;
//lado.b.dir=0xff;
//    for(x=0;x<=15;x++){
//        Surtidor_PutChar(x);
//        CyDelay(100);
//        if((Surtidor_GetRxBufferSize()>=1)&&(lado.a.dir==0xff)){
//           lado.a.dir=x;	
//           Surtidor_ClearRxBuffer();
//        }
//        if((Surtidor_GetRxBufferSize()>=1)&&(x!=lado.a.dir)){
//           lado.b.dir=x;
//           Surtidor_ClearRxBuffer();
//        }
//    }
//    if((lado.a.dir!=0xff)&&(lado.b.dir!=0xff)){
//        return 2;
//    }
//    else{
//        if((lado.a.dir!=0xff)||(lado.b.dir!=0xff)){
//            return 1;
//        }
//        else{
//            return 0;
//        }
//    }
//}



/*******************************************************************************
* Function Name: PollPump
********************************************************************************
*
*
*******************************************************************************/

uint8 PollPump(uint8 address)
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