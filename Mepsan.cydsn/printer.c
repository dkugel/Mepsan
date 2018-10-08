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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8 LINE_FEED = 0x0A;
char VolSimbol[1] = "G";


/***********  Temporal messages ******************/
uint8 msn_EDS[22]       = "  EDS LA PROVIDENCIA  ";
uint8 msn_EDS2[22]      = "    OMAR MALAGON      ";
uint8 msn_EDS3[22]      = "   Tel: 3208669161    ";
uint8 msn_EDS4[22]      = "    NIT: 4172693-6    ";
uint8 msn_EDS5[22]      = " Km 2 via Sutamarchan ";
uint8 msn_EDS6[22]      = "   TINJACA - BOYACA   ";
uint8 msn_footer[22]    = "GRACIAS POR SU COMPRA ";
uint8 msn_footer2[22]   = "    VUELVA PRONTO     ";
uint8 msn_footer3[22]   = "Firma:                ";
uint8 msn_footer4[22]   = "______________________";
uint8 msn_copy[22]      = "        COPIA         ";
uint8 msn_diesel[9]     = "Diesel   ";
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
uint8 msn_din[13]         = "Importe    : ";
uint8 msn_cierre[13]      = "Cierre No  : ";
uint8 msn_venta[13]       = "Venta No   : ";
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
uint8 msn_lecact[16]      ="Volumen actual: ";
uint8 msn_lecaan[16]      ="Vol. anterior:  ";
uint8 msn_dif[18]         ="Vol. dispensado:  ";
uint8 SEPARATOR[24]       = "************************";
uint8 ASK_TIME[]={0x5A, 0xA5, 0x03, 0x81, 0x20, 0x07};



