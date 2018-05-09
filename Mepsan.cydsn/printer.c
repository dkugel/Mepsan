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

uint8 LINE_FEED = 0x0A;
char VolSimbol[1] = "G";


/***********  Temporal messages ******************/
uint8 msn_EDS[22]       = " ESTACION DE SERVICIO ";
uint8 msn_EDS2[22]      = "    PRUEBAS MEPSAN    ";
uint8 msn_EDS3[22]      = "   Tel: (57)123456    ";
uint8 msn_EDS4[22]      = "   NIT: 900.123.456   ";
uint8 msn_plate[7]      = "123-ABC";
uint8 msn_footer[22]    = " SISPETROL INGENIERIA ";
uint8 msn_footer2[22]   = "  SOLUCIONES PARA EDS ";
uint8 msn_diesel[8]     = "Diesel  ";
uint8 msn_gasoline[9]   = "Corriente";

/*********** informative messages ****************/

uint8 msn_fecha[13]       = "Fecha      : ";
uint8 msn_hora[13]        = "Hora       : ";
uint8 msn_placa[13]       = "Placa      : ";
uint8 PRN_MILLEAGE[13]    = "Kilometraje: ";
uint8 PRN_TRANSTYPE[13]   = "Tipo       : ";
uint8 msn_pos[13]         = "Posicion   : ";
uint8 msn_product[13]     = "Producto   : ";
uint8 msn_ppu[13]         = "Ppu        : ";
uint8 msn_vol[13]         = "Volumen    : ";
uint8 msn_voltot[13]      = "Tot.Vol    : ";
uint8 msn_din[13]         = "Dinero     : ";
uint8 msn_cierre[13]      = "Cierre No  : ";
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




