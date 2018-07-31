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
    //CyDelay(10); 
    address = MepsanResponse[1] & 0x0F;
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
            for(int8 i = 0; i < 8; i++){
                if(side.a.ProcessedTotals[((side.a.MepsanStore[4])&0x0F)-1][TotalRequestType][i] != 0x30)
                    break;
                side.a.ProcessedTotals[((side.a.MepsanStore[4])&0x0F)-1][TotalRequestType][i] = 0x00;                
            }
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
            side.a.ProcessedvolumeSale[0] = (side.a.volumeSale[0]  >> 4) + 0x30;
            side.a.ProcessedvolumeSale[1] = (side.a.volumeSale[0]  & 0x0F) + 0x30;
            side.a.ProcessedvolumeSale[2] = (side.a.volumeSale[1]  >> 4) + 0x30;
            side.a.ProcessedvolumeSale[3] = (side.a.volumeSale[1]  & 0x0F) + 0x30;
            side.a.ProcessedvolumeSale[4] = (side.a.volumeSale[2]  >> 4) + 0x30;
            side.a.ProcessedvolumeSale[5] = (side.a.volumeSale[2]  & 0x0F) + 0x30;
            side.a.ProcessedvolumeSale[6] = (side.a.volumeSale[3]  >> 4) + 0x30;
            side.a.ProcessedvolumeSale[7] = (side.a.volumeSale[3]  & 0x0F) + 0x30;
            for(int8 i = 0; i < DecVol+1; i++){
                if(side.a.ProcessedvolumeSale[i] != 0x30 )
                    break;
                side.a.ProcessedvolumeSale[i] = 0x00;                
            }
            side.a.ProcessedmoneySale[0] = (side.a.moneySale[0]  >> 4) + 0x30;
            side.a.ProcessedmoneySale[1] = (side.a.moneySale[0]  & 0x0F) + 0x30;
            side.a.ProcessedmoneySale[2] = (side.a.moneySale[1]  >> 4) + 0x30;
            side.a.ProcessedmoneySale[3] = (side.a.moneySale[1]  & 0x0F) + 0x30;
            side.a.ProcessedmoneySale[4] = (side.a.moneySale[2]  >> 4) + 0x30;
            side.a.ProcessedmoneySale[5] = (side.a.moneySale[2]  & 0x0F) + 0x30;
            side.a.ProcessedmoneySale[6] = (side.a.moneySale[3]  >> 4) + 0x30;
            side.a.ProcessedmoneySale[7] = (side.a.moneySale[3]  & 0x0F) + 0x30;
            
            for(int8 i = 0; i < 8; i++){
                if(side.a.ProcessedmoneySale[i] != 0x30)
                    break;
                side.a.ProcessedmoneySale[i] = 0x00;                
            }
            EnablePin_Write (0u);
        }
    }
    if(address == side.b.dir){
        for(uint8 MepRx = 1; MepRx < size + 1; MepRx++){
            side.b.MepsanStore[MepRx - 1] = MepsanResponse[MepRx];
        }
        if(side.b.MepsanStore[2] == 0x65){
            for(uint8 x = 5; x < (side.b.MepsanStore[3]-1) + 5; x++ ){
                side.b.totalsNozzle[((side.b.MepsanStore[4])&0x0F)-1][TotalRequestType][x-5] = side.b.MepsanStore[x] ;
            }
            
            side.b.ProcessedTotals[((side.b.MepsanStore[4])&0x0F)-1][TotalRequestType][0] = (side.b.totalsNozzle[((side.b.MepsanStore[4])&0x0F)-1][TotalRequestType][0] >> 4) + 0x30;
            side.b.ProcessedTotals[((side.b.MepsanStore[4])&0x0F)-1][TotalRequestType][1] = (side.b.totalsNozzle[((side.b.MepsanStore[4])&0x0F)-1][TotalRequestType][0] & 0x0F) + 0x30;
            side.b.ProcessedTotals[((side.b.MepsanStore[4])&0x0F)-1][TotalRequestType][2] = (side.b.totalsNozzle[((side.b.MepsanStore[4])&0x0F)-1][TotalRequestType][1] >> 4) + 0x30;
            side.b.ProcessedTotals[((side.b.MepsanStore[4])&0x0F)-1][TotalRequestType][3] = (side.b.totalsNozzle[((side.b.MepsanStore[4])&0x0F)-1][TotalRequestType][1] & 0x0F) + 0x30;
            side.b.ProcessedTotals[((side.b.MepsanStore[4])&0x0F)-1][TotalRequestType][4] = (side.b.totalsNozzle[((side.b.MepsanStore[4])&0x0F)-1][TotalRequestType][2] >> 4) + 0x30;
            side.b.ProcessedTotals[((side.b.MepsanStore[4])&0x0F)-1][TotalRequestType][5] = (side.b.totalsNozzle[((side.b.MepsanStore[4])&0x0F)-1][TotalRequestType][2] & 0x0F) + 0x30;
            side.b.ProcessedTotals[((side.b.MepsanStore[4])&0x0F)-1][TotalRequestType][6] = (side.b.totalsNozzle[((side.b.MepsanStore[4])&0x0F)-1][TotalRequestType][3] >> 4) + 0x30;
            side.b.ProcessedTotals[((side.b.MepsanStore[4])&0x0F)-1][TotalRequestType][7] = (side.b.totalsNozzle[((side.b.MepsanStore[4])&0x0F)-1][TotalRequestType][3] & 0x0F) + 0x30;
            side.b.ProcessedTotals[((side.b.MepsanStore[4])&0x0F)-1][TotalRequestType][8] = (side.b.totalsNozzle[((side.b.MepsanStore[4])&0x0F)-1][TotalRequestType][4] >> 4) + 0x30;
            side.b.ProcessedTotals[((side.b.MepsanStore[4])&0x0F)-1][TotalRequestType][9] = (side.b.totalsNozzle[((side.b.MepsanStore[4])&0x0F)-1][TotalRequestType][4] & 0x0F) + 0x30;
            for(int8 i = 0; i < 8; i++){
                if(side.b.ProcessedTotals[((side.b.MepsanStore[4])&0x0F)-1][TotalRequestType][i] != 0x30)
                    break;
                side.b.ProcessedTotals[((side.b.MepsanStore[4])&0x0F)-1][TotalRequestType][i] = 0x00;                
            }
            EnablePin_Write (1u);
            UART_1_PutChar((0x50|address));
            UART_1_PutChar(0xC0|(side.b.MepsanStore[1]&0x0F));
            UART_1_PutChar(0xFA); 
            CyDelay(4);
            EnablePin_Write (0u);
        }
        if(side.b.MepsanStore[2] == 0x02){
            for(uint8 x = 4; x < 8; x++ ){
                side.b.volumeSale[x-4] = side.b.MepsanStore[x];
                side.b.moneySale[x-4] = side.b.MepsanStore[x+4];
            }
            side.b.ProcessedvolumeSale[0] = (side.b.volumeSale[0]  >> 4) + 0x30;
            side.b.ProcessedvolumeSale[1] = (side.b.volumeSale[0]  & 0x0F) + 0x30;
            side.b.ProcessedvolumeSale[2] = (side.b.volumeSale[1]  >> 4) + 0x30;
            side.b.ProcessedvolumeSale[3] = (side.b.volumeSale[1]  & 0x0F) + 0x30;
            side.b.ProcessedvolumeSale[4] = (side.b.volumeSale[2]  >> 4) + 0x30;
            side.b.ProcessedvolumeSale[5] = (side.b.volumeSale[2]  & 0x0F) + 0x30;
            side.b.ProcessedvolumeSale[6] = (side.b.volumeSale[3]  >> 4) + 0x30;
            side.b.ProcessedvolumeSale[7] = (side.b.volumeSale[3]  & 0x0F) + 0x30;
            for(int8 i = 0; i < DecVol+1; i++){
                if(side.b.ProcessedvolumeSale[i] != 0x30 )
                    break;
                side.b.ProcessedvolumeSale[i] = 0x00;                
            }
            side.b.ProcessedmoneySale[0] = (side.b.moneySale[0]  >> 4) + 0x30;
            side.b.ProcessedmoneySale[1] = (side.b.moneySale[0]  & 0x0F) + 0x30;
            side.b.ProcessedmoneySale[2] = (side.b.moneySale[1]  >> 4) + 0x30;
            side.b.ProcessedmoneySale[3] = (side.b.moneySale[1]  & 0x0F) + 0x30;
            side.b.ProcessedmoneySale[4] = (side.b.moneySale[2]  >> 4) + 0x30;
            side.b.ProcessedmoneySale[5] = (side.b.moneySale[2]  & 0x0F) + 0x30;
            side.b.ProcessedmoneySale[6] = (side.b.moneySale[3]  >> 4) + 0x30;
            side.b.ProcessedmoneySale[7] = (side.b.moneySale[3]  & 0x0F) + 0x30;
            for(int8 i = 0; i < 8; i++){
                if(side.b.ProcessedmoneySale[i] != 0x30)
                    break;
                side.b.ProcessedmoneySale[i] = 0x00;                
            }
            EnablePin_Write (0u);
        }
    }
    if(address == side.c.dir){
        for(uint8 MepRx = 1; MepRx < size + 1; MepRx++){
            side.c.MepsanStore[MepRx - 1] = MepsanResponse[MepRx];
        }
        if(side.c.MepsanStore[2] == 0x65){
            for(uint8 x = 5; x < (side.c.MepsanStore[3]-1) + 5; x++ ){
                side.c.totalsNozzle[((side.c.MepsanStore[4])&0x0F)-1][TotalRequestType][x-5] = side.c.MepsanStore[x] ;
            }
            
            side.c.ProcessedTotals[((side.c.MepsanStore[4])&0x0F)-1][TotalRequestType][0] = (side.c.totalsNozzle[((side.c.MepsanStore[4])&0x0F)-1][TotalRequestType][0] >> 4) + 0x30;
            side.c.ProcessedTotals[((side.c.MepsanStore[4])&0x0F)-1][TotalRequestType][1] = (side.c.totalsNozzle[((side.c.MepsanStore[4])&0x0F)-1][TotalRequestType][0] & 0x0F) + 0x30;
            side.c.ProcessedTotals[((side.c.MepsanStore[4])&0x0F)-1][TotalRequestType][2] = (side.c.totalsNozzle[((side.c.MepsanStore[4])&0x0F)-1][TotalRequestType][1] >> 4) + 0x30;
            side.c.ProcessedTotals[((side.c.MepsanStore[4])&0x0F)-1][TotalRequestType][3] = (side.c.totalsNozzle[((side.c.MepsanStore[4])&0x0F)-1][TotalRequestType][1] & 0x0F) + 0x30;
            side.c.ProcessedTotals[((side.c.MepsanStore[4])&0x0F)-1][TotalRequestType][4] = (side.c.totalsNozzle[((side.c.MepsanStore[4])&0x0F)-1][TotalRequestType][2] >> 4) + 0x30;
            side.c.ProcessedTotals[((side.c.MepsanStore[4])&0x0F)-1][TotalRequestType][5] = (side.c.totalsNozzle[((side.c.MepsanStore[4])&0x0F)-1][TotalRequestType][2] & 0x0F) + 0x30;
            side.c.ProcessedTotals[((side.c.MepsanStore[4])&0x0F)-1][TotalRequestType][6] = (side.c.totalsNozzle[((side.c.MepsanStore[4])&0x0F)-1][TotalRequestType][3] >> 4) + 0x30;
            side.c.ProcessedTotals[((side.c.MepsanStore[4])&0x0F)-1][TotalRequestType][7] = (side.c.totalsNozzle[((side.c.MepsanStore[4])&0x0F)-1][TotalRequestType][3] & 0x0F) + 0x30;
            side.c.ProcessedTotals[((side.c.MepsanStore[4])&0x0F)-1][TotalRequestType][8] = (side.c.totalsNozzle[((side.c.MepsanStore[4])&0x0F)-1][TotalRequestType][4] >> 4) + 0x30;
            side.c.ProcessedTotals[((side.c.MepsanStore[4])&0x0F)-1][TotalRequestType][9] = (side.c.totalsNozzle[((side.c.MepsanStore[4])&0x0F)-1][TotalRequestType][4] & 0x0F) + 0x30;
            
            EnablePin_Write (1u);
            UART_1_PutChar((0x50|address));
            UART_1_PutChar(0xC0|(side.c.MepsanStore[1]&0x0F));
            UART_1_PutChar(0xFA); 
            CyDelay(4);
            EnablePin_Write (0u);
        }
        if(side.c.MepsanStore[2] == 0x02){
            for(uint8 x = 4; x < 8; x++ ){
                side.c.volumeSale[x-4] = side.c.MepsanStore[x];
                side.c.moneySale[x-4] = side.c.MepsanStore[x+4];
            }
            side.c.ProcessedvolumeSale[0] = (side.c.volumeSale[0]  >> 4) + 0x30;
            side.c.ProcessedvolumeSale[1] = (side.c.volumeSale[0]  & 0x0F) + 0x30;
            side.c.ProcessedvolumeSale[2] = (side.c.volumeSale[1]  >> 4) + 0x30;
            side.c.ProcessedvolumeSale[3] = (side.c.volumeSale[1]  & 0x0F) + 0x30;
            side.c.ProcessedvolumeSale[4] = (side.c.volumeSale[2]  >> 4) + 0x30;
            side.c.ProcessedvolumeSale[5] = (side.c.volumeSale[2]  & 0x0F) + 0x30;
            side.c.ProcessedvolumeSale[6] = (side.c.volumeSale[3]  >> 4) + 0x30;
            side.c.ProcessedvolumeSale[7] = (side.c.volumeSale[3]  & 0x0F) + 0x30;
            
            side.c.ProcessedmoneySale[0] = (side.c.moneySale[0]  >> 4) + 0x30;
            side.c.ProcessedmoneySale[1] = (side.c.moneySale[0]  & 0x0F) + 0x30;
            side.c.ProcessedmoneySale[2] = (side.c.moneySale[1]  >> 4) + 0x30;
            side.c.ProcessedmoneySale[3] = (side.c.moneySale[1]  & 0x0F) + 0x30;
            side.c.ProcessedmoneySale[4] = (side.c.moneySale[2]  >> 4) + 0x30;
            side.c.ProcessedmoneySale[5] = (side.c.moneySale[2]  & 0x0F) + 0x30;
            side.c.ProcessedmoneySale[6] = (side.c.moneySale[3]  >> 4) + 0x30;
            side.c.ProcessedmoneySale[7] = (side.c.moneySale[3]  & 0x0F) + 0x30;
            
            EnablePin_Write (0u);
        }
    }
    if(address == side.d.dir){
        for(uint8 MepRx = 1; MepRx < size + 1; MepRx++){
            side.d.MepsanStore[MepRx - 1] = MepsanResponse[MepRx];
        }
        if(side.d.MepsanStore[2] == 0x65){
            for(uint8 x = 5; x < (side.d.MepsanStore[3]-1) + 5; x++ ){
                side.d.totalsNozzle[((side.d.MepsanStore[4])&0x0F)-1][TotalRequestType][x-5] = side.d.MepsanStore[x] ;
            }
            
            side.d.ProcessedTotals[((side.d.MepsanStore[4])&0x0F)-1][TotalRequestType][0] = (side.d.totalsNozzle[((side.d.MepsanStore[4])&0x0F)-1][TotalRequestType][0] >> 4) + 0x30;
            side.d.ProcessedTotals[((side.d.MepsanStore[4])&0x0F)-1][TotalRequestType][1] = (side.d.totalsNozzle[((side.d.MepsanStore[4])&0x0F)-1][TotalRequestType][0] & 0x0F) + 0x30;
            side.d.ProcessedTotals[((side.d.MepsanStore[4])&0x0F)-1][TotalRequestType][2] = (side.d.totalsNozzle[((side.d.MepsanStore[4])&0x0F)-1][TotalRequestType][1] >> 4) + 0x30;
            side.d.ProcessedTotals[((side.d.MepsanStore[4])&0x0F)-1][TotalRequestType][3] = (side.d.totalsNozzle[((side.d.MepsanStore[4])&0x0F)-1][TotalRequestType][1] & 0x0F) + 0x30;
            side.d.ProcessedTotals[((side.d.MepsanStore[4])&0x0F)-1][TotalRequestType][4] = (side.d.totalsNozzle[((side.d.MepsanStore[4])&0x0F)-1][TotalRequestType][2] >> 4) + 0x30;
            side.d.ProcessedTotals[((side.d.MepsanStore[4])&0x0F)-1][TotalRequestType][5] = (side.d.totalsNozzle[((side.d.MepsanStore[4])&0x0F)-1][TotalRequestType][2] & 0x0F) + 0x30;
            side.d.ProcessedTotals[((side.d.MepsanStore[4])&0x0F)-1][TotalRequestType][6] = (side.d.totalsNozzle[((side.d.MepsanStore[4])&0x0F)-1][TotalRequestType][3] >> 4) + 0x30;
            side.d.ProcessedTotals[((side.d.MepsanStore[4])&0x0F)-1][TotalRequestType][7] = (side.d.totalsNozzle[((side.d.MepsanStore[4])&0x0F)-1][TotalRequestType][3] & 0x0F) + 0x30;
            side.d.ProcessedTotals[((side.d.MepsanStore[4])&0x0F)-1][TotalRequestType][8] = (side.d.totalsNozzle[((side.d.MepsanStore[4])&0x0F)-1][TotalRequestType][4] >> 4) + 0x30;
            side.d.ProcessedTotals[((side.d.MepsanStore[4])&0x0F)-1][TotalRequestType][9] = (side.d.totalsNozzle[((side.d.MepsanStore[4])&0x0F)-1][TotalRequestType][4] & 0x0F) + 0x30;
            
            EnablePin_Write (1u);
            UART_1_PutChar((0x50|address));
            UART_1_PutChar(0xC0|(side.d.MepsanStore[1]&0x0F));
            UART_1_PutChar(0xFA); 
            CyDelay(4);
            EnablePin_Write (0u);
        }
        if(side.d.MepsanStore[2] == 0x02){
            for(uint8 x = 4; x < 8; x++ ){
                side.d.volumeSale[x-4] = side.d.MepsanStore[x];
                side.d.moneySale[x-4] = side.d.MepsanStore[x+4];
            }
            side.d.ProcessedvolumeSale[0] = (side.d.volumeSale[0]  >> 4) + 0x30;
            side.d.ProcessedvolumeSale[1] = (side.d.volumeSale[0]  & 0x0F) + 0x30;
            side.d.ProcessedvolumeSale[2] = (side.d.volumeSale[1]  >> 4) + 0x30;
            side.d.ProcessedvolumeSale[3] = (side.d.volumeSale[1]  & 0x0F) + 0x30;
            side.d.ProcessedvolumeSale[4] = (side.d.volumeSale[2]  >> 4) + 0x30;
            side.d.ProcessedvolumeSale[5] = (side.d.volumeSale[2]  & 0x0F) + 0x30;
            side.d.ProcessedvolumeSale[6] = (side.d.volumeSale[3]  >> 4) + 0x30;
            side.d.ProcessedvolumeSale[7] = (side.d.volumeSale[3]  & 0x0F) + 0x30;
            
            side.d.ProcessedmoneySale[0] = (side.d.moneySale[0]  >> 4) + 0x30;
            side.d.ProcessedmoneySale[1] = (side.d.moneySale[0]  & 0x0F) + 0x30;
            side.d.ProcessedmoneySale[2] = (side.d.moneySale[1]  >> 4) + 0x30;
            side.d.ProcessedmoneySale[3] = (side.d.moneySale[1]  & 0x0F) + 0x30;
            side.d.ProcessedmoneySale[4] = (side.d.moneySale[2]  >> 4) + 0x30;
            side.d.ProcessedmoneySale[5] = (side.d.moneySale[2]  & 0x0F) + 0x30;
            side.d.ProcessedmoneySale[6] = (side.d.moneySale[3]  >> 4) + 0x30;
            side.d.ProcessedmoneySale[7] = (side.d.moneySale[3]  & 0x0F) + 0x30;
            
            EnablePin_Write (0u);
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
    CyDelay(3);
    EnablePin_Write (0u);
    size = UART_1_GetRxBufferSize();       
    for(uint8 MepRx = 1; MepRx < size + 1; MepRx++){
        MepsanResponse[MepRx] = UART_1_ReadRxData();
    } 
    MepsanResponse[0] = size;
    UART_1_ClearRxBuffer();
    if(size > 3){
        GetResponse();
        CyDelay(100);
    }else{
        CyDelay(100); 
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
    CyDelay(100); 
    
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
    CyDelay(100); 
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

