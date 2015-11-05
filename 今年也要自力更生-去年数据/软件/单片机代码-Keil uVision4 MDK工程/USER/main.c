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
uint8_t BufFront = 0, BufRear = 1;//以上为UART的缓冲区 
uint8_t target[2] = {0};//目标的坐标，设置后GoToTarget()
uint8_t target_t[2] = {0};//临时的目标坐标，主要是由SetNextHole()设置
const uint8_t holes[4][2] = {{57,57},{195,57},{57,195},{195,195}};
uint8_t nearest[2];//调用NearestHole()之后得到的最近坑的坐标
uint8_t my_hole_number;//离已方最近的坑的编号(上面holes的下标，0~3)
uint8_t tar_hole_number;//己方的目标坑编号，由SetNextHole()设置
uint16_t mypos[2];//己方坐标
uint8_t Done[4] = {0};//取1表示为已探坑，0表示未探或者正在路上
uint8_t EmyDone[4] = {0};

uint32_t UARTCount = 0;
uint32_t LastUART = 0;//上一次正确读出UART的时间

uint8_t Senser[2] = {1,1};

//对敌判断
uint8_t emy_hole_number;//nearest hole of emy
uint8_t nearest_emy[2];//position of it
uint8_t emy_target[5] = {0};//0~3holes 4:me
uint8_t emy_target_hole = 0;//is emy's target a hole?
int8_t emy_speed_avg = 0;//敌方均速

/*GameStatus的状态，即游戏状态*/
#define BEGIN 1
#define GOT 2

/*MyStatus的状态，即自己的策略*/
#define GoToCenter 1
#define Fight 2
#define GoToBehind 3

uint8_t GameStatus = 0;//30秒时间片分段
uint8_t TotalScore = 0;//总分数
uint8_t MyStatus = 0;
uint8_t IsBehind = 0;//是否已经抵达敌方后面

uint8_t IsMeInHole = 0;
uint8_t IsEmyInHole = 0;

uint8_t LastScoreHole = 4;

uint8_t MyScore = 0;
uint8_t EmyScore = 0;

/*各种结构体*/
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

struct Position
{
	uint8_t x, y;
}LastEmy, NowEmy;


//运动控制
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

//目标设置及策略
void SetTarget(uint8_t x, uint8_t y)//设置入target[]
{
  target[0] = x;
  target[1] = y;
}

void SetTarget_Temp(uint8_t x, uint8_t y)//写入target_t[]
{
  target_t[0] = x;
  target_t[1] = y;
}

uint8_t SetTarget_Behind(uint8_t x, uint8_t y)//找到敌后方并设置为目标，如果不可行（比如后方在场地外）则返回0
{
	int16_t temp[2];
	temp[0] = (int16_t)2 * x - holes[emy_hole_number][0];
	temp[1] = (int16_t)2 * y - holes[emy_hole_number][1];
	if (temp[0] > 50 && temp[0] < 205 && temp[1] > 50 && temp[1] < 205 && !IsAHole(temp[0],temp[1]))
	{
		target[0] = temp[0];
		target[1] = temp[1];
		return 1;
	}
	else
		return 0;
}

void NearestHole(uint8_t x, uint8_t y)//寻找离自己最近的坑并写入nearest[]及my_hole_number
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

void NearestHole_emy(uint8_t x, uint8_t y)//寻找离敌最近的坑并写入nearest_emy[]及emy_hole_number
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

float Distance(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)//距离
{
  float dx = x1-x2, dy = y1-y2;
  return sqrtf(dx*dx+dy*dy);
}


uint8_t IsAHole(uint8_t x, uint8_t y)//判断此坐标是否在坑内
{
	if (((x >= holes[0][0] - 12 && x <= holes[0][0] + 12) || (x >= holes[1][0] - 12 && x <= holes[1][0] + 12))
		&& ((y >= holes[0][1] - 12 && y <= holes[0][1] + 12) || (y >= holes[2][1] - 12 && y <= holes[2][1] + 12)))
		return 1;
	else return 0;
}

