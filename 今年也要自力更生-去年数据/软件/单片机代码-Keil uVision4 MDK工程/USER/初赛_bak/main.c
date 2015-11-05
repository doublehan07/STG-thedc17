/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_conf.h"
#include "stm32f10x.h"
#include "main.h"
#include "comunication.h"
#include "sysconfig.h"
#include "public.h"
#include "hardware.h"
#include "adc.h"
#include "spi_memory.h"
#include "pwm.h"
#include "motor_ctr.h"
#include "IIC_6050.h"
#include "MovementCtr.h"
#include <math.h>
#include <string.h>






/* Private functions ---------------------------------------------------------*/

#define BUFSIZE 234
uint8_t DataBuf[BUFSIZE] = {0x0a, 0};
uint8_t BufFront = 0, BufRear = 1;
uint8_t target[2] = {0};
uint8_t holes[4][2] = {{54,54},{199,54},{54,199},{199,199}};
uint8_t nearest[2];
uint16_t mypos[2];

typedef struct
{
  unsigned char head_x;
  unsigned char head_y;
  unsigned char rear_x;
  unsigned char rear_y;
  unsigned char score;
}PlayerInf;

PlayerInf myInf;

struct Player
{
    unsigned char player_loc;
    PlayerInf Player[4];
    unsigned char available;
    unsigned char time;
    unsigned char is_over;
}Inf;

struct Status
{
  uint16_t dis;
  int16_t dir;
}Last, Now;

uint8_t GetData( void )
{
  uint8_t ret = 0;
  while ((BufRear + BUFSIZE - BufFront) % BUFSIZE >= 26)
  {
    if (DataBuf[(BufFront)% BUFSIZE] == 0x0a && DataBuf[(BufFront + 25)% BUFSIZE] == 0x0d)
    {
      uint8_t i;
      uint8_t *Data = (uint8_t*)&Inf;
      for (i = 1; i < 25; i++)
      {
        Data[i - 1] = DataBuf[(BufFront + i) % BUFSIZE];
      }
      BufFront = (BufFront + 26)% BUFSIZE;
      ret = 1;
    }
    else
      BufFront++;
  }
  return ret;
}

void SetTarget(uint8_t x, uint8_t y)
{
  target[0] = x;
  target[1] = y;
}

void NearestHole(uint8_t x, uint8_t y)
{
  uint8_t i = 0;
  if (x > 128)
    i += 1;
  if (y > 128)
    i += 2;
  nearest[0] = holes[i][0];
  nearest[1] = holes[i][1];
}

float Distance(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
  float dx = x1-x2, dy = y1-y2;
  return sqrtf(dx*dx+dy*dy);
}

void BackTo(int16_t dis)
{
	SetWay(0, -dis);
}

void GoTo(int16_t dis)
{
	if (dis <= 0)
		SetSpeed(0);
//	else if (dis < 10)
//	{
//		SetSpeed(10);SetType(GT);
//	}
	else if (dis < 60)
	{
		SetSpeed(25);SetType(GT);
	}
	else
	{
		SetSpeed(dis * 5 / 6 - 25);SetType(GT);
	}		
}

