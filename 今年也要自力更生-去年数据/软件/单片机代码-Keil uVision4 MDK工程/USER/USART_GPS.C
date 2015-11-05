#include "USART_GPS.H"
#include "comunication.h"
#include "public.h"


#define USART2_PORT GPIOA
#define USART2_TX_PIN GPIO_Pin_2
#define USART2_RX_PIN GPIO_Pin_3

#define USART2_BITERATE  4800

void USART2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//??????APB2????
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//??????APB2????
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//??????APB2????

	GPIO_InitStructure.GPIO_Pin = USART2_TX_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//??????50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//??????
	GPIO_Init(USART2_PORT, &GPIO_InitStructure);//?????GPIOx???
	GPIO_InitStructure.GPIO_Pin = USART2_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//????
	GPIO_Init(USART2_PORT, &GPIO_InitStructure);//?????GPIOx???	
	
	

	USART_DeInit(USART2);//???USARTx?????????
	
	USART_InitStructure.USART_BaudRate = USART2_BITERATE;//*MCUFrequency/72;//???
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8???
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//??????1????
	USART_InitStructure.USART_Parity = USART_Parity_No;//???
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//????????
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//???????
	USART_Init(USART2,&USART_InitStructure);	
	
	USART_Cmd(USART2, ENABLE);//??????USART??
	
}


uint8_t USART2_WaitData(void)
{
	while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE)==RESET);
	return USART_ReceiveData(USART2);
}


void USART2_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;


  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}



GPS_Data_Struct GPS_Data;

void USART_GPS_GetRMCData(void)
{
	static uint8_t stemp[12]={0};
	static uint8_t sp=0;
	static uint8_t dp=0;
  uint8_t temp=0;
	if(sp||dp)  //started
	{
		if(dp)  //rmc data started
		{
			switch(dp)
			{
				case 1: //time
					if((temp=USART_ReceiveData(USART2))==',')
					{
						stemp[sp]=0;
						sp=0;
						dp++;
						GPS_Data.refreshflag=REFRESHING;
						CopyString(stemp,GPS_Data.time,12);
					}
					else	
						stemp[sp++]=temp;
				  break;
				case 2: //status
					if(USART_ReceiveData(USART2)==',')
					{
						stemp[sp]=0;
						sp=0;
						dp++;
						CopyString(stemp,GPS_Data.status,2);
					}
					else	
						stemp[sp++]=USART_ReceiveData(USART2);
				  break;
				case 3: //latitude
					if(USART_ReceiveData(USART2)==',')
					{
						stemp[sp]=0;
						sp=0;
						dp++;
						CopyString(stemp,GPS_Data.latitude,12);
					}
					else	
						stemp[sp++]=USART_ReceiveData(USART2);
				  break;
				case 4: //ns
					if(USART_ReceiveData(USART2)==',')
					{
						stemp[sp]=0;
						sp=0;
						dp++;
						CopyString(stemp,GPS_Data.ns,2);
					}
					else	
						stemp[sp++]=USART_ReceiveData(USART2);
				  break;
				case 5: //longitude
					if(USART_ReceiveData(USART2)==',')
					{
						stemp[sp]=0;
						sp=0;
						dp++;
						CopyString(stemp,GPS_Data.longitude,12);
					}
					else	
						stemp[sp++]=USART_ReceiveData(USART2);
				  break;
				case 6: //ew
					if(USART_ReceiveData(USART2)==',')
					{
						stemp[sp]=0;
						sp=0;
						dp++;
						CopyString(stemp,GPS_Data.ew,2);
					}
					else	
						stemp[sp++]=USART_ReceiveData(USART2);
				  break;
				case 7: //speed
					if(USART_ReceiveData(USART2)==',')
					{
						stemp[sp]=0;
						sp=0;
						dp++;
						CopyString(stemp,GPS_Data.speed,8);
					}
					else	
						stemp[sp++]=USART_ReceiveData(USART2);
				  break;
				case 8: //course
					if(USART_ReceiveData(USART2)==',')
					{
						stemp[sp]=0;
						sp=0;
						dp++;
						CopyString(stemp,GPS_Data.course,8);
					}
					else	
						stemp[sp++]=USART_ReceiveData(USART2);
				  break;
				case 9: //date
					if(USART_ReceiveData(USART2)==',')
					{
						stemp[sp]=0;
						sp=0;
						dp=0;
						GPS_Data.refreshflag=REFRESHED;
						CopyString(stemp,GPS_Data.date,8);
					}
					else	
						stemp[sp++]=USART_ReceiveData(USART2);
					break;
				default:
					break;
			}
		}
		else   //check rmc data
		{
			stemp[sp++]=USART_ReceiveData(USART2);
			if(stemp[sp-1]==',')  //rmc hearder full
			{
				stemp[sp-1]=0;   //string end flag
				if(CompareString("$GPRMC",stemp))  //not rmc data
				{
					sp=0;
					dp=0;
				}
				else
				{
					sp=0;
					dp++;
				}
			}
		}
	}
	else //not start
	{
		if((stemp[sp]=USART_ReceiveData(USART2))=='$')
		{
			sp++;
		}
	}
}

