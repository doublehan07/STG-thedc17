#ifndef _USART_GPS_H
#define _USART_GPS_H


#include "stm32f10x.h"


#define REFRESHING 1
#define REFRESHED 0
typedef struct 
{
	uint8_t time[12];
	uint8_t status[2];
	uint8_t latitude[12];
	uint8_t ns[2];
	uint8_t longitude[12];
	uint8_t ew[2];
	uint8_t speed[8];
	uint8_t course[8];
	uint8_t date[8];
	uint8_t altitude[8];
	uint8_t refreshflag;
}GPS_Data_Struct;


extern GPS_Data_Struct GPS_Data;

void USART_GPS_Init(void);
uint8_t USART2_WaitData(void);
void USART_GPS_ISV(void);


#endif