volatile uint8_t ChangeFlag = 0;
volatile uint8_t MakeNextMove = 0;
//50 6.5cm/100ms
//20 2.5
//80 10.5
//20 * 1s = 180degree
//80 * 0.25s = 180degree
//50 * 0.4s = 180degree
void GoToTarget()
{
  int16_t mydir, tardir;
  int16_t transdir, dis;
  Last = Now;
  myInf = Inf.Player[Inf.player_loc];
  mydir = FixedTan(myInf.head_x - myInf.rear_x, myInf.head_y - myInf.rear_y);
  mypos[0] = (myInf.head_x + myInf.rear_x) >> 1;
  mypos[1] = (myInf.head_y + myInf.rear_y) >> 1;
  tardir = FixedTan(target[0] - mypos[0], target[1] - mypos[1]);
  dis = (int16_t)(Distance(mypos[0], mypos[1], target[0], target[1]) + 0.5f);
  NearestHole(mypos[0],mypos[1]);
  Now.dir = mydir;
  Now.dis = dis;
	StageGyro = (Last.dir - Now.dir + 540) % 360 - 180;
//	mydir -= StageGyro / 2;
	if (Now.dis - Last.dis < 0)
		dis += (Now.dis - Last.dis) / 2;
	if (MakeNextMove <= 0)
	{
		if (Inf.is_over && Now.dis > 4 && dis > 0 && !ChangeFlag)
		{
//			if (dis < 0) dis = 0;
			transdir = (540 + tardir - mydir) % 360 - 180;
			if (dis > 10)
			{
				if (TurnFinish || Type != ST)
				{
					if (transdir > 10 || transdir < -10)
					{
						SetDegree(-transdir);
						if (transdir > 30 || transdir < -30)
						{
							SetType(ST);
							SetSpeed(0);
						}
						else
						{
							SetType(GT);
							GoTo(dis);
						}
					}
					else
					{
						SetDegree(0);
						SetType(GT);
						GoTo(dis);
					}
				}
			}
			else
			{
				if (transdir > 90 || transdir < -90)
				{
					transdir = (540 + transdir) % 360 - 180;
					SetWay(-transdir, -10);
				}
				else
				{
					SetWay(-transdir, 10);
				}
			}
/*			if (transdir - DeltaDegree() > 5 || DeltaDegree() - transdir > 5)
				SetDegree(-transdir);*/
		}
		else
			Stop();
	}
	else
		MakeNextMove--;
	ChangeFlag = 0;
}

uint8_t Done[4] = {0};

void SetNextHole(uint8_t mode)//mode : 0 : 下一个 ; 1 : 清除 ; 2 : 排除
{
  static uint8_t tar = 3;
  if (mode)
  {
    if (mode == 1)
    {
      memset(Done, 0, sizeof(Done));
      Done[tar] = 1;
    }
    else if (mode == 2)
    {
      Done[tar] = 1;
    }
  }
  tar = (tar + 1) % 4;
  if (Done[0] && Done[1] && Done[2] && Done[3])
    memset(Done, 0, sizeof(Done));
  while (Done[tar])
  {
    tar = (tar + 1) % 4;
  }
  SetTarget(holes[tar][0], holes[tar][1]);
	
	ChangeFlag = 1;
}


/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */

void GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

uint8_t IsTargetAvailable(uint8_t available)
{
  uint8_t i;
  for (i = 0; i < 4; i++)
  {
    if (target[0] == holes[i][0] && target[1] == holes[i][1])
    {
      if ((available >> i) & 0x01)
        return 0;
      else
        return 1;
    }
  }
  return 1;
}

int main(void)
{
	uint32_t LastT = 0, LastIIC = 0;
	int16_t testData[5][2] = {{0,50},{90,0},{-180,0},{90,0},{0,-50}};
	uint8_t i = 0;
	float dis;
	USARTInit(9600);

	System_Init();
	USARTInit(9600);
	USART2Init(9600);
	HardwareDriver_Init();
	GPIOInit();
	
	SetNextHole(0);
//	SetNextHole(0);

	while(1)
	{
		if (LastIIC <= TimeTicket)
		{
			LastIIC = TimeTicket + 3;
			IIC_Exec();
		}
		if (!IIC_OK)
		{
			ReInitMPU6050();
		}
#if 1
		if (!Inf.is_over || Distance(myInf.head_x, myInf.head_y, myInf.rear_x, myInf.rear_y) > 15)
		{
			Stop();
		}
/*    if (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12) && GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13))
    {
			Stop();
    }
    else if (Inf.is_over && GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12))
    {
			BackTo(50);
      if (target[0] == nearest[0] && target[1] == nearest[1])
        SetNextHole(2);
      else
        SetNextHole(0);
    }
    else if (Inf.is_over && GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13))
    {
			GoTo(50);
    }*/
		if ((!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12) || !GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13))
			&& (mypos[0] > 140 || mypos[0] < 125 || mypos[1] > 200 || mypos[1] < 50)) 
		{
			BackTo(50);
			MakeNextMove = 3;
      if (target[0] == nearest[0] && target[1] == nearest[1])
        SetNextHole(2);
      else
        SetNextHole(0);
		}
    if (Inf.available)
    {
      if (!IsTargetAvailable(Inf.available))
      {
//        SetNextHole(2);
      }
    }
    if (!Inf.is_over || Distance(myInf.head_x, myInf.head_y, myInf.rear_x, myInf.rear_y) > 15)
    {
			Stop();
    }
		dis = Distance(mypos[0], mypos[1], target[0], target[1]);
    if (dis < 5)
    {
      uint8_t score0 = myInf.score;
			uint32_t time = TimeTicket + 10000;
      while(score0 == myInf.score && time > TimeTicket);
			SetNextHole(1);
    }
    if (!Inf.is_over)
    {
			Stop();
    }
