#include "debug_exec.h"

#include "public.h"
#include "ADC.h"
#include "motor_ctr.h"
#include "IIC_6050.h"



volatile uint8_t USART1_SBF=0;   //USART1 send busy flag

uint8_t USART1_SendViaDMA(uint8_t *d, uint16_t size)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	if(USART1_SBF)
		return 1;
	
	USART1_SBF=1;
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART1->DR));//?????
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)d;//?????
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//????????????
	DMA_InitStructure.DMA_BufferSize = size;//??DMA???DMA?????,???????
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//?????????
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//?????????
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//?????8?
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//?????8?
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//?????????
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//????
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//????????
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);//?????CAN????
	
	DMA_Cmd(DMA1_Channel4,ENABLE);//???????????x(DMA1 ??4)

	return 0;
}


void DMA1_Channel4_IRQHandler(void)
{
	DMA_ClearITPendingBit(DMA1_IT_TC4 );//??DMA??x????????(DMA1??4??????)
	DMA_ClearFlag(DMA1_FLAG_TC4 );//??DMA??x??????(DMA1??4???????)
	DMA_Cmd(DMA1_Channel4,DISABLE);//???????????x(DMA1 ??4)
	USART1_SBF=0;
}




uint16_t getstringsize(uint8_t *s)
{
	uint16_t count=0;
	while(*(s+(count++)));
	return count;
}

uint8_t USART1_SendStringViaDMA(uint8_t *s)
{
	return USART1_SendViaDMA(s, getstringsize(s));
}

	uint8_t stemp[40]={0};
	uint8_t ftemp[12]={0};


void senddata(uint8_t *s, uint32_t f, uint8_t fn, uint8_t fd)
{
	CopyString(s, stemp, sizeof(stemp));
	FigToString(f, fn, fd, ftemp);
	StringAdd(stemp, ftemp, sizeof(stemp));
	USART1_SendStringViaDMA(stemp);
}

void USART1_ReceiveInt_Exec(void)
{
	static uint8_t writeaddr=0;
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);//??USARTx???????
	USART_ClearFlag(USART1, USART_FLAG_RXNE);//??USARTx???????
	if(writeaddr==0)
	{
		switch(USART_ReceiveData(USART1))
		{
//			case 0: senddata("\n ADC Voltage IN8:", ADC_Voltage[ADC_IN8], 4, 1); break;
//			case 1: senddata("\n ADC Voltage IN9:", ADC_Voltage[ADC_IN9], 4, 1); break;
			case 0: senddata("\n ADC Voltage MOTOR1:", ADC_Voltage[ADC_MOTOR1], 4, 1); break;
			case 1: senddata("\n ADC Voltage MOTOR2:", ADC_Voltage[ADC_MOTOR2], 4, 1); break;
			case 2: senddata("\n ADC Voltage MOTOR3:", ADC_Voltage[ADC_MOTOR3], 4, 1); break;
			case 3: senddata("\n ADC Voltage MOTOR4:", ADC_Voltage[ADC_MOTOR4], 4, 1); break;
//			case 6: senddata("\n ADC Voltage IN14:", ADC_Voltage[ADC_AIN14], 4, 1); break;
//			case 7: senddata("\n ADC Voltage IN15:", ADC_Voltage[ADC_AIN15], 4, 1); break;
//			case 8: senddata("\n ADC Voltage TEMPERATURE:", ADC_Voltage[ADC_TEMPERATURE], 4, 1); break;
//			case 9: senddata("\n ADC Voltage VCC:", ADC_Voltage[ADC_VCC], 4, 1); break;
			case 4: USART1_SendStringViaDMA("\n Enter Motor 1 speed:"); writeaddr=1; break;
			case 5: USART1_SendStringViaDMA("\n Enter Motor 2 speed:"); writeaddr=2; break;
//			case 6: USART1_SendStringViaDMA("\n Enter Motor 3 speed:"); writeaddr=3; break;
//			case 7: USART1_SendStringViaDMA("\n Enter Motor 4 speed:"); writeaddr=4; break;
			case 6: 
				if (MotorSpeed[0] >= 0)
					senddata("\n Motor 1 speed:", MotorSpeed[0], 3, 3);
				else
					senddata("\n Motor 1 speed:-", -MotorSpeed[0], 3, 3);
				break;
			case 7: 
				if (MotorSpeed[1] >= 0)
					senddata("\n Motor 2 speed:", MotorSpeed[1], 3, 3); 
				else
					senddata("\n Motor 2 speed:-", -MotorSpeed[1], 3, 3); 
			break;
//			case 10: senddata("\n Motor 3 speed:", MotorSpeed[2], 3, 3); break;
//			case 11: senddata("\n Motor 4 speed:", MotorSpeed[3], 3, 3); break;
			case 8: senddata("\n FullVoltage[0]:", FullVoltage[0], 4, 1); break;
			case 9: senddata("\n FullVoltage[1]:", FullVoltage[1], 4, 1); break;
			case 10:
				if (MotorVoltage[0] >= 0)
					senddata("\n MotorVoltage[0]:", MotorVoltage[0], 3, 3);
				else
					senddata("\n MotorVoltage[0]:-", -MotorVoltage[0], 3, 3);
			break;
			case 11:
				if (MotorVoltage[1] >= 0)
					senddata("\n MotorVoltage[1]:", MotorVoltage[1], 3, 3);
				else
					senddata("\n MotorVoltage[1]:-", -MotorVoltage[1], 3, 3);
			break;
			case 12: 
				if (dV[0] >= 0)
					senddata("\n dV[0]:", dV[0], 4, 1);
				else
					senddata("\n dV[0]:-", -dV[0], 4, 1); break;
			case 13: 
				if (dV[1] >= 0)
					senddata("\n dV[1]:", dV[1], 4, 1);
				else
					senddata("\n dV[1]:-", -dV[1], 4, 1); break;
			case 14: 
				if (Gyro >= 0)
					senddata("\n Gyro:", Gyro * 10, 3, 1);
				else
					senddata("\n Gyro:-", -Gyro * 10, 3, 1);
				break;
			default: USART1_SendStringViaDMA("invalid command");break;
		}
	}
	else
	{
		switch(writeaddr)
		{
			case 1: 
				MotorSpeedSet[0]=USART_ReceiveData(USART1); 
				USART1_SendStringViaDMA("\n Motor 1 speed setting complete");
				break;
			case 2: 
				MotorSpeedSet[1]=USART_ReceiveData(USART1); 
				USART1_SendStringViaDMA("\n Motor 2 speed setting complete");
				break;
/*			case 3: 
				MotorSpeedSet[2]=USART_ReceiveData(USART1); 
				USART1_SendStringViaDMA("\n Motor 3 speed setting complete");
				break;
			case 4: 
				MotorSpeedSet[3]=USART_ReceiveData(USART1); 
				USART1_SendStringViaDMA("\n Motor 4 speed setting complete");
				break;*/
			default: break;
		}
		writeaddr=0;
	
	}
}




void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE))  //receive interrupt
		USART1_ReceiveInt_Exec();
}
