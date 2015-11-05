#include "pwm.h"

//timer8 channel 1-4
#define PWM_MOTOR_PORT GPIOC
#define PWM_MOTOR_PIN_1  GPIO_Pin_6
#define PWM_MOTOR_PIN_2  GPIO_Pin_7
#define PWM_MOTOR_PIN_3  GPIO_Pin_8
#define PWM_MOTOR_PIN_4  GPIO_Pin_9

//timer4 channel 3-4
#define PWM_EX_PORT GPIOB
#define PWM_EX_PIN_3  GPIO_Pin_8
#define PWM_EX_PIN_4  GPIO_Pin_9


void PWM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	//clock enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//??????APB2????
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//??????APB2????
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//??????APB2????
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//??????APB1????
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);//??????APB2????


	
	
	//gpio intialize
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//??????50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//??????
	
	//EX
	GPIO_InitStructure.GPIO_Pin =PWM_EX_PIN_3|PWM_EX_PIN_4 ;
	GPIO_Init(PWM_EX_PORT, &GPIO_InitStructure);//?????GPIOx???
	
	//MOTOR
	GPIO_InitStructure.GPIO_Pin = PWM_MOTOR_PIN_1|PWM_MOTOR_PIN_2|PWM_MOTOR_PIN_3|PWM_MOTOR_PIN_4;
	GPIO_Init(PWM_MOTOR_PORT, &GPIO_InitStructure);//?????GPIOx???


	//timer4 initialize
	
	//timer4 base initialize
	TIM_TimeBaseStructure.TIM_Prescaler = 11;//????
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//??????
	TIM_TimeBaseStructure.TIM_Period = 199;//?????????????????????????
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x00;//?????
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//???
	
	//timer4 channel3 intialize
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//PWM??1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//??
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//??
	TIM_OCInitStructure.TIM_Pulse = 60;//????,????????
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//?????
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;//?????
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;//???
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//???
	TIM_OC3Init(TIM4,&TIM_OCInitStructure);//???
	
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);//?????TIM?????????CCR3

	//timer4 channel4 initialize
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//PWM??1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//??
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//??
	TIM_OCInitStructure.TIM_Pulse =140 ;//????,????????
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//?????
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;//?????
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;//???
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//???
	TIM_OC4Init(TIM4,&TIM_OCInitStructure);//???

	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);//?????TIM?????????CCR4

	//enable pwm out put
	TIM_ARRPreloadConfig(TIM4, ENABLE);//??????TIMx?ARR????????

	TIM_Cmd(TIM4, ENABLE);//??????TIMx??


	//timer8 initialize
	
	//timer8 base initialize
	TIM_TimeBaseStructure.TIM_Prescaler = 10;//????
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//??????
	TIM_TimeBaseStructure.TIM_Period = 799;//?????????????????????????
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x00;//?????
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);//???

	//timer8 channel 1 initial
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//PWM??1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//??
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//??
	TIM_OCInitStructure.TIM_Pulse = 0;//????,????????
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//?????
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//?????
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;//???
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//???
	TIM_OC1Init(TIM8,&TIM_OCInitStructure);//???
  TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);//?????TIM?????????CCR1

	//timer8 channel 2 initial
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//PWM??1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//??
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//??
	TIM_OCInitStructure.TIM_Pulse = 0;//????,????????
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//?????
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//?????
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;//???
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//???
	TIM_OC2Init(TIM8,&TIM_OCInitStructure);//???
  TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);//?????TIM?????????CCR1

	//timer8 channel 3 initial
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//PWM??1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//??
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//??
	TIM_OCInitStructure.TIM_Pulse = 0;//????,????????
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//?????
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//?????
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;//???
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//???
	TIM_OC3Init(TIM8,&TIM_OCInitStructure);//???
  TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);//?????TIM?????????CCR1

	//timer8 channel 4 initial
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//PWM??1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//??
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//??
	TIM_OCInitStructure.TIM_Pulse = 0;//????,????????
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//?????
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//?????
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;//???
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//???
	TIM_OC4Init(TIM8,&TIM_OCInitStructure);//???
  TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);//?????TIM?????????CCR1

	TIM_Cmd(TIM8, ENABLE);//??????TIMx??
	TIM_ARRPreloadConfig(TIM8, ENABLE);//??????TIMx?ARR????????
	
	TIM_CtrlPWMOutputs(TIM8,ENABLE);
	
	
}




