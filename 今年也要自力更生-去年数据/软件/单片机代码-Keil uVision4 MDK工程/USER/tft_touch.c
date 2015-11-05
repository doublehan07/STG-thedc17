#include "tft_touch.h"
#include "adc.h"
#include "public.h"

#define TOUCH_PORT GPIOC
#define TOUCH_X1_PIN GPIO_Pin_4
#define TOUCH_Y1_PIN GPIO_Pin_5
#define TOUCH_X2_PIN GPIO_Pin_6
#define TOUCH_Y2_PIN GPIO_Pin_7

void TFT_Touch_SetWaitTouch(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//X low, Y pull high, measure y
	GPIO_InitStructure.GPIO_Pin = TOUCH_Y2_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//??????10MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//????
	GPIO_Init(TOUCH_PORT, &GPIO_InitStructure);//?????GPIOx???
	

	GPIO_InitStructure.GPIO_Pin = TOUCH_Y1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//????
	GPIO_Init(TOUCH_PORT, &GPIO_InitStructure);//?????GPIOx???
	
	
	GPIO_InitStructure.GPIO_Pin = TOUCH_X1_PIN | TOUCH_X2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//????
	GPIO_Init(TOUCH_PORT, &GPIO_InitStructure);//?????GPIOx???
	
	GPIO_ResetBits(TOUCH_PORT,TOUCH_X1_PIN | TOUCH_X2_PIN);	

}

uint8_t TFT_Touch_TouchDetect(void)
{
	return GPIO_ReadInputDataBit(TOUCH_PORT, TOUCH_Y1_PIN);
}


void TFT_Touch_SetMeasure_X(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = TOUCH_X2_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//??????50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//????
	GPIO_Init(TOUCH_PORT, &GPIO_InitStructure);//?????GPIOx???
	
	GPIO_InitStructure.GPIO_Pin = TOUCH_X1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//????
	GPIO_Init(TOUCH_PORT, &GPIO_InitStructure);//?????GPIOx???

	GPIO_InitStructure.GPIO_Pin = TOUCH_Y1_PIN | TOUCH_Y2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//????
	GPIO_Init(TOUCH_PORT, &GPIO_InitStructure);//?????GPIOx???
	
	GPIO_SetBits(TOUCH_PORT, TOUCH_Y2_PIN);
	GPIO_ResetBits(TOUCH_PORT, TOUCH_Y1_PIN);
	
}


void TFT_Touch_SetMeasure_Y(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = TOUCH_Y2_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//??????50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//????
	GPIO_Init(TOUCH_PORT, &GPIO_InitStructure);//?????GPIOx???
	
	GPIO_InitStructure.GPIO_Pin = TOUCH_Y1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//????
	GPIO_Init(TOUCH_PORT, &GPIO_InitStructure);//?????GPIOx???

	GPIO_InitStructure.GPIO_Pin = TOUCH_X1_PIN | TOUCH_X2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//????
	GPIO_Init(TOUCH_PORT, &GPIO_InitStructure);//?????GPIOx???
	
	GPIO_SetBits(TOUCH_PORT, TOUCH_X2_PIN);
	GPIO_ResetBits(TOUCH_PORT, TOUCH_X1_PIN);
}


int16_t TFT_Touch_X, TFT_Touch_Y;
void TFT_Touch_Exec(void)
{
	uint16_t proc[10];
	uint8_t count;
	uint16_t addtemp=0;
	if(TFT_Touch_TouchDetect()) //untouch
	{
		TFT_Touch_X=-1;
		TFT_Touch_Y=-1;
	}
	else    //touched
	{
		//x process
		TFT_Touch_SetMeasure_X();
		for(count=0;count!=10;count++)
		{
			Delay_Simple(1000);
			proc[count]=ADCValue[ADC_TOUCH_X];
		}
		Array16_SmallToBig(proc,10);  
		
		//delete the biggest two and smallest two
		//calculate average
		for(count=2;count!=8;count++)
		  addtemp+=proc[count];
		TFT_Touch_X=addtemp/6;
		
		//y process
		TFT_Touch_SetMeasure_Y();
		for(count=0;count!=10;count++)
		{
			Delay_Simple(1000);
			proc[count]=ADCValue[ADC_TOUCH_Y];
		}
		
		//delete the biggest two and smallest two
		//calculate average
		Array16_SmallToBig(proc,10);
		addtemp=0;
		for(count=2;count!=8;count++)
		  addtemp+=proc[count];
		TFT_Touch_Y=addtemp/6;
		
		//set wait
		TFT_Touch_SetWaitTouch();
	}
}

void TFT_Touch_Init(void)
{
	TFT_Touch_SetWaitTouch();
}





