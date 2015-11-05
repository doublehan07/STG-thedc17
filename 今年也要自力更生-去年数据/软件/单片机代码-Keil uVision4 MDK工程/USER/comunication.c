#include "comunication.h"
#include "sysconfig.h"
#include "spi_memory.h"

void USARTInit(uint32_t bitrate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//??????APB2????
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//??????APB2????
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//??????APB2????

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//??????50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//??????
	GPIO_Init(GPIOA, &GPIO_InitStructure);//?????GPIOx???
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//????
	GPIO_Init(GPIOA, &GPIO_InitStructure);//?????GPIOx???	
	
	

	USART_DeInit(USART1);//???USARTx?????????
	
	USART_InitStructure.USART_BaudRate = bitrate;//*MCUFrequency/72;//???
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8???
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//??????1????
	USART_InitStructure.USART_Parity = USART_Parity_No;//???
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//????????
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//???????
	USART_Init(USART1,&USART_InitStructure);	
	
	USART_Cmd(USART1, ENABLE);//??????USART??
	
	USARTSendString("\n USART initial success!");

  //initialize interrupt
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		//RTC????
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;	//?????1?,????3?
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;	//?????0?,????4?
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//???????
	NVIC_Init(&NVIC_InitStructure);		//??NVIC_InitStruct???????????NVIC???
	
	//enable usart interrupt
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);//??USARTx???????
	USART_ClearFlag(USART1, USART_FLAG_RXNE);//??USARTx???????
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//receive
	
	//enable uasrt send dma request
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);//????????USART?DMA??
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//??????AHB????
	DMA_ClearITPendingBit(DMA1_IT_TC4 );//??DMA??x????????(DMA1??4??????)
	DMA_ClearFlag(DMA1_FLAG_TC4 );//??DMA??x??????(DMA1??4???????)
	DMA_DeInit(DMA1_Channel4);//?DMA???x?????????(DMA1 ??1)

  //initialize dma interrupt
	DMA_ITConfig(DMA1_Channel4,DMA_IT_TC, ENABLE);//???????????x??(DMA1 ??4)
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;		//RTC????
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//?????1?,????3?
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	//?????0?,????4?
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//???????
	NVIC_Init(&NVIC_InitStructure);		//??NVIC_InitStruct???????????NVIC???

}

void USART2Init(uint32_t bitrate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//??????APB2????
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//??????APB2????
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//??????APB2????

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//??????50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//??????
	GPIO_Init(GPIOA, &GPIO_InitStructure);//?????GPIOx???
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//????
	GPIO_Init(GPIOA, &GPIO_InitStructure);//?????GPIOx???	
	
	

	USART_DeInit(USART2);//???USARTx?????????
	
	USART_InitStructure.USART_BaudRate = bitrate;//*MCUFrequency/72;//???
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8???
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//??????1????
	USART_InitStructure.USART_Parity = USART_Parity_No;//???
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//????????
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//???????
	USART_Init(USART2,&USART_InitStructure);	
	
	USART_Cmd(USART2, ENABLE);//??????USART??
	
	USARTSendString("\n USART2 initial success!");

  //initialize interrupt
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		//RTC????
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//?????1?,????3?
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	//?????0?,????4?
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//???????
	NVIC_Init(&NVIC_InitStructure);		//??NVIC_InitStruct???????????NVIC???
	
	//enable usart interrupt
	USART_ClearITPendingBit(USART2, USART_IT_RXNE);//??USARTx???????
	USART_ClearFlag(USART2, USART_FLAG_RXNE);//??USARTx???????
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//receive
/*	
	//enable uasrt send dma request
	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);//????????USART?DMA??
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//??????AHB????
	DMA_ClearITPendingBit(DMA1_IT_TC7 );//??DMA??x????????(DMA1??4??????)
	DMA_ClearFlag(DMA1_FLAG_TC7 );//??DMA??x??????(DMA1??4???????)
	DMA_DeInit(DMA1_Channel7);//?DMA???x?????????(DMA1 ??1)

  //initialize dma interrupt
	DMA_ITConfig(DMA1_Channel7,DMA_IT_TC, ENABLE);//???????????x??(DMA1 ??4)
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;		//RTC????
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//?????1?,????3?
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	//?????0?,????4?
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//???????
	NVIC_Init(&NVIC_InitStructure);		//??NVIC_InitStruct???????????NVIC???
*/
}

