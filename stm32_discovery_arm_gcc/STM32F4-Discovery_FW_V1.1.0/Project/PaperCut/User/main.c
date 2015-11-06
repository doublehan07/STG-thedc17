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
//m#include "adc.h"  
#include "usart.h"
#include "timer.h"
#include "pwm.h"
#include "adc_dma.h"
#include "infrared.h"
#include "common.h"
/** @addtogroup STM32F4_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup TIM_PWM_Output
  * @{
  */ 

uint16_t PrescalerValue = 0;
extern __IO uint16_t ADC3ConvertedValue ;
extern __IO uint32_t ADC3ConvertedVoltage ;
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
  TIM1_TIM3_Config();
  //pwm_config(PrescalerValue);
	pwm1_config(PrescalerValue);
  //init_uart();
  USART2_Configuration();
  UART5_Configuration();
  //ADC3_CH12_DMA_Config();
  ADC1_CH4_CH5_DMA_Config();
  ADC_SoftwareStartConv(ADC1);
  Infrared_GPIO_Config();
  NVIC_Config();
  ADC3_CH12_DMA_Config();
  TIM2_Config();
	TIM5_Config();
  // uint8_t txDMABuffer[23]={0x11}; 
  // uint16_t txDMARear = sizeof(txDMABuffer); 
  // drv_tx(txDMABuffer txDMARear);
	motor_config();
	motor_wake();
  while (1)
  {
    //USART2_puts("hello");
		motor_backward();
    //ADC3ConvertedVoltage = ADC3ConvertedValue *3300/0xFFF;

    // while(RESET ==ADC_GetFlagStatus(ADC3,ADC_FLAG_EOC));
    // //0~4095 对应到 0~VDD
    // adcValue=ADC_GetConversionValue(ADC3);
    
    //pwm_set_DutyCycle1();
  //   int j = 1000;
  //   while(j--){
  //     pwm_set_DutyCycle1(i);
  //   }
  //   if(i==0)
  //itoa(ADC3ConvertedVoltage, str, 10);
  //sprintf( str, "%05X", ADC3ConvertedVoltage );
  // Usart2Put('A');
  // USART2_puts(str);

  // USART_SendData(UART5,'a');
  // USART_SendData(USART2,'b');  
  //printf("system_stm32f4xx");
  Delay(0xBFFFF);  
    //printf("%d\n",ADC3ConvertedVoltage);
   }
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
