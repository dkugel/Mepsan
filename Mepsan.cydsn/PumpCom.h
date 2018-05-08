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


uint8 GetAddress (uint8 dir);
uint8 PumpState(uint8 address);
uint8 Authorize(uint8 address);
uint8 ReturnStatus(uint8 address);
uint8 PriceUpdate(uint8 address, uint8 *price);
void GetResponse(void);
void TotalRequest(uint8 address, uint8 type, uint8 nozzle);
void GetTotal(void);
void GetACK(void);
void ProccessResponse(uint8 address);

/* [] END OF FILE */