void PrintReceipt(uint8 address){
    int8 screen_size;    
    sale_number[5]=(int_salenumber/10000)+48;
	sale_number[4]=((int_salenumber%10000)/1000)+48;
	sale_number[3]=(((int_salenumber%10000)%1000)/100)+48;
	sale_number[2]=((((int_salenumber%10000)%1000)%100)/10)+48;	
	sale_number[1]=((((int_salenumber%10000)%1000)%100)%10)+48;
    for(uint8 i = 1; i<6; i++){
        EEPROM_1_WriteByte(sale_number[i],i);
    }
    while(screen_size == 0){
        for(uint8 LCDRx = 0; LCDRx < 6; LCDRx++){
            screen_PutChar(ASK_TIME[LCDRx]);
        }
        CyDelay(5);
        screen_size = screen_GetRxBufferSize();
    }
    if(screen_size >= 13 ){
        for(uint8 LCDRx = 0; LCDRx < screen_size; LCDRx++){
            touch1[LCDRx] = screen_ReadRxData();
        }
        screen_size = 0;
    }
	for(uint8 x = 0; x < 22; x ++){
		PRINTER_A_PutChar(msn_EDS[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 22; x ++){
		PRINTER_A_PutChar(msn_EDS2[x]);
	}
	PRINTER_A_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 22; x ++){
		PRINTER_A_PutChar(msn_EDS3[x]);
	}
	PRINTER_A_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 22; x ++){
		PRINTER_A_PutChar(msn_EDS4[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 22; x ++){
		PRINTER_A_PutChar(msn_EDS5[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 22; x ++){
		PRINTER_A_PutChar(msn_EDS6[x]);
	}    
    PRINTER_A_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 24; x ++){
		PRINTER_A_PutChar(SEPARATOR[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
    /********** FECHA ***************/
	for(uint8 x = 0; x < 13; x ++){
		PRINTER_A_PutChar(msn_fecha[x]);
	}
    PRINTER_A_PutChar(((touch1[8] >> 4) + 0x30)) ;
	PRINTER_A_PutChar(((touch1[8] & 0x0F) + 0x30));
    PRINTER_A_PutChar('/');
    PRINTER_A_PutChar(((touch1[7] >> 4) + 0x30)) ;
    PRINTER_A_PutChar(((touch1[7] & 0x0F) + 0x30));
    PRINTER_A_PutChar('/');    
    PRINTER_A_PutChar(((touch1[6] >> 4) + 0x30));
    PRINTER_A_PutChar(((touch1[6] & 0x0F) + 0x30));
	PRINTER_A_PutChar(LINE_FEED);
    /********** HORA ***************/
	for(uint8 x = 0; x < 13; x ++){
		PRINTER_A_PutChar(msn_hora[x]);
	}
    PRINTER_A_PutChar(((touch1[10] >> 4) + 0x30)) ;
    PRINTER_A_PutChar(((touch1[10] & 0x0F) + 0x30));
    PRINTER_A_PutChar(':');
    PRINTER_A_PutChar(((touch1[11] >> 4) + 0x30)) ;
    PRINTER_A_PutChar(((touch1[11] & 0x0F) + 0x30));
    PRINTER_A_PutChar(':');
    PRINTER_A_PutChar(((touch1[12] >> 4) + 0x30));
	PRINTER_A_PutChar(((touch1[12] & 0x0F) + 0x30));
	
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 24; x ++){
		PRINTER_A_PutChar(SEPARATOR[x]);
	}
            
	/***** body ******/
	if(address == side.a.dir){
        if(side.a.Copy == 1){
            for(uint8 x = 0; x < 22; x ++){
        		PRINTER_A_PutChar(msn_copy[x]);
        	}
            PRINTER_A_PutChar(LINE_FEED);
            for(uint8 x = 0; x < 24; x ++){
        		PRINTER_A_PutChar(SEPARATOR[x]);
        	}            
        }
        PRINTER_A_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 13; x ++){
    		PRINTER_A_PutChar(msn_venta[x]);
    	}
        if(side.a.Copy == 1){
           for(uint8 x = 5; x > 0; x --){
        		PRINTER_A_PutChar(sale_number_copy[x]);
        	} 
        }else{
            for(uint8 x = 5; x > 0; x --){
        		PRINTER_A_PutChar(sale_number[x]);
        	}
            int_salenumber++;
        }
        PRINTER_A_PutChar(LINE_FEED);
    	for(uint8 x = 0; x < 13; x ++){
    		PRINTER_A_PutChar(msn_placa[x]);
    	}
		for(uint8 x = 0; x < 8; x ++){
			PRINTER_A_PutChar(side.a.msn_plate[x]);
		}  
        PRINTER_A_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 13; x ++){
			PRINTER_A_PutChar(PRN_MILLEAGE[x]);
		}          
        for(uint8 x = 0; x <6; x ++){
			PRINTER_A_PutChar(side.a.km[x]);
		}  
        PRINTER_A_PutChar(LINE_FEED);
		for(uint8 x = 0; x < 13; x ++){
			PRINTER_A_PutChar(msn_pos[x]);
		}
		PRINTER_A_PutChar((side.a.dir)+ 0x31);
		PRINTER_A_PutChar(LINE_FEED);
		for(uint8 x = 0; x < 13; x ++){
			PRINTER_A_PutChar(msn_product[x]);
		}
        if(side.a.Nozzle == 0x01){
    		for(uint8 x = 0; x < 9; x ++){
    			PRINTER_A_PutChar(msn_diesel[x]);
    		}
        }else{
            for(uint8 x = 0; x < 9; x ++){
    			PRINTER_A_PutChar(msn_gasoline[x]);
    		}
        } 
        PRINTER_A_PutChar(LINE_FEED); 	
		for(uint8 x = 0; x < 13; x ++){
			PRINTER_A_PutChar(msn_ppu[x]);
		}
		PRINTER_A_PutChar(PRN_CURRENCY[0]);  
		PRINTER_A_PutChar(' ');
		for(uint8 x = 1; x < 6; x ++){
			PRINTER_A_PutChar(side.a.ProcessedPPU[side.a.Nozzle-1][x]);
		}
        PRINTER_A_PutChar(LINE_FEED);
		for(uint8 x = 0; x < 13; x ++){
			PRINTER_A_PutChar(msn_vol[x]);
		}
		for(uint8 x = 0; x < 8; x ++){
			if((8-x) == DecVol)
				PRINTER_A_PutChar('.');
			PRINTER_A_PutChar(side.a.ProcessedvolumeSale[x]);        
		}
		PRINTER_A_PutChar(' ');
		PRINTER_A_PutChar(VolSimbol[0]);
		PRINTER_A_PutChar(LINE_FEED); 
		for(uint8 x = 0; x < 13; x ++){
			PRINTER_A_PutChar(msn_din[x]);
		}     
		PRINTER_A_PutChar(PRN_CURRENCY[0]);  
		PRINTER_A_PutChar(' ');
		for(uint8 x = 0; x < 8; x ++){
			PRINTER_A_PutChar(side.a.ProcessedmoneySale[x]);
		}		
		PRINTER_A_PutChar(LINE_FEED);
        side.a.Copy = 1;
	}	
	if(address == side.b.dir){
		if(side.b.Copy == 1){
            for(uint8 x = 0; x < 22; x ++){
        		PRINTER_A_PutChar(msn_copy[x]);
        	}
            PRINTER_A_PutChar(LINE_FEED);
            for(uint8 x = 0; x < 24; x ++){
        		PRINTER_A_PutChar(SEPARATOR[x]);
        	}            
        }    
        PRINTER_A_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 13; x ++){
    		PRINTER_A_PutChar(msn_venta[x]);
    	}
        if(side.b.Copy == 1){
           for(uint8 x = 5; x > 0; x --){
        		PRINTER_A_PutChar(sale_number_copy[x]);
        	} 
        }else{
            for(uint8 x = 5; x > 0; x --){
        		PRINTER_A_PutChar(sale_number[x]);
        	}
            int_salenumber++;
        }
        PRINTER_A_PutChar(LINE_FEED);
    	for(uint8 x = 0; x < 13; x ++){
    		PRINTER_A_PutChar(msn_placa[x]);
    	}
		for(uint8 x = 0; x < 8; x ++){
			PRINTER_A_PutChar(side.b.msn_plate[x]);
		}  
        PRINTER_A_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 13; x ++){
			PRINTER_A_PutChar(PRN_MILLEAGE[x]);
		}          
        for(uint8 x = 0; x <6; x ++){
			PRINTER_A_PutChar(side.b.km[x]);
		}          
        PRINTER_A_PutChar(LINE_FEED);
		for(uint8 x = 0; x < 13; x ++){
			PRINTER_A_PutChar(msn_pos[x]);
		}
		PRINTER_A_PutChar((side.b.dir)+ 0x31);
		PRINTER_A_PutChar(LINE_FEED);
		for(uint8 x = 0; x < 13; x ++){
			PRINTER_A_PutChar(msn_product[x]);
		}
		if(side.b.Nozzle == 0x02){
    		for(uint8 x = 0; x < 9; x ++){
    			PRINTER_A_PutChar(msn_diesel[x]);
    		}
        }else{
            for(uint8 x = 0; x < 9; x ++){
    			PRINTER_A_PutChar(msn_gasoline[x]);
    		}
        }
        PRINTER_A_PutChar(LINE_FEED); 	
		for(uint8 x = 0; x < 13; x ++){
			PRINTER_A_PutChar(msn_ppu[x]);
		}
		PRINTER_A_PutChar(PRN_CURRENCY[0]);  
		PRINTER_A_PutChar(' ');
		for(uint8 x = 1; x < 6; x ++){
			PRINTER_A_PutChar(side.b.ProcessedPPU[side.b.Nozzle - 1][x]);
		}
        PRINTER_A_PutChar(LINE_FEED);
		for(uint8 x = 0; x < 13; x ++){
			PRINTER_A_PutChar(msn_vol[x]);
		}
		for(uint8 x = 0; x < 8; x ++){
			if((8-x) == DecVol)
				PRINTER_A_PutChar('.');
			PRINTER_A_PutChar(side.b.ProcessedvolumeSale[x]);        
		}
		PRINTER_A_PutChar(' ');
		PRINTER_A_PutChar(VolSimbol[0]);
		PRINTER_A_PutChar(LINE_FEED); 
		for(uint8 x = 0; x < 13; x ++){
			PRINTER_A_PutChar(msn_din[x]);
		}     
		PRINTER_A_PutChar(PRN_CURRENCY[0]);  
		PRINTER_A_PutChar(' ');
		for(uint8 x = 0; x < 8; x ++){
			PRINTER_A_PutChar(side.b.ProcessedmoneySale[x]);
		}		
		PRINTER_A_PutChar(LINE_FEED);
        side.b.Copy = 1;
	}
	
	
	
	/******FOOTER*****/
	for(uint8 x = 0; x < 24; x ++){
		PRINTER_A_PutChar(SEPARATOR[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);    
	for(uint8 x = 0; x < 22; x ++){
		PRINTER_A_PutChar(msn_footer3[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
    PRINTER_A_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 22; x ++){
		PRINTER_A_PutChar(msn_footer4[x]);
	}
	PRINTER_A_PutChar(LINE_FEED);    
    for(uint8 x = 0; x < 24; x ++){
		PRINTER_A_PutChar(SEPARATOR[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 22; x ++){
		PRINTER_A_PutChar(msn_footer[x]);
	}
	PRINTER_A_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 22; x ++){
		PRINTER_A_PutChar(msn_footer2[x]);
	}    
    PRINTER_A_PutChar(LINE_FEED);
    PRINTER_A_PutChar(LINE_FEED);
    PRINTER_A_PutChar(LINE_FEED);
    PRINTER_A_PutChar(LINE_FEED);
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 6; x ++){
        sale_number_copy[x] = sale_number[x];
    }
}

void PrintShift(void){
    uint8 screen_size,pos;
    char8 lectanterior[10],lectactual[10],memoria[10],resultado[10];
    uint32 actual,anterior,resta;
    for(uint8 LCDRx = 0; LCDRx < 6; LCDRx++){
        screen_PutChar(ASK_TIME[LCDRx]);
    }
    CyDelay(10);
    screen_size = screen_GetRxBufferSize();
    if(screen_size >= 13 ){
        for(uint8 LCDRx = 0; LCDRx < screen_size; LCDRx++){
            touch1[LCDRx] = screen_ReadRxData();
        }
        screen_size = 0;
    }
    PRINTER_A_PutChar(0x1D);
    PRINTER_A_PutChar(0x70);
    PRINTER_A_PutChar(0x01);
    PRINTER_A_PutChar(0x00);
    int_shiftnumber++;
    shift_number[5]=(int_shiftnumber/10000)+48;
	shift_number[4]=((int_shiftnumber%10000)/1000)+48;
	shift_number[3]=(((int_shiftnumber%10000)%1000)/100)+48;
	shift_number[2]=((((int_shiftnumber%10000)%1000)%100)/10)+48;	
	shift_number[1]=((((int_shiftnumber%10000)%1000)%100)%10)+48;
    for(uint8 i = 7; i<13; i++){
        EEPROM_1_WriteByte(shift_number[i-6],i);
    }
    PRINTER_A_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 22; x ++){
		PRINTER_A_PutChar(msn_EDS[x]);   // Encabezado de estación
	}
	PRINTER_A_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 22; x ++){
		PRINTER_A_PutChar(msn_EDS2[x]);
	}
	PRINTER_A_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 22; x ++){
		PRINTER_A_PutChar(msn_EDS3[x]);
	}
	PRINTER_A_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 22; x ++){
		PRINTER_A_PutChar(msn_EDS4[x]);
	}
	PRINTER_A_PutChar(LINE_FEED);  
	for(uint8 x = 0; x < 24; x ++){
		PRINTER_A_PutChar(SEPARATOR[x]); //Separador de sección
	}
    /********** FECHA ***************/
	for(uint8 x = 0; x < 13; x ++){
		PRINTER_A_PutChar(msn_fecha[x]);
	}
    PRINTER_A_PutChar(((touch1[6] >> 4) + 0x30));
    PRINTER_A_PutChar(((touch1[6] & 0x0F) + 0x30));
    PRINTER_A_PutChar('/');
    PRINTER_A_PutChar(((touch1[7] >> 4) + 0x30)) ;
    PRINTER_A_PutChar(((touch1[7] & 0x0F) + 0x30));
    PRINTER_A_PutChar('/');
    PRINTER_A_PutChar(((touch1[8] >> 4) + 0x30)) ;
	PRINTER_A_PutChar(((touch1[8] & 0x0F) + 0x30));

	PRINTER_A_PutChar(LINE_FEED);
    /********** HORA ***************/
	for(uint8 x = 0; x < 13; x ++){
		PRINTER_A_PutChar(msn_hora[x]);
	}
    PRINTER_A_PutChar(((touch1[10] >> 4) + 0x30)) ;
    PRINTER_A_PutChar(((touch1[10] & 0x0F) + 0x30));
    PRINTER_A_PutChar(':');
    PRINTER_A_PutChar(((touch1[11] >> 4) + 0x30)) ;
    PRINTER_A_PutChar(((touch1[11] & 0x0F) + 0x30));
    PRINTER_A_PutChar(':');
    PRINTER_A_PutChar(((touch1[12] >> 4) + 0x30));
	PRINTER_A_PutChar(((touch1[12] & 0x0F) + 0x30));
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 24; x ++){
		PRINTER_A_PutChar(SEPARATOR[x]);
	} 
    ////////////////////////////////////////
	for(uint8 x = 0; x < 13; x ++){
		PRINTER_A_PutChar(msn_cierre[x]);
	}     
	for(uint8 x = 5; x > 0; x --){
		PRINTER_A_PutChar(shift_number[x]);   //Número de cierre
	}
	PRINTER_A_PutChar(LINE_FEED);
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 13; x ++){
        PRINTER_A_PutChar(msn_pos[x]);
    }
    PRINTER_A_PutChar(side.a.dir + 0x31);
    PRINTER_A_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 16; x ++){
		PRINTER_A_PutChar(msn_lecact[x]);
	}
	PRINTER_A_PutChar(LINE_FEED);	
                          
    for(uint8 x = 0; x < 10; x ++){ 
        if((10-x) == DecVol)
            PRINTER_A_PutChar('.');
        PRINTER_A_PutChar(side.a.ProcessedTotals[0][0][x]);
    }
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 10; x++){
        pos = x;
        if(side.a.ProcessedTotals[0][0][x] != 0x00 )
            break;
    }
	for(uint8 x = 0; x < 16; x ++){
		PRINTER_A_PutChar(msn_lecaan[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 10; x ++){
        memoria[x] = EEPROM_1_ReadByte(36+x);
    }
    for(uint8 x = 0; x < 10; x ++){ 
        if((10-x) == DecVol)
            PRINTER_A_PutChar('.');
        PRINTER_A_PutChar(memoria[x]);
    }
    PRINTER_A_PutChar(LINE_FEED);
    
    for(uint8 x = pos; x < 10; x++ ){
        lectactual[x-pos] = side.a.ProcessedTotals[0][0][x];
        lectanterior[x-pos] = memoria[x];
        resultado[x] = 0x00;
    }
    
    anterior = atoi(lectanterior);
    actual = atoi(lectactual);
    resta =  actual - anterior;
    itoa(resta,resultado,10);
    
    for(uint8 x = 0; x < 18; x ++){
        PRINTER_A_PutChar(msn_dif[x]);
    }
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 10; x ++){
        if((10-x) == DecVol)
            PRINTER_A_PutChar('.');
        PRINTER_A_PutChar(resultado[x]);
    }
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 24; x ++){
        PRINTER_A_PutChar(SEPARATOR[x]);
    }  
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 13; x ++){
        PRINTER_A_PutChar(msn_pos[x]);
    }
    PRINTER_A_PutChar(side.b.dir + 0x31);
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 16; x ++){
		PRINTER_A_PutChar(msn_lecact[x]);
	}
	PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 10; x ++){ 
        if((10-x) == DecVol)
            PRINTER_A_PutChar('.');
        PRINTER_A_PutChar(side.b.ProcessedTotals[0][0][x]);
    }
    PRINTER_A_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 16; x ++){
		PRINTER_A_PutChar(msn_lecaan[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 10; x ++){
        memoria[x] = EEPROM_1_ReadByte(46+x);
    }
    for(uint8 x = 0; x < 10; x ++){ 
        if((10-x) == DecVol)
            PRINTER_A_PutChar('.');
        PRINTER_A_PutChar(memoria[x]);
    }
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = pos; x < 10; x++ ){
        lectactual[x-pos] = side.b.ProcessedTotals[0][0][x];
        lectanterior[x-pos] = memoria[x];
        resultado[x] = 0x00;
    }
    
    anterior = atoi(lectanterior);
    actual = atoi(lectactual);
    resta =  actual - anterior;
    itoa(resta,resultado,10);
    for(uint8 x = 0; x < 18; x ++){
        PRINTER_A_PutChar(msn_dif[x]);
    }
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 10; x ++){
        if((10-x) == DecVol)
            PRINTER_A_PutChar('.');
        PRINTER_A_PutChar(resultado[x]);
    }
    PRINTER_A_PutChar(LINE_FEED);
    
    if((Positions-1)>= 3){
        for(uint8 x = 0; x < 24; x ++){
            PRINTER_A_PutChar(SEPARATOR[x]);
        }           
        PRINTER_A_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 13; x ++){
            PRINTER_A_PutChar(msn_pos[x]);
        }
        PRINTER_A_PutChar(side.c.dir + 0x31);
        PRINTER_A_PutChar(LINE_FEED);              
        for(uint8 x = 0; x < 10; x ++){ 
            if((10-x) == DecVol)
                PRINTER_A_PutChar('.');
            PRINTER_A_PutChar(side.c.ProcessedTotals[0][0][x]);
        }
        PRINTER_A_PutChar(LINE_FEED);
    	for(uint8 x = 0; x < 16; x ++){
    		PRINTER_A_PutChar(msn_lecaan[x]);
    	}
        PRINTER_A_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 24; x ++){
            PRINTER_A_PutChar(SEPARATOR[x]);
        }   
        PRINTER_A_PutChar(LINE_FEED);
    	for(uint8 x = 0; x < 13; x ++){
            PRINTER_A_PutChar(msn_pos[x]);
        }
        PRINTER_A_PutChar(side.d.dir + 0x31);
        PRINTER_A_PutChar(LINE_FEED);                      
        for(uint8 x = 0; x < 10; x ++){ 
            if((10-x) == DecVol)
                PRINTER_A_PutChar('.');
            PRINTER_A_PutChar(side.d.ProcessedTotals[0][0][x]);
        }
        PRINTER_A_PutChar(LINE_FEED);
    	for(uint8 x = 0; x < 16; x ++){
    		PRINTER_A_PutChar(msn_lecaan[x]);
    	}
        
        for(uint8 x = 0; x < 24; x ++){
            PRINTER_A_PutChar(SEPARATOR[x]);
        }            
	    PRINTER_A_PutChar(LINE_FEED);
    } 
    /******FOOTER*****/
	for(uint8 x = 0; x < 24; x ++){
		PRINTER_A_PutChar(SEPARATOR[x]);
	}
	PRINTER_A_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 22; x ++){
		PRINTER_A_PutChar(msn_footer[x]);
	}
	PRINTER_A_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 22; x ++){
		PRINTER_A_PutChar(msn_footer2[x]);
	}           
    PRINTER_A_PutChar(LINE_FEED);
    PRINTER_A_PutChar(LINE_FEED);
    PRINTER_A_PutChar(LINE_FEED);
    PRINTER_A_PutChar(LINE_FEED);
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 10; x++){
        EEPROM_1_WriteByte(side.a.ProcessedTotals[0][0][x],36+x);        
    }
    for(uint8 x = 0; x < 10; x++){
        EEPROM_1_WriteByte(side.b.ProcessedTotals[0][0][x],46+x);
    }
    
}





/* [] END OF FILE */
