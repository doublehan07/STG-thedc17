//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�о�԰����
//���̵�ַ��http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��    ��   : HuangKai
//  ��������   : 2014-0101
//  ����޸�   : 
//  ��������   : OLED 4�ӿ���ʾ����(51ϵ��)
//              ˵��: 
//              ----------------------------------------------------------------
//              GND    ��Դ��
//              VCC  ��5V��3.3v��Դ
//              D0   ��PD6��SCL��
//              D1   ��PD7��SDA��
//              RES  ��PD4
//              DC   ��PD5
//              CS   ��PD3               
//              ----------------------------------------------------------------
// �޸���ʷ   :
// ��    ��   : 
// ��    ��   : HuangKai
// �޸�����   : �����ļ�
//��Ȩ���У�����ؾ���
//Copyright(C) �о�԰����2014/3/16
//All rights reserved
//******************************************************************************/
#ifndef __SPIOLED_H
#define __SPIOLED_H			  	 

#include "stm32f10x.h"

#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  



 		     
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����


//OLED�����ú���
void SPIOLED_WRByte(uint8_t dat,uint8_t cmd);	    
void SPIOLED_DisplayOn(void);
void SPIOLED_DisplayOff(void);	   							   		    
void SPIOLED_Init(void);
void SPIOLED_Clear(void);
void SPIOLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void SPIOLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void SPIOLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr);
void SPIOLED_ShowNum(uint8_t x,uint8_t y,u32 num,uint8_t len,uint8_t size);
void SPIOLED_ShowString(uint8_t x,uint8_t y, uint8_t *p);	 
void SPIOLED_Set_Pos(unsigned char x, unsigned char y);
void SPIOLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);
void SPIOLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void SPIOLED_WriteMem(uint16_t color, uint16_t x, uint16_t y);
#endif  
	 



