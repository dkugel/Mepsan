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
#include <project.h>

uint16 PollCounter;
uint8 CurrentState;
uint8 Receipt;
uint8 sale_number[6];
uint8 sale_number_copy[6];
uint8 int_salenumber;
uint8 shift_number[6];
uint8 int_shiftnumber;
uint8 LCDhose;


/*
*********************************************************************************************************
*
*                            Estructura dispensador
*          
*********************************************************************************************************
*/

struct position{
    uint8 dir;                      //Direccion
    uint8 totalsNozzle[4][2][5];   //Totales Manguera: [0-3] // [0]=Volumen [1]=Dinero // [8 ó 12 - 4 ó 5]
    uint8 ppuNozzle[4][3];         //PPU autorizados [Manguera 0-3][Datos]
    uint8 moneySale[4];            //Venta de Dinero vendido
    uint8 volumeSale[4];           //Venta de Volumen vendido
    uint8 ppuSale[14];              //Venta de PPU vendido
    uint8 productSale;              //Venta de Producto vendido
    uint8 states[5];                //Estados a Gbr
    uint8 MepsanStore[50];          //Datos Mepsan por posición
    char8 ProcessedTotals[4][3][10];//Totales en ASCII
    uint8 ProcessedmoneySale[8];            //Venta de Dinero vendido
    uint8 ProcessedvolumeSale[8];           //Venta de Volumen vendido
    uint8 ProcessedPPU[4][6];
    uint8 msn_plate[8];             //placa vehiculo por lado
    uint8 MepRequest;
    uint8 Nozzle;
    uint8 km[7];
    uint8 Copy;
};

struct pump{
   struct position a;
   struct position b;
   struct position c;
   struct position d;
};

struct pump side;       //lado del surtidor

enum MEPSAN_REQUEST
{
    MEPSAN_STATE     = 0x00,
    MEPSAN_AUTHORIZE = 0x06,
    MEPSAN_R_FILLING = 0x04
};

uint8 MepsanResponse[50];
uint8 MepsanSendTo[50];
uint8 PumpAddress[15];
uint16 CRC;
uint8 crc_lo,crc_hi;
uint8 Positions;
uint8 TotalRequestType;
uint8 DecVol;

/*
*********************************************************************************************************
*
*                            Var SCREENS
*          
*********************************************************************************************************
*/
uint8 touch1[20];
/*
*********************************************************************************************************
*
*                            Var GILB.
*          
*********************************************************************************************************
*/

uint8 dirGbr;

enum GILB_STATES
{
    GILB_FAIL       = 0x00,
    GILB_IDLE       = 0x60,
    GILB_CALLING    = 0x70,
    GILB_AUTHORIZED = 0x80,
    GILB_BUSY       = 0x90,
    GILB_PEOT       = 0xA0,
    GILB_FEOT       = 0xB0,
    GILB_STOPPED    = 0xC0,
    GILB_SEND_DATA  = 0xD0, //READY FOR LONG COMM
    GILB_UNKNOWN    = 0xFF  //PUMP DESCONECTED
};