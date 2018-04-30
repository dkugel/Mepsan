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

#define CRC16 0x8005

uint16_t gen_crc16(const uint8_t *data, uint16_t size)
{
    uint16_t out = 0;
    int bits_read = 0, bit_flag;

    /* Sanity check: */
    if(data == NULL)
        return 0;

    while(size > 0)
    {
        bit_flag = out >> 15;

        /* Get next bit: */
        out <<= 1;
        out |= (*data >> bits_read) & 1; // item a) work from the least significant bits

        /* Increment bit counter: */
        bits_read++;
        if(bits_read > 7)
        {
            bits_read = 0;
            data++;
            size--;
        }

        /* Cycle check: */
        if(bit_flag)
            out ^= CRC16;

    }

    // item b) "push out" the last 16 bits
    int i;
    for (i = 0; i < 16; ++i) {
        bit_flag = out >> 15;
        out <<= 1;
        if(bit_flag)
            out ^= CRC16;
    }

    // item c) reverse the bits
    uint16_t crc = 0;
    i = 0x8000;
    int j = 0x0001;
    for (; i != 0; i >>=1, j <<= 1) {
        if (i & out) crc |= j;
    }

    return crc;
}


/*******************************************************************************
* Function Name: GetAddress
********************************************************************************
*
*
*******************************************************************************/
uint8 GetAddress (uint8 pos){     
    side.a.dir = 0xFF;
    side.b.dir = 0xFF;
    side.c.dir = 0xFF;
    side.d.dir = 0xFF;
    uint8 size;
    EnablePin_Write (1u);
    UART_1_PutChar((0x50|pos));
    UART_1_PutChar(0x20);
    UART_1_PutChar(0xFA); 
    CyDelay(4);
    EnablePin_Write (0u);
    size = UART_1_GetRxBufferSize();
    for(uint8 MepRx = 0; MepRx < size; MepRx++){
       MepsanResponse[MepRx] = UART_1_ReadRxData();        
    }
    UART_1_ClearRxBuffer(); 
    CyDelay(300); 
    if(size>=1){              
        return MepsanResponse[0] & 0x0F;
        
    }else{
       return 0xFF;
    }
}


/*******************************************************************************
* Function Name: ReturnStatus
********************************************************************************
*
*
*******************************************************************************/
uint8 Authorize(uint8 address){
    uint8 size, status;    
    for(uint8 x = 0; x < 100; x ++){
        MepsanSendTo[x] = 0x00;
    }
    EnablePin_Write (1u);
    MepsanSendTo[0] = (0x50|address);
    MepsanSendTo[1] = (0x30);
    MepsanSendTo[2] = (0x01);
    MepsanSendTo[3] = (0x01);
    MepsanSendTo[4] = (MEPSAN_AUTHORIZE);
    CRC    = gen_crc16(MepsanSendTo,5);
    crc_hi = (CRC & 0xFF00) >> 8;
    crc_lo = (CRC & 0x00FF) ;
    MepsanSendTo[5] = crc_lo;
    MepsanSendTo[6] = crc_hi;
    MepsanSendTo[7] = 0x03;
    MepsanSendTo[8] = 0xFA;
    for(uint8 x = 0; x <= 9; x++){    
        UART_1_PutChar(MepsanSendTo[x]);
    }
    CyDelay(4);
    EnablePin_Write (0u);
    CyDelay(200);
    return status;
}


/*******************************************************************************
* Function Name: PumpState
********************************************************************************
*
*
*******************************************************************************/

uint8 PumpState(uint8 address)
{ 
    uint8 size;
    EnablePin_Write (1u);
    UART_1_PutChar((0x50|address));
    UART_1_PutChar(0x20);
    UART_1_PutChar(0xFA); 
    CyDelay(4);
    EnablePin_Write (0u);
    size = UART_1_GetRxBufferSize();
    for(uint8 MepRx = 0; MepRx < size; MepRx++){
        MepsanResponse[MepRx] = UART_1_ReadRxData();
    }
    UART_1_ClearRxBuffer();
    CyDelay(300);    
    if(size>=1){              
        return MepsanResponse[1];
    }
    else{
       return 0;
    }
    
}


