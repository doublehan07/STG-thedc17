#ifndef __MOTOR_H
#define __MOTOR_H
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include <stdbool.h>

#define PHASE1_on() GPIO_SetBits(GPIOA,GPIO_Pin_7) 
#define PHASE1_off() GPIO_ResetBits(GPIOA,GPIO_Pin_7) 

#define PHASE2_on() GPIO_SetBits(GPIOE,GPIO_Pin_12) 
#define PHASE2_off() GPIO_ResetBits(GPIOE,GPIO_Pin_12) 

#define nSLEEP1_on() GPIO_SetBits(GPIOA,GPIO_Pin_6) 
#define nSLEEP1_off() GPIO_ResetBits(GPIOA,GPIO_Pin_6) 

#define nSLEEP2_on() GPIO_SetBits(GPIOB,GPIO_Pin_1) 
#define nSLEEP2_off() GPIO_ResetBits(GPIOB,GPIO_Pin_1) 

//init motor drv8801
void motor_config(void);
//set speed
void motor_setSpeed(uint32_t);
//set direction
void motor_setDirection(bool);
//enable
void motor_enable(void);
#endif
