#ifndef _GUI_SERVICE_H
#define _GUI_SERVICE_H

#include "stm32f10x.h"
#include "includes.h"

void GUI_Service_Init(void);


#define GUI_TASK_PRIO       		8 
#define GUI_STK_SIZE  					1536
extern OS_STK GUI_TASK_STK[GUI_STK_SIZE];
void GUI_task(void *pdata);


#endif


