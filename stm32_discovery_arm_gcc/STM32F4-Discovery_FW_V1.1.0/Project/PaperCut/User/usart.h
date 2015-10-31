#ifndef __USART_H
#define __USART_H
#include "stm32f4xx_usart.h"
#include "strategy.h"
void USART2_puts(char*);
void USART2_Configuration(void);
void USART2_IRQHandler(void);
void USART5_puts(char*);
void USART5_Configuration(void);
void USART5_IRQHandler(void);
#endif
