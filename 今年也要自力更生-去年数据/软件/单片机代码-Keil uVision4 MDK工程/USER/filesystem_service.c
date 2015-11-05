#include "filesystem_service.h"
#include "exfuns.h"
#include "comunication.h"

void FileSystem_Service_Init(void)
{
	if(f_mount(0,&fs))
		USARTSendString("load flash error");
}

