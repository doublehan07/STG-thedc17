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

#define M6050

volatile float Gyro = 0;

int16_t Adjust = 0;

uint32_t LastTick = 0;

uint8_t IIC_OK = 1;

uint16_t num[3] = {0};

void ResetMPU(void)
{
	VCC_OFF;
//	I2C_Cmd(I2C2,DISABLE);
	Delay_mS(1000);
//	I2C_Cmd(I2C2,ENABLE);
	VCC_ON;
	Delay_mS(100);
}


void IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//??????APB2????
	//GPIO Initialize
	GPIO_InitStructure.GPIO_Pin = IIC_SCL_PIN | IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//??????10MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;//??????
	GPIO_Init(IIC_SCL_PORT, &GPIO_InitStructure);//?????GPIOx???
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//??????APB2????
	//VCC initialize
	GPIO_InitStructure.GPIO_Pin = VCC_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//??????10MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//????
	GPIO_Init(VCC_PORT, &GPIO_InitStructure);//?????GPIOx???
	
	USARTSendString("\n IIC initial success!");
}

void delay5us(void)//5us
{
	uint8_t i, j;
	uint8_t time;
	time=5;
	for(; time > 0; time--){
		for(j = 0; j < 5; j++){
			for(i = 0; i < 10; i++);
		}
	}
}

void iicstart(void)
{
	GPIO_SetBits(IIC_SDA_PORT,IIC_SDA_PIN); //sda=1;
	delay5us();
	GPIO_SetBits(IIC_SCL_PORT,IIC_SCL_PIN); //scl=1;
	delay5us();
	GPIO_ResetBits(IIC_SDA_PORT,IIC_SDA_PIN); //sda=0;
	delay5us();
}

void iicstop(void)
{
	GPIO_ResetBits(IIC_SDA_PORT,IIC_SDA_PIN); //sda=0;
	delay5us();
	GPIO_SetBits(IIC_SCL_PORT,IIC_SCL_PIN); //scl=1;
	delay5us();
	GPIO_SetBits(IIC_SDA_PORT,IIC_SDA_PIN); //sda=1;
	delay5us();
}


uint8_t IIC_Success = 1;
void nack(void)
{
	uint16_t i = 0;
	GPIO_SetBits(IIC_SCL_PORT,IIC_SCL_PIN);//scl=1;
	delay5us();
	while(GPIO_ReadInputDataBit(IIC_SDA_PORT,IIC_SDA_PIN)&&(i<0x2b0)) {i++;}
	if (i==0x2b0)
		IIC_Success = 0;
	GPIO_ResetBits(IIC_SCL_PORT,IIC_SCL_PIN);//scl=0;
	delay5us();
}

void ack(void)
{
	GPIO_ResetBits(IIC_SDA_PORT,IIC_SDA_PIN);//scl=1;
	GPIO_SetBits(IIC_SCL_PORT,IIC_SCL_PIN);//scl=1;
	delay5us();
	GPIO_ResetBits(IIC_SCL_PORT,IIC_SCL_PIN);//scl=0;
	GPIO_SetBits(IIC_SDA_PORT,IIC_SDA_PIN);//scl=0;
	delay5us();
}

void iicwritebyte(uint8_t a)
{
	uint8_t i;
	GPIO_ResetBits(IIC_SCL_PORT,IIC_SCL_PIN); //scl=0;
	delay5us();
	for(i=0;i<8;i++)
	{
		if(a&0x80)
			GPIO_SetBits(IIC_SDA_PORT,IIC_SDA_PIN);//sda=1;
		else
			GPIO_ResetBits(IIC_SDA_PORT,IIC_SDA_PIN);
		a<<=1;
		GPIO_SetBits(IIC_SCL_PORT,IIC_SCL_PIN);//scl=1;
		delay5us();
		GPIO_ResetBits(IIC_SCL_PORT,IIC_SCL_PIN);//scl=0;
		delay5us();
	}
	GPIO_SetBits(IIC_SDA_PORT,IIC_SDA_PIN);//sda=1;
	delay5us();
}

uint8_t iicreadbyte()
{
	uint8_t i,temp;
	temp=0;
	GPIO_SetBits(IIC_SDA_PORT,IIC_SDA_PIN);//sda=1;
	delay5us();
	GPIO_ResetBits(IIC_SCL_PORT,IIC_SCL_PIN); //scl=0;
	delay5us();
	for(i=0;i<8;i++)
	{
		GPIO_SetBits(IIC_SCL_PORT,IIC_SCL_PIN); //scl=1;
		delay5us();
		delay5us();
		temp=(temp<<1)|GPIO_ReadInputDataBit(IIC_SDA_PORT,IIC_SDA_PIN);
		delay5us();
		GPIO_ResetBits(IIC_SCL_PORT,IIC_SCL_PIN); //scl=0;
		delay5us();
	}
	GPIO_SetBits(IIC_SDA_PORT,IIC_SDA_PIN);//sda=1;
	delay5us();
	GPIO_ResetBits(IIC_SCL_PORT,IIC_SCL_PIN);//scl=0;
	delay5us();
	return temp;
}



void W_6050(uint8_t REG_add,uint8_t REG_data)
{
	if (IIC_OK)
	{
		iicstart();

    iicwritebyte(0xD0);
    nack();
		
    iicwritebyte(REG_add);
    nack();
    iicwritebyte(REG_data);
    nack();
		
    iicstop();
		delay5us();
		delay5us();
	}
}

