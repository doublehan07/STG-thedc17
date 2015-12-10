#ifndef __MOTOR_H
#define __MOTOR_H
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include <stdbool.h>
#include "usart.h"
#include "pid.h"
#include "adc_dma.h"
#include <stdio.h>
#include "math.h"

#define Kp 5

#define PHASE1_on() GPIO_SetBits(GPIOA,GPIO_Pin_7) 
#define PHASE1_off() GPIO_ResetBits(GPIOA,GPIO_Pin_7) 

#define PHASE2_on() GPIO_SetBits(GPIOE,GPIO_Pin_12) 
#define PHASE2_off() GPIO_ResetBits(GPIOE,GPIO_Pin_12) 

#define nSLEEP1_on() GPIO_SetBits(GPIOA,GPIO_Pin_6) 
#define nSLEEP1_off() GPIO_ResetBits(GPIOA,GPIO_Pin_6) 

#define nSLEEP2_on() GPIO_SetBits(GPIOB,GPIO_Pin_1) 
#define nSLEEP2_off() GPIO_ResetBits(GPIOB,GPIO_Pin_1) 

//extern uint16_t ADC1ConvertedValue[2];

//typedef void (*FunType)(void);

//FunType fp ;

//init motor drv8801
//void motor_config(FunType);
void motor_config(void);
//set speed
//void motor_setSpeed(uint32_t);
//set direction
//void motor_setDirection(bool);

void motor_sleep(void);

void motor_wake(void);

//enable
//void motor_enable(void);
//brake
void motor_brake(void);

void motor_setSpeed(uint32_t speed);

void motor_setLeftSpeed(uint32_t speed);

void motor_setRightSpeed(uint32_t speed);

void motor_forward(void);

void motor_backward(void);

void motor_left_forward(void);

void motor_right_forward(void);

void motor_left_backward(void);

void motor_right_backward(void);

void motor_turn_left(int speed , int delta_speed);

void motor_turn_right(int speed , int delta_speed);

uint16_t get_left_adcVal(void);

uint16_t get_right_adcVal(void);

void headTo(char x , char y , int16_t angleErr);



#endif

