#include <stm32f4xx.h>
#include "usart.h"
#include <stdio.h>
#include "strategy.h"

#if 1
#pragma import(__use_no_semihosting)             
//±ê×¼¿âÐèÒªµÄÖ§³Öº¯Êý                 
struct __FILE { 
    int handle; 
    /* Whatever you require here. If the only file you are using is */ 
    /* standard output using printf() for debugging, no file handling */ 
    /* is required. */ 
}; 
/* FILE is typedef¡¯ d in stdio.h. */ 
FILE __stdout;       
//¶¨Òå_sys_exit()ÒÔ±ÜÃâÊ¹ÓÃ°ëÖ÷»úÄ£Ê½    
_sys_exit(int x) { 
    x = x; 
} 
//ÖØ¶¨Òåfputcº¯Êý 
int fputc(int Data, FILE *f){   
    while(!USART_GetFlagStatus(USART2,USART_FLAG_TXE));   //USART_GetFlagStatus£ºµÃµ½·¢ËÍ×´Ì¬Î»
                                                          //USART_FLAG_TXE:·¢ËÍ¼Ä´æÆ÷Îª¿Õ 1£ºÎª¿Õ£»0£ºÃ¦×´Ì¬
    USART_SendData(USART2,Data);                          //·¢ËÍÒ»¸ö×Ö·û
       
    return Data;                                          //·µ»ØÒ»¸öÖµ
}
#endif 

void USART2_puts(char* s)
{
    while(*s) {
        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
        USART_SendData(USART2, *s);
        s++;
    }
}

void Usart2Put(uint8_t ch)
{
      USART_SendData(USART2, (uint8_t) ch);
      //Loop until the end of transmission
      while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
      {
      }
}
uint8_t Usart2Get(void){
     while ( USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
        return (uint8_t)USART_ReceiveData(USART2);
}



void USART2_Configuration(void)
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    /* --------------------------- System Clocks Configuration -----------------*/
    /* USART1 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    /* GPIOA clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    
    /*-------------------------- GPIO Configuration ----------------------------*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* Connect USART pins to AF */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);   // USART1_TX
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);  // USART1_RX
    
    
    /* USARTx configuration ------------------------------------------------------*/
    /* USARTx configured as follow:
     *  - BaudRate = 9600 baud
     *  - Word Length = 8 Bits
     *  - One Stop Bit
     *  - No parity
     *  - Hardware flow control disabled (RTS and CTS signals)
     *  - Receive and transmit enabled
     */
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//使能接受中断，在接受移位 寄存器中有数据是产生
    //USART_ITConfig(USART1, USART_IT_TXE, ENABLE);//使能发送中断，在发送完数据 后产生。
    USART_Cmd(USART2, ENABLE);

    

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);     
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 3;    
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&NVIC_InitStructure); 
}

void USART2_IRQHandler(void)
{ 
    //unsigned char i; 
    static char counter = 0;
    char ReceivePacket[23];

    if(USART_GetFlagStatus(USART2,USART_IT_RXNE)==SET) 
    {               
        // i = USART_ReceiveData(USART2); 
        // USART_SendData(USART2,i); 
        // if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
        // { 
        //     USART_ClearITPendingBit(USART2, USART_IT_RXNE);
        // }   
      ReceivePacket[counter] = USART_ReceiveData(USART2);
      counter++;
      if(counter >= 23)
      {
        counter = 0;
        parseReceivedPack(ReceivePacket);
      }
    }
    if(USART_GetFlagStatus(USART2,USART_IT_TC)==SET)
    {
        if(USART_GetITStatus(USART2, USART_IT_TC) != RESET) 
        { 
            USART_ClearITPendingBit(USART2, USART_IT_TC);
        }
    }
}

// #pragma import(__use_no_semihosting)
// _sys_exit(int x)     //定义_sys_exit()以避免使用半主机模式 
// { 
//     x = x; 
// }   
// struct __FILE  //标准库需要的支持函数 
// { 
// int handle; 
// }; 
// /* FILE is typedef’ d in stdio.h. */ 
// FILE __stdout;




/*******************************************************************************
* Function Name  : int fputc(int ch, FILE *f)
* Description    : Retargets the C library printf function to the USART.printfÖØ¶¨Ïò
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
// int fputc(int ch, FILE *f )
// {
//   /* Write a character to the USART */
//   USART_SendData(USART2, (u8) ch);

//    //Loop until the end of transmission 
//   while(!(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == SET))
//   {
//   }

//   return ch;
// }

/*******************************************************************************
* Function Name  : int fgetc(FILE *f)
* Description    : Retargets the C library printf function to the USART.fgetcÖØ¶¨Ïò
* Input          : None
* Output         : None
* Return         : ¶ÁÈ¡µ½µÄ×Ö·û
*******************************************************************************/
int fgetc(FILE *f)
{
  /* Loop until received a char */
  while(!(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET))
  {
  }
  
    /* Read a character from the USART and RETURN */
  return (USART_ReceiveData(USART2));
}

