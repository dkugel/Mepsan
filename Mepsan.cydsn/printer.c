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
uint8 msn_lecact[16]      ="Valor actual:   ";
uint8 msn_lecaan[16]      ="Valor anterior: ";
uint8 SEPARATOR[24]       = "************************";
uint8 ASK_TIME[]={0x5A, 0xA5, 0x03, 0x81, 0x20, 0x07};



void PrintReceipt(uint8 address){
    int8 screen_size;
    int_salenumber++;
    sale_number[5]=(int_salenumber/10000)+48;
	sale_number[4]=((int_salenumber%10000)/1000)+48;
	sale_number[3]=(((int_salenumber%10000)%1000)/100)+48;
	sale_number[2]=((((int_salenumber%10000)%1000)%100)/10)+48;	
	sale_number[1]=((((int_salenumber%10000)%1000)%100)%10)+48;
    for(uint8 i = 1; i<6; i++){
        EEPROM_1_WriteByte(sale_number[i],i);
    }
    for(uint8 LCDRx = 0; LCDRx < 6; LCDRx++){
        screen_PutChar(ASK_TIME[LCDRx]);
    }
    CyDelay(4);
    screen_size = screen_GetRxBufferSize();
    if(screen_size >= 13 ){
        for(uint8 LCDRx = 0; LCDRx < screen_size; LCDRx++){
            touch1[LCDRx] = screen_ReadRxData();
        }
        screen_size = 0;
    }
	for(uint8 x = 0; x < 22; x ++){
		UART_3_PutChar(msn_EDS[x]);
	}
    UART_3_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 22; x ++){
		UART_3_PutChar(msn_EDS2[x]);
	}
	UART_3_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 22; x ++){
		UART_3_PutChar(msn_EDS3[x]);
	}
	UART_3_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 22; x ++){
		UART_3_PutChar(msn_EDS4[x]);
	}
    UART_3_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 24; x ++){
		UART_3_PutChar(SEPARATOR[x]);
	}
    UART_3_PutChar(LINE_FEED);
    /********** FECHA ***************/
	for(uint8 x = 0; x < 13; x ++){
		UART_3_PutChar(msn_fecha[x]);
	}
    UART_3_PutChar(((touch1[6] >> 4) + 0x30));
    UART_3_PutChar(((touch1[6] & 0x0F) + 0x30));
    UART_3_PutChar('/');
    UART_3_PutChar(((touch1[7] >> 4) + 0x30)) ;
    UART_3_PutChar(((touch1[7] & 0x0F) + 0x30));
    UART_3_PutChar('/');
    UART_3_PutChar(((touch1[8] >> 4) + 0x30)) ;
	UART_3_PutChar(((touch1[8] & 0x0F) + 0x30));

	UART_3_PutChar(LINE_FEED);
    /********** HORA ***************/
	for(uint8 x = 0; x < 13; x ++){
		UART_3_PutChar(msn_hora[x]);
	}
    UART_3_PutChar(((touch1[10] >> 4) + 0x30)) ;
    UART_3_PutChar(((touch1[10] & 0x0F) + 0x30));
    UART_3_PutChar(':');
    UART_3_PutChar(((touch1[11] >> 4) + 0x30)) ;
    UART_3_PutChar(((touch1[11] & 0x0F) + 0x30));
    UART_3_PutChar(':');
    UART_3_PutChar(((touch1[12] >> 4) + 0x30));
	UART_3_PutChar(((touch1[12] & 0x0F) + 0x30));
	
    UART_3_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 24; x ++){
		UART_3_PutChar(SEPARATOR[x]);
	}
    UART_3_PutChar(LINE_FEED); 
    for(uint8 x = 0; x < 13; x ++){
		UART_3_PutChar(msn_venta[x]);
	}
    for(uint8 x = 5; x > 0; x --){
		UART_3_PutChar(sale_number[x]);
	}
    UART_3_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 13; x ++){
		UART_3_PutChar(msn_placa[x]);
	}
	/***** body ******/
	if(address == side.a.dir){
		for(uint8 x = 0; x < 8; x ++){
			UART_3_PutChar(side.a.msn_plate[x]);
		}  
        UART_3_PutChar(LINE_FEED);
		for(uint8 x = 0; x < 13; x ++){
			UART_3_PutChar(msn_pos[x]);
		}
		UART_3_PutChar((side.a.dir)+ 0x31);
		UART_3_PutChar(LINE_FEED);
		for(uint8 x = 0; x < 13; x ++){
			UART_3_PutChar(msn_product[x]);
		}
		for(uint8 x = 0; x < 8; x ++){
			UART_3_PutChar(msn_diesel[x]);
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
		UART_3_PutChar(' ');
		UART_3_PutChar(VolSimbol[0]);
		UART_3_PutChar(LINE_FEED); 
		for(uint8 x = 0; x < 13; x ++){
			UART_3_PutChar(msn_din[x]);
		}     
		UART_3_PutChar(PRN_CURRENCY[0]);  
		UART_3_PutChar(' ');
		for(uint8 x = 0; x < 8; x ++){
			UART_3_PutChar(side.a.ProcessedmoneySale[x]);
		}
		UART_3_PutChar(LINE_FEED); 	
		for(uint8 x = 0; x < 13; x ++){
			UART_3_PutChar(msn_ppu[x]);
		}
		UART_3_PutChar(PRN_CURRENCY[0]);  
		UART_3_PutChar(' ');
		for(uint8 x = 1; x < 6; x ++){
			UART_3_PutChar(side.a.ProcessedPPU[0][x]);
		}
		UART_3_PutChar(LINE_FEED);
	}
	
	if(address == side.b.dir){
		for(uint8 x = 0; x < 8; x ++){
			UART_3_PutChar(side.b.msn_plate[x]);
		}
        UART_3_PutChar(LINE_FEED);
		for(uint8 x = 0; x < 13; x ++){
			UART_3_PutChar(msn_pos[x]);
		}
		UART_3_PutChar((side.b.dir)+ 0x31);
		UART_3_PutChar(LINE_FEED);
		for(uint8 x = 0; x < 13; x ++){
			UART_3_PutChar(msn_product[x]);
		}
		for(uint8 x = 0; x < 8; x ++){
			UART_3_PutChar(msn_diesel[x]);
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
		UART_3_PutChar(' ');
		UART_3_PutChar(VolSimbol[0]);
		UART_3_PutChar(LINE_FEED); 
		for(uint8 x = 0; x < 13; x ++){
			UART_3_PutChar(msn_din[x]);
		}     
		UART_3_PutChar(PRN_CURRENCY[0]);  
		UART_3_PutChar(' ');
		for(uint8 x = 0; x < 8; x ++){
			UART_3_PutChar(side.b.ProcessedmoneySale[x]);
		}
		UART_3_PutChar(LINE_FEED); 	
		for(uint8 x = 0; x < 13; x ++){
			UART_3_PutChar(msn_ppu[x]);
		}
		UART_3_PutChar(PRN_CURRENCY[0]);  
		UART_3_PutChar(' ');
		for(uint8 x = 1; x < 6; x ++){
			UART_3_PutChar(side.b.ProcessedPPU[0][x]);
		}
		UART_3_PutChar(LINE_FEED);
	}
	
	if(address == side.c.dir){
		for(uint8 x = 0; x < 8; x ++){
			UART_3_PutChar(side.c.msn_plate[x]);
		}  
        UART_3_PutChar(LINE_FEED);
		for(uint8 x = 0; x < 13; x ++){
			UART_3_PutChar(msn_pos[x]);
		}
		UART_3_PutChar((side.c.dir)+ 0x31);
		UART_3_PutChar(LINE_FEED);
		for(uint8 x = 0; x < 13; x ++){
			UART_3_PutChar(msn_product[x]);
		}
		for(uint8 x = 0; x < 8; x ++){
			UART_3_PutChar(msn_diesel[x]);
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
		UART_3_PutChar(' ');
		UART_3_PutChar(VolSimbol[0]);
		UART_3_PutChar(LINE_FEED); 
		for(uint8 x = 0; x < 13; x ++){
			UART_3_PutChar(msn_din[x]);
		}     
		UART_3_PutChar(PRN_CURRENCY[0]);  
		UART_3_PutChar(' ');
		for(uint8 x = 0; x < 8; x ++){
			UART_3_PutChar(side.c.ProcessedmoneySale[x]);
		}
		UART_3_PutChar(LINE_FEED); 	
		for(uint8 x = 0; x < 13; x ++){
			UART_3_PutChar(msn_ppu[x]);
		}
		UART_3_PutChar(PRN_CURRENCY[0]);  
		UART_3_PutChar(' ');
		for(uint8 x = 1; x < 6; x ++){
			UART_3_PutChar(side.c.ProcessedPPU[0][x]);
		}
		UART_3_PutChar(LINE_FEED);
	}
	
    if(address == side.d.dir){
		for(uint8 x = 0; x < 8; x ++){
			UART_3_PutChar(side.d.msn_plate[x]);
		}  
		for(uint8 x = 0; x < 13; x ++){
			UART_3_PutChar(msn_pos[x]);
		}
		UART_3_PutChar((side.d.dir)+ 0x31);
		UART_3_PutChar(LINE_FEED);
		UART_3_PutChar(LINE_FEED);
		for(uint8 x = 0; x < 13; x ++){
			UART_3_PutChar(msn_product[x]);
		}
		for(uint8 x = 0; x < 8; x ++){
			UART_3_PutChar(msn_diesel[x]);
		}
		for(uint8 x = 0; x < 13; x ++){
			UART_3_PutChar(msn_vol[x]);
		}
		for(uint8 x = 0; x < 8; x ++){
			if((8-x) == DecVol)
				UART_3_PutChar('.');
			UART_3_PutChar(side.d.ProcessedvolumeSale[x]);        
		}
		UART_3_PutChar(' ');
		UART_3_PutChar(VolSimbol[0]);
		UART_3_PutChar(LINE_FEED); 
		for(uint8 x = 0; x < 13; x ++){
			UART_3_PutChar(msn_din[x]);
		}     
		UART_3_PutChar(PRN_CURRENCY[0]);  
		UART_3_PutChar(' ');
		for(uint8 x = 0; x < 8; x ++){
			UART_3_PutChar(side.d.ProcessedmoneySale[x]);
		}
		UART_3_PutChar(LINE_FEED); 	
		for(uint8 x = 0; x < 13; x ++){
			UART_3_PutChar(msn_ppu[x]);
		}
		UART_3_PutChar(PRN_CURRENCY[0]);  
		UART_3_PutChar(' ');
		for(uint8 x = 1; x < 6; x ++){
			UART_3_PutChar(side.d.ProcessedPPU[0][x]);
		}
		UART_3_PutChar(LINE_FEED);
	}
	
	/******FOOTER*****/
	for(uint8 x = 0; x < 24; x ++){
		UART_3_PutChar(SEPARATOR[x]);
	}
	UART_3_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 22; x ++){
		UART_3_PutChar(msn_footer[x]);
	}
	UART_3_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 22; x ++){
		UART_3_PutChar(msn_footer2[x]);
	}
        
    UART_3_PutChar(LINE_FEED);
    UART_3_PutChar(LINE_FEED);
    UART_3_PutChar(LINE_FEED);
    UART_3_PutChar(LINE_FEED);
    UART_3_PutChar(LINE_FEED);
}

