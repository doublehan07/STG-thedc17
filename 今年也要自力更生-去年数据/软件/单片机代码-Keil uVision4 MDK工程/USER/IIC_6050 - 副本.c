#include "IIC_6050.h"
#include "MPU6050.h"

#include "debug_exec.h"
#include "motor_ctr.h"
#include "sysconfig.h"
#include "public.h"
#include "MovementCtr.h"

#define IIC_SDA_PORT GPIOB
#define IIC_SDA_PIN  GPIO_Pin_11
#define IIC_SCL_PORT GPIOB
#define IIC_SCL_PIN  GPIO_Pin_10
#define VCC_PORT     GPIOA
#define VCC_PIN      GPIO_Pin_8
#define VCC_ON       GPIO_SetBits(VCC_PORT, VCC_PIN)
#define VCC_OFF      GPIO_ResetBits(VCC_PORT, VCC_PIN)

volatile float Gyro = 0;

uint32_t LastTick = 0;

uint8_t IIC_OK = 1;

uint16_t num[3] = {0};

void ResetMPU(void)
{
	VCC_OFF;
	I2C_Cmd(I2C2,DISABLE);
	Delay_mS(1000);
	I2C_Cmd(I2C2,ENABLE);
	VCC_ON;
	Delay_mS(100);
}

void IIC_Init(void)
{
	I2C_InitTypeDef I2C_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//clock initialize
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);//??????APB1????
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//??????APB2????
	
	//GPIO Initialize
	GPIO_InitStructure.GPIO_Pin = IIC_SCL_PIN | IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//??????10MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;//??????
	GPIO_Init(IIC_SCL_PORT, &GPIO_InitStructure);//?????GPIOx???
	
	//VCC initialize
	GPIO_InitStructure.GPIO_Pin = VCC_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//??????10MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//????
	GPIO_Init(VCC_PORT, &GPIO_InitStructure);//?????GPIOx???

	
	//I2C2 Initialize
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;//I2C??
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;//????Tlow / Thigh = 2
	I2C_InitStructure.I2C_OwnAddress1 = 0x0000;//??????
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;//????
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;//??7???
	I2C_InitStructure.I2C_ClockSpeed = 60000;//??????,???????400KHz
	I2C_Init(I2C2,&I2C_InitStructure);//?????

	I2C_Cmd(I2C2,ENABLE);//??????I2C??

	I2C_AcknowledgeConfig(I2C2, ENABLE);
	
	USARTSendString("\n IIC initial success!");
}


void W_6050(uint8_t REG_add,uint8_t REG_data)
{
//	USARTSendString("1");
	if (IIC_OK)
	{
		uint32_t time = TimeTicket + 4;
		while(I2C_GetFlagStatus(I2C2,I2C_FLAG_BUSY) && time >= TimeTicket);
		if (time < TimeTicket)
		{
			IIC_OK = 0;
			USARTSendString("aaa");
			I2C_GenerateSTOP(I2C2,ENABLE);
			return;
		}
		I2C_GenerateSTART(I2C2,ENABLE);

	//	USARTSendString("2");

		while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)==0 && time >= TimeTicket);
		if (time < TimeTicket)
		{
			IIC_OK = 0;
			USARTSendString("bbb");
			I2C_GenerateSTOP(I2C2,ENABLE);
			return;
		}
		I2C_Send7bitAddress(I2C2,0xD0,I2C_Direction_Transmitter);
		
		while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==0 && time >= TimeTicket);
		if (time < TimeTicket)
		{
			IIC_OK = 0;
			USARTSendString("ccc");
			I2C_GenerateSTOP(I2C2,ENABLE);
			return;
		}
		
		I2C_SendData(I2C2,REG_add);
		while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==0 && time >= TimeTicket);
		if (time < TimeTicket)
		{
			IIC_OK = 0;
			USARTSendString("ddd");
			I2C_GenerateSTOP(I2C2,ENABLE);
			return;
		}
		
		I2C_SendData(I2C2,REG_data);
		while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==0 && time >= TimeTicket);
		if (time < TimeTicket)
		{
			IIC_OK = 0;
			USARTSendString("eee");
			I2C_GenerateSTOP(I2C2,ENABLE);
			return;
		}		
		
		I2C_GenerateSTOP(I2C2,ENABLE);
	}
}

