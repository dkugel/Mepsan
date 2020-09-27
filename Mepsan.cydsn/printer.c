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


//uint8 msn_EDS0[32]      = "      JUAN ELIECER CORTES       ";
/***********  Temporal messages ******************/
uint8 msn_POS[32]       = "      TIQUETE SISTEMA POS       ";
uint8 msn_EDS[32]       = " ESTACION DE SERVICIO EL OASIS  ";
uint8 msn_EDS2[32]      = "     Oscar Javier Gonzalez      ";
uint8 msn_EDS3[32]      = "       NIT: 7.819.086-5         ";
uint8 msn_EDS4[32]      = "        Cra 10 No. 4-26         ";
uint8 msn_EDS5[32]      = "        Tel: 3157886265         "; 
uint8 msn_EDS6[32]      = "      Puerto Lleras - Meta      ";
uint8 msn_footer[32]    = "      GRACIAS POR SU COMPRA     ";
uint8 msn_footer2[32]   = "          VUELVA PRONTO         ";
uint8 msn_footer3[32]   = "Firma:                          ";
uint8 msn_footer4[32]   = "________________________________";
uint8 msn_copy[32]      = "             COPIA              ";
uint8 msn_original[32]  = "            ORIGINAL            ";
uint8 msn_diesel[9]     = "Diesel   ";
uint8 msn_gasoline[10]  = "Corriente ";
uint8 msn_extra[9]      = "Extra    ";
uint8 msn_POS2[13]      = "Cliente: ";
uint8 msn_POS3[13]      = "C.C/NIT: ";
uint8 msn_POS4[32]      = "Codigo                          ";
uint8 msn_POS5[32]      = "Descripcion de item             ";
uint8 msn_POS6[32]      = "Cantidad     PPU      Valor     ";
uint8 msn_POS7[32]      = "Discriminacion de impuestos     ";
uint8 msn_POS8[32]      = "             Base       Impuesto";
uint8 msn_POS9[13]      = "E  IVA  0%  ";
uint8 msn_POS10[13]     = "SUBTOTAL   :";
uint8 msn_POS11[13]     = "IVA        :";
uint8 msn_POS12[13]     = "TOTAL      :";
uint8 msn_POS13[32]     = "Agente retenedor de IVA         ";
uint8 msn_POS14[32]     = "Res. DIAN No. 123456789         ";
uint8 msn_POS15[32]     = "Del 2020/09/01                  ";
uint8 msn_POS16[32]     = "Al  2022/09/01                  ";
uint8 msn_POS17[32]     = "Autoriza de SUF-00              ";
uint8 msn_POS18[32]     = "Al  SUF-1300000000              ";
uint8 msn_POS19[32]     = "Vigencia: 24 meses              ";
uint8 prefijo[4]        = "SUF-";
uint8 equipo[13]        = "Equipo   : 1 ";

/*********** informative messages ****************/

uint8 msn_fecha[13]       = "Fecha      : ";
uint8 msn_hora[13]        = "Hora       : ";
uint8 msn_placa[13]       = "Placa      : ";
uint8 PRN_MILLEAGE[13]    = "Kilometraje: ";
uint8 PRN_TRANSTYPE[13]   = "Tipo       : ";
uint8 msn_pos[13]         = "Posicion   : ";
uint8 msn_product[13]     = "Producto   : ";
uint8 msn_ppu[13]         = "PPU        : ";
uint8 msn_vol[13]         = "Volumen    : ";
uint8 msn_voltot[13]      = "Tot.Vol    : ";
uint8 msn_din[13]         = "Importe    : ";
uint8 msn_cierre[13]      = "Cierre No  : ";
uint8 msn_venta[13]       = "No         : ";
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
uint8 SEPARATOR[32]       = "--------------------------------";
uint8 ASK_TIME[]={0x5A, 0xA5, 0x03, 0x81, 0x20, 0x07};



