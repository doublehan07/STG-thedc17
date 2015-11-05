#ifndef _ANALOG_H
#define _ANALOG_H

#include "stm32f10x_conf.h"
#include "stm32f10x.h"

void BLControlInit(void);
void SetBL(uint16_t BL);
uint16_t GetADCValue(uint8_t channel);
void ADCInit(void);
void ADCInterrupt(void);


extern uint16_t ADCValue[5];


#endif

