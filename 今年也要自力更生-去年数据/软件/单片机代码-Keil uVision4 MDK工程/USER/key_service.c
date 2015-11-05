#include "key_service.h"


OS_STK KEY_TASK_STK[KEY_STK_SIZE];
void Key_task(void *pdata)
{
	while(1)
	{
		Key_Exec();
		OSTimeDly(100);
	}
}

