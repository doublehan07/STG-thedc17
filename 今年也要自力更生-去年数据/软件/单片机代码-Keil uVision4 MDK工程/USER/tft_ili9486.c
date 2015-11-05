#include "tft_ili9486.h"

void TFT_Delay(uint32_t t)
{
	for(;t!=0;t--);
}



void WriteComm(uint16_t i)
{
	//TFT_CTL->BRR=CS0;
	//TFT_CTL->BSRR=RD0;
	TFT_CTL->BRR=RS;
	TFT_DB->ODR=i;
	TFT_CTL->BRR=WR0;
	TFT_CTL->BSRR=WR0;
	//TFT_CTL->BSRR=CS0;
	/*GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_WriteBit(TFT_CTL, CS0, Bit_RESET);
	GPIO_WriteBit(TFT_CTL, RD0, Bit_SET);
  GPIO_WriteBit(TFT_CTL, RS, Bit_RESET);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//??????50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//????
	GPIO_Init(TFT_DB, &GPIO_InitStructure);//?????GPIOx???  
  GPIO_Write(TFT_DB, i);
	
  GPIO_WriteBit(TFT_CTL, WR0, Bit_RESET); 
	GPIO_WriteBit(TFT_CTL, WR0, Bit_SET); 
	GPIO_WriteBit(TFT_CTL, CS0, Bit_SET);*/
}

void WriteData(uint16_t i)
{
  //TFT_CTL->BRR=CS0;
	//TFT_CTL->BSRR=RD0;
	TFT_CTL->BSRR=RS;
	TFT_DB->ODR=i;
	TFT_CTL->BRR=WR0;
	TFT_CTL->BSRR=WR0;
	//TFT_CTL->BSRR=CS0;
	
	/*GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_WriteBit(TFT_CTL, CS0, Bit_RESET);
	GPIO_WriteBit(TFT_CTL, RD0, Bit_SET);
  GPIO_WriteBit(TFT_CTL, RS, Bit_SET);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//??????50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//????
	GPIO_Init(TFT_DB, &GPIO_InitStructure);//?????GPIOx???  
	  
  GPIO_Write(TFT_DB, i);
	
  GPIO_WriteBit(TFT_CTL, WR0, Bit_RESET); 
	GPIO_WriteBit(TFT_CTL, WR0, Bit_SET); 
	GPIO_WriteBit(TFT_CTL, CS0, Bit_SET);*/
}

uint16_t ReadStatus(void)
{	
  uint16_t ReadValue;
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_WriteBit(TFT_CTL, CS0, Bit_RESET);
	GPIO_WriteBit(TFT_CTL, RD0, Bit_SET);
  GPIO_WriteBit(TFT_CTL, RS, Bit_RESET);
	
  GPIO_WriteBit(TFT_CTL, RD0, Bit_RESET);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//??????10MHz
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//????
  GPIO_Init(TFT_DB, &GPIO_InitStructure);//?????GPIOx???
  ReadValue = GPIO_ReadInputData(TFT_DB);//?????GPIO????

  
	GPIO_WriteBit(TFT_CTL, CS0, Bit_SET);
	return ReadValue;

}

uint16_t ReadData(void)
{	
  uint16_t ReadValue;
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_WriteBit(TFT_CTL, CS0, Bit_RESET);
	GPIO_WriteBit(TFT_CTL, RD0, Bit_SET);
  GPIO_WriteBit(TFT_CTL, RS, Bit_SET);
	
  GPIO_WriteBit(TFT_CTL, RD0, Bit_RESET);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//??????10MHz
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//????
  GPIO_Init(TFT_DB, &GPIO_InitStructure);//?????GPIOx???
  ReadValue = GPIO_ReadInputData(TFT_DB);//?????GPIO????

 
	GPIO_WriteBit(TFT_CTL, CS0, Bit_SET);
	return ReadValue;

}