void USART_GPS_GetGGAData(void)
{
	static uint8_t stemp[12]={0};
	static uint8_t sp=0;
	static uint8_t dp=0;
  uint8_t temp=0;
	if(sp||dp)  //started
	{
		if(dp)  //rmc data started
		{
			switch(dp)
			{
				case 1: //time
					if((temp=USART_ReceiveData(USART2))==',')
					{
						stemp[sp]=0;
						sp=0;
						dp++;
						GPS_Data.refreshflag=REFRESHING;
						//CopyString(stemp,GPS_Data.time,12);
					}
					else	
						stemp[sp++]=temp;
				  break;
				case 2: //latitude
					if(USART_ReceiveData(USART2)==',')
					{
						stemp[sp]=0;
						sp=0;
						dp++;
						//CopyString(stemp,GPS_Data.status,2);
					}
					else	
						stemp[sp++]=USART_ReceiveData(USART2);
				  break;
				case 3: //ns
					if(USART_ReceiveData(USART2)==',')
					{
						stemp[sp]=0;
						sp=0;
						dp++;
						//CopyString(stemp,GPS_Data.latitude,12);
					}
					else	
						stemp[sp++]=USART_ReceiveData(USART2);
				  break;
				case 4: //longitude
					if(USART_ReceiveData(USART2)==',')
					{
						stemp[sp]=0;
						sp=0;
						dp++;
						//CopyString(stemp,GPS_Data.ns,2);
					}
					else	
						stemp[sp++]=USART_ReceiveData(USART2);
				  break;
				case 5: //ew
					if(USART_ReceiveData(USART2)==',')
					{
						stemp[sp]=0;
						sp=0;
						dp++;
						//CopyString(stemp,GPS_Data.longitude,12);
					}
					else	
						stemp[sp++]=USART_ReceiveData(USART2);
				  break;
				case 6: //position fix
					if(USART_ReceiveData(USART2)==',')
					{
						stemp[sp]=0;
						sp=0;
						dp++;
						//CopyString(stemp,GPS_Data.ew,2);
					}
					else	
						stemp[sp++]=USART_ReceiveData(USART2);
				  break;
				case 7: //satellites used
					if(USART_ReceiveData(USART2)==',')
					{
						stemp[sp]=0;
						sp=0;
						dp++;
						//CopyString(stemp,GPS_Data.speed,8);
					}
					else	
						stemp[sp++]=USART_ReceiveData(USART2);
				  break;
				case 8: //hdop
					if(USART_ReceiveData(USART2)==',')
					{
						stemp[sp]=0;
						sp=0;
						dp++;
						//CopyString(stemp,GPS_Data.course,8);
					}
					else	
						stemp[sp++]=USART_ReceiveData(USART2);
				  break;
				case 9: //altitude
					if(USART_ReceiveData(USART2)==',')
					{
						stemp[sp]=0;
						sp=0;
						dp=0;
						GPS_Data.refreshflag=REFRESHED;
						CopyString(stemp,GPS_Data.altitude,8);
					}
					else	
						stemp[sp++]=USART_ReceiveData(USART2);
					break;
				default:
					break;
			}
		}
		else   //check rmc data
		{
			stemp[sp++]=USART_ReceiveData(USART2);
			if(stemp[sp-1]==',')  //rmc hearder full
			{
				stemp[sp-1]=0;   //string end flag
				if(CompareString("$GPGGA",stemp))  //not rmc data
				{
					sp=0;
					dp=0;
				}
				else
				{
					sp=0;
					dp++;
				}
			}
		}
	}
	else //not start
	{
		if((stemp[sp]=USART_ReceiveData(USART2))=='$')
		{
			sp++;
		}
	}
}

void USART2_ISV_Init(void)
{
	USART2_NVIC_Configuration();
	USART_ClearITPendingBit(USART2, USART_IT_RXNE);//??USARTx???????
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//?????????USART??
}



void USART_GPS_ISV(void)
{
	USART_ClearITPendingBit(USART2, USART_IT_RXNE);//??USARTx???????	
	USART_GPS_GetGGAData();
	USART_GPS_GetRMCData();
}



void USART_GPS_Init(void)
{
	USART2_Init();
	USART2_ISV_Init();
}










