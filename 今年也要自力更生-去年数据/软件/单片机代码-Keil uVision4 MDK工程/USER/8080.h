#ifndef _8080_H
#define _8080_H


#include "stm32f10x.h"


void Port8080_Init(void);
void Port8080_SetOut(void);
void Port8080_SetIn(void);
void Port8080_Deselect(void);
void Port8080_Select(void);
void Port8080_CommandTransfer(void);
void Port8080_DataTransfer(void);
void Port8080_Write(uint16_t data);
uint16_t Port8080_Read(void);
void Port8080_WriteCommand(uint16_t command);
void Port8080_WriteData(uint16_t data);
void Port8080_RST(void);


#endif