void PrintReceipt(uint8 address){
    int8 screen_size, date_sum;  
    screen_size = 0;
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
        CyDelay(4);
        screen_size = screen_GetRxBufferSize();
    }
    if(screen_size >= 13 ){
        for(uint8 LCDRx = 0; LCDRx < screen_size; LCDRx++){
            touch1[LCDRx] = screen_ReadRxData();
        }
        screen_size = 0;
    }
    date_sum = touch1[6] + touch1[7] + touch1[8];
    while(touch1[8] == 0x00 || touch1[6] == 00){
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
    }    
	for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(msn_EDS[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(msn_EDS2[x]);
	}
	PRINTER_A_PutChar(LINE_FEED);
//    for(uint8 x = 0; x < 32; x ++){
//		PRINTER_A_PutChar(msn_EDS0[x]);
//	}
//	PRINTER_A_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(msn_EDS3[x]);
	}
	PRINTER_A_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(msn_EDS4[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(msn_EDS5[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(msn_EDS6[x]);
	}    
    PRINTER_A_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(SEPARATOR[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
    /********** FECHA ***************/
	for(uint8 x = 0; x < 13; x ++){
		PRINTER_A_PutChar(msn_fecha[x]);
	}
    
    date_copy[0] = (touch1[8] >> 4) + 0x30;
    date_copy[1] = (touch1[8] & 0x0F) + 0x30;
    date_copy[2] = (touch1[7] >> 4) + 0x30;
    date_copy[3] = (touch1[7] & 0x0F) + 0x30;
    date_copy[4] = (touch1[6] >> 4) + 0x30;
    date_copy[5] = (touch1[6] & 0x0F) + 0x30;
        
    
    hour_copy[0] = (touch1[10] >> 4) + 0x30;
    hour_copy[1] = (touch1[10] & 0x0F) + 0x30;
    hour_copy[2] = (touch1[11] >> 4) + 0x30;
    hour_copy[3] = (touch1[11] & 0x0F) + 0x30;
    hour_copy[4] = (touch1[12] >> 4) + 0x30;
    hour_copy[5] = (touch1[12] & 0x0F) + 0x30;
    
        
            
	/***** body ******/
	if(address == side.a.dir){
        if(side.a.Copy == 1){
            
            PRINTER_A_PutChar(date_copy[0]);
            PRINTER_A_PutChar(date_copy[1]);
            PRINTER_A_PutChar('/');
            PRINTER_A_PutChar(date_copy[2]);
            PRINTER_A_PutChar(date_copy[3]);
            PRINTER_A_PutChar('/');
            PRINTER_A_PutChar(date_copy[4]);
            PRINTER_A_PutChar(date_copy[5]);
            
            PRINTER_A_PutChar(LINE_FEED);
            
            /********** HORA ***************/
        	for(uint8 x = 0; x < 13; x ++){
        		PRINTER_A_PutChar(msn_hora[x]);
        	}
            
            PRINTER_A_PutChar(hour_copy[0]);
            PRINTER_A_PutChar(hour_copy[1]);
            PRINTER_A_PutChar(':');
            PRINTER_A_PutChar(hour_copy[2]);
            PRINTER_A_PutChar(hour_copy[3]);
            PRINTER_A_PutChar(':');
            PRINTER_A_PutChar(hour_copy[4]);
            PRINTER_A_PutChar(hour_copy[5]);
            
            PRINTER_A_PutChar(LINE_FEED);
            
            for(uint8 x = 0; x < 32; x ++){
        		PRINTER_A_PutChar(SEPARATOR[x]);
        	}
            
            for(uint8 x = 0; x < 32; x ++){
        		PRINTER_A_PutChar(msn_copy[x]);
        	}
            PRINTER_A_PutChar(LINE_FEED);
            for(uint8 x = 0; x < 32; x ++){
        		PRINTER_A_PutChar(SEPARATOR[x]);
        	}            
        }
        else{
            
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
            for(uint8 x = 0; x < 32; x ++){
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
        }
        if(side.a.Nozzle == 0x02){
            for(uint8 x = 0; x < 9; x ++){
    			PRINTER_A_PutChar(msn_gasoline[x]);
    		}
        }
        if(side.a.Nozzle == 0x03){
            for(uint8 x = 0; x < 9; x ++){
    			
                PRINTER_A_PutChar(msn_extra[x]);
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

		for(uint8 x = 0; x < 7; x ++){
			PRINTER_A_PutChar(side.a.ProcessedmoneySale[x]);
		}		
		PRINTER_A_PutChar(LINE_FEED);
        side.a.Copy = 1;
	}	
	if(address == side.b.dir){
		if(side.b.Copy == 1) {
            
            PRINTER_A_PutChar(date_copy[0]);
            PRINTER_A_PutChar(date_copy[1]);
            PRINTER_A_PutChar('/');
            PRINTER_A_PutChar(date_copy[2]);
            PRINTER_A_PutChar(date_copy[3]);
            PRINTER_A_PutChar('/');
            PRINTER_A_PutChar(date_copy[4]);
            PRINTER_A_PutChar(date_copy[5]);
            
            PRINTER_A_PutChar(LINE_FEED);
            
            /********** HORA ***************/
        	for(uint8 x = 0; x < 13; x ++){
        		PRINTER_A_PutChar(msn_hora[x]);
        	}
            
            PRINTER_A_PutChar(hour_copy[0]);
            PRINTER_A_PutChar(hour_copy[1]);
            PRINTER_A_PutChar(':');
            PRINTER_A_PutChar(hour_copy[2]);
            PRINTER_A_PutChar(hour_copy[3]);
            PRINTER_A_PutChar(':');
            PRINTER_A_PutChar(hour_copy[4]);
            PRINTER_A_PutChar(hour_copy[5]);
            
            PRINTER_A_PutChar(LINE_FEED);
            
            for(uint8 x = 0; x < 32; x ++){
        		PRINTER_A_PutChar(SEPARATOR[x]);
        	}
            
            for(uint8 x = 0; x < 32; x ++){
        		PRINTER_A_PutChar(msn_copy[x]);
        	}
            PRINTER_A_PutChar(LINE_FEED);
            for(uint8 x = 0; x < 32; x ++){
        		PRINTER_A_PutChar(SEPARATOR[x]);
        	}            
        }
        else{
            
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
            for(uint8 x = 0; x < 32; x ++){
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
		if(side.b.Nozzle == 0x01){
    		for(uint8 x = 0; x < 9; x ++){
                PRINTER_A_PutChar(msn_gasoline[x]);    			
    		}
        }
        if(side.b.Nozzle == 0x02){
            for(uint8 x = 0; x < 9; x ++){    			
                PRINTER_A_PutChar(msn_diesel[x]);  
    		}
        }
        if(side.b.Nozzle == 0x03){
            for(uint8 x = 0; x < 9; x ++){
    			PRINTER_A_PutChar(msn_extra[x]);
    		}
        }
        PRINTER_A_PutChar(LINE_FEED); 	
		for(uint8 x = 0; x < 13; x ++){
			PRINTER_A_PutChar(msn_ppu[x]);
		}
		PRINTER_A_PutChar(PRN_CURRENCY[0]);  
		PRINTER_A_PutChar(' ');
		for(uint8 x = 1; x < 6; x ++){
			PRINTER_A_PutChar(side.b.ProcessedPPU[side.b.Nozzle-1][x]);//side.b.Nozzle - 1
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
		for(uint8 x = 0; x < 7; x ++){
			PRINTER_A_PutChar(side.b.ProcessedmoneySale[x]);
		}		
		PRINTER_A_PutChar(LINE_FEED);
        side.b.Copy = 1;
	}

	
//    if(address == side.c.dir){
//        if(side.c.Copy == 1){
//            for(uint8 x = 0; x < 32; x ++){
//        		PRINTER_A_PutChar(msn_copy[x]);
//        	}
//            PRINTER_A_PutChar(LINE_FEED);
//            for(uint8 x = 0; x < 32; x ++){
//        		PRINTER_A_PutChar(SEPARATOR[x]);
//        	}            
//        }
//        PRINTER_A_PutChar(LINE_FEED);
//        for(uint8 x = 0; x < 13; x ++){
//    		PRINTER_A_PutChar(msn_venta[x]);
//    	}
//        if(side.c.Copy == 1){
//           for(uint8 x = 5; x > 0; x --){
//        		PRINTER_A_PutChar(sale_number_copy[x]);
//        	} 
//        }else{
//            for(uint8 x = 5; x > 0; x --){
//        		PRINTER_A_PutChar(sale_number[x]);
//        	}
//            int_salenumber++;
//        }
//        PRINTER_A_PutChar(LINE_FEED);
//    	for(uint8 x = 0; x < 13; x ++){
//    		PRINTER_A_PutChar(msn_placa[x]);
//    	}
//		for(uint8 x = 0; x < 8; x ++){
//			PRINTER_A_PutChar(side.c.msn_plate[x]);
//		}  
//        PRINTER_A_PutChar(LINE_FEED);
//        for(uint8 x = 0; x < 13; x ++){
//			PRINTER_A_PutChar(PRN_MILLEAGE[x]);
//		}          
//        for(uint8 x = 0; x <6; x ++){
//			PRINTER_A_PutChar(side.c.km[x]);
//		}  
//        PRINTER_A_PutChar(LINE_FEED);
//		for(uint8 x = 0; x < 13; x ++){
//			PRINTER_A_PutChar(msn_pos[x]);
//		}
//		PRINTER_A_PutChar((side.c.dir)+ 0x31);
//		PRINTER_A_PutChar(LINE_FEED);
//		for(uint8 x = 0; x < 13; x ++){
//			PRINTER_A_PutChar(msn_product[x]);
//		}
//        
//		for(uint8 x = 0; x < 9; x ++){
//			PRINTER_A_PutChar(msn_diesel[x]);
//		}
//        
//        PRINTER_A_PutChar(LINE_FEED); 	
//		for(uint8 x = 0; x < 13; x ++){
//			PRINTER_A_PutChar(msn_ppu[x]);
//		}
//		PRINTER_A_PutChar(PRN_CURRENCY[0]);  
//		PRINTER_A_PutChar(' ');
//		for(uint8 x = 1; x < 6; x ++){
//			PRINTER_A_PutChar(side.c.ProcessedPPU[side.c.Nozzle-1][x]);
//		}
//        PRINTER_A_PutChar(LINE_FEED);
//		for(uint8 x = 0; x < 13; x ++){
//			PRINTER_A_PutChar(msn_vol[x]);
//		}
//		for(uint8 x = 0; x < 8; x ++){
//			if((8-x) == DecVol)
//				PRINTER_A_PutChar('.');
//			PRINTER_A_PutChar(side.c.ProcessedvolumeSale[x]);        
//		}
//		PRINTER_A_PutChar(' ');
//		PRINTER_A_PutChar(VolSimbol[0]);
//		PRINTER_A_PutChar(LINE_FEED); 
//		for(uint8 x = 0; x < 13; x ++){
//			PRINTER_A_PutChar(msn_din[x]);
//		}     
//		PRINTER_A_PutChar(PRN_CURRENCY[0]);  
//		PRINTER_A_PutChar(' ');
//		for(uint8 x = 0; x < 8; x ++){
//			PRINTER_A_PutChar(side.c.ProcessedmoneySale[x]);
//		}		
//		PRINTER_A_PutChar(LINE_FEED);
//        side.c.Copy = 1;
//	}
//	
//    if(address == side.d.dir){
//        if(side.d.Copy == 1){
//            for(uint8 x = 0; x < 32; x ++){
//        		PRINTER_A_PutChar(msn_copy[x]);
//        	}
//            PRINTER_A_PutChar(LINE_FEED);
//            for(uint8 x = 0; x < 32; x ++){
//        		PRINTER_A_PutChar(SEPARATOR[x]);
//        	}            
//        }
//        PRINTER_A_PutChar(LINE_FEED);
//        for(uint8 x = 0; x < 13; x ++){
//    		PRINTER_A_PutChar(msn_venta[x]);
//    	}
//        if(side.d.Copy == 1){
//           for(uint8 x = 5; x > 0; x --){
//        		PRINTER_A_PutChar(sale_number_copy[x]);
//        	} 
//        }else{
//            for(uint8 x = 5; x > 0; x --){
//        		PRINTER_A_PutChar(sale_number[x]);
//        	}
//            int_salenumber++;
//        }
//        PRINTER_A_PutChar(LINE_FEED);
//    	for(uint8 x = 0; x < 13; x ++){
//    		PRINTER_A_PutChar(msn_placa[x]);
//    	}
//		for(uint8 x = 0; x < 8; x ++){
//			PRINTER_A_PutChar(side.d.msn_plate[x]);
//		}  
//        PRINTER_A_PutChar(LINE_FEED);
//        for(uint8 x = 0; x < 13; x ++){
//			PRINTER_A_PutChar(PRN_MILLEAGE[x]);
//		}          
//        for(uint8 x = 0; x <6; x ++){
//			PRINTER_A_PutChar(side.d.km[x]);
//		}  
//        PRINTER_A_PutChar(LINE_FEED);
//		for(uint8 x = 0; x < 13; x ++){
//			PRINTER_A_PutChar(msn_pos[x]);
//		}
//		PRINTER_A_PutChar((side.d.dir)+ 0x31);
//		PRINTER_A_PutChar(LINE_FEED);
//		for(uint8 x = 0; x < 13; x ++){
//			PRINTER_A_PutChar(msn_product[x]);
//		}        
//        for(uint8 x = 0; x < 9; x ++){
//			PRINTER_A_PutChar(msn_gasoline[x]);
//		}        
//        PRINTER_A_PutChar(LINE_FEED); 	
//		for(uint8 x = 0; x < 13; x ++){
//			PRINTER_A_PutChar(msn_ppu[x]);
//		}
//		PRINTER_A_PutChar(PRN_CURRENCY[0]);  
//		PRINTER_A_PutChar(' ');
//		for(uint8 x = 1; x < 6; x ++){
//			PRINTER_A_PutChar(side.d.ProcessedPPU[side.d.Nozzle-1][x]);
//		}
//        PRINTER_A_PutChar(LINE_FEED);
//		for(uint8 x = 0; x < 13; x ++){
//			PRINTER_A_PutChar(msn_vol[x]);
//		}
//		for(uint8 x = 0; x < 8; x ++){
//			if((8-x) == DecVol)
//				PRINTER_A_PutChar('.');
//			PRINTER_A_PutChar(side.d.ProcessedvolumeSale[x]);        
//		}
//		PRINTER_A_PutChar(' ');
//		PRINTER_A_PutChar(VolSimbol[0]);
//		PRINTER_A_PutChar(LINE_FEED); 
//		for(uint8 x = 0; x < 13; x ++){
//			PRINTER_A_PutChar(msn_din[x]);
//		}     
//		PRINTER_A_PutChar(PRN_CURRENCY[0]);  
//		PRINTER_A_PutChar(' ');
//		for(uint8 x = 0; x < 8; x ++){
//			PRINTER_A_PutChar(side.d.ProcessedmoneySale[x]);
//		}		
//		PRINTER_A_PutChar(LINE_FEED);
//        side.d.Copy = 1;
//	}
	

	/******FOOTER*****/
	for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(SEPARATOR[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);    
	for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(msn_footer3[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
    PRINTER_A_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(msn_footer4[x]);
	}
	PRINTER_A_PutChar(LINE_FEED);    
    for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(SEPARATOR[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(msn_footer[x]);
	}
	PRINTER_A_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 32; x ++){
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
    screen_size = 0;
    char8 lectanterior[10],lectactual[10],memoria[10],resultado[10];
    int actual,anterior,resta;
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
    while(touch1[8] == 0x00){
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
    }

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
	for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(msn_EDS[x]);   // Encabezado de estación
	}
	PRINTER_A_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(msn_EDS2[x]);
	}	
	PRINTER_A_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(msn_EDS3[x]);
	}
	PRINTER_A_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(msn_EDS4[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(msn_EDS5[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(msn_EDS6[x]);
	}
	PRINTER_A_PutChar(LINE_FEED);  
	for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(SEPARATOR[x]); //Separador de sección
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
    for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(SEPARATOR[x]);
	} 
    PRINTER_A_PutChar(LINE_FEED);
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
    PRINTER_A_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 16; x ++){
		PRINTER_A_PutChar(msn_lecact[x]);
	}
	PRINTER_A_PutChar(LINE_FEED);
    
    for(uint8 x = 0; x < 13; x ++){
		PRINTER_A_PutChar(msn_product[x]);
	}
	PRINTER_A_PutChar(LINE_FEED);	
	for(uint8 x = 0; x < 9; x ++){
		PRINTER_A_PutChar(msn_diesel[x]);
	}
    for(uint8 x = 0; x < 10; x ++){ 
        if((10-x) == DecVol)
            PRINTER_A_PutChar('.');
        PRINTER_A_PutChar(side.a.ProcessedTotals[0][0][x]);
    }
    PRINTER_A_PutChar(LINE_FEED);
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
    for(uint8 x = pos; x < 10; x++ ){
        lectactual[x-pos] = side.a.ProcessedTotals[0][0][x];
        lectanterior[x-pos] = memoria[x];
        resultado[x] = 0x00;
    }
    
    int d;
    anterior = sscanf(lectanterior,"%d",&d);
    actual = sscanf(lectactual,"%d",&d);
    resta =  actual - anterior;
    //itoa(resta,resultado,10);
    sprintf(resultado, "%d", resta);
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 18; x ++){
        PRINTER_A_PutChar(msn_dif[x]);
    }
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 10; x ++){
        if((10-x) == DecVol)
            PRINTER_A_PutChar('.');
        PRINTER_A_PutChar(resultado[x]);
    }
    for(uint8 x = 0; x < 10; x++){
        EEPROM_1_WriteByte(side.a.ProcessedTotals[0][0][x],36+x);        
    }
    PRINTER_A_PutChar(LINE_FEED);
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 16; x ++){
		PRINTER_A_PutChar(msn_lecact[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 13; x ++){  //Segundo producto
		PRINTER_A_PutChar(msn_product[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 9; x ++){
    	PRINTER_A_PutChar(msn_extra[x]);
    }
    for(uint8 x = 0; x < 10; x ++){ 
        if((10-x) == DecVol)
            PRINTER_A_PutChar('.');
        PRINTER_A_PutChar(side.a.ProcessedTotals[1][0][x]);
    }
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 16; x ++){
		PRINTER_A_PutChar(msn_lecaan[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 10; x ++){
        memoria[x] = EEPROM_1_ReadByte(56+x);
    }
    for(uint8 x = 0; x < 10; x ++){ 
        if((10-x) == DecVol)
            PRINTER_A_PutChar('.');
        PRINTER_A_PutChar(memoria[x]);
    }
    for(uint8 x = pos; x < 10; x++ ){
        lectactual[x-pos] = side.a.ProcessedTotals[1][0][x];
        lectanterior[x-pos] = memoria[x];
        resultado[x] = 0x00;
    }
    
    anterior = sscanf(lectanterior,"%d",&d);
    actual = sscanf(lectactual,"%d",&d);
    resta =  actual - anterior;
    //itoa(resta,resultado,10);
    sprintf(resultado, "%d", resta);
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 18; x ++){
        PRINTER_A_PutChar(msn_dif[x]);
    }
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 10; x ++){
        if((10-x) == DecVol)
            PRINTER_A_PutChar('.');
        PRINTER_A_PutChar(resultado[x]);
    }
    for(uint8 x = 0; x < 10; x++){
        EEPROM_1_WriteByte(side.a.ProcessedTotals[1][0][x],56+x);        
    }
//    for(uint8 x = 0; x < 13; x ++){
//		PRINTER_A_PutChar(msn_product[x]);
//	}        
//    for(uint8 x = 0; x < 9; x ++){
//		PRINTER_A_PutChar(msn_gasoline[x]);
//	}
//    for(uint8 x = 0; x < 10; x ++){ 
//        if((10-x) == DecVol)
//            PRINTER_A_PutChar('.');
//        PRINTER_A_PutChar(side.a.ProcessedTotals[2][0][x]);
//    }
//    PRINTER_A_PutChar(LINE_FEED); 	
                          
    
//    for(uint8 x = 0; x < 10; x++){
//        pos = x;
//        if(side.a.ProcessedTotals[0][0][x] != 0x00 )
//            break;
//    }
	
    
    //Lecturas lado B
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 32; x ++){
        PRINTER_A_PutChar(SEPARATOR[x]);
    }  
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 13; x ++){
        PRINTER_A_PutChar(msn_pos[x]);
    }
    PRINTER_A_PutChar(side.b.dir + 0x31);
    PRINTER_A_PutChar(LINE_FEED);
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 16; x ++){
		PRINTER_A_PutChar(msn_lecact[x]);
	}
	PRINTER_A_PutChar(LINE_FEED);
    
    for(uint8 x = 0; x < 13; x ++){
		PRINTER_A_PutChar(msn_product[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 10; x ++){
		PRINTER_A_PutChar(msn_gasoline[x]);
	}
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
    
    anterior = sscanf(lectanterior,"%d",&d);
    actual = sscanf(lectactual,"%d",&d);
    resta =  actual - anterior;
    //itoa(resta,resultado,10);
    sprintf(resultado, "%d", resta);
    for(uint8 x = 0; x < 18; x ++){
        PRINTER_A_PutChar(msn_dif[x]);
    }
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 10; x ++){
        if((10-x) == DecVol)
            PRINTER_A_PutChar('.');
        PRINTER_A_PutChar(resultado[x]);
    }
    for(uint8 x = 0; x < 10; x++){
        EEPROM_1_WriteByte(side.b.ProcessedTotals[0][0][x],46+x);        
    }
    PRINTER_A_PutChar(LINE_FEED);
    PRINTER_A_PutChar(LINE_FEED);

    for(uint8 x = 0; x < 13; x ++){
		PRINTER_A_PutChar(msn_product[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 9; x ++){
    	PRINTER_A_PutChar(msn_extra[x]);
    }
    for(uint8 x = 0; x < 10; x ++){ 
        if((10-x) == DecVol)
            PRINTER_A_PutChar('.');
        PRINTER_A_PutChar(side.b.ProcessedTotals[1][0][x]);
    }
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 16; x ++){
		PRINTER_A_PutChar(msn_lecaan[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 10; x ++){
        memoria[x] = EEPROM_1_ReadByte(66+x);
    }
    for(uint8 x = 0; x < 10; x ++){ 
        if((10-x) == DecVol)
            PRINTER_A_PutChar('.');
        PRINTER_A_PutChar(memoria[x]);
    }
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = pos; x < 10; x++ ){
        lectactual[x-pos] = side.b.ProcessedTotals[1][0][x];
        lectanterior[x-pos] = memoria[x];
        resultado[x] = 0x00;
    }
    
    anterior = sscanf(lectanterior,"%d",&d);
    actual = sscanf(lectactual,"%d",&d);
    resta =  actual - anterior;
    //itoa(resta,resultado,10);
    sprintf(resultado, "%d", resta);
    for(uint8 x = 0; x < 18; x ++){
        PRINTER_A_PutChar(msn_dif[x]);
    }
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 10; x ++){
        if((10-x) == DecVol)
            PRINTER_A_PutChar('.');
        PRINTER_A_PutChar(resultado[x]);
    }
    for(uint8 x = 0; x < 10; x++){
        EEPROM_1_WriteByte(side.b.ProcessedTotals[1][0][x],66+x);        
    }
    PRINTER_A_PutChar(LINE_FEED);
    
    
//    for(uint8 x = 0; x < 13; x ++){
//		PRINTER_A_PutChar(msn_product[x]);
//	}        
//    PRINTER_A_PutChar(LINE_FEED);
//    for(uint8 x = 0; x < 9; x ++){
//		PRINTER_A_PutChar(msn_diesel[x]);
//	}
//    for(uint8 x = 0; x < 10; x ++){ 
//        if((10-x) == DecVol)
//            PRINTER_A_PutChar('.');
//        PRINTER_A_PutChar(side.b.ProcessedTotals[0][1][x]);
//    }
//    PRINTER_A_PutChar(LINE_FEED); 
	
    
    if((Positions-1)>= 3){
        for(uint8 x = 0; x < 32; x ++){
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
        for(uint8 x = 0; x < 32; x ++){
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
	for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(SEPARATOR[x]);
	}
	PRINTER_A_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(msn_footer[x]);
	}
	PRINTER_A_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(msn_footer2[x]);
	}           
    PRINTER_A_PutChar(LINE_FEED);
    PRINTER_A_PutChar(LINE_FEED);
    PRINTER_A_PutChar(LINE_FEED);
    PRINTER_A_PutChar(LINE_FEED);
    PRINTER_A_PutChar(LINE_FEED);
    
}

void PrintPOS(uint8 address){
    int8 screen_size, date_sum;  
    screen_size = 0;
    sale_number[5]=(int_salenumber/10000)+48;
	sale_number[4]=((int_salenumber%10000)/1000)+48;
	sale_number[3]=(((int_salenumber%10000)%1000)/100)+48;
	sale_number[2]=((((int_salenumber%10000)%1000)%100)/10)+48;	
	sale_number[1]=((((int_salenumber%10000)%1000)%100)%10)+48;
    for(uint8 i = 1; i<6; i++){
        EEPROM_1_WriteByte(sale_number[i],i);
    }
    do {
        for(uint8 LCDRx = 0; LCDRx < 6; LCDRx++){            
            screen_PutChar(ASK_TIME[LCDRx]);
        }
        CyDelay(4);
        screen_size = screen_GetRxBufferSize();
    }while(screen_size < 13);
    
    if(screen_size >= 13 ){
        for(uint8 LCDRx = 0; LCDRx < screen_size; LCDRx++){
            touch1[LCDRx] = screen_ReadRxData();
        }
        screen_size = 0;
    }
    /* Encabezado */
    for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(msn_POS[x]);
	}    
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 13; x ++){
		PRINTER_A_PutChar(msn_venta[x]);
	}
    if(address == side.a.dir){    
        if(side.a.Copy == 1){
            for(uint8 x = 0; x < 4; x ++){
        		PRINTER_A_PutChar(prefijo[x]);
            }
            for(uint8 x = 5; x > 0; x --){
        		PRINTER_A_PutChar(sale_number_copy[x]);
            } 
        }else{
            for(uint8 x = 0; x < 4; x ++){
        		PRINTER_A_PutChar(prefijo[x]);
            }
            for(uint8 x = 5; x > 0; x --){
        		PRINTER_A_PutChar(sale_number[x]);
        	}
            int_salenumber++;
        }
    }
    PRINTER_A_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(SEPARATOR[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(msn_EDS[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(msn_EDS2[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(msn_EDS3[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(msn_EDS4[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(msn_EDS5[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(msn_EDS6[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(SEPARATOR[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
    /********** FECHA ***************/
	for(uint8 x = 0; x < 13; x ++){
		PRINTER_A_PutChar(msn_fecha[x]);
	}    
    date_copy[0] = (touch1[8] >> 4) + 0x30;
    date_copy[1] = (touch1[8] & 0x0F) + 0x30;
    date_copy[2] = (touch1[7] >> 4) + 0x30;
    date_copy[3] = (touch1[7] & 0x0F) + 0x30;
    date_copy[4] = (touch1[6] >> 4) + 0x30;
    date_copy[5] = (touch1[6] & 0x0F) + 0x30;
        
    
    hour_copy[0] = (touch1[10] >> 4) + 0x30;
    hour_copy[1] = (touch1[10] & 0x0F) + 0x30;
    hour_copy[2] = (touch1[11] >> 4) + 0x30;
    hour_copy[3] = (touch1[11] & 0x0F) + 0x30;
    hour_copy[4] = (touch1[12] >> 4) + 0x30;
    hour_copy[5] = (touch1[12] & 0x0F) + 0x30;
    
    if(address == side.a.dir){
        if(side.a.Copy == 1){
            PRINTER_A_PutChar(date_copy[0]);
            PRINTER_A_PutChar(date_copy[1]);
            PRINTER_A_PutChar('/');
            PRINTER_A_PutChar(date_copy[2]);
            PRINTER_A_PutChar(date_copy[3]);
            PRINTER_A_PutChar('/');
            PRINTER_A_PutChar(date_copy[4]);
            PRINTER_A_PutChar(date_copy[5]);
            
            PRINTER_A_PutChar(LINE_FEED);
            
            /********** HORA ***************/
        	for(uint8 x = 0; x < 13; x ++){
        		PRINTER_A_PutChar(msn_hora[x]);
        	}
            
            PRINTER_A_PutChar(hour_copy[0]);
            PRINTER_A_PutChar(hour_copy[1]);
            PRINTER_A_PutChar(':');
            PRINTER_A_PutChar(hour_copy[2]);
            PRINTER_A_PutChar(hour_copy[3]);
            PRINTER_A_PutChar(':');
            PRINTER_A_PutChar(hour_copy[4]);
            PRINTER_A_PutChar(hour_copy[5]);
            
            PRINTER_A_PutChar(LINE_FEED);
            
            for(uint8 x = 0; x < 32; x ++){
        		PRINTER_A_PutChar(SEPARATOR[x]);
        	}
            
            for(uint8 x = 0; x < 32; x ++){
        		PRINTER_A_PutChar(msn_copy[x]);
        	}
            PRINTER_A_PutChar(LINE_FEED);
            for(uint8 x = 0; x < 32; x ++){
        		PRINTER_A_PutChar(SEPARATOR[x]);
        	} 
        }else{
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
            for(uint8 x = 0; x < 32; x ++){
        		PRINTER_A_PutChar(SEPARATOR[x]);
        	}
            for(uint8 x = 0; x < 32; x ++){
        		PRINTER_A_PutChar(msn_original[x]);
        	}
            PRINTER_A_PutChar(LINE_FEED);
            for(uint8 x = 0; x < 32; x ++){
        		PRINTER_A_PutChar(SEPARATOR[x]);
        	}
        }
        PRINTER_A_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 13; x ++){
			PRINTER_A_PutChar(equipo[x]);
		}
        PRINTER_A_PutChar(LINE_FEED);
		for(uint8 x = 0; x < 13; x ++){
			PRINTER_A_PutChar(msn_pos[x]);
		}
        PRINTER_A_PutChar((side.a.dir)+ 0x31);
		PRINTER_A_PutChar(LINE_FEED);        
        for(uint8 x = 0; x < 32; x ++){
    		PRINTER_A_PutChar(SEPARATOR[x]);
    	}
        PRINTER_A_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 9; x ++){
			PRINTER_A_PutChar(msn_POS2[x]);
		}
        for(uint8 x = 0; x < 20; x ++){
			PRINTER_A_PutChar(side.a.msn_plate[x]);
		} 
        PRINTER_A_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 9; x ++){
			PRINTER_A_PutChar(msn_POS3[x]);
		}
        for(uint8 x = 0; x <20; x ++){
			PRINTER_A_PutChar(side.a.km[x]); //CC / NIT
		}
        PRINTER_A_PutChar(LINE_FEED);        
        for(uint8 x = 0; x < 32; x ++){
    		PRINTER_A_PutChar(SEPARATOR[x]);
    	}
        PRINTER_A_PutChar(LINE_FEED);  
        for(uint8 x = 0; x < 32; x ++){
    		PRINTER_A_PutChar(msn_POS4[x]);
    	}
        PRINTER_A_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 32; x ++){
    		PRINTER_A_PutChar(msn_POS5[x]);
    	}
        PRINTER_A_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 32; x ++){
    		PRINTER_A_PutChar(msn_POS6[x]);
    	}
        PRINTER_A_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 32; x ++){
    		PRINTER_A_PutChar(SEPARATOR[x]);
    	}
        PRINTER_A_PutChar(LINE_FEED);
        if(side.a.Nozzle == 0x01){
    		for(uint8 x = 0; x < 9; x ++){
    			PRINTER_A_PutChar(msn_diesel[x]);
    		}
        }
        if(side.a.Nozzle == 0x02){
            for(uint8 x = 0; x < 9; x ++){
    			PRINTER_A_PutChar(msn_gasoline[x]);
    		}
        }
        if(side.a.Nozzle == 0x03){
            for(uint8 x = 0; x < 9; x ++){
    			
                PRINTER_A_PutChar(msn_extra[x]);
    		}
        } 
        PRINTER_A_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 8; x ++){
			if((8-x) == DecVol)
				PRINTER_A_PutChar('.');
			PRINTER_A_PutChar(side.a.ProcessedvolumeSale[x]);        
		}
		PRINTER_A_PutChar(' ');        
		PRINTER_A_PutChar(VolSimbol[0]); //Volumen
        PRINTER_A_PutChar(' ');
        PRINTER_A_PutChar(' ');
        PRINTER_A_PutChar(' ');
        PRINTER_A_PutChar(' ');
        PRINTER_A_PutChar(' ');     
        PRINTER_A_PutChar(PRN_CURRENCY[0]);  //Precio		   
		for(uint8 x = 1; x < 6; x ++){
			PRINTER_A_PutChar(side.a.ProcessedPPU[side.a.Nozzle-1][x]);
		}
        PRINTER_A_PutChar(' ');
        PRINTER_A_PutChar(' ');
        PRINTER_A_PutChar(' ');
        PRINTER_A_PutChar(' ');
        
        PRINTER_A_PutChar(PRN_CURRENCY[0]);  //Importe		       
		for(uint8 x = 0; x < 8; x ++){
			PRINTER_A_PutChar(side.a.ProcessedmoneySale[x]);
		}		
		side.a.Copy = 1;
        PRINTER_A_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 32; x ++){
    		PRINTER_A_PutChar(SEPARATOR[x]);
    	}
        PRINTER_A_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 32; x ++){
    		PRINTER_A_PutChar(msn_POS7[x]); //Discriminacion de impuestos
    	}
        PRINTER_A_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 32; x ++){
    		PRINTER_A_PutChar(msn_POS8[x]); //Base       Impuesto
    	}
        PRINTER_A_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 13; x ++){
    		PRINTER_A_PutChar(msn_POS9[x]);
    	}
        PRINTER_A_PutChar(' '); 
        PRINTER_A_PutChar(PRN_CURRENCY[0]);  //Importe
		PRINTER_A_PutChar(' ');        
		for(uint8 x = 0; x < 8; x ++){
			PRINTER_A_PutChar(side.a.ProcessedmoneySale[x]);
		}
        PRINTER_A_PutChar(' '); 
        PRINTER_A_PutChar(' '); 
        PRINTER_A_PutChar(' '); 
        PRINTER_A_PutChar('$');
        PRINTER_A_PutChar('0');
        PRINTER_A_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 13; x ++){
    		PRINTER_A_PutChar(msn_POS10[x]);
    	}
        PRINTER_A_PutChar(PRN_CURRENCY[0]);  //Importe
		PRINTER_A_PutChar(' ');        
		for(uint8 x = 0; x < 8; x ++){
			PRINTER_A_PutChar(side.a.ProcessedmoneySale[x]);
		}
        PRINTER_A_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 13; x ++){
    		PRINTER_A_PutChar(msn_POS11[x]);
    	}
        PRINTER_A_PutChar('$');
        PRINTER_A_PutChar('0');
        PRINTER_A_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 13; x ++){
    		PRINTER_A_PutChar(msn_POS12[x]);
    	}
        PRINTER_A_PutChar(PRN_CURRENCY[0]);  //Importe
		PRINTER_A_PutChar(' ');        
		for(uint8 x = 0; x < 8; x ++){
			PRINTER_A_PutChar(side.a.ProcessedmoneySale[x]);
		}
        PRINTER_A_PutChar(LINE_FEED);
        for(uint8 x = 0; x < 32; x ++){
    		PRINTER_A_PutChar(SEPARATOR[x]);
    	}
        
    } /*Posicion a*/
    
    
    /* Resolución */
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(msn_POS13[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(msn_POS14[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(msn_POS15[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(msn_POS16[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(msn_POS17[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(msn_POS18[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(msn_POS19[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
    for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(SEPARATOR[x]);
	}
    /******FOOTER*****/	
    PRINTER_A_PutChar(LINE_FEED);    
	for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(msn_footer3[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
    PRINTER_A_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(msn_footer4[x]);
	}
	PRINTER_A_PutChar(LINE_FEED);    
    for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(SEPARATOR[x]);
	}
    PRINTER_A_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 32; x ++){
		PRINTER_A_PutChar(msn_footer[x]);
	}
	PRINTER_A_PutChar(LINE_FEED);
	for(uint8 x = 0; x < 32; x ++){
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



/* [] END OF FILE */
