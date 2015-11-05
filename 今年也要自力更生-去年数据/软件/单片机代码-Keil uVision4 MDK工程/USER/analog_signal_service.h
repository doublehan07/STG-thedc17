#ifndef _ANALOG_SIGNAL_SERVICE_H
#define _ANALOG_SIGNAL_SERVICE_H

#include "stm32f10x.h"
#include "includes.h"
#include "systime.h"

#define ANALOG_SIGNAL_TASK_PRIO       		6 
#define ANALOG_SIGNAL_STK_SIZE  					64
extern OS_STK ANALOG_SIGNAL_TASK_STK[ANALOG_SIGNAL_STK_SIZE];
void Analog_Signal_task(void *pdata);

#define yearadd 0
#define yearsub 1
#define monthadd 2
#define monthsub 3
#define dayadd 4
#define daysub 5
#define houradd 6
#define hoursub 7
#define minuteadd 8
#define minutesub 9
#define secadd 10
#define secsub 11
void SetTime(uint8_t command);

volatile extern uint16_t ADCValueProc[8];

volatile extern uint16_t Voltage[9]; //mV
volatile extern uint16_t CPUTemperature; //0.01degree
volatile extern uint16_t Temperature;  //0.125degree
volatile extern datestruct DateTime;
volatile extern uint8_t HeartRate;  //bpm
volatile extern uint8_t BatteryPercent;

#endif


