#include "key.h"

#include "sysconfig.h"

#define KEY_PORT GPIOC
#define KEY1_PIN GPIO_Pin_4
#define KEY2_PIN GPIO_Pin_5


void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	

	//clock initial
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//??????APB2????

	
	
	//GPIO Initial
  GPIO_InitStructure.GPIO_Pin = KEY1_PIN|KEY2_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//??????10MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//????
	GPIO_Init(GPIOC, &GPIO_InitStructure);//?????GPIOx???
}

uint8_t GetKeyState(uint8_t key)
{
	if(key==1)
		return GPIO_ReadInputDataBit(KEY_PORT, KEY1_PIN);
	else if(key==2)
		return GPIO_ReadInputDataBit(KEY_PORT, KEY2_PIN);
	else
		return 0;
}

uint8_t KeyState[2]={1,1};
uint8_t KeyReleaseState[2]={0};
uint32_t KeyPressedTime[2]={0};

void Key_Exec(void)
{
	uint8_t i;
	uint8_t temp;
	for(i=0;i!=2;i++)
	{
		temp=GetKeyState(i+1);
		if(KeyState[i]>temp)  //pressed
		{
			KeyPressedTime[i]=TimeTicket;
			KeyState[i]=0;
		}
		else if(KeyState[i]<temp)  //released
		{
			if(TimeTicket-KeyPressedTime[i]<2000)  //short press
				KeyReleaseState[i]=1;
			else   //long press
				KeyReleaseState[i]=2;
			KeyState[i]=1;
		}
		else
		  ;
	}
}

