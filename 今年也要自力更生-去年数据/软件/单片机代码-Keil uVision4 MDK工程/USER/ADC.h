#ifndef _ADC_H
#define _ADC_H

#include "stm32f10x_conf.h"
#include "stm32f10x.h"



void ADC_Init_User(void);


#define ADC_IN8         0
#define ADC_IN9					1
#define ADC_MOTOR1			2
#define ADC_MOTOR2			3
#define ADC_MOTOR3			4
#define ADC_MOTOR4			5
#define ADC_AIN14				6
#define ADC_AIN15				7
#define ADC_TEMPERATURE 8
#define ADC_VCC		      9



extern volatile uint16_t ADC_Voltage[10];


#endif

