



#ifndef _TFT_ILI9486_H
#define _TFT_ILI9486_H
#include "stm32f10x.h"




#define ROW  480			//????
#define COL  320			//????

#define  TFT_CTL GPIOC
#define  TFT_DB  GPIOB

#define  CS0  GPIO_Pin_12
#define  RS   GPIO_Pin_11
#define  WR0  GPIO_Pin_10
#define  RD0  GPIO_Pin_9
#define  RST  GPIO_Pin_8




//---------------------------------------------------------------------

void TFT_Delay(uint32_t t);
void WriteComm(uint16_t i);
void WriteData(uint16_t i);
void LCDInit(void);
void BlockWrite(uint16_t Xstart,uint16_t Xend,uint16_t Ystart,uint16_t Yend) ;
void DrawBlock(uint16_t color,uint16_t x0, uint16_t x1, uint16_t y0, uint16_t y1);
uint16_t ReadStatus(void);
uint16_t ReadData(void);
void WriteMemory(uint16_t *color, uint16_t x0, uint16_t x1, uint16_t y0, uint16_t y1);
void ReadMemory(uint16_t *color, uint16_t x0, uint16_t x1, uint16_t y0, uint16_t y1);




#endif
