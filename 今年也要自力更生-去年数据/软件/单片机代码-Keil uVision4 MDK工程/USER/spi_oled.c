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

#include "spi_oled.h"
#include "oledfont.h"  	 
#include "public.h"


#define SPIOLED_PORT GPIOA
#define SPIOLED_CS_PIN GPIO_Pin_4
#define SPIOLED_SCK_PIN GPIO_Pin_5
#define SPIOLED_DC_PIN GPIO_Pin_6
#define SPIOLED_MOSI_PIN GPIO_Pin_7

#define SPIOLED_RST_PORT GPIOA
#define SPIOLED_RST_PIN GPIO_Pin_0

unsigned char BMP1[128*8];

void SPI1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	//clock initialize
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//??????APB2????
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//??????APB2????
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);//??????APB1????

	//gpio initialize
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//??????50MHz
	GPIO_InitStructure.GPIO_Pin = SPIOLED_MOSI_PIN | SPIOLED_SCK_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//????
	GPIO_Init(SPIOLED_PORT, &GPIO_InitStructure);//?????GPIOx???
	
	GPIO_InitStructure.GPIO_Pin = SPIOLED_CS_PIN | SPIOLED_DC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//????
	GPIO_Init(SPIOLED_PORT, &GPIO_InitStructure);//?????GPIOx???

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//??????50MHz
	GPIO_InitStructure.GPIO_Pin = SPIOLED_RST_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//????
	GPIO_Init(SPIOLED_RST_PORT, &GPIO_InitStructure);//?????GPIOx???
	
	//SPI2_initialize
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//???????
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//????SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//8????
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//?????
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//??????
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//??NSS???SSI???
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;//?????2
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//?????MSB???
	SPI_InitStructure.SPI_CRCPolynomial = 7;//??CRC???????
	SPI_Init(SPI1,&SPI_InitStructure);//?????

	SPI_Cmd(SPI1,ENABLE);//??????SPI??
}







//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 		

void SPIOLED_Select(void)
{
	GPIO_ResetBits(SPIOLED_PORT,SPIOLED_CS_PIN);
}

void SPIOLED_Deselect(void)
{
	GPIO_SetBits(SPIOLED_PORT,SPIOLED_CS_PIN);
}

void SPIOLED_SetCommand(void)
{
	GPIO_ResetBits(SPIOLED_PORT,SPIOLED_DC_PIN);	
}

void SPIOLED_SetData(void)
{
	GPIO_SetBits(SPIOLED_PORT,SPIOLED_DC_PIN);	
}


void SPIOLED_RST(void)
{
	GPIO_SetBits(SPIOLED_RST_PORT,SPIOLED_RST_PIN);	
	Delay_mS(100);
	GPIO_ResetBits(SPIOLED_RST_PORT,SPIOLED_RST_PIN);	
	Delay_mS(100);
	GPIO_SetBits(SPIOLED_RST_PORT,SPIOLED_RST_PIN);	
}

//��SSD1106д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
void SPIOLED_WRByte(uint8_t dat,uint8_t cmd)
{	
	uint8_t i;
	if(cmd)
	  SPIOLED_SetData();
	else 
	  SPIOLED_SetCommand();		  
	SPIOLED_Select();
	
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==0);
	SPI_I2S_SendData(SPI1,dat);//????SPI/I2S??????
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==0);//?????SPI/I2S???????
	i=SPI_I2S_ReceiveData(SPI1);
	
	SPIOLED_Deselect();
	SPIOLED_SetData();
} 

void SPIOLED_SetPos(uint8_t x, uint8_t y) 
{ 
	SPIOLED_WRByte(0xb0+y,OLED_CMD);
	SPIOLED_WRByte(((x&0xf0)>>4)|0x10,OLED_CMD);
	SPIOLED_WRByte((x&0x0f)|0x01,OLED_CMD); 
}   	  
/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
void SPIOLED_DrawBMP(uint8_t x0, uint8_t y0,uint8_t x1, uint8_t y1,uint8_t BMP[])
{ 	
 unsigned int j=0;
 uint8_t x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		SPIOLED_SetPos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	SPIOLED_WRByte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 
void SPIOLED_Clear(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		SPIOLED_WRByte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		SPIOLED_WRByte (0x02,OLED_CMD);      //������ʾλ�á��е͵�ַ
		SPIOLED_WRByte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)SPIOLED_WRByte(0x00,OLED_DATA); 
	} //������ʾ
}


//��ʼ��SSD1306					    
void SPIOLED_Init(void)
{ 	
	SPI1_Init();
	
	SPIOLED_RST();
 
					  
	SPIOLED_WRByte(0xAE,OLED_CMD);//--turn off oled panel
	SPIOLED_WRByte(0x02,OLED_CMD);//---set low column address
	SPIOLED_WRByte(0x10,OLED_CMD);//---set high column address
	SPIOLED_WRByte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	SPIOLED_WRByte(0x81,OLED_CMD);//--set contrast control register
	SPIOLED_WRByte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	SPIOLED_WRByte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	SPIOLED_WRByte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
	SPIOLED_WRByte(0xA6,OLED_CMD);//--set normal display
	SPIOLED_WRByte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	SPIOLED_WRByte(0x3f,OLED_CMD);//--1/64 duty
	SPIOLED_WRByte(0x81,OLED_CMD); //�Աȶ�����
	SPIOLED_WRByte(0xfF,OLED_CMD); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
	SPIOLED_WRByte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	SPIOLED_WRByte(0x00,OLED_CMD);//-not offset
	SPIOLED_WRByte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	SPIOLED_WRByte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	SPIOLED_WRByte(0xD9,OLED_CMD);//--set pre-charge period
	SPIOLED_WRByte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	SPIOLED_WRByte(0xDA,OLED_CMD);//--set com pins hardware configuration
	SPIOLED_WRByte(0x12,OLED_CMD);
	SPIOLED_WRByte(0xDB,OLED_CMD);//--set vcomh
	SPIOLED_WRByte(0x40,OLED_CMD);//Set VCOM Deselect Level
	SPIOLED_WRByte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	SPIOLED_WRByte(0x02,OLED_CMD);//
	SPIOLED_WRByte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	SPIOLED_WRByte(0x14,OLED_CMD);//--set(0x10) disable
	SPIOLED_WRByte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	SPIOLED_WRByte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	SPIOLED_WRByte(0xAF,OLED_CMD);//--turn on oled panel
	
	SPIOLED_WRByte(0xAF,OLED_CMD); /*display ON*/ 
	SPIOLED_Clear();
	SPIOLED_SetPos(0,0); 	
}  


void SPIOLED_WriteMem(uint16_t color, uint16_t x, uint16_t y)
{
	uint8_t dataput;
	uint16_t point;
	if(x)
		x--;
	else
		x=127;
	dataput=0x01<<(y%8);
	point=(y/8)*128+x;
	if(color)
		BMP1[point]|=dataput;
	else
		BMP1[point]&=~dataput;
}





























