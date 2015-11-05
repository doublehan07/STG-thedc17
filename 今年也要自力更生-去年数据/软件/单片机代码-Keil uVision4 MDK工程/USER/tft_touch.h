#ifndef _TFT_TOUCH_H
#define _TFT_TOUCH_H


#include "stm32f10x.h"



extern int16_t TFT_Touch_X, TFT_Touch_Y;
void TFT_Touch_Exec(void);
void TFT_Touch_Init(void);

#endif