uint8_t R_6050(uint8_t REG_add)
{
  uint8_t temp;
	iicstart();
	
	iicwritebyte(0xD0);
	nack();
	iicwritebyte(REG_add);
	nack();

  iicstart();
	
	iicwritebyte(0xD1);
	nack();
	temp=iicreadbyte();
	if (IIC_Success == 1)
	{
		nack();
		IIC_Success = 1;
	}
	iicstop();
	
  return temp;
}



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
	IIC_Success = 1;
	iicstart();
	
	iicwritebyte(0xD0);
	nack();
	iicwritebyte(REG_add);
	nack();

  iicstart();
	
	iicwritebyte(0xD1);
	nack();
	temp+=iicreadbyte() << 8;
	ack();
	temp+=iicreadbyte();
	if (IIC_Success == 1)
	{
		nack();
		IIC_Success = 1;
	}
	iicstop();
	
  return temp;
}

void InitMPU6050(void)
{
	uint8_t i = 0;
	USARTSendString("\n MPU initializing...");
	do
	{
		SetSource(Gyro_Stage);
		ResetMPU();
#ifdef M6050
		W_6050(PWR_MGMT_1, 0x80);	//reset
#else
		W_6050(PWR_M, 0x80);   //
#endif
		Delay_mS(100);
#ifdef M6050
		W_6050(PWR_MGMT_1, 0x00);	
		W_6050(SMPLRT_DIV, 0x03);//1k/(3+1) = 250Hz
		W_6050(CONFIG, 0x03);//6,5Hz,18.6ms;3,44Hz,4.9ms
		W_6050(GYRO_CONFIG, 0x18);//2000degree/s
		W_6050(ACCEL_CONFIG, 0x01);
		senddata("OK:", R_6050(WHO_AM_I), 3, 4);
		if (R_6050(WHO_AM_I) == 0x68)
		{
			IIC_OK = 1;
			USARTSendString("\n MPU initial success!");
			LastTick = TimeTicket;
//			num[0] = num[1] = num[2] = 0;
		}
		else
		{
			IIC_OK = 0;
			USARTSendString("\n MPU initial failed!Retrying...");
		}
#else
		W_6050(SMPL, 0x07);    //
		W_6050(DLPF, 0x1E);    //±2000°
		W_6050(INT_C, 0x00 );  //
		W_6050(PWR_M, 0x00);   //
		IIC_OK = 1;
#endif
	}while(!IIC_OK && ++i < 5);
	if (!IIC_OK)
		USARTSendString("failed!");
	else
	{
		SetSource(Gyro_MPU);
		MakeAdjustMent();
	}
}

void MakeAdjustMent(void)
{
	int16_t Last = 0;
	if (IIC_OK)
	{
		uint8_t i, j = 0;
		for (i = 0; i < 100; i++)
		{
#ifdef M6050
			int16_t Data = R_6050_2(GYRO_ZOUT_H);
#else
			int16_t Data = R_6050_2(GZ_H);
#endif
			if (IIC_Success)
			{
				if (Data - Last <= 1 && Data - Last >= -1)
				{
					j++;
					Adjust += Data;
				}
				Last = Data;
			}
		}
		Adjust /= j;
	}
}

void ReInitMPU6050(void)
{
	static uint8_t step = 0;
	static uint32_t time = 0;
	USARTSendString("\n MPU Reinitializing...");
	if (step == 0)
	{
		SetSource(Gyro_Stage);
		VCC_OFF;
		step++;
		time = TimeTicket+1000;
	}
	else if (step == 1 && time <= TimeTicket)
	{
		VCC_ON;
		step++;
		time = TimeTicket+100;
	}
	else if (step == 2 && time <= TimeTicket)
	{
#ifdef M6050
		W_6050(PWR_MGMT_1, 0x80);	//reset
#else
		W_6050(PWR_M, 0x80);   //
#endif
		step++;
		time = TimeTicket+100;
	}
	else if (step == 3 && time <= TimeTicket)
	{
#ifdef M6050
		W_6050(PWR_MGMT_1, 0x00);	
		W_6050(SMPLRT_DIV, 0x03);//1k/(3+1) = 250Hz
		W_6050(CONFIG, 0x03);//6,5Hz,18.6ms;3,44Hz,4.9ms
		W_6050(GYRO_CONFIG, 0x18);//2000degree/s
		W_6050(ACCEL_CONFIG, 0x01);
		if (R_6050(WHO_AM_I) == 0x68)
		{
			IIC_OK = 1;
			SetSource(Gyro_MPU);
			LastTick = TimeTicket;
//			num[0] = num[1] = num[2] = 0;
		}
		else
		{
			IIC_OK = 0;
		}
#else
		W_6050(SMPL, 0x07);    //
		W_6050(DLPF, 0x1E);    //±2000°
		W_6050(INT_C, 0x00 );  //
		W_6050(PWR_M, 0x00);   //
		IIC_OK = 1;
#endif
		step = 0;
	}
}

void IIC_Exec(void)
{
	if (IIC_OK)
	{
#ifdef M6050
		int16_t Data = R_6050_2(GYRO_ZOUT_H);
#else
		int16_t Data = R_6050_2(GZ_H);
#endif
		num[2]++;
		if (IIC_Success)
		{
//			USARTSendString("abc");
			LastTick = TimeTicket;
#ifdef M6050
			Gyro = (float)(Data - Adjust) / 16.384;
#else
			Gyro = (float)(Data - Adjust) / 14.375;
#endif
			num[0]++;
/*			senddata("\n G:", Data, 3, 3);
			if (Gyro > 0.1)
				senddata("\n G:", (int32_t)(Gyro * 10), 4, 3);
			else if (Gyro < -0.1)
				senddata("\n G:-", (int32_t)(-Gyro * 10), 4, 3);*/
		}
		else
		{
			num[1]++;
			if (Data > 0)
				senddata("W:", Data, 5, 6);
			else
				senddata("W:-", -Data, 5, 6);
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
