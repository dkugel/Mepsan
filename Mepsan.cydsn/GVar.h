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

/*
*********************************************************************************************************
*
*                            Estructura dispensador
*          
*********************************************************************************************************
*/

struct position{
    uint8 dir;                      //Direccion
    uint8 totalsNozzle[4][3][15];   //Totales Manguera: [0-3] // [0]=Volumen [1]=Dinero [2]=PPU // [8 ó 12 - 4 ó 5]
    uint8 ppuNozzle[4][8];         //PPU autorizados [Manguera 0-3][Datos]
    uint8 moneySale[14];            //Venta de Dinero vendido
    uint8 volumeSale[14];           //Venta de Volumen vendido
    uint8 ppuSale[14];              //Venta de PPU vendido
    uint8 productSale;              //Venta de Producto vendido
};

struct pump{
   struct position a;
   struct position b;
   struct position c;
   struct position d;
};

struct pump side;       //lado del surtidor