void LCDInit(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//??????APB2????
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//??????APB2????
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//??????APB2????


	

  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);//?????????
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//?????????


	
  GPIO_InitStructure.GPIO_Pin = RD0 | WR0 | RS | CS0 | RST;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//??????50MHz
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//????
  GPIO_Init(TFT_CTL, &GPIO_InitStructure);//?????GPIOx???
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//??????50MHz
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//????
  GPIO_Init(TFT_DB, &GPIO_InitStructure);//?????GPIOx???



	
	GPIO_WriteBit(TFT_CTL, RST, Bit_SET);  
	TFT_Delay(5000);
	
	GPIO_WriteBit(TFT_CTL, RST, Bit_RESET);
	TFT_Delay(50000);

	GPIO_WriteBit(TFT_CTL, RST, Bit_SET);
	TFT_Delay(50000);
	
	TFT_CTL->BRR=CS0;
	TFT_CTL->BSRR=RD0;
WriteComm(0XF1);
WriteData(0x36);
WriteData(0x04);
WriteData(0x00);
WriteData(0x3C);
WriteData(0x0F);
WriteData(0x8F);


WriteComm(0XF2);
WriteData(0x18);
WriteData(0xA3);
WriteData(0x12);
WriteData(0x02);
WriteData(0xb2);
WriteData(0x12);
WriteData(0xFF);
WriteData(0x10);
WriteData(0x00);

WriteComm(0XF8);
WriteData(0x21);
WriteData(0x04);


WriteComm(0xF9);
WriteData(0x00);
WriteData(0x08);  


WriteComm(0xC0);
WriteData(0x0f);
WriteData(0x0f);

WriteComm(0xC1);
WriteData(0x42);

WriteComm(0xC2);
WriteData(0x22);

WriteComm(0XC5);
WriteData(0x01);
WriteData(0x29);
WriteData(0x80);

WriteComm(0Xb6);
WriteData(0x00);
WriteData(0x02);
WriteData(0x3b);

WriteComm(0xB1);
WriteData(0xB0);
WriteData(0x11);

WriteComm(0xB4);
WriteData(0x02);

WriteComm(0xE0);
WriteData(0x0F);
WriteData(0x18);
WriteData(0x15);
WriteData(0x09);
WriteData(0x0B);
WriteData(0x04);
WriteData(0x49);
WriteData(0x64);
WriteData(0x3D);
WriteData(0x08);
WriteData(0x15);
WriteData(0x06);
WriteData(0x12);
WriteData(0x07);
WriteData(0x00);
WriteComm(0XE1);
WriteData(0x0F);
WriteData(0x38);
WriteData(0x35);
WriteData(0x0a);
WriteData(0x0c);
WriteData(0x03);
WriteData(0x4A);
WriteData(0x42);
WriteData(0x36);
WriteData(0x04);
WriteData(0x0F);
WriteData(0x03);
WriteData(0x1F);
WriteData(0x1B);
WriteData(0x00);


WriteComm(0x36);
WriteData(0x40);
WriteComm(0x3A);
WriteData(0x66);
WriteComm(0x20);

WriteComm(0x11);
TFT_Delay(1000000);
WriteComm(0x29);

  DrawBlock(0xffff,0,319,0,479);
  //DrawBlock(0xffff,0,319,0,479);	
		TFT_CTL->BSRR=CS0;	
}


void BlockWrite(uint16_t Xstart,uint16_t Xend,uint16_t Ystart,uint16_t Yend)
{
	//OTM3225A

	WriteComm(0x002A);
	WriteData(Xstart>>8);
	WriteData(Xstart&0xff);
	WriteData(Xend>>8);
	WriteData(Xend&0xff);	

	WriteComm(0x002b);   
	WriteData(Ystart>>8);
	WriteData(Ystart&0xff);
	WriteData(Yend>>8);
	WriteData(Yend&0xff);	 
	
	WriteComm(0x003A); 	//HS
	WriteData(0X0055); 	 	

	WriteComm(0x002C);  // write ram
}



