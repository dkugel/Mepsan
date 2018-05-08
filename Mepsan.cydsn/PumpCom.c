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

/*******************************************************************************
* Function Name: gen_crc16
********************************************************************************
*
*
*******************************************************************************/
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
* Function Name: GetResponse
********************************************************************************
*
*
*******************************************************************************/
void GetResponse(void){
    uint8 size, address;
    size = MepsanResponse[0];          
    CyDelay(10); 
    address = MepsanResponse[1] & 0x00;
    if(address == side.a.dir){
        for(uint8 MepRx = 1; MepRx < size + 1; MepRx++){
            side.a.MepsanStore[MepRx - 1] = MepsanResponse[MepRx];
        }
        if(side.a.MepsanStore[2] == 0x65){
            for(uint8 x = 5; x < (side.a.MepsanStore[3]-1) + 5; x++ ){
                side.a.totalsNozzle[((side.a.MepsanStore[4])&0x0F)-1][TotalRequestType][x-5] = side.a.MepsanStore[x] ;
            }
            
            side.a.ProcessedTotals[((side.a.MepsanStore[4])&0x0F)-1][TotalRequestType][0] = (side.a.totalsNozzle[((side.a.MepsanStore[4])&0x0F)-1][TotalRequestType][0] >> 4) + 0x30;
            side.a.ProcessedTotals[((side.a.MepsanStore[4])&0x0F)-1][TotalRequestType][1] = (side.a.totalsNozzle[((side.a.MepsanStore[4])&0x0F)-1][TotalRequestType][0] & 0x0F) + 0x30;
            side.a.ProcessedTotals[((side.a.MepsanStore[4])&0x0F)-1][TotalRequestType][2] = (side.a.totalsNozzle[((side.a.MepsanStore[4])&0x0F)-1][TotalRequestType][1] >> 4) + 0x30;
            side.a.ProcessedTotals[((side.a.MepsanStore[4])&0x0F)-1][TotalRequestType][3] = (side.a.totalsNozzle[((side.a.MepsanStore[4])&0x0F)-1][TotalRequestType][1] & 0x0F) + 0x30;
            side.a.ProcessedTotals[((side.a.MepsanStore[4])&0x0F)-1][TotalRequestType][4] = (side.a.totalsNozzle[((side.a.MepsanStore[4])&0x0F)-1][TotalRequestType][2] >> 4) + 0x30;
            side.a.ProcessedTotals[((side.a.MepsanStore[4])&0x0F)-1][TotalRequestType][5] = (side.a.totalsNozzle[((side.a.MepsanStore[4])&0x0F)-1][TotalRequestType][2] & 0x0F) + 0x30;
            side.a.ProcessedTotals[((side.a.MepsanStore[4])&0x0F)-1][TotalRequestType][6] = (side.a.totalsNozzle[((side.a.MepsanStore[4])&0x0F)-1][TotalRequestType][3] >> 4) + 0x30;
            side.a.ProcessedTotals[((side.a.MepsanStore[4])&0x0F)-1][TotalRequestType][7] = (side.a.totalsNozzle[((side.a.MepsanStore[4])&0x0F)-1][TotalRequestType][3] & 0x0F) + 0x30;
            side.a.ProcessedTotals[((side.a.MepsanStore[4])&0x0F)-1][TotalRequestType][8] = (side.a.totalsNozzle[((side.a.MepsanStore[4])&0x0F)-1][TotalRequestType][4] >> 4) + 0x30;
            side.a.ProcessedTotals[((side.a.MepsanStore[4])&0x0F)-1][TotalRequestType][9] = (side.a.totalsNozzle[((side.a.MepsanStore[4])&0x0F)-1][TotalRequestType][4] & 0x0F) + 0x30;
            
            EnablePin_Write (1u);
            UART_1_PutChar((0x50|address));
            UART_1_PutChar(0xC0|(side.a.MepsanStore[1]&0x0F));
            UART_1_PutChar(0xFA); 
            CyDelay(4);
            EnablePin_Write (0u);
        }
        if(side.a.MepsanStore[2] == 0x02){
            for(uint8 x = 4; x < 8; x++ ){
                side.a.volumeSale[x-4] = side.a.MepsanStore[x];
                side.a.moneySale[x-4] = side.a.MepsanStore[x+4];
            }
            EnablePin_Write (1u);
            UART_1_PutChar((0x50|address));
            UART_1_PutChar(0xC0|(side.a.MepsanStore[1]&0x0F));
            UART_1_PutChar(0xFA); 
            CyDelay(4);
            EnablePin_Write (0u);
        }
    }
    if(address == side.b.dir){
        for(uint8 MepRx = 1; MepRx < size + 1; MepRx++){
            side.b.MepsanStore[MepRx - 1] = MepsanResponse[MepRx];
        }
    }
    if(address == side.c.dir){
        for(uint8 MepRx = 1; MepRx < size + 1; MepRx++){
            side.c.MepsanStore[MepRx - 1] = MepsanResponse[MepRx];
        }
    }
    if(address == side.d.dir){
        for(uint8 MepRx = 1; MepRx < size + 1; MepRx++){
            side.d.MepsanStore[MepRx - 1] = MepsanResponse[MepRx];
        }
    } 
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
    for(uint8 x = 0; x < 50; x ++){
        MepsanSendTo[x] = 0x00;
    }
    EnablePin_Write (1u);
    MepsanSendTo[0] = (0x50|address);
    MepsanSendTo[1] = (0x30);
    MepsanSendTo[2] = (address + 1);
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
* Function Name: ReturnStatus
********************************************************************************
*
*
*******************************************************************************/
uint8 ReturnStatus(uint8 address){
    uint8 size, status;    
    for(uint8 x = 0; x < 50; x ++){
        MepsanSendTo[x] = 0x00;
    }
    EnablePin_Write (1u);
    MepsanSendTo[0] = (0x50|address);
    MepsanSendTo[1] = (0x30);
    MepsanSendTo[2] = (address + 1);
    MepsanSendTo[3] = (0x01);
    MepsanSendTo[4] = (MEPSAN_STATE);
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
    size = UART_1_GetRxBufferSize();
    for(uint8 MepRx = 0; MepRx < size; MepRx++){
        MepsanResponse[MepRx] = UART_1_ReadRxData();
    }       
    UART_1_ClearRxBuffer();
    CyDelay(300); 
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
    for(uint8 x = 0; x < 50; x ++){
        MepsanResponse[x] = 0x00;
    }
    uint8 size;
    EnablePin_Write (1u);
    UART_1_PutChar((0x50|address));
    UART_1_PutChar(0x20);
    UART_1_PutChar(0xFA); 
    CyDelay(4);
    EnablePin_Write (0u);
    size = UART_1_GetRxBufferSize();   
    MepsanResponse[0] = size;
    for(uint8 MepRx = 1; MepRx < size + 1; MepRx++){
        MepsanResponse[MepRx] = UART_1_ReadRxData();
    }          
    UART_1_ClearRxBuffer();
    if(size > 3)
        GetResponse();
    CyDelay(300); 
    if(address == side.a.dir){
        for(uint8 MepRx = 0; MepRx < size; MepRx++){
            side.a.MepsanStore[MepRx] = MepsanResponse[MepRx];
        }        
    }
    if(address == side.b.dir){
        for(uint8 MepRx = 0; MepRx < size; MepRx++){
            side.b.MepsanStore[MepRx] = MepsanResponse[MepRx];
        }
    }
    if(address == side.c.dir){
        for(uint8 MepRx = 0; MepRx < size; MepRx++){
            side.c.MepsanStore[MepRx] = MepsanResponse[MepRx];
        }
    }
    if(address == side.d.dir){
        for(uint8 MepRx = 0; MepRx < size; MepRx++){
            side.d.MepsanStore[MepRx] = MepsanResponse[MepRx];
        }
    } 
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
    MepsanSendTo[2] = (0x05); //Price change transaction
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
    size = UART_1_GetRxBufferSize();
    for(uint8 MepRx = 0; MepRx < size; MepRx++){
        MepsanResponse[MepRx] = UART_1_ReadRxData();
    }       
    UART_1_ClearRxBuffer();
    CyDelay(200); 
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


void ProccessResponse(uint8 address){
    uint8 ValidateA,ValidateB,x;            
    if(address == side.a.dir){
        ValidateA = (side.a.MepsanStore[1]|0xF0);
        ValidateB =  side.a.MepsanStore[2];
        if(ValidateB == 0x65){
            for(x = 5; x < side.a.MepsanStore[3]+5; x++ ){
                side.a.totalsNozzle[(side.a.MepsanStore[4])&0x0F][(side.a.MepsanStore[4]&0xF0)>>4][x] = side.a.MepsanStore[x] ;
            }
        }
//        for(uint8 MepRx = 0; MepRx < 50; MepRx++){
//            side.a.MepsanStore[MepRx] = MepsanResponse[MepRx];
//        }
    }
//    if(address == side.b.dir){
//        for(uint8 MepRx = 0; MepRx < 50; MepRx++){
//            side.b.MepsanStore[MepRx] = MepsanResponse[MepRx];
//        }
//    }
//    if(address == side.c.dir){
//        for(uint8 MepRx = 0; MepRx < 50; MepRx++){
//            side.c.MepsanStore[MepRx] = MepsanResponse[MepRx];
//        }
//    }
//    if(address == side.d.dir){
//        for(uint8 MepRx = 0; MepRx < 50; MepRx++){
//            side.d.MepsanStore[MepRx] = MepsanResponse[MepRx];
//        }
//    }
    
    
    
    
    
    
    UART_1_ClearRxBuffer();        
    if(ValidateA == 0xC0 && ValidateB != 0x65){
        GetACK();
    }
    if(ValidateB == 0x65){
        GetTotal();
    }    
}