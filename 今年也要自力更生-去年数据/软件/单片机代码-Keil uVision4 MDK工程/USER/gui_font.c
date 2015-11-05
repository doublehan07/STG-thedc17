#include "gui_font.h"

#include "ff.h"
#include "comunication.h"
#include "spi_memory.h"

uint32_t kt24_addr=4*1024*1024;
GUI_FONT kt24;
GUI_XBF_DATA kt24XBF_DATA;

uint32_t kt16_addr=0;
GUI_FONT kt16;
GUI_XBF_DATA kt16XBF_DATA;


 int _cbGetData(U32 Off, U16 numbytes, uint32_t *fileaddr, void *pbuf)
{
	SPIFlash_Read(pbuf, (*fileaddr)+Off, numbytes);

	
	return 0;

}


void CreateXBF_Fontkt16(void)
{
	GUI_XBF_CreateFont(&kt16, &kt16XBF_DATA, GUI_XBF_TYPE_PROP, _cbGetData, &kt16_addr);
}

void FontInit(void)
{
	CreateXBF_Fontkt16();
	//CreateXBF_Fontkt24();
	//CreateXBF_Fontkt32();
	//CreateXBF_Fontkt40();
	//CreateXBF_Fontkt56();
	GUI_UC_SetEncodeUTF8();  
	
	
}



