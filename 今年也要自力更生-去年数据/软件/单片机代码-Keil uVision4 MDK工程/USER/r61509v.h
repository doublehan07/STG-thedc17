#ifndef _R61509V_H
#define _R61509V_H

#include "stm32f10x.h"

void R61509V_Init(void);
void R61509V_DrawRectangle(uint16_t color, uint16_t x1,uint16_t x2, uint16_t y1,uint16_t y2);
void R61509V_PutData(uint16_t *color, uint16_t x1,uint16_t x2, uint16_t y1,uint16_t y2);

#endif