#elif 0
		if (!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12) || !GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13))
    {
			BackTo(50);
    }
		else
			GoTo(50);

#elif 0
		if (TimeTicket >= 3000 && TimeTicket < 4000)
			SetWay(0, 1);
		else if (TimeTicket >= 4000 && TimeTicket < 12000)
		{
			if (TimeTicket % 10000 == 4000)
				SetWay(0, 20);
			else if (TimeTicket % 10000 == 5000)
			{
				SetWay(90, 0);
			}
			else if (TimeTicket % 10000 == 8000)
			{
				SetWay(-90, 0);
			}
			else if (TimeTicket % 10000 == 11000)
			{
				SetWay(0, -20);
			}
		}
		else
		{
			SetWay(0, 0);
		}
#else
		if (TimeTicket >= LastT + 1200)
		{
			SetWay(testData[i][0], testData[i][1]);
			i = (i+1) % 5;
//			i = 4;
			LastT = TimeTicket;
		}
#endif
	}	
}

		
	
	
	
	
		
		
  
void USART2SendOneChar(uint8_t d)
{
	  while(USART_GetFlagStatus(USART2, USART_FLAG_TXE)==RESET);
		USART_SendData(USART2, d);
}

void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE))
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);//??USARTx???????
		USART_ClearFlag(USART2, USART_FLAG_RXNE);//??USARTx???????
		if (BufRear >= BUFSIZE)
			BufRear = 0;
		DataBuf[BufRear++] = USART_ReceiveData(USART2);
		if (GetData())
		{
			GoToTarget();
		}
	}
}


 void assert_failed(uint8_t* file, uint32_t line)
{
	USARTSendString("\n Error occurred: ");
	USARTSendString(file);
	USARTSendString("\n At line: ");
	USARTSendFigDec(line, 8);
	while(1);
}

const uint8_t utan[42] = {45,47,50,52,54,56,57,59,60,62,63,64,65,66,67,68,68,69,70,70,71,72,72,73,73,74,74,74,75,75,75,76,76,76,77,77,77,77,78,78,78,78};

uint8_t FindTan(uint16_t s)
{
  if (s >= 573)
    return 89;
  if (s >= 287)
    return 88;
  if (s >= 191)
    return 87;
  if (s >= 144)
    return 86;
  if (s >= 115)
    return 85;
  if (s >= 96)
    return 84;
  if (s >= 82)
    return 83;
  if (s >= 72)
    return 82;
  if (s >= 64)
    return 81;
  if (s >= 57)
    return 80;
  if (s >= 52)
    return 79;
  return utan[s-10];
}

int16_t FixedTan(int16_t x, int16_t y)
{
  uint16_t s;
  uint8_t pi = 0, result;
  int8_t signx = 1, signy = 1;
  if (x < 0)
  {
    x = -x;
    signx = -1;
  }
  if (y < 0)
  {
    y = -y;
    signy = -1;
  }
  if (!y)
  {
    return 90 - signx * 90;
  }
  if (!x)
  {
    return signy * 90;
  }
  if (x <= y)
  {
    s = 10 * y / x;
    if (10 * y % x >= x/2)
      s++;
  }
  else
  {
    s = 10 * x / y;
    if (10 * x % y >= y/2)
      s++;
    pi = 1;
  }
  result = FindTan(s);
  if (pi) result = 90 - result;
  if (signx > 0)
    return signy * result;
  else
    return signy * (180 - result);
}




/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
