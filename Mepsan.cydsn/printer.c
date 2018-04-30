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

uint8 LINE_FEED = 0x0A;


/***********  Temporal messages ******************/
uint8 msn_EDS[22]       = " ESTACION DE SERVICIO ";
uint8 msn_EDS2[22]      = "    PRUEBAS MEPSAN    ";
uint8 msn_EDS3[22]      = "   Tel: (57)123456    ";
uint8 msn_EDS4[22]      = "   NIT: 900.123.456   ";

/*********** informative messages ****************/

uint8 msn_fecha[13]       = "Fecha      : ";
uint8 msn_hora[13]        = "Hora       : ";
uint8 msn_placa[13]       = "Placa      : ";
uint8 PRN_MILLEAGE[13]    = "Kilometraje: ";
uint8 PRN_TRANSTYPE[13]   = "Tipo       : ";
uint8 msn_pos[13]         = "Posicion   : ";
uint8 msn_product[13]     = "Producto   : ";
uint8 msn_ppu[13]         = "Ppu        : ";
uint8 msn_vol[15]         = "Volumen    : ";
uint8 msn_din[15]         = "Dinero     : ";
uint8 msn_numero[13]      = "Consecutivo: ";
uint8 PRN_BALANCE[13]     = "Saldo      : ";
uint8 PRN_COMPANY[13]     = "Empresa    : ";
uint8 PRN_ACCOUNT[13]     = "Cuenta     : ";
uint8 PRN_SERIAL[13]      = "Serial     : ";
uint8 PRN_ACCOUNTTYPE[13] = "Tipo Cuenta: ";
uint8 PRN_VISITSDAY[13]   = "Visitas Dia: ";
uint8 PRN_VISITSWEEK[13]  = "Visitas Sem: ";
uint8 PRN_VISITSMONTH[13] = "Visitas Mes: ";
uint8 PRN_VOLUMEDAY[13]   = "Volumen Dia: ";
uint8 PRN_VOLUMEWEEK[13]  = "Volumen Sem: ";
uint8 PRN_VOLUMEMONTH[13] = "Volumen Mes: ";
uint8 PRN_MONEYDAY[13]    = "Dinero Dia : ";
uint8 PRN_MONEYWEEK[13]   = "Dinero Sem : ";
uint8 PRN_MONEYMONTH[13]  = "Dinero Mes : ";
uint8 PRN_PRESET[13]      = "Valor Prog.: ";
uint8 PRN_SIGNATURE[13]   = "Firma      : ";
uint8 PRN_ID[13]          = "Cedula     : ";
uint8 PRN_CURRENCY[1]     = "$";
uint8 SEPARATOR[24]       = "************************";




void PrintReceipt(void){
    for(uint8 x = 0; x < 13; x ++){
        UART_3_PutChar(msn_product[x]);
    }
    UART_3_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 13; x ++){
        UART_3_PutChar(msn_pos[x]);
    }
    UART_3_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 13; x ++){
        UART_3_PutChar(msn_placa[x]);
    }
    UART_3_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 24; x ++){
        UART_3_PutChar(SEPARATOR[x]);
    }
    UART_3_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 13; x ++){
        UART_3_PutChar(msn_hora[x]);
    }
    UART_3_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 13; x ++){
        UART_3_PutChar(msn_fecha[x]);
    }
    UART_3_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 24; x ++){
        UART_3_PutChar(SEPARATOR[x]);
    }
    UART_3_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 20; x ++){
        UART_3_PutChar(msn_EDS4[x]);
    }
    UART_3_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 20; x ++){
        UART_3_PutChar(msn_EDS3[x]);
    }
    UART_3_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 20; x ++){
        UART_3_PutChar(msn_EDS2[x]);
    }
    UART_3_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 20; x ++){
        UART_3_PutChar(msn_EDS[x]);
    }
    UART_3_PutChar(LINE_FEED);
    
}






/* [] END OF FILE */