uint8_t R_6050(uint8_t REG_add)
{
  uint8_t temp;
	while(I2C_GetFlagStatus(I2C2,I2C_FLAG_BUSY));
	I2C_GenerateSTART(I2C2,ENABLE);
	
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)==0);
	I2C_Send7bitAddress(I2C2,0xD0,I2C_Direction_Transmitter);
	
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==0);

  I2C_SendData(I2C2,REG_add);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==0);

  I2C_GenerateSTART(I2C2,ENABLE);
	
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)==0);

	I2C_Send7bitAddress(I2C2,0xD0,I2C_Direction_Receiver);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)==0);

	I2C_AcknowledgeConfig(I2C2, DISABLE);
	I2C_GenerateSTOP(I2C2,ENABLE);
	
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED)==0);
	temp=I2C_ReceiveData(I2C2);
	
	
  return temp;
}


uint8_t IIC_Success = 0;

/*int16_t R_6050_2(uint8_t REG_add)
{
  int16_t temp = 0;
	uint32_t time = TimeTicket + 3;
	IIC_Success = 0;
	if(I2C_GetFlagStatus(I2C2,I2C_FLAG_BUSY));
	
	if (time < TimeTicket)
		return 0;
	
	I2C_GenerateSTART(I2C2,ENABLE);
	
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)==0 && time >= TimeTicket);
	
	if (time < TimeTicket)
		return 1;
	
	I2C_Send7bitAddress(I2C2,0xD0,I2C_Direction_Transmitter);
	
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==0 && time >= TimeTicket);

	if (time < TimeTicket)
		return 2;

  I2C_SendData(I2C2,REG_add);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==0 && time >= TimeTicket);

	if (time < TimeTicket)
		return 3;

  I2C_GenerateSTART(I2C2,ENABLE);
	
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)==0 && time >= TimeTicket);

	if (time < TimeTicket)
		return 4;

	I2C_Send7bitAddress(I2C2,0xD0,I2C_Direction_Receiver);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)==0 && time >= TimeTicket);

	if (time < TimeTicket)
		return 5;

	I2C_AcknowledgeConfig(I2C2, ENABLE);
	
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED)==0 && time >= TimeTicket);
	
	if (time < TimeTicket)
		return 6;

	temp += I2C_ReceiveData(I2C2)<<8;
	
	I2C_AcknowledgeConfig(I2C2, DISABLE);
	I2C_GenerateSTOP(I2C2,ENABLE);
	
//	temp <<= 8;
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED)==0 && time >= TimeTicket);

	if (time < TimeTicket)
		return 7;

	temp +=I2C_ReceiveData(I2C2);	
	

	IIC_Success = 1;
  return temp;
}*/