uint8_t IsNearHole(uint8_t x, uint8_t y)//判断此坐标是否在坑附近
{
	if (((x >= holes[0][0] - 40 && x <= holes[0][0] + 40) || (x >= holes[1][0] - 40 && x <= holes[1][0] + 40))
		&& ((y >= holes[0][1] - 40 && y <= holes[0][1] + 40) || (y >= holes[2][1] - 40 && y <= holes[2][1] + 40)))
		return 1;
	else return 0;
}

void CalTarget(void)
{
	static uint8_t LastTime = 0;
//	int16_t time = 180 - Inf.time;
	NearestHole(mypos[0], mypos[1]);
	NearestHole_emy(enemyInf.head_x, enemyInf.head_y);
	//状态转换区
	if (Inf.time % 30 == 0 && LastTime != Inf.time)//30s时间片开始
	{
		LastTime = Inf.time;
		GameStatus = BEGIN;
		TotalScore = Inf.Player[0].score + Inf.Player[1].score;
		NearestHole(myInf.head_x, myInf.head_y);
		SetNextHole(3);
		memset(EmyDone, 0, sizeof(EmyDone));
		LastScoreHole = 4;
		MyScore = myInf.score;
		EmyScore = enemyInf.score;
	}
	if (TotalScore < Inf.Player[0].score + Inf.Player[1].score)//30s内分数已被得到，设定目标为场地中央
	{
		GameStatus = GOT;
		IsBehind = 0;
		TotalScore = Inf.Player[0].score + Inf.Player[1].score;
		MyStatus = GoToCenter;
		if (EmyScore < enemyInf.score)
		{
			LastScoreHole = emy_hole_number;
			EmyScore = enemyInf.score;
		}
		if (MyScore < myInf.score)
		{
			LastScoreHole = my_hole_number;
			MyScore = myInf.score;
		}
	}
	if (!IsAHole(enemyInf.head_x, enemyInf.head_y))//敌方是否在坑中
	{
		IsEmyInHole = 0;
	}
	if (!IsAHole(mypos[0], mypos[1]))//己方是否在坑中
	{
		IsMeInHole = 0;
	}
	//依据状态执行
	if (GameStatus == BEGIN)
	{
		if (IsAHole(mypos[0], mypos[1]) && Done[my_hole_number] && Distance(mypos[0], mypos[1], target[0], target[1]) > 30)
			IsMeInHole = 1;
		if (IsMeInHole)
		{
			SetTarget(holes[my_hole_number][0], holes[my_hole_number][1]);
			if (Inf.available && !IsTargetAvailable(Inf.available))
			{
				SetTarget(128, 128);
				IsMeInHole = 0;
			}
		}
		else
		{
			if (Distance(mypos[0], mypos[1], target[0], target[1]) > 5 && (Inf.time % 30) && (Inf.time % 30 <= 6))
				SetTarget(128, 128);//最终剩余不到6秒并且仍未到坑中央
			else
			{
				if (IsAHole(enemyInf.head_x, enemyInf.head_y))//敌方是否在坑中得分
				{
					if (!Done[emy_hole_number] && emy_hole_number != tar_hole_number)
						SetNextHole(4);//强制变更目标为敌方所在坑
					else if (Done[emy_hole_number])
						IsEmyInHole = 1;
				}
				SetTarget(target_t[0], target_t[1]);//无论是否改变目标，都使用SetNextHole()设置的目标（因为可能会在主函数中被调用）
			}
		}
	}
	else if (GameStatus == GOT)//刚改变成此状态时会前往场地中央
	{
		//状态转换区
		if (TotalScore > Inf.Player[0].score + Inf.Player[1].score)
		{
			TotalScore = Inf.Player[0].score + Inf.Player[1].score;
			if (MyScore > myInf.score)
			{
				IsMeInHole = 1;
				MyScore = myInf.score;
			}
			if (EmyScore > enemyInf.score)
			{
				IsEmyInHole = 1;
				EmyScore = enemyInf.score;
			}
		}
		if ((int16_t)(Distance(mypos[0], mypos[1], 128, 128) + 0.5f) <= 10 && MyStatus == GoToCenter)//已经抵达场地中央
		{
			MyStatus = Fight;
			TurnToEmy();
		}
		if ((int16_t)(Distance(mypos[0], mypos[1], target[0], target[1]) + 0.5f) <= 8 && MyStatus == GoToBehind)//已经抵达目标后方
		{
			MyStatus = Fight;
			if (IsNearHole(enemyInf.head_x, enemyInf.head_y) && (int16_t)(Distance(mypos[0], mypos[1], enemyInf.head_x, enemyInf.head_y) + 0.5f) <= 50)
				IsBehind = 1;
		}
		//状态执行区
		if (IsMeInHole)
		{
			SetTarget(holes[my_hole_number][0], holes[my_hole_number][1]);
			if (Inf.available && !IsTargetAvailable(Inf.available))
			{
				SetTarget(128, 128);
				IsMeInHole = 0;
			}
		}
		else
		{
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
				else if (enemyInf.head_x > 50 && enemyInf.head_x < 205 && enemyInf.head_y > 50 && enemyInf.head_y < 205 && IsBehind)
					SetTarget(enemyInf.head_x, enemyInf.head_y);
				else
					SetTarget(128, 128);
				if (IsAHole(enemyInf.head_x, enemyInf.head_y))
				{
					SetTarget(128, 128);
					IsEmyInHole = 1;
				}
			}
		}
	}
	else//错误状态恢复
		GameStatus = BEGIN;
}

