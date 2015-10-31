#ifndef __MOTOR_H
#define __MOTOR_H
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include <stdbool.h>
//init motor drv8801
void motor_config(void);
//set speed
void motor_setSpeed(uint32_t);
//set direction
void motor_setDirection(bool);
//enable
void motor_enable(void);
#endif
