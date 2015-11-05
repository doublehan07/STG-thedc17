#ifndef _MOTOR_CTR_H
#define _MOTOR_CTR_H

#include "stm32f10x.h"

void MOTOR_Init(void);
void Motor_Exec(void);
void MOTOR_Voltage_Control(void);

extern int16_t MotorVoltage[4];
extern uint8_t PWMInited;
extern uint16_t FullVoltage[2];
extern int16_t dV[2];
extern volatile int16_t MotorSpeed[2];
extern volatile int16_t MotorSpeedSet[2];
extern volatile uint16_t MSZV[4];
//extern volatile uint16_t MSMR[4];

#endif