void CalEmy(void)
{
	LastEmy = NowEmy;
	NowEmy.x = enemyInf.head_x;
	NowEmy.y = enemyInf.head_y;
	memset(emy_target, 0, sizeof(emy_target));//清零
	if (Distance(LastEmy.x, LastEmy.y, NowEmy.x, NowEmy.y) > 5)//敌移动距离超过5
	{
		int16_t emydir = FixedTan(NowEmy.x - LastEmy.x, NowEmy.y - LastEmy.y);
		int16_t transdir;
		uint8_t i;
		for (i = 0; i < 4; i++)//(移动方向)和(敌与坑方向)差小于20度则判定为有目的
		{
			int16_t tardir = FixedTan(holes[i][0] - NowEmy.x, holes[i][1] - NowEmy.y);
			transdir = (540 + tardir - emydir) % 360 - 180;
			if (transdir < 20 && transdir > -20)
			{
				emy_target[i] = 1;
				emy_target_hole = 1;
				if (Distance(holes[i][0], holes[i][1], NowEmy.x, NowEmy.y) < 45)
					EmyDone[i] = 1;
			}
		}
		transdir = (540 + FixedTan(mypos[0] - NowEmy.x, mypos[1] - NowEmy.y) - emydir) % 360 - 180;
		if (transdir < 20 && transdir > -20)//是否有可能朝己方而来
			emy_target[4] = 1;		
	}
	if ((int16_t)(Distance(LastEmy.x, LastEmy.y, NowEmy.x, NowEmy.y) + 0.5f) < 15)//如果敌方运动速度不科学则不计入平均速度
		emy_speed_avg = (emy_speed_avg * 9 + (int16_t)(Distance(LastEmy.x, LastEmy.y, NowEmy.x, NowEmy.y) + 0.5f)) / 10;
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
					if (transdir > 8 || transdir < -8)
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

void TurnToEmy(void)
{
  int16_t mydir, emydir;
  int16_t transdir;
  mydir = FixedTan(myInf.head_x - myInf.rear_x, myInf.head_y - myInf.rear_y);
  emydir = FixedTan(NowEmy.x - mypos[0], NowEmy.y - mypos[1]);
	if (MakeNextMove <= 0)
	{
		if (Inf.is_over && !ChangeFlag)
		{
			transdir = (540 + emydir - mydir) % 360 - 180;
			if (TurnFinish)
			{
				SetType(ST);
				SetSpeed(0);
				if (transdir > 8 || transdir < -8)
				{
					SetDegree(-transdir);
				}
				else
				{
					SetDegree(0);
				}
			}
		}
		else
			Stop();
	}
}

void SetNextHole(uint8_t mode)//mode : 0 : 下一个 ; 1 : 清除所有已探记录并设置当前坑为已探 ; 2 : 排除当前目标 ; 3 : 清零，取离已最近 ; 4 : 敌人所在坑
{
  static uint8_t tar = 3;
	if (Senser[0] || Senser[1] || mode == 4)
	{
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
				if (IsMeInHole)
					Done[my_hole_number] = 1;
				if (IsEmyInHole)
					Done[emy_hole_number] = 1;		
				if (LastScoreHole < 4)
					Done[LastScoreHole] = 1;
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
		if (mode != 4 && emy_target_hole)
		{
			uint8_t i, cal = 0, temp;
			for (i = 0; i < 4; i++)
			{
				if (emy_target[i] && !Done[(i + 2) % 4])
				{
					temp = (i + 2) % 4;
					cal++;
				}
			}
			if (cal != 1)
			{
				cal = 0;
				for (i = 0; i < 4; i++)
				{
					if (!emy_target[i] && !Done[i])
					{
						temp = i;
						cal++;
					}
				}
			}
			if (cal)
				tar = temp;
		}
		while (Done[tar])
		{
			tar = (tar + 1) % 4;
		}
		SetTarget_Temp(holes[tar][0], holes[tar][1]);
		tar_hole_number = tar;
	}
	else
		SetTarget_Temp(128, 128);
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
      if ((available << i) & 0x08)
        return 0;
      else
        return 1;
    }
  }
  return 1;
}

