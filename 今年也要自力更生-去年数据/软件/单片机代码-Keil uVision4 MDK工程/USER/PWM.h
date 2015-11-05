#ifndef _PWM_H
#define _PWM_H


#include "stm32f10x.h"


void PWM_Init(void);

#define PWM_Motor_1  (TIM8->CCR1)
#define PWM_Motor_2  (TIM8->CCR2)
#define PWM_Motor_3  (TIM8->CCR3)
#define PWM_Motor_4  (TIM8->CCR4)

#define PWM_Ex_3			(TIM4->CCR3)
#define PWM_Ex_4			(TIM4->CCR4)
#endif