void PrintShift(void){
    int8 screen_size;
    for(uint8 LCDRx = 0; LCDRx < 6; LCDRx++){
        screen_PutChar(ASK_TIME[LCDRx]);
    }
    screen_size = screen_GetRxBufferSize();
    if(screen_size >= 13 ){
        for(uint8 LCDRx = 0; LCDRx < screen_size; LCDRx++){
            touch1[LCDRx] = screen_ReadRxData();
        }
        screen_size = 0;
    }
    CyDelay(4);
    int_shiftnumber++;
    shift_number[5]=(int_shiftnumber/10000)+48;
	shift_number[4]=((int_shiftnumber%10000)/1000)+48;
	shift_number[3]=(((int_shiftnumber%10000)%1000)/100)+48;
	shift_number[2]=((((int_shiftnumber%10000)%1000)%100)/10)+48;	
	shift_number[1]=((((int_shiftnumber%10000)%1000)%100)%10)+48;
    for(uint8 i = 7; i<13; i++){
        EEPROM_1_WriteByte(shift_number[i-6],i);
    }
    UART_3_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 22; x ++){
		UART_3_PutChar(msn_EDS[x]);   // Encabezado de estación
	}
	UART_3_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 22; x ++){
		UART_3_PutChar(msn_EDS2[x]);
	}
	UART_3_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 22; x ++){
		UART_3_PutChar(msn_EDS3[x]);
	}
	UART_3_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 22; x ++){
		UART_3_PutChar(msn_EDS4[x]);
	}
	UART_3_PutChar(LINE_FEED);  
	for(uint8 x = 0; x < 24; x ++){
		UART_3_PutChar(SEPARATOR[x]); //Separador de sección
	}
    /********** FECHA ***************/
	for(uint8 x = 0; x < 13; x ++){
		UART_3_PutChar(msn_fecha[x]);
	}
    UART_3_PutChar(((touch1[6] >> 4) + 0x30));
    UART_3_PutChar(((touch1[6] & 0x0F) + 0x30));
    UART_3_PutChar('/');
    UART_3_PutChar(((touch1[7] >> 4) + 0x30)) ;
    UART_3_PutChar(((touch1[7] & 0x0F) + 0x30));
    UART_3_PutChar('/');
    UART_3_PutChar(((touch1[8] >> 4) + 0x30)) ;
	UART_3_PutChar(((touch1[8] & 0x0F) + 0x30));

	UART_3_PutChar(LINE_FEED);
    /********** HORA ***************/
	for(uint8 x = 0; x < 13; x ++){
		UART_3_PutChar(msn_hora[x]);
	}
    UART_3_PutChar(((touch1[10] >> 4) + 0x30)) ;
    UART_3_PutChar(((touch1[10] & 0x0F) + 0x30));
    UART_3_PutChar(':');
    UART_3_PutChar(((touch1[11] >> 4) + 0x30)) ;
    UART_3_PutChar(((touch1[11] & 0x0F) + 0x30));
    UART_3_PutChar(':');
    UART_3_PutChar(((touch1[12] >> 4) + 0x30));
	UART_3_PutChar(((touch1[12] & 0x0F) + 0x30));
    UART_3_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 24; x ++){
		UART_3_PutChar(SEPARATOR[x]);
	} 
    ////////////////////////////////////////
	for(uint8 x = 0; x < 13; x ++){
		UART_3_PutChar(msn_cierre[x]);
	}     
	for(uint8 x = 5; x > 0; x --){
		UART_3_PutChar(shift_number[x]);   //Número de cierre
	}
	UART_3_PutChar(LINE_FEED);
    UART_3_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 13; x ++){
        UART_3_PutChar(msn_pos[x]);
    }
    UART_3_PutChar(side.a.dir + 0x31);
    UART_3_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 16; x ++){
		UART_3_PutChar(msn_lecact[x]);
	}
	UART_3_PutChar(LINE_FEED);	
                          
    for(uint8 x = 0; x < 10; x ++){ 
        if((10-x) == DecVol)
            UART_3_PutChar('.');
        UART_3_PutChar(side.a.ProcessedTotals[0][0][x]);
    }
    UART_3_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 16; x ++){
		UART_3_PutChar(msn_lecaan[x]);
	}
    UART_3_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 10; x ++){ 
        if((10-x) == DecVol)
            UART_3_PutChar('.');
        UART_3_PutChar(EEPROM_1_ReadByte(36+x));
    }
    UART_3_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 24; x ++){
        UART_3_PutChar(SEPARATOR[x]);
    }  
    UART_3_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 13; x ++){
        UART_3_PutChar(msn_pos[x]);
    }
    UART_3_PutChar(side.b.dir + 0x31);
    UART_3_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 16; x ++){
		UART_3_PutChar(msn_lecact[x]);
	}
	UART_3_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 10; x ++){ 
        if((10-x) == DecVol)
            UART_3_PutChar('.');
        UART_3_PutChar(side.b.ProcessedTotals[0][0][x]);
    }
    UART_3_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 16; x ++){
		UART_3_PutChar(msn_lecaan[x]);
	}
    UART_3_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 10; x ++){ 
        if((10-x) == DecVol)
            UART_3_PutChar('.');
        UART_3_PutChar(EEPROM_1_ReadByte(46+x));
    }
    UART_3_PutChar(LINE_FEED);
    if((Positions-1)>= 3){
        for(uint8 x = 0; x < 24; x ++){
            UART_3_PutChar(SEPARATOR[x]);
        }           
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_pos[x]);
        }
        UART_3_PutChar(side.c.dir + 0x31);
        UART_3_PutChar(LINE_FEED);              
        for(uint8 x = 0; x < 10; x ++){ 
            if((10-x) == DecVol)
                UART_3_PutChar('.');
            UART_3_PutChar(side.c.ProcessedTotals[0][0][x]);
        }
        UART_3_PutChar(LINE_FEED);
    	for(uint8 x = 0; x < 16; x ++){
    		UART_3_PutChar(msn_lecaan[x]);
    	}
        UART_3_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 24; x ++){
            UART_3_PutChar(SEPARATOR[x]);
        }   
        UART_3_PutChar(LINE_FEED);
    	for(uint8 x = 0; x < 13; x ++){
            UART_3_PutChar(msn_pos[x]);
        }
        UART_3_PutChar(side.d.dir + 0x31);
        UART_3_PutChar(LINE_FEED);                      
        for(uint8 x = 0; x < 10; x ++){ 
            if((10-x) == DecVol)
                UART_3_PutChar('.');
            UART_3_PutChar(side.d.ProcessedTotals[0][0][x]);
        }
        UART_3_PutChar(LINE_FEED);
    	for(uint8 x = 0; x < 16; x ++){
    		UART_3_PutChar(msn_lecaan[x]);
    	}
        
        for(uint8 x = 0; x < 24; x ++){
            UART_3_PutChar(SEPARATOR[x]);
        }            
	    UART_3_PutChar(LINE_FEED);
    } 
    /******FOOTER*****/
	for(uint8 x = 0; x < 24; x ++){
		UART_3_PutChar(SEPARATOR[x]);
	}
	UART_3_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 22; x ++){
		UART_3_PutChar(msn_footer[x]);
	}
	UART_3_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 22; x ++){
		UART_3_PutChar(msn_footer2[x]);
	}           
    UART_3_PutChar(LINE_FEED);
    UART_3_PutChar(LINE_FEED);
    UART_3_PutChar(LINE_FEED);
    UART_3_PutChar(LINE_FEED);
    UART_3_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 10; x++){
        EEPROM_1_WriteByte(side.a.ProcessedTotals[0][0][x],36+x);
    }
    for(uint8 x = 0; x < 10; x++){
        EEPROM_1_WriteByte(side.b.ProcessedTotals[0][0][x],46+x);
    }
}





/* [] END OF FILE */