void PrintReceipt(uint8 address){
    if(address == side.a.dir){
        for(uint8 x = 0; x < 22; x ++){
            UART_3_PutChar(msn_footer2[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 22; x ++){
            UART_3_PutChar(msn_footer[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 24; x ++){
            UART_3_PutChar(SEPARATOR[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_ppu[x]);
        }
        for(uint8 x = 1; x < 6; x ++){
            UART_3_PutChar(side.a.ProcessedPPU[0][x]);
        }
        UART_3_PutChar(LINE_FEED);    
        for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_din[x]);
        }     
        UART_3_PutChar(PRN_CURRENCY[0]);        
        for(uint8 x = 0; x < 8; x ++){
            UART_3_PutChar(side.a.ProcessedmoneySale[x]);
        } 
        UART_3_PutChar(LINE_FEED);    
        for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_vol[x]);
        }
        for(uint8 x = 0; x < 8; x ++){
            if((8-x) == DecVol)
                UART_3_PutChar('.');
            UART_3_PutChar(side.a.ProcessedvolumeSale[x]);        
        }            
        UART_3_PutChar(VolSimbol[0]);           
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_product[x]);
        }
        for(uint8 x = 0; x < 8; x ++){
            UART_3_PutChar(msn_diesel[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_pos[x]);
        }
        UART_3_PutChar((side.a.dir)+ 0x31);
        UART_3_PutChar(LINE_FEED);    
        for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_placa[x]);
        }
        for(uint8 x = 0; x < 7; x ++){
            UART_3_PutChar(msn_plate[x]);
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
        for(uint8 x = 0; x < 22; x ++){
            UART_3_PutChar(msn_EDS4[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 22; x ++){
            UART_3_PutChar(msn_EDS3[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 22; x ++){
            UART_3_PutChar(msn_EDS2[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 22; x ++){
            UART_3_PutChar(msn_EDS[x]);
        }
        UART_3_PutChar(LINE_FEED);
    }
    if(address == side.b.dir){
        for(uint8 x = 0; x < 22; x ++){
        UART_3_PutChar(msn_footer2[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 22; x ++){
            UART_3_PutChar(msn_footer[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 24; x ++){
            UART_3_PutChar(SEPARATOR[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_ppu[x]);
        }
        for(uint8 x = 1; x < 6; x ++){
            UART_3_PutChar(side.b.ProcessedPPU[0][x]);
        }
        UART_3_PutChar(LINE_FEED);    
        for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_din[x]);
        }     
        UART_3_PutChar(PRN_CURRENCY[0]);        
        for(uint8 x = 0; x < 8; x ++){
            UART_3_PutChar(side.b.ProcessedmoneySale[x]);
        } 
        UART_3_PutChar(LINE_FEED);    
        for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_vol[x]);
        }
        for(uint8 x = 0; x < 8; x ++){
            if((8-x) == DecVol)
                UART_3_PutChar('.');
            UART_3_PutChar(side.b.ProcessedvolumeSale[x]);        
        }            
        UART_3_PutChar(VolSimbol[0]);           
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_product[x]);
        }
        for(uint8 x = 0; x < 9; x ++){
            UART_3_PutChar(msn_gasoline[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_pos[x]);
        }
        UART_3_PutChar((side.b.dir)+ 0x31);
        UART_3_PutChar(LINE_FEED);    
        for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_placa[x]);
        }
        for(uint8 x = 0; x < 7; x ++){
            UART_3_PutChar(msn_plate[x]);
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
        for(uint8 x = 0; x < 22; x ++){
            UART_3_PutChar(msn_EDS4[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 22; x ++){
            UART_3_PutChar(msn_EDS3[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 22; x ++){
            UART_3_PutChar(msn_EDS2[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 22; x ++){
            UART_3_PutChar(msn_EDS[x]);
        }
        UART_3_PutChar(LINE_FEED);
    }
    if(address == side.c.dir){
        for(uint8 x = 0; x < 22; x ++){
            UART_3_PutChar(msn_footer2[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 22; x ++){
            UART_3_PutChar(msn_footer[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 24; x ++){
            UART_3_PutChar(SEPARATOR[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_ppu[x]);
        }
        for(uint8 x = 1; x < 6; x ++){
            UART_3_PutChar(side.c.ProcessedPPU[0][x]);
        }
        UART_3_PutChar(LINE_FEED);    
        for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_din[x]);
        }     
        UART_3_PutChar(PRN_CURRENCY[0]);        
        for(uint8 x = 0; x < 8; x ++){
            UART_3_PutChar(side.c.ProcessedmoneySale[x]);
        } 
        UART_3_PutChar(LINE_FEED);    
        for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_vol[x]);
        }
        for(uint8 x = 0; x < 8; x ++){
            if((8-x) == DecVol)
                UART_3_PutChar('.');
            UART_3_PutChar(side.c.ProcessedvolumeSale[x]);        
        }            
        UART_3_PutChar(VolSimbol[0]);           
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_product[x]);
        }
        for(uint8 x = 0; x < 8; x ++){
            UART_3_PutChar(msn_diesel[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_pos[x]);
        }
        UART_3_PutChar((side.c.dir)+ 0x31);
        UART_3_PutChar(LINE_FEED);    
        for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_placa[x]);
        }
        for(uint8 x = 0; x < 7; x ++){
            UART_3_PutChar(msn_plate[x]);
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
        for(uint8 x = 0; x < 22; x ++){
            UART_3_PutChar(msn_EDS4[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 22; x ++){
            UART_3_PutChar(msn_EDS3[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 22; x ++){
            UART_3_PutChar(msn_EDS2[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 22; x ++){
            UART_3_PutChar(msn_EDS[x]);
        }
        UART_3_PutChar(LINE_FEED);
    }
    if(address == side.d.dir){
        for(uint8 x = 0; x < 22; x ++){
        UART_3_PutChar(msn_footer2[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 22; x ++){
            UART_3_PutChar(msn_footer[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 24; x ++){
            UART_3_PutChar(SEPARATOR[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_ppu[x]);
        }
        for(uint8 x = 1; x < 6; x ++){
            UART_3_PutChar(side.d.ProcessedPPU[0][x]);
        }
        UART_3_PutChar(LINE_FEED);    
        for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_din[x]);
        }     
        UART_3_PutChar(PRN_CURRENCY[0]);        
        for(uint8 x = 0; x < 8; x ++){
            UART_3_PutChar(side.d.ProcessedmoneySale[x]);
        } 
        UART_3_PutChar(LINE_FEED);    
        for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_vol[x]);
        }
        for(uint8 x = 0; x < 8; x ++){
            if((8-x) == DecVol)
                UART_3_PutChar('.');
            UART_3_PutChar(side.d.ProcessedvolumeSale[x]);        
        }            
        UART_3_PutChar(VolSimbol[0]);           
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_product[x]);
        }
        for(uint8 x = 0; x < 9; x ++){
            UART_3_PutChar(msn_gasoline[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_pos[x]);
        }
        UART_3_PutChar((side.d.dir)+ 0x31);
        UART_3_PutChar(LINE_FEED);    
        for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_placa[x]);
        }
        for(uint8 x = 0; x < 7; x ++){
            UART_3_PutChar(msn_plate[x]);
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
        for(uint8 x = 0; x < 22; x ++){
            UART_3_PutChar(msn_EDS4[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 22; x ++){
            UART_3_PutChar(msn_EDS3[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 22; x ++){
            UART_3_PutChar(msn_EDS2[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 22; x ++){
            UART_3_PutChar(msn_EDS[x]);
        }
        UART_3_PutChar(LINE_FEED);
    }
}

void PrintShift(uint8 address){
    
    if(address == side.a.dir){
        for(uint8 x = 0; x < 22; x ++){
            UART_3_PutChar(msn_footer2[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 22; x ++){
            UART_3_PutChar(msn_footer[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 24; x ++){
            UART_3_PutChar(SEPARATOR[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_voltot[x]);
        }
        for(uint8 x = 2; x < 10; x ++){ //recorta los dos primeros datos de totales
            if((10-x) == DecVol)
                UART_3_PutChar('.');
            UART_3_PutChar(side.a.ProcessedTotals[0][0][x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_pos[x]);
        }
        UART_3_PutChar((side.a.dir)+ 0x31);
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_cierre[x]);
        }        
        ///////////////////////////////////////////////
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 24; x ++){
            UART_3_PutChar(SEPARATOR[x]);
        }    
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_hora[x]);
        }                                 //Hora y Fecha
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_fecha[x]);
        }
        ///////////////////////////////////////////////        
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 24; x ++){
            UART_3_PutChar(SEPARATOR[x]); //Separador de sección
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 22; x ++){
            UART_3_PutChar(msn_EDS4[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 22; x ++){
            UART_3_PutChar(msn_EDS3[x]);
        }                                 // Encabezado de estación
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 22; x ++){
            UART_3_PutChar(msn_EDS2[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 22; x ++){
            UART_3_PutChar(msn_EDS[x]);
        }
        UART_3_PutChar(LINE_FEED);   
    }
    if(address == side.b.dir){
        for(uint8 x = 0; x < 22; x ++){
            UART_3_PutChar(msn_footer2[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 22; x ++){
            UART_3_PutChar(msn_footer[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 24; x ++){
            UART_3_PutChar(SEPARATOR[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_voltot[x]);
        }
        for(uint8 x = 2; x < 10; x ++){ //recorta los dos primeros datos de totales
            if((10-x) == DecVol)
                UART_3_PutChar('.');
            UART_3_PutChar(side.b.ProcessedTotals[0][0][x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_pos[x]);
        }
        UART_3_PutChar((side.b.dir)+ 0x31);
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_cierre[x]);
        }        
        ///////////////////////////////////////////////
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 24; x ++){
            UART_3_PutChar(SEPARATOR[x]);
        }    
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_hora[x]);
        }                                 //Hora y Fecha
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_fecha[x]);
        }
        ///////////////////////////////////////////////        
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 24; x ++){
            UART_3_PutChar(SEPARATOR[x]); //Separador de sección
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 22; x ++){
            UART_3_PutChar(msn_EDS4[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 22; x ++){
            UART_3_PutChar(msn_EDS3[x]);
        }                                 // Encabezado de estación
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 22; x ++){
            UART_3_PutChar(msn_EDS2[x]);
        }
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 22; x ++){
            UART_3_PutChar(msn_EDS[x]);
        }
        UART_3_PutChar(LINE_FEED);   
    }
}




/* [] END OF FILE */