void DrawBlock(uint16_t color,uint16_t x0, uint16_t x1, uint16_t y0, uint16_t y1)
{
	uint16_t i,j;
	//BlockWrite(x0,x1,y0,y1);
	TFT_CTL->BRR=CS0;
	
	
	WriteComm(0x002A);
  /*TFT_CTL->BSRR=RD0;
	TFT_CTL->BRR=RS;
	TFT_DB->ODR=0x002A;
	TFT_CTL->BRR=RD0;
	TFT_CTL->BSRR=RD0;*/
	
	
	WriteData(x0>>8);
	WriteData(x0&0xff);
	WriteData(x1>>8);
	WriteData(x1&0xff);	

	WriteComm(0x002b);   
	WriteData(y0>>8);
	WriteData(y0&0xff);
	WriteData(y1>>8);
	WriteData(y1&0xff);	 
	
	WriteComm(0x003A); 	//HS
	WriteData(0X0055); 	 	

	WriteComm(0x002C);  // write ram	

	
	GPIO_WriteBit(TFT_CTL, CS0, Bit_RESET);	


  
	GPIO_WriteBit(TFT_CTL, RS, Bit_SET);
	GPIO_Write(TFT_DB,color);

	for(i=0;i<y1-y0+1;i++)
	{
	    for(j=0;j<x1-x0+1;j++)
		{    

			
			TFT_CTL->BRR = WR0;//GPIO_WriteBit(TFT_CTL, WR0, Bit_RESET); 
			TFT_CTL->BSRR = WR0;//GPIO_WriteBit(TFT_CTL, WR0, Bit_SET); 


		}
	}

	GPIO_WriteBit(TFT_CTL, CS0, Bit_SET);
}




