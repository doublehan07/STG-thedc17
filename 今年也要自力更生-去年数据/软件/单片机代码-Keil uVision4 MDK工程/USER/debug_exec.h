#ifndef _DEBUG_EXEC_H
#define _DEBUG_EXEC_H

#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "comunication.h"

void senddata(uint8_t *s, uint32_t f, uint8_t fn, uint8_t fd);


#endif


