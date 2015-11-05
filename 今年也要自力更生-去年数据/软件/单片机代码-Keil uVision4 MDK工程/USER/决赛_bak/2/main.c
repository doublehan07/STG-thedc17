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
uint8_t target_t[2] = {0};
const uint8_t holes[4][2] = {{57,57},{195,57},{57,195},{195,195}};
uint8_t nearest[2];
uint8_t nearest_emy[2];
uint8_t emy_hole_number;
uint8_t tar_hole_number;
uint8_t my_hole_number;
uint16_t mypos[2];
uint8_t Done[4] = {0};//取1表示为坑

uint8_t emy_target[5] = {0};//0~3holes 4:me

#define BEGIN 1
#define GOT 2

#define GoToCenter 1
#define Fight 2
#define GoToBehind 3

uint8_t GameStatus = 0;//30秒时间片分段
uint8_t TotalScore = 0;//总分数
uint8_t MyStatus = 0;
uint8_t IsBehind = 0;

typedef struct
{
  unsigned char head_x;
  unsigned char head_y;
  unsigned char rear_x;
  unsigned char rear_y;
  unsigned char score;
}PlayerInf;

PlayerInf myInf, enemyInf;

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

void SetTarget_Temp(uint8_t x, uint8_t y)
{
  target_t[0] = x;
  target_t[1] = y;
}

uint8_t SetTarget_Behind(uint8_t x, uint8_t y)
{
	int16_t temp[2];
	temp[0] = (int16_t)2 * x - holes[emy_hole_number][0];
	temp[1] = (int16_t)2 * y - holes[emy_hole_number][1];
	if (temp[0] > 10 && temp[0] < 245 && temp[1] > 10 && temp[1] < 245 && !IsAHole(temp[0],temp[1]))
	{
		target[0] = temp[0];
		target[1] = temp[1];
		return 1;
	}
	else
		return 0;
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
	my_hole_number = i;
}

void NearestHole_emy(uint8_t x, uint8_t y)
{
  uint8_t i = 0;
  if (x > 128)
    i += 1;
  if (y > 128)
    i += 2;
  nearest_emy[0] = holes[i][0];
  nearest_emy[1] = holes[i][1];
	emy_hole_number = i;
}

float Distance(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
  float dx = x1-x2, dy = y1-y2;
  return sqrtf(dx*dx+dy*dy);
}

void BackTo(int16_t dis)
{
	if (GameStatus == GOT)
	{
		if (dis <= 0)
			SetSpeed(0);
		else
		{
			SetSpeed(-dis * 1 / 2 - 30);SetType(BT);
		}
	}
	else
	{
		if (dis <= 0)
			SetSpeed(0);
		else if (dis < 54)
		{
			SetSpeed(-20);SetType(BT);
		}
		else
		{
			SetSpeed(-dis * 5 / 6 + 25);SetType(BT);
		}		
	}
}

void Back(int16_t dis)
{
	SetWay(0, -dis);
}

void GoTo(int16_t dis)
{
	if (GameStatus == GOT)
	{
		if (dis <= 0)
			SetSpeed(0);
		else
		{
			SetSpeed(dis * 1 / 2 + 30);SetType(GT);
		}
	}
	else
	{
		if (dis <= 0)
			SetSpeed(0);
		else if (dis < 54)
		{
			SetSpeed(20);SetType(GT);
		}
		else
		{
			SetSpeed(dis * 5 / 6 - 25);SetType(GT);
		}		
	}
}


uint8_t IsAHole(uint8_t x, uint8_t y)
{
	if (((x >= holes[0][0] - 12 && x <= holes[0][0] + 12) || (x >= holes[1][0] - 12 && x <= holes[1][0] + 12))
		&& ((y >= holes[0][1] - 12 && y <= holes[0][1] + 12) || (y >= holes[2][1] - 12 && y <= holes[2][1] + 12)))
		return 1;
	else return 0;
}

uint8_t IsNearHole(uint8_t x, uint8_t y)
{
	if (((x >= holes[0][0] - 40 && x <= holes[0][0] + 40) || (x >= holes[1][0] - 40 && x <= holes[1][0] + 40))
		&& ((y >= holes[0][1] - 40 && y <= holes[0][1] + 40) || (y >= holes[2][1] - 40 && y <= holes[2][1] + 40)))
		return 1;
	else return 0;
}