/*******************************************************************************
* Function Name: PriceUpdate
********************************************************************************
*
*
*******************************************************************************/

uint8 PriceUpdate(uint8 address, uint8 *price)
{ 
    uint8 size;
    EnablePin_Write (1u);
    MepsanSendTo[0] = (0x50|address);
    MepsanSendTo[1] = (0x30);
    MepsanSendTo[2] = (0x05);
    MepsanSendTo[3] = (0x03); //Data bytes in transaction
    MepsanSendTo[4] = (price[0]);
    MepsanSendTo[5] = (price[1]);
    MepsanSendTo[6] = (price[2]);
    CRC    = gen_crc16(MepsanSendTo,7);
    crc_hi = (CRC & 0xFF00) >> 8;
    crc_lo = (CRC & 0x00FF) ;
    MepsanSendTo[7] = crc_lo;
    MepsanSendTo[8] = crc_hi;
    MepsanSendTo[9] = 0x03;
    MepsanSendTo[10] = 0xFA; 
    for(uint8 x = 0; x <= 11; x++){    
        UART_1_PutChar(MepsanSendTo[x]);
    }
    CyDelay(4);
    EnablePin_Write (0u);
    size = UART_1_GetRxBufferSize();
    for(uint8 MepRx = 0; MepRx < size; MepRx++){
        MepsanResponse[MepRx] = UART_1_ReadRxData();   
    }
    UART_1_ClearRxBuffer();
    CyDelay(500);    
    if(size>=1){
        return  MepsanResponse[1];
    }
    else{
       return 0;
    }    
}

/*******************************************************************************
* Function Name: TotalRequest
********************************************************************************
*
*
*******************************************************************************/

void TotalRequest(uint8 address, uint8 type, uint8 nozzle)
{ 
    uint8 size;
    uint8 request;
    request = ((type<<4)&0xF0)|(nozzle);
    EnablePin_Write (1u);
    MepsanSendTo[0] = (0x50|address);
    MepsanSendTo[1] = (0x30);
    MepsanSendTo[2] = (0x65);
    MepsanSendTo[3] = (0x01); //Data bytes in transaction
    MepsanSendTo[4] = (request);
    CRC    = gen_crc16(MepsanSendTo,5);
    crc_hi = (CRC & 0xFF00) >> 8;
    crc_lo = (CRC & 0x00FF) ;
    MepsanSendTo[5] = crc_lo;
    MepsanSendTo[6] = crc_hi;
    MepsanSendTo[7] = 0x03;
    MepsanSendTo[8] = 0xFA; 
    for(uint8 x = 0; x <= 9; x++){    
        UART_1_PutChar(MepsanSendTo[x]);
    }
    CyDelay(4);
    EnablePin_Write (0u);                   
}

/*******************************************************************************
* Function Name: GetTotal
********************************************************************************
*
*
*******************************************************************************/

void GetTotal(void){
//    for(uint8 x = 4; x < MepsanResponse[3] + 4; x++ ){
//    }
}
/*******************************************************************************
* Function Name: GetTotal
********************************************************************************
*
*
*******************************************************************************/

void GetACK(){
    uint8 size;
    size = UART_1_GetRxBufferSize();        
    for(uint8 MepRx = 0; MepRx < size; MepRx++){
        MepsanResponse[MepRx] = UART_1_ReadRxData();        
    }    
    UART_1_ClearRxBuffer();
}

void ProccessResponse(void){
    uint8 size,ValidateA,ValidateB;
    for(uint8 x = 0; x < 50; x++){
        MepsanResponse[x] = UART_1_ReadRxData();   
    }    
    size = UART_1_GetRxBufferSize();        
    for(uint8 MepRx = 0; MepRx < size; MepRx++){        
        MepsanResponse[MepRx] = UART_1_ReadRxData(); 
        ValidateA = (MepsanResponse[1]|0xF0);
        ValidateB =  MepsanResponse[2];
        
    } 
    ValidateA = (MepsanResponse[1]|0xF0);
    ValidateB =  MepsanResponse[2];
    UART_1_ClearRxBuffer();
    
    if(size > 0){
        if(ValidateA == 0xC0 && ValidateB != 0x65){
            GetACK();
        }
        if(ValidateB == 0x65){
            GetTotal();
        }
    }
}