void USARTSendOneChar(uint8_t d)
{
	  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
		USART_SendData(USART1, d);
	
}


void USARTSendData(uint8_t *p, uint32_t size)
{
	for(;size!=0;size--)
	{
	  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
		USART_SendData(USART1, *p);
		p++;
	}
}

void USARTSendString(uint8_t *s)
{
	while(*s)
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
		USART_SendData(USART1,*s);
		s++;
	}
}

uint8_t USARTWaitData(void)
{
	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==RESET);
	return USART_ReceiveData(USART1);
}

void USARTSendFigDec(uint32_t fig,uint8_t a)
{
	uint8_t b=9;
	uint32_t div=1000000000;
	for(;b!=a;b--)
		div/=10;
	
  for(;a!=0;a--)
	{
	  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
		USART_SendData(USART1,'0'+(fig%div)/(div/10));
		div/=10;
	}
}


void USARTSendSysFrequency(void)
{ 
	RCC_ClocksTypeDef RCC_Clocks;
	
	RCC_GetClocksFreq(&RCC_Clocks);//???????????
	
	USARTSendString("\n SYSCLK_Frequency:");
  USARTSendFigDec(RCC_Clocks.HCLK_Frequency,9);
	USARTSendString("\n PCLK1_Frequency:");
  USARTSendFigDec(RCC_Clocks.PCLK1_Frequency,9);
	USARTSendString("\n PCLK2_Frequency:");
  USARTSendFigDec(RCC_Clocks.PCLK2_Frequency,9);
	USARTSendString("\n ADCCLK_Frequency:");
  USARTSendFigDec(RCC_Clocks.ADCCLK_Frequency,9);
}

uint8_t USARTWaitString(uint8_t *s,uint8_t size)
{
	USARTSendString("wait for string");
	for(;size!=0;size--)
	{
		if((*(s++)=USARTWaitData())==0)
		{
			USARTSendString("\n string received");
			return 0;
		}
		USARTSendString("\n received remain:");
		USARTSendFigDec(size-1,3);
	}
	USARTSendString("space not enough");
	return 1;
}


void USARTWaitArray(uint8_t *array, uint16_t size)
{
	for(;size!=0;size--)
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==RESET);
		*(array++)=USART_ReceiveData(USART1);
	}
}


void USARTDMAInit(uint8_t *buff, uint16_t size)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	
	DMA_DeInit(DMA1_Channel5);//?DMA???x?????????(DMA1 ??1)

	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);//????????USART?DMA??

	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART1->DR));//?????
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)buff;//?????
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//???????????
	DMA_InitStructure.DMA_BufferSize = size;//??DMA???DMA?????,???????
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//?????????
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//?????????
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//?????8?
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//?????8?
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//?????????
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;//??????
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//????????
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);//?????CAN????

	DMA_Cmd(DMA1_Channel5,ENABLE);//???????????x(DMA1 ??5)

}





void USARTWriteFileDirect(uint32_t address, uint16_t size) //size=byte*512
{
	uint8_t usartbuff[512];
	
	USARTDMAInit(usartbuff,512);
	
	


	for(;size!=0;size--)
	{
		while(DMA_GetFlagStatus(DMA1_FLAG_HT5 )==0);
		SPIFlash_Write(&usartbuff[0], address, 256);
		address+=256;
		
		while(DMA_GetFlagStatus(DMA1_FLAG_TC5 )==0);
		DMA_ClearFlag(DMA1_FLAG_GL5 );//??DMA??x??????(DMA1??5?????)

		SPIFlash_Write(&usartbuff[256], address, 256);
		address+=256;


	}
	USARTSendString("file write success!");
	DMA_Cmd(DMA1_Channel5,DISABLE);//???????????x(DMA1 ??5)

}



/*void USART1_IRQHandler(void)
{
	
}*/