void WriteMemory(uint16_t *color, uint16_t x0, uint16_t x1, uint16_t y0, uint16_t y1)
{
	uint16_t x,y;	

	GPIO_WriteBit(TFT_CTL, CS0, Bit_RESET);

  

  TFT_CTL->BRR = RS;//GPIO_WriteBit(TFT_CTL, RS, Bit_RESET);//	
  TFT_DB->ODR = 0x2a;//GPIO_Write(TFT_DB, 0x2a);
  TFT_CTL->BRR = WR0;//GPIO_WriteBit(TFT_CTL, WR0, Bit_RESET); 
	TFT_CTL->BSRR = WR0;//GPIO_WriteBit(TFT_CTL, WR0, Bit_SET); 	
	
	
  TFT_CTL->BSRR = RS;//GPIO_WriteBit(TFT_CTL, RS, Bit_SET);	//
  TFT_DB->ODR = x0>>8;//GPIO_Write(TFT_DB, x0>>8);	
  TFT_CTL->BRR = WR0;//GPIO_WriteBit(TFT_CTL, WR0, Bit_RESET); 
	TFT_CTL->BSRR = WR0;//GPIO_WriteBit(TFT_CTL, WR0, Bit_SET); 	
	
  TFT_DB->ODR = x0&0xff;//GPIO_Write(TFT_DB, x0&0xff);	
  TFT_CTL->BRR = WR0;//GPIO_WriteBit(TFT_CTL, WR0, Bit_RESET);// 
	TFT_CTL->BSRR = WR0;//GPIO_WriteBit(TFT_CTL, WR0, Bit_SET);

  TFT_DB->ODR = x1>>8;//GPIO_Write(TFT_DB, x1>>8);	
  TFT_CTL->BRR = WR0;//GPIO_WriteBit(TFT_CTL, WR0, Bit_RESET); 
	TFT_CTL->BSRR = WR0;//GPIO_WriteBit(TFT_CTL, WR0, Bit_SET);
	
	TFT_DB->ODR = x1&0xff;//GPIO_Write(TFT_DB, x1&0xff);	
  TFT_CTL->BRR = WR0;//GPIO_WriteBit(TFT_CTL, WR0, Bit_RESET); 
	TFT_CTL->BSRR = WR0;//GPIO_WriteBit(TFT_CTL, WR0, Bit_SET);
	

  TFT_CTL->BRR = RS;//	GPIO_WriteBit(TFT_CTL, RS, Bit_RESET);//
  TFT_DB->ODR = 0x2b;//GPIO_Write(TFT_DB, 0x2b);
  TFT_CTL->BRR = WR0;//GPIO_WriteBit(TFT_CTL, WR0, Bit_RESET); 
	TFT_CTL->BSRR = WR0;//GPIO_WriteBit(TFT_CTL, WR0, Bit_SET); 	
	
	
  TFT_CTL->BSRR = RS;//	GPIO_WriteBit(TFT_CTL, RS, Bit_SET);//
  TFT_DB->ODR = y0>>8;//GPIO_Write(TFT_DB, y0>>8);	
  TFT_CTL->BRR = WR0;//GPIO_WriteBit(TFT_CTL, WR0, Bit_RESET); 
	TFT_CTL->BSRR = WR0;//GPIO_WriteBit(TFT_CTL, WR0, Bit_SET); 	
	
  TFT_DB->ODR = y0&0xff;//GPIO_Write(TFT_DB, y0&0xff);	
  TFT_CTL->BRR = WR0;//GPIO_WriteBit(TFT_CTL, WR0, Bit_RESET); 
	TFT_CTL->BSRR = WR0;//GPIO_WriteBit(TFT_CTL, WR0, Bit_SET);

  TFT_DB->ODR = y1>>8;//GPIO_Write(TFT_DB, y1>>8);	
  TFT_CTL->BRR = WR0;//GPIO_WriteBit(TFT_CTL, WR0, Bit_RESET); 
	TFT_CTL->BSRR = WR0;//GPIO_WriteBit(TFT_CTL, WR0, Bit_SET);
	
	TFT_DB->ODR = y1&0xff;//GPIO_Write(TFT_DB, y1&0xff);	
  TFT_CTL->BRR = WR0;//GPIO_WriteBit(TFT_CTL, WR0, Bit_RESET); 
	TFT_CTL->BSRR = WR0;//GPIO_WriteBit(TFT_CTL, WR0, Bit_SET);
	
	
  TFT_CTL->BRR = RS;//GPIO_WriteBit(TFT_CTL, RS, Bit_RESET);//	
  TFT_DB->ODR = 0x2C;//GPIO_Write(TFT_DB, 0x2C);
  TFT_CTL-> BRR= WR0;//GPIO_WriteBit(TFT_CTL, WR0, Bit_RESET); 
	TFT_CTL->BSRR = WR0;//GPIO_WriteBit(TFT_CTL, WR0, Bit_SET); 	
	
	
  TFT_CTL->BSRR = RS;//GPIO_WriteBit(TFT_CTL, RS, Bit_SET);	//
  //TFT_Delay(20);

			TFT_CTL->BSRR = WR0;//GPIO_WriteBit(TFT_CTL, WR0, Bit_RESET); 
			
			TFT_CTL->BRR = WR0;//GPIO_WriteBit(TFT_CTL, WR0, Bit_SET);			

	
	for(y=y1-y0;y!=0xffff;y--)
	{
		for(x=x1-x0;x!=0xffff;x--)
		{
			TFT_DB->ODR = *color;//GPIO_Write(TFT_DB, *color);	
			TFT_CTL->BSRR = WR0;//GPIO_WriteBit(TFT_CTL, WR0, Bit_RESET); 
			color++;
			TFT_CTL->BRR = WR0;//GPIO_WriteBit(TFT_CTL, WR0, Bit_SET);			
		}
	}



	GPIO_WriteBit(TFT_CTL, CS0, Bit_SET);
 
	
	
}


