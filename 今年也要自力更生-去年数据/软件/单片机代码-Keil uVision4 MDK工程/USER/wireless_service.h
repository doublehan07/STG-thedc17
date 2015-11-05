#ifndef _WIRELESS_SERVICE_H
#define _WIRELESS_SERVICE_H


#include "stm32f10x.h"
#include "spi_nrf24.h"
#include "includes.h"

void Wireless_Service_Init(void);



#define WIRELESS_TASK_PRIO       		 2
#define WIRELESS_STK_SIZE  					128
extern OS_STK WIRELESS_TASK_STK[WIRELESS_STK_SIZE];
void Wireless_task(void *pdata);




typedef struct
{
	uint8_t status;
	double distance;
	double course;
	int16_t x;
	int16_t y;
}WirelessProc_Struct;


typedef struct 
{
	double xlabel;
	double ylabel;
	double distance;
	double angle;
}fellow;   


extern WirelessData_Struct WirelessDataRX;
extern WirelessData_Struct WirelessDataTX;
extern WirelessProc_Struct WirelessDataProc;

#endif


