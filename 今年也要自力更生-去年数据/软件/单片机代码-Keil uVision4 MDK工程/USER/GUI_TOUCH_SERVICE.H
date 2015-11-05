#ifndef _GUI_TOUCH_SERVICE_H
#define _GUI_TOUCH_SERVICE_H

#include "stm32f10x.h"
#include "includes.h"
#include "gui.h"

#define GUI_TOUCH_TASK_PRIO       		4 
#define GUI_TOUCH_STK_SIZE  					32
extern OS_STK GUI_TOUCH_TASK_STK[GUI_TOUCH_STK_SIZE];
void GUI_TOUCH_task(void *pdata);

#endif