void ReadMemory(uint16_t *color, uint16_t x0, uint16_t x1, uint16_t y0, uint16_t y1)
{
	uint16_t x,y;	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	
	GPIO_WriteBit(TFT_CTL, CS0, Bit_RESET);

 

  GPIO_WriteBit(TFT_CTL, RS, Bit_RESET);	
  GPIO_Write(TFT_DB, 0x2a);
  GPIO_WriteBit(TFT_CTL, WR0, Bit_RESET); 
	GPIO_WriteBit(TFT_CTL, WR0, Bit_SET); 	
	
	
  GPIO_WriteBit(TFT_CTL, RS, Bit_SET);	
  GPIO_Write(TFT_DB, x0>>8);	
  GPIO_WriteBit(TFT_CTL, WR0, Bit_RESET); 
	GPIO_WriteBit(TFT_CTL, WR0, Bit_SET); 	
	
  GPIO_Write(TFT_DB, x0&0xff);	
  GPIO_WriteBit(TFT_CTL, WR0, Bit_RESET); 
	GPIO_WriteBit(TFT_CTL, WR0, Bit_SET);

  GPIO_Write(TFT_DB, x1>>8);	
  GPIO_WriteBit(TFT_CTL, WR0, Bit_RESET); 
	GPIO_WriteBit(TFT_CTL, WR0, Bit_SET);
	
	GPIO_Write(TFT_DB, x1&0xff);	
  GPIO_WriteBit(TFT_CTL, WR0, Bit_RESET); 
	GPIO_WriteBit(TFT_CTL, WR0, Bit_SET);
	

  GPIO_WriteBit(TFT_CTL, RS, Bit_RESET);	
  GPIO_Write(TFT_DB, 0x2b);
  GPIO_WriteBit(TFT_CTL, WR0, Bit_RESET); 
	GPIO_WriteBit(TFT_CTL, WR0, Bit_SET); 	
	
	
  GPIO_WriteBit(TFT_CTL, RS, Bit_SET);	
  GPIO_Write(TFT_DB, y0>>8);	
  GPIO_WriteBit(TFT_CTL, WR0, Bit_RESET); 
	GPIO_WriteBit(TFT_CTL, WR0, Bit_SET); 	
	
  GPIO_Write(TFT_DB, y0&0xff);	
  GPIO_WriteBit(TFT_CTL, WR0, Bit_RESET); 
	GPIO_WriteBit(TFT_CTL, WR0, Bit_SET);

  GPIO_Write(TFT_DB, y1>>8);	
  GPIO_WriteBit(TFT_CTL, WR0, Bit_RESET); 
	GPIO_WriteBit(TFT_CTL, WR0, Bit_SET);
	
	GPIO_Write(TFT_DB, y1&0xff);	
  GPIO_WriteBit(TFT_CTL, WR0, Bit_RESET); 
	GPIO_WriteBit(TFT_CTL, WR0, Bit_SET);
	
	
  GPIO_WriteBit(TFT_CTL, RS, Bit_RESET);	
  GPIO_Write(TFT_DB, 0x2e);
  GPIO_WriteBit(TFT_CTL, WR0, Bit_RESET); 
	GPIO_WriteBit(TFT_CTL, WR0, Bit_SET); 	
	
	
  GPIO_WriteBit(TFT_CTL, RS, Bit_SET);	

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//??????10MHz
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//????
  GPIO_Init(TFT_DB, &GPIO_InitStructure);//?????GPIOx???
  
	GPIO_WriteBit(TFT_CTL, RD0, Bit_RESET); 
	GPIO_WriteBit(TFT_CTL, RD0, Bit_SET);			

	
	for(y=y1-y0;y!=0xffff;y--)
	{
		for(x=x1-x0;x!=0xffff;x--)
		{
			GPIO_WriteBit(TFT_CTL, RD0, Bit_RESET); 
			*color=GPIO_ReadInputData(TFT_DB);
			GPIO_WriteBit(TFT_CTL, RD0, Bit_SET);			
			color++;
		}
	}

  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//??????10MHz
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//????
  GPIO_Init(TFT_DB, &GPIO_InitStructure);//?????GPIOx???

	GPIO_WriteBit(TFT_CTL, CS0, Bit_SET);
 
	
	
}



	
