#ifndef _IIC_H
#define _IIC_H

#include "stm32f10x.h"

extern volatile float Gyro;
extern uint8_t IIC_OK;
void IIC_Init(void);

void W_6050(uint8_t REG_add,uint8_t REG_data);
uint8_t R_6050(uint8_t REG_add);
int16_t R_6050_2(uint8_t REG_add);

void InitMPU6050(void);
void ReInitMPU6050(void);
void MakeAdjustMent(void);

void IIC_Exec(void);

#endif 


