#ifndef _COMUNICATION_H
#define _COMUNICATION_H

#include "stm32f10x_conf.h"
#include "stm32f10x.h"

//#include "sysconfig.h"



void USARTInit(uint32_t bitrate);
void USART2Init(uint32_t bitrate);
void USARTSendData(uint8_t *p, uint32_t size);
void USARTSendString(uint8_t *s);
uint8_t USARTWaitData(void);
void USARTSendFigDec(uint32_t fig,uint8_t a);
void USARTSendOneChar(uint8_t d);

void USARTSendSysFrequency(void);
uint8_t USARTWaitString(uint8_t *s,uint8_t size);
void USARTWaitArray(uint8_t *array, uint16_t size);
void USARTDMAInit(uint8_t *buff, uint16_t size);
void USARTWriteFile(uint8_t *sfilename, uint16_t size); //size=byte*512
void USARTWriteFileDirect(uint32_t address, uint16_t size); //size=byte*512


#endif

