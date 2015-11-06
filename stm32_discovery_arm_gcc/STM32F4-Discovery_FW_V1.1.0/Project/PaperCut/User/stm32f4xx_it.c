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
#include "stm32f4_discovery.h"
#include "common.h"
#include "motor.h"
#include "usart.h"
#include "strategy.h"

extern uint8_t isParsed , isFinished , toBeReceive , requestToBeSent;
char counter, lastReceived , currentReceived;
extern char ReceivedPacket[23] , toBeParsedPack[23];
extern char receive_data[32];
extern struct RECEIVE ReceiveAI ;
extern struct SEND SendAI;
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
      
      // if(isFinished == 0 && toBeReceive == 0){

      // }//所有数据不可靠
        
      if (currentReceived == 0x0A && lastReceived == 0x0D)
      {
        isFinished = 1;//看到结尾符
        toBeReceive = 1;//正在接收数据
        //counter=0;
        //Usart2Put(currentReceived);
      }

      if(toBeReceive == 1)
      {
        //Usart2Put(currentReceived);
        if(isFinished){
          isFinished = 0;
          //return ;
        }else{
          ReceivedPacket[counter] = currentReceived;
          
          counter++;
          // if (counter == 21)
          // {
          //   toBeReceive = 0;
          //   counter++;
          //   ReceivedPacket[counter] = 0x0A;
          // }
          if (counter == 22)
          {
            toBeReceive = 0;
            counter = 0;
            ReceivedPacket[22]=0x0A;
             //if(ReceivedPacket[21] == 0x0D && ReceivedPacket[22] == 0x0A){
            //parseReceivedPack(ReceivedPacket);
            
               //USART2_puts(s);
               //USART2_puts("   ");
            // for( ; i<23 ; i++){
              //USART_SendData(USART2,ReceivedPacket[i]);
            //}

            isFinished = 1;
             //printf("%s",ReceivedPacket);
						//printf("h");
						parseReceivedPack(ReceivedPacket);
						//printf("i");
          }
        }
			}
		}
}


void UART5_IRQHandler(void)
{

}


void TIM2_IRQHandler(void){
  //TIM_ClearFlag
	
	static char if_turn_l = 0, if_turn_r = 0, if_backward = 0;
	static int counter_l = 0, counter_r = 0, counter_b = 0;
	if(ReceiveAI.Status != 0x01){
		motor_sleep();
		Usart2Put(0x01);
	}else{
		motor_wake();
		Usart2Put(0x02);
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
				motor_turn_right(-700,1000); //1500 is ok
      }
   
      else if(GPIOD->IDR&GPIO_Pin_9) //右边碰线
			{ 
        //USART_SendData(USART2,'c');
				if_turn_l = 1;
        motor_turn_left(-700,1000); //1300 is ok
      }
        
      else //没有碰线
			{ 
        //USART_SendData(USART2,'d');
				motor_setSpeed(750); //800 is ok
				motor_forward();
      }
  }
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}


void TIM5_IRQHandler(void){
  if(isParsed == 1){
      //Send
		//printf("h");
		//printf("h");
      //if (USART_GetFlagStatus(USART2, USART_IT_TC) == SET){
		//int i= 0;
//		for( ;i<3;i++)
//        printf("%c",*((uint8_t *)&SendAI+i));
		
				//USART_SendData(USART2, *(uint8_t *)&SendAI); //这一行将直接发送SendAI，SendAI数据使用
				//strategy.c的void parseSendPack(char OurID, char TargetID, char Prop)设置！！
		//requestToBeSent=1;
		isParsed=0;
		
		
      //}
  }
	
	if(requestToBeSent == 1){
		int i = 0 ;
		for(;i<3;i++){
			printf("%c",*((uint8_t *)&SendAI+i));
		}
		//printf("%s",*(uint8_t *)&SendAI);
		requestToBeSent = 0;
	}
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