int main(void)
{
	uint32_t LastT = 0, LastIIC = 0;//上一次正确读出IIC的时间
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
/*IIC通信*/
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
/*探坑及后退*/
		if (!Inf.is_over || Distance(myInf.head_x, myInf.head_y, myInf.rear_x, myInf.rear_y) > 15 || LastUART + 300 < TimeTicket)
		//结束或坐标飘了或300ms没有收到数据
		{
			Stop();
		}
		if (((!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12) && Senser[0]) || (!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13) && Senser[1]))//传感器返回为坑
			&& (mypos[0] > 140 || mypos[0] < 125 || mypos[1] > 200 || mypos[1] < 50)//中场线则不视为坑
			&& (Distance(mypos[0], mypos[1], target[0], target[1]) < 40 || (mypos[0] > 240 || mypos[0] < 15 || mypos[1] > 240 || mypos[1] < 15)))//离目标较近或在场地边缘
		{
			Back(50);
			MakeNextMove = 3;
      if (target_t[0] == nearest[0] && target_t[1] == nearest[1] && Distance(mypos[0], mypos[1], target_t[0], target_t[1]) < 30)
        SetNextHole(2);
      else
        SetNextHole(0);
		}
		if (UARTCount > 0 && UARTCount < 10 && (!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12) || !GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)))
		{
			Senser[0] = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12);
			Senser[1] = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13);
		}
    if (Inf.available)
    {
      if (!IsTargetAvailable(Inf.available))
      {
        SetNextHole(2);
      }
    }
    if (!Inf.is_over || Distance(myInf.head_x, myInf.head_y, myInf.rear_x, myInf.rear_y) > 15 || LastUART + 300 < TimeTicket)
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

		
	
	
	
	

uint8_t GetData( void )//取出串口中的数据
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
			UARTCount++;
			LastUART = TimeTicket;//记录最后一次获得数据的时间
			myInf = Inf.Player[Inf.player_loc];
			enemyInf = Inf.Player[1 - Inf.player_loc];
			mypos[0] = (myInf.head_x + myInf.rear_x) >> 1;
			mypos[1] = (myInf.head_y + myInf.rear_y) >> 1;
			CalEmy();//判断对手动向
			CalTarget();//进行行动目标的确定
			GoToTarget();//往目标前进
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
