#include "gui_service.h"

#include "dialog.h"
#include "gui_font.h"
#include "gui_pages_watch.h"
#include "wm.h"
#include "sysconfig.h"
#include "comunication.H"
#include "spi_oled.h"
#include "bmp.h"


void ChangeSkin(void)
{
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
}


//exec after fs init
void GUI_Service_Init()
{
	GUI_Init();
	FontInit();
	ChangeSkin();
	SPIOLED_DrawBMP(0,0,128,8,BMP1);
	
	CreatePage1();

}


OS_STK GUI_TASK_STK[GUI_STK_SIZE];
void GUI_task(void *pdata)
{
	while(1)
	{
		GUI_Exec();
		SPIOLED_DrawBMP(0,0,128,8,BMP1);
		OSTimeDly(100);
	}
}