int16_t R_6050_2(uint8_t REG_add)
{
  int16_t temp = 0;
	uint32_t time = TimeTicket + 1;
	IIC_Success = 0;
	while(I2C_GetFlagStatus(I2C2,I2C_FLAG_BUSY) && time >= TimeTicket);
	
	if (time < TimeTicket)
		return 0;
	
	I2C_GenerateSTART(I2C2,ENABLE);
	
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)==0 && time >= TimeTicket);
	
	if (time < TimeTicket)
		return 1;
	
	I2C_Send7bitAddress(I2C2,0xD0,I2C_Direction_Transmitter);
	
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==0 && time >= TimeTicket);

	if (time < TimeTicket)
		return 2;

  I2C_SendData(I2C2,REG_add);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==0 && time >= TimeTicket);

	if (time < TimeTicket)
		return 3;

  I2C_GenerateSTART(I2C2,ENABLE);
	
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)==0 && time >= TimeTicket);

	if (time < TimeTicket)
		return 4;

	I2C_Send7bitAddress(I2C2,0xD0,I2C_Direction_Receiver);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)==0 && time >= TimeTicket);

	if (time < TimeTicket)
		return 5;

	I2C_AcknowledgeConfig(I2C2, DISABLE);
	I2C_GenerateSTOP(I2C2,ENABLE);
	
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED)==0 && time >= TimeTicket);
	
	if (time < TimeTicket)
		return 6;

	temp += I2C_ReceiveData(I2C2)<<8;
	
	while(I2C_GetFlagStatus(I2C2,I2C_FLAG_BUSY) && time >= TimeTicket);
	
	if (time < TimeTicket)
		return 7;
	
	I2C_GenerateSTART(I2C2,ENABLE);
	
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)==0 && time >= TimeTicket);
	
	if (time < TimeTicket)
		return 8;
	
	I2C_Send7bitAddress(I2C2,0xD0,I2C_Direction_Transmitter);
	
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==0 && time >= TimeTicket);

	if (time < TimeTicket)
		return 9;

  I2C_SendData(I2C2,REG_add+1);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==0 && time >= TimeTicket);

	if (time < TimeTicket)
		return 10;

  I2C_GenerateSTART(I2C2,ENABLE);
	
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)==0 && time >= TimeTicket);

	if (time < TimeTicket)
		return 11;

	I2C_Send7bitAddress(I2C2,0xD0,I2C_Direction_Receiver);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)==0 && time >= TimeTicket);

	if (time < TimeTicket)
		return 12;

	I2C_AcknowledgeConfig(I2C2, DISABLE);
	I2C_GenerateSTOP(I2C2,ENABLE);
	
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED)==0 && time >= TimeTicket);
	
	if (time < TimeTicket)
		return 13;

	temp +=I2C_ReceiveData(I2C2);	
	

	IIC_Success = 1;
  return temp;
}

void InitMPU6050(void)
{
	uint8_t i = 0;
	uint8_t type = 0;
	USARTSendString("\n MPU initializing...");
	do
	{
		I2C_AcknowledgeConfig(I2C2, ENABLE);
		IIC_OK = 1;
		type = Type;
		SetType(NT);
		ResetMPU();
		W_6050(PWR_MGMT_1, 0x80);	//reset
		Delay_mS(100);
		W_6050(PWR_MGMT_1, 0x00);	
		W_6050(SMPLRT_DIV, 0x03);//1k/(3+1) = 250Hz
		W_6050(CONFIG, 0x03);//6,5Hz,18.6ms;3,44Hz,4.9ms
		W_6050(GYRO_CONFIG, 0x18);//2000degree/s
		W_6050(ACCEL_CONFIG, 0x01);
		if (IIC_OK)
		{
			USARTSendString("\n MPU initial success!");
			LastTick = TimeTicket;
//			num[0] = num[1] = num[2] = 0;
		}
		else
		{
			USARTSendString("\n MPU initial failed!Retrying...");
		}
	}while(!IIC_OK && ++i < 5);
	if (!IIC_OK)
		USARTSendString("failed!");
	else
		SetType(type);
}

void IIC_Exec(void)
{
	if (IIC_OK)
	{
		int16_t Data = R_6050_2(GYRO_ZOUT_H);
		num[2]++;
		if (IIC_Success)
		{
//			USARTSendString("abc");
			LastTick = TimeTicket;
			Gyro = (float)(Data - 2) / 16.384;
			num[0]++;
//			senddata("\n G:", Data, 3, 3);
/*			if (Gyro > 0.1)
				senddata("\n G:", (int32_t)(Gyro * 10), 4, 3);
			else if (Gyro < -0.1)
				senddata("\n G:-", (int32_t)(-Gyro * 10), 4, 3);*/
		}
		else
		{
			num[1]++;
			senddata("W:", Data, 2, 3);
			if (LastTick < TimeTicket - 80)
//			if (num[1] > 25)
			{
				Gyro = 0;
				IIC_OK = 0;
				USARTSendString("Rst");
			}
		}
		if (num[2] == 1000)
		{
//			senddata("OK:", num[0], 3, 3);
//			senddata("WR:", num[1], 4, 4);
			num[0] = num[1] = num[2] = 0;
		}
	}
/*		else
		{
			USARTSendString("\n WrongData");
		}*/
}
