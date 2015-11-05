#include "sysconfig.h"
#include "comunication.h"





//===========================clock===================================


void SetMCUFrequency(void)
{

	USARTSendString("start hse");
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);//??????(SYSCLK)
  RCC_PLLCmd(DISABLE);//??????PLL 
	
	RCC_PLLConfig(RCC_PLLSource_HSE_Div2,RCC_PLLMul_11); 
		
	RCC_PLLCmd(ENABLE);//??????PLL 
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);//??????(SYSCLK)
	RCC_PCLK1Config(RCC_HCLK_Div2);//设置低速AHB时钟（PCLK1）
	RCC_PCLK2Config(RCC_HCLK_Div1);//设置低速AHB时钟（PCLK2）		
	SystemCoreClockUpdate();
		  
}


void SysTickInit(void)
{
	RCC_ClocksTypeDef RCC_Clocks;
	
	RCC_GetClocksFreq(&RCC_Clocks);//返回不同片上时钟的频率
	
	SysTick_Config(44000);
//	SysTick_Config(RCC_Clocks.SYSCLK_Frequency / 1000);
}

volatile uint32_t TimeTicket=0;


void TimeTicketSevice(void)
{
	TimeTicket++;
}

//-------------------------------------------------------------------

void ClockInit(void)
{


	SetMCUFrequency();
	SysTickInit();
	
}


//============================other===================================

void DisableJTAG(void)
{
	//clock initialize
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//使能或者失能APB2外设时钟

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST, ENABLE);//改变指定管脚的映射

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//改变指定管脚的映射

	
}

//========================interrupt==================================

void InterruptInit(void)
{

}



//###################################################################



void System_Init(void)
{
	DisableJTAG();
	ClockInit();
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//??????AHB????

}





