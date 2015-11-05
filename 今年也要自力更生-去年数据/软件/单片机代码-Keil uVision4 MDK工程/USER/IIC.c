#include "IIC.h"

#define IIC_SDA_PORT GPIOB
#define IIC_SDA_PIN  GPIO_Pin_11
#define IIC_SCL_PORT GPIOB
#define IIC_SCL_PIN  GPIO_Pin_10

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
	
	
	
	//I2C2 Initialize
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;//I2C??
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;//????Tlow / Thigh = 2
	I2C_InitStructure.I2C_OwnAddress1 = 0x0000;//??????
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;//????
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;//??7???
	I2C_InitStructure.I2C_ClockSpeed = 100000;//??????,???????400KHz
	I2C_Init(I2C2,&I2C_InitStructure);//?????

	I2C_Cmd(I2C2,ENABLE);//??????I2C??


}

volatile uint16_t Temperature=0;
void IIC_ReadLM75ATemp(void)
{
  uint16_t temp;
	while(I2C_GetFlagStatus(I2C2,I2C_FLAG_BUSY));

	I2C_GenerateSTART(I2C2,ENABLE);//??I2Cx??START??
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)==0);//??????I2C??

	I2C_Send7bitAddress(I2C2,0x90,I2C_Direction_Receiver);//?????I2C???????
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)==0);//??????I2C??

	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED)==0);//??????I2C??
	temp=I2C_ReceiveData(I2C2);
	temp=temp<<8;
	
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED)==0);//??????I2C??
	temp+=I2C_ReceiveData(I2C2);	
	
	I2C_GenerateSTOP(I2C2,ENABLE);//??I2Cx??STOP??

	temp=(temp>>5)*125;
	
	Temperature=temp;
}


