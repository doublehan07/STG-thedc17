#ifndef _KEY_H
#define _KEY_H

#include "stm32f10x.h"

void Key_Init(void);
void Key_Exec(void);


extern uint8_t KeyState[2];
extern uint8_t KeyReleaseState[2];
extern uint32_t KeyPressedTime[2];

#endif

