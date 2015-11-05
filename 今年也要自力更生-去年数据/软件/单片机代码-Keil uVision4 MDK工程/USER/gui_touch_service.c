#include "gui_touch_service.h"

OS_STK GUI_TOUCH_TASK_STK[GUI_TOUCH_STK_SIZE];
void GUI_TOUCH_task(void *pdata)
{
	while(1)
	{
		GUI_TOUCH_Exec();
		OSTimeDly(10);
	}
}

