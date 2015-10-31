/**
  ******************************************************************************
  * @file    PaperCut
  * @author  Zion
  * @version V1.0.0
  * @date    19-September-2011
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
#include "stm32f4_discovery.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx.h"
//#include "usart_dma.h"
//#include <stdio.h>
//#include "dma.h"
//#include "adc.h"  
#include "usart.h"
#include "timer.h"
#include "pwm.h"
/** @addtogroup STM32F4_Discovery_Peripheral_Examples
  * @{
  */
#define ADC3_DR_ADDRESS     ((uint32_t)0x4001224C)
/** @addtogroup TIM_PWM_Output
  * @{
  */ 

uint16_t PrescalerValue = 0;
void Delay(vu32);
void ADC3_CH12_DMA_Config(void);
__IO uint16_t ADC3ConvertedValue = 0;
__IO uint32_t ADC3ConvertedVoltage = 0;
/**
  * @brief  Main programm
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */
  //uint16_t adcValue;
  //ADC_Config();
  /* TIM Configuration */
  TIM_Config();
  pwm_config(PrescalerValue);
  //init_uart();
  USART2_Configuration();
  ADC3_CH12_DMA_Config();
  ADC_SoftwareStartConv(ADC3);
  // uint8_t txDMABuffer[23]={0x11}; 
  // uint16_t txDMARear = sizeof(txDMABuffer); 
  // drv_tx(txDMABuffer txDMARear);
  
  while (1)
  {
    //USART2_puts("hello");
    ADC3ConvertedVoltage = ADC3ConvertedValue *3300/0xFFF;
    //ADC_SoftwareStartConv(ADC3);
    // while(RESET ==ADC_GetFlagStatus(ADC3,ADC_FLAG_EOC));
    // //0~4095 对应到 0~VDD
    // adcValue=ADC_GetConversionValue(ADC3);
    
    //pwm_set_DutyCycle1();
  //   int j = 1000;
  //   while(j--){
  //     pwm_set_DutyCycle1(i);
  //   }
  //   if(i==0)
  //     i=20;
  //char str[20]={0};
  //itoa(ADC3ConvertedVoltage, str, 10);
  //sprintf( str, "%05X", ADC3ConvertedVoltage );
  // Usart2Put('A');
  // USART2_puts(str);
  //Delay(0x8FFFF);  
  USART_SendData(USART2,'A');
  //printf("%s",str);
   //USART2_puts(ADC_ConvertedValue);
   //USART2_puts("ADC_ConvertedValue", ADC_ConvertedValue*3300/4096);
   }
}

/*****************************************************
函数: void Delay(vu32 nCount)
功能: 延时指定时间
参数: vu32 nCount 延时指定时间
返回: 无
******************************************************/
void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}

void ADC3_CH12_DMA_Config(void)
{
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  DMA_InitTypeDef       DMA_InitStructure;
  GPIO_InitTypeDef      GPIO_InitStructure;

  /* Enable ADC3, DMA2 and GPIO clocks ****************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

  /* DMA2 Stream0 channel0 configuration **************************************/
  DMA_InitStructure.DMA_Channel = DMA_Channel_2;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC3_DR_ADDRESS;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC3ConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = 1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
  DMA_Cmd(DMA2_Stream0, ENABLE);

  /* Configure ADC3 Channel12 pin as analog input ******************************/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* ADC Common Init **********************************************************/
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

  /* ADC3 Init ****************************************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC3, &ADC_InitStructure);

  /* ADC3 regular channel12 configuration *************************************/
  ADC_RegularChannelConfig(ADC3, ADC_Channel_12, 1, ADC_SampleTime_3Cycles);

 /* Enable DMA request after last transfer (Single-ADC mode) */
  ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);

  /* Enable ADC3 DMA */
  ADC_DMACmd(ADC3, ENABLE);

  /* Enable ADC3 */
  ADC_Cmd(ADC3, ENABLE);
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  while (1)
  {}
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
