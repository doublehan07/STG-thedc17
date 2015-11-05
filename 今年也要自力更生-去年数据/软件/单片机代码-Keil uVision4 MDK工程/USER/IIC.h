#ifndef _IIC_H
#define _IIC_H

#include "stm32f10x.h"

void IIC_Init(void);


volatile extern uint16_t Temperature;
void IIC_ReadLM75ATemp(void);


#endif 


