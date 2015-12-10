/**
  ******************************************************************************
  * @file    TIM_PWM_Output/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Main Interrupt Service Routines.
  *         This file provides template for all exceptions handler and
  *         peripherals interrupt service routine.
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
#include "stm32f4xx_it.h"
#include "common.h"
#include "motor.h"
#include "usart.h"
#include "strategy.h"
#include "map.h"

extern uint8_t isParsed , isFinished , toBeReceive , requestToBeSent;
char counter, lastReceived , currentReceived ;
extern char ReceivedPacket[23] , toBeParsedPack[23];
extern char receive_data[32];
extern struct RECEIVE ReceiveAI ;
extern struct SEND SendAI;
extern char destination[2];
extern int16_t destinationAngle;
extern char haveDestination;
extern char IMUinitSuccess ;
extern char KeepForward , lastKeepForward ;


//extern FunType fp;
/** @addtogroup STM32F4_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup TIM_PWM_Output
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

//检测在黑线还是白线，驱动电机
void EXTI9_5_IRQHandler(void)/*{{{*/
{

  
    //Delay(0xFFFFFF);  
  
}/*}}}*/

void USART2_IRQHandler(void)
{ 

  if(USART_GetFlagStatus(USART2,USART_IT_RXNE)==SET) 
    {   
      //USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);        
      lastReceived = currentReceived ;
      currentReceived = USART_ReceiveData(USART2);
      if (currentReceived == 0x0A && lastReceived == 0x0D)
      {
        isFinished = 1;//看到结尾符
        toBeReceive = 1;//正在接收数据
      }

      if(toBeReceive == 1)
      {
        if(isFinished){
          isFinished = 0;
        }else{
          ReceivedPacket[counter] = currentReceived;
          
          counter++;
          if (counter == 22)
          {
            toBeReceive = 0;
            counter = 0;
            ReceivedPacket[22]=0x0A;
            isFinished = 1;
						parseReceivedPack(ReceivedPacket);
          }
        }
			}
		}
}


void UART5_IRQHandler(void)
{//接受惯导传来的姿态位置信息
	if(USART_GetFlagStatus(UART5,USART_IT_RXNE)==SET) 
    {   
			char ch = USART_ReceiveData(UART5);
			imu_rev_process(ch);
			//printf("%c",ch);
		}
}


void TIM2_IRQHandler(void){//轮询上位机指令进行电机控制
  //TIM_ClearFlag
	
	static char if_turn_l = 0, if_turn_r = 0, if_backward = 0;
	static int counter_l = 0, counter_r = 0, counter_b = 0;
	static char debug = 0;
	/**
	KeepForward = 0;测试用
	到时候记得去掉这个flag
	**/
	KeepForward = 0;
	if(ReceiveAI.Status != 0x01)
	{
		motor_sleep();
	}
	else
	{
		motor_wake();
		
		//turn right to get prop
//		if(debug == 0)
//		{
//			motor_turn_right(-600,1000); //1500 is ok
//			Delay(0xFFFF00);
//			motor_forward();
//			motor_setSpeed(800);
//			Delay(0xFFFF00);
//			debug++;
//		}
	}

	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
			if(if_turn_l == 1 && !GPIOD->IDR&GPIO_Pin_8)
			{
				counter_l++;
				if(counter_l >= 14000)
				{
					if_turn_l = 0;
					counter_l = 0;
				}
			}
			else if(if_turn_r == 1 && !GPIOD->IDR&GPIO_Pin_9)
			{
				counter_r++;
				if(counter_r >= 14000)
				{
					if_turn_r = 0;
					counter_r = 0;
				}
			}
			else if(if_backward == 1) //May cause problem. Ban reverse runnning! - by haldak
			{
				counter_b++;
				if(counter_b == 15000)
				{
					motor_turn_left(-500,1000); //1600 is ok
				}
				if(counter_b >= 30000)
				{
					counter_b = 0;
					if_backward = 0;
		  	}
	  	}
		
      if(GPIOD->IDR&GPIO_Pin_8 && GPIOD->IDR&GPIO_Pin_9) //两边碰线
			{
        //USART_SendData(USART2,'a');
				motor_setSpeed(1000); //800 is ok
				if_backward = 1;
				motor_backward();
      }
        
      else if(GPIOD->IDR&GPIO_Pin_8) //左边碰线
			{ 
 				//USART_SendData(USART2,'b');
				if_turn_r = 1;
				motor_turn_right(-600,1000); //1500 is ok
      }
   
      else if(GPIOD->IDR&GPIO_Pin_9) //右边碰线
			{ 
        //USART_SendData(USART2,'c');
				if_turn_l = 1;
        motor_turn_left(-600,1000); //1300 is ok
      }
        
      else //没有碰线
			{ 
				if(haveDestination){
					headTo(destination[0],destination[1], destinationAngle);
				}else{
				motor_setSpeed(800); //800 is ok
				motor_forward();
				}
				KeepForward = 1;
      }
  }
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}


void TIM5_IRQHandler(void){//不断对更新的接收包进行决策
  if(isParsed == 1){
		isParsed=0;
		checkWhetherToBeAttacked();
		useAttackProp();
		useTrapProp();//待验证
		saveMap();//待验证
		/**
		每次收到一个包，都判断下当前运动状态和上次收到包时的运动状态。
		若都是处于前进状态，再判断是否角度没有变换。
		若是，基本确定是走了直线，将当前位置和上次位置进行角度换算
		**/
		if(!IMUinitSuccess){
			if(KeepForward == 1 && lastKeepForward == 1){
				checkWhetherInitIMUSuccess();
			}
			lastKeepForward = KeepForward ;
		}
		/**
		如果有目标，查看目标是否已经丢失
		如果没有目标，搜索合适的道具点并且headTo
		**/
		if(IMUinitSuccess == 1){
			if(!haveDestination){
				checkWhetherToPickProp();
			}else{
				chechWhetherStillShouldHeadTo();
			}
		}
  }
	getProp();
//	if(requestToBeSent == 1){
//		sendDefaultPack();
//		requestToBeSent = 0;
//	}
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
}


