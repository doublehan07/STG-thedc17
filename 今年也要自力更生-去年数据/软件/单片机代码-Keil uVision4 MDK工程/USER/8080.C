#include "8080.h"
#include "public.h"

#define TFT_RST_PORT GPIOC
#define TFT_RST_PIN GPIO_Pin_12

#define TFT_CS_PORT GPIOC
#define TFT_CS_PIN GPIO_Pin_9

#define TFT_RS_PORT GPIOC
#define TFT_RS_PIN GPIO_Pin_8

#define TFT_WR_PORT GPIOA
#define TFT_WR_PIN GPIO_Pin_0

#define TFT_RD_PORT GPIOA
#define TFT_RD_PIN GPIO_Pin_1

#define TFT_DB_PORT GPIOB   //low 8 bits
#define TFT_DB_PIN  GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7




void Port8080_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//clock initialize
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//??????APB2????
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//??????APB2????
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//??????APB2????
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//??????APB2????

	//gpio initialize
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//??????50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//????
	
	//RST
	GPIO_InitStructure.GPIO_Pin = TFT_RST_PIN;
	GPIO_Init(TFT_RST_PORT, &GPIO_InitStructure);//?????GPIOx???

	//CS
	GPIO_InitStructure.GPIO_Pin = TFT_CS_PIN;
	GPIO_Init(TFT_CS_PORT, &GPIO_InitStructure);//?????GPIOx???
	//default deselect
	GPIO_SetBits(TFT_CS_PORT, TFT_CS_PIN);//??????????


	//RS
	GPIO_InitStructure.GPIO_Pin = TFT_RS_PIN;
	GPIO_Init(TFT_RS_PORT, &GPIO_InitStructure);//?????GPIOx???

	//WR
	GPIO_InitStructure.GPIO_Pin = TFT_WR_PIN;
	GPIO_Init(TFT_WR_PORT, &GPIO_InitStructure);//?????GPIOx???
	GPIO_SetBits(TFT_WR_PORT, TFT_WR_PIN);//??????????


	//RD
	GPIO_InitStructure.GPIO_Pin = TFT_RD_PIN;
	GPIO_Init(TFT_RD_PORT, &GPIO_InitStructure);//?????GPIOx???
	GPIO_SetBits(TFT_RD_PORT, TFT_RD_PIN);//??????????
	
}


void Port8080_SetOut(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = TFT_DB_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//??????50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//????
	GPIO_Init(TFT_DB_PORT, &GPIO_InitStructure);//?????GPIOx???
}


void Port8080_SetIn(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = TFT_DB_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//????
	GPIO_Init(TFT_DB_PORT, &GPIO_InitStructure);//?????GPIOx???
}


void Port8080_Deselect(void)
{
	TFT_CS_PORT->BSRR = TFT_CS_PIN;//GPIO_SetBits(TFT_CS_PORT, TFT_CS_PIN);//??????????
}

void Port8080_Select(void)
{
	TFT_CS_PORT->BRR = TFT_CS_PIN;//GPIO_ResetBits(TFT_CS_PORT, TFT_CS_PIN);//??????????	
}

void Port8080_CommandTransfer(void)
{
	TFT_RS_PORT->BRR = TFT_RS_PIN;//GPIO_ResetBits(TFT_RS_PORT, TFT_RS_PIN);//??????????
}

void Port8080_DataTransfer(void)
{
  TFT_RS_PORT->BSRR = TFT_RS_PIN;//	GPIO_SetBits(TFT_RS_PORT, TFT_RS_PIN);//??????????	
}


//upper 8 bits transfer first
void Port8080_Write(uint16_t data)
{
	TFT_WR_PORT->BRR = TFT_WR_PIN;//GPIO_ResetBits(TFT_WR_PORT, TFT_WR_PIN);//??????????
	TFT_DB_PORT->ODR=((TFT_DB_PORT->ODR)&0XFF00)+(data>>8); //GPIO_Write(TFT_DB_PORT, (GPIO_ReadOutputData(TFT_DB_PORT)&0xff00)+(data>>8));//???GPIO????????
  TFT_WR_PORT->BSRR = TFT_WR_PIN;//	GPIO_SetBits(TFT_WR_PORT, TFT_WR_PIN);//??????????
	TFT_WR_PORT->BRR = TFT_WR_PIN;//GPIO_ResetBits(TFT_WR_PORT, TFT_WR_PIN);//??????????
	TFT_DB_PORT->ODR=((TFT_DB_PORT->ODR)&0XFF00)+data;	GPIO_Write(TFT_DB_PORT, (GPIO_ReadOutputData(TFT_DB_PORT)&0xff00)+data);//???GPIO????????
	TFT_WR_PORT->BSRR = TFT_WR_PIN;//GPIO_SetBits(TFT_WR_PORT, TFT_WR_PIN);//??????????
}


uint16_t Port8080_Read(void)
{
	uint16_t temp;
	GPIO_ResetBits(TFT_RD_PORT, TFT_RD_PIN);//??????????
	temp=GPIO_ReadInputData(TFT_DB_PORT);//???GPIO????????
	GPIO_SetBits(TFT_RD_PORT, TFT_RD_PIN);//??????????
	temp=temp<<8;
	GPIO_ResetBits(TFT_RD_PORT, TFT_RD_PIN);//??????????
	temp+=GPIO_ReadInputData(TFT_DB_PORT)&0x00ff;//???GPIO????????
	GPIO_SetBits(TFT_RD_PORT, TFT_RD_PIN);//??????????
	return temp;
}


void Port8080_WriteCommand(uint16_t command)
{
	Port8080_CommandTransfer();
	Port8080_Write(command);
}

void Port8080_WriteData(uint16_t data)
{
	Port8080_DataTransfer();
	Port8080_Write(data);
}

void Port8080_RST(void)
{
	GPIO_ResetBits(TFT_RST_PORT,TFT_RST_PIN);
	Delay_mS(10);
	GPIO_SetBits(TFT_RST_PORT,TFT_RST_PIN);
}



