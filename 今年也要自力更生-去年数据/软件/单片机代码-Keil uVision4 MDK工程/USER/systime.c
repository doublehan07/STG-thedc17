#include "systime.h"

#include "comunication.h"
#include "public.h"

	
	



static void RTC_NVIC_Config(void)
{	
  NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		//RTC全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//先占优先级1位,从优先级3位
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//先占优先级0位,从优先级4位
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//使能该通道中断
	NVIC_Init(&NVIC_InitStructure);		//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}

uint8_t RTC_Init(void)
{
	//检查是不是第一次配置时钟
 
	if (BKP_ReadBackupRegister(BKP_DR1) != 0x5050)		//从指定的后备寄存器中读出数据:读出了与写入的指定数据不相乎
		{	 			
		USARTSendString("\n first time config rtc");
			
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
		PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问 
		BKP_DeInit();	//复位备份区域 	
			
		RCC_LSEConfig(RCC_LSE_ON);//????????(LSE)
    USARTSendString("\n Start LSE");
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);	//检查指定的RCC标志位设置与否,等待低速晶振就绪

    USARTSendString("\n LSE start success");
			
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//设置RTC时钟(RTCCLK),选择LSE作为RTC时钟    
		RCC_RTCCLKCmd(ENABLE);	//使能RTC时钟  
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		RTC_WaitForSynchro();		//等待RTC寄存器同步  

		USARTSendString("\n synchoro done");
			
		RTC_ITConfig(RTC_IT_SEC, ENABLE);		//使能RTC秒中断
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成

		USARTSendString("\n last write done");
			
		RTC_EnterConfigMode();/// 允许配置	
		RTC_SetPrescaler(32768); //设置RTC预分频的值
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成

		USARTSendString("\n set prescaler done");
			

		RTC_ExitConfigMode(); //退出配置模式  
		BKP_WriteBackupRegister(BKP_DR1, 0X5050);	//向指定的后备寄存器中写入用户程序数据
		}
	else//系统继续计时
		{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
		PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问 
		//BKP_DeInit();	//复位备份区域 	
			

			
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//设置RTC时钟(RTCCLK),选择LSE作为RTC时钟    
		RCC_RTCCLKCmd(ENABLE);	//使能RTC时钟  
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
   
		RTC_WaitForSynchro();	//等待最近一次对RTC寄存器的写操作完成

		USARTSendString("\n synchro done");
			
		RTC_ITConfig(RTC_IT_SEC, ENABLE);	//使能RTC秒中断
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成

		USARTSendString("\n last write done");
		RTC_EnterConfigMode();/// 允许配置	
		RTC_SetPrescaler(32768); //设置RTC预分频的值
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成

		USARTSendString("\n set prescaler done");
			

		RTC_ExitConfigMode(); //退出配置模式  
			
		}
    				     
	  RTC_NVIC_Config();//RCT中断分组设置	
		USARTSendString("\n RTC config success");
			
  
	return 0; //ok

}		 				    
	

void RTC_IRQHandler(void)
{		 
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)//秒钟中断
	{							
		//USARTSendString("one second");
 	}
	if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)//闹钟中断
	{
		RTC_ClearITPendingBit(RTC_IT_ALR);		//清闹钟中断	  	   
  	} 				  								 
	RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);		//清闹钟中断
	RTC_WaitForLastTask();	  	    						 	   	 
}





