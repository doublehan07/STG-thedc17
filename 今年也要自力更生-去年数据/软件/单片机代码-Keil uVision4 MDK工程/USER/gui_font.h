#ifndef _GUI_FONT_H
#define _GUI_FONT_H


#include "stm32f10x.h"
#include "gui.h"
#include "ff.h"


extern FIL kt16_file;
extern GUI_FONT kt16;
extern GUI_XBF_DATA kt16XBF_DATA;

extern FIL kt24_file;
extern GUI_FONT kt24;
extern GUI_XBF_DATA kt24XBF_DATA;

extern FIL kt32_file;
extern GUI_FONT kt32;
extern GUI_XBF_DATA kt32XBF_DATA;

extern FIL kt40_file;
extern GUI_FONT kt40;
extern GUI_XBF_DATA kt40XBF_DATA;

extern FIL kt56_file;
extern GUI_FONT kt56;
extern GUI_XBF_DATA kt56XBF_DATA;


void FontInit(void);

#endif