void CalTarget(void)
{
//	int16_t time = 180 - Inf.time;
	if (Inf.time % 30 == 0 && GameStatus != BEGIN)
	{
		GameStatus = BEGIN;
		TotalScore = Inf.Player[0].score + Inf.Player[1].score;
		NearestHole(myInf.head_x, myInf.head_y);
		SetNextHole(3);
	}
	if (TotalScore < Inf.Player[0].score + Inf.Player[1].score)
	{
		GameStatus = GOT;
		IsBehind = 0;
		TotalScore = Inf.Player[0].score + Inf.Player[1].score;
		MyStatus = GoToCenter;
	}
	if (GameStatus == BEGIN)
	{
		if (Distance(mypos[0], mypos[1], target[0], target[1]) > 5 && (Inf.time % 30) && (Inf.time % 30 <= 5))
			SetTarget(128, 128);
		else
		{
			if (IsAHole(enemyInf.head_x, enemyInf.head_y))
			{
				NearestHole_emy(enemyInf.head_x, enemyInf.head_y);
				if (!Done[emy_hole_number] && emy_hole_number != tar_hole_number)
					SetNextHole(4);
			}
			SetTarget(target_t[0], target_t[1]);
		}
	}
	else if (GameStatus == GOT)
	{
		if ((int16_t)(Distance(mypos[0], mypos[1], 128, 128) + 0.5f) <= 10 && MyStatus == GoToCenter)
			MyStatus = Fight;
		if ((int16_t)(Distance(mypos[0], mypos[1], target[0], target[1]) + 0.5f) <= 8 && MyStatus == GoToBehind)
		{
			MyStatus = Fight;
			if (IsNearHole(enemyInf.head_x, enemyInf.head_y) && (int16_t)(Distance(mypos[0], mypos[1], enemyInf.head_x, enemyInf.head_y) + 0.5f) <= 50)
				IsBehind = 1;
		}
		if (MyStatus == GoToCenter)
			SetTarget(128, 128);
		else if (MyStatus == Fight)
		{
			if (IsNearHole(enemyInf.head_x, enemyInf.head_y) && !IsBehind)
			{
				NearestHole_emy(enemyInf.head_x, enemyInf.head_y);
				if (SetTarget_Behind(enemyInf.head_x, enemyInf.head_y))
					MyStatus = GoToBehind;
				else
					SetTarget(128, 128);
			}
			else
				SetTarget(enemyInf.head_x, enemyInf.head_y);
			if (IsAHole(enemyInf.head_x, enemyInf.head_y))
				SetTarget(128, 128);
		}
	}
	else
		GameStatus = BEGIN;
}

void CalEmy(void)
{
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
  mydir = FixedTan(myInf.head_x - myInf.rear_x, myInf.head_y - myInf.rear_y);
  mypos[0] = (myInf.head_x + myInf.rear_x) >> 1;
  mypos[1] = (myInf.head_y + myInf.rear_y) >> 1;
  tardir = FixedTan(target[0] - mypos[0], target[1] - mypos[1]);
  dis = (int16_t)(Distance(mypos[0], mypos[1], target[0], target[1]) + 0.5f);
  NearestHole(mypos[0],mypos[1]);
  Now.dir = mydir;
  Now.dis = dis;
	StageGyro = (Last.dir - Now.dir + 540) % 360 - 180;
//	StageGyro /= 2;
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
					SetDegree(-transdir);
					BackTo(10);
				}
				else
				{
					SetDegree(-transdir);
					GoTo(10);
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


void SetNextHole(uint8_t mode)//mode : 0 : 下一个 ; 1 : 清除 ; 2 : 排除 ; 3 : 清零，离已最近 ; 4 : 敌人所在坑
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
		else if (mode == 3)
    {
      memset(Done, 0, sizeof(Done));
			tar = my_hole_number + 3;
    }
		else if (mode == 4)
    {
      tar = emy_hole_number + 3;
    }
  }
  tar = (tar + 1) % 4;
  if (Done[0] && Done[1] && Done[2] && Done[3])
    memset(Done, 0, sizeof(Done));
  while (Done[tar])
  {
    tar = (tar + 1) % 4;
  }
  SetTarget_Temp(holes[tar][0], holes[tar][1]);
	tar_hole_number = tar;
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
//	int16_t testData[5][2] = {{0,50},{90,0},{-180,0},{90,0},{0,-50}};
	int16_t testData[4][2] = {{90,0},{-90,0},{90,0},{-90,0}};
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

//	ChangeFlag = 1;

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
#if 0
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
			Back(50);
			MakeNextMove = 3;
      if (target_t[0] == nearest[0] && target_t[1] == nearest[1] && Distance(mypos[0], mypos[1], target_t[0], target_t[1]) < 30)
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
/*		dis = Distance(mypos[0], mypos[1], target[0], target[1]);
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
    }*/
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
		if (TimeTicket >= LastT + 12000) 
		{
			SetWay(testData[i][0], testData[i][1]);
			i = (i+1) % 4;
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
			myInf = Inf.Player[Inf.player_loc];
			enemyInf = Inf.Player[1 - Inf.player_loc];
			CalEmy();
			CalTarget();
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
