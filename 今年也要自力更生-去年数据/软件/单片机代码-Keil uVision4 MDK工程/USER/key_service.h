#ifndef _KEY_SERVICE_H
#define _KEY_SERVICE_H

#include "stm32f10x.h"
#include "key.h"
#include "includes.h"

#define KEY_TASK_PRIO       		4 
#define KEY_STK_SIZE  					64
extern OS_STK KEY_TASK_STK[KEY_STK_SIZE];
void Key_task(void *pdata);


#endif


