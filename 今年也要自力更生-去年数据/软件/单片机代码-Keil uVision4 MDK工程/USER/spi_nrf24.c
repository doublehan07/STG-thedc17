#include "spi_nrf24.h"
#include "includes.h"


 uint8_t  TxAddr[]={0x34,0x43,0x10,0x10,0x01};//发送地址

uint8_t NRFDelay;
uint32_t datacount=0;
 
uint8_t TxAddr[TX_ADDR_WITDH];
uint8_t RXData[RX_DATA_WITDH];

 
#define SPI3_PORT      GPIOB
#define SPI3_SCK_PIN   GPIO_Pin_3
#define SPI3_MISO_PIN  GPIO_Pin_4
#define SPI3_MOSI_PIN  GPIO_Pin_5

#define SPI3_CSN_PORT   GPIOA
#define SPI3_CSN_PIN    GPIO_Pin_15
 
#define NRF24_PORT       GPIOB
#define NRF24_EN_PIN     GPIO_Pin_0
#define NRF24_IRQ_PIN    GPIO_Pin_2
 

void Delay(unsigned int dly)
{
    unsigned int i,j;

    for(i=0;i<dly;i++)
    	for(j=0;j<255;j++);
} 


void SPI3_Select(void)
{
	OS_CPU_SR cpu_sr;
	OS_ENTER_CRITICAL();
	GPIO_ResetBits(SPI3_CSN_PORT, SPI3_CSN_PIN);//??????????????
	OS_EXIT_CRITICAL();
}

void SPI3_Deselect(void)
{
	OS_CPU_SR cpu_sr;
	OS_ENTER_CRITICAL();
	GPIO_SetBits(SPI3_CSN_PORT, SPI3_CSN_PIN);//??????????????
	OS_EXIT_CRITICAL();
}


void NRF24_Enable(void)
{
	OS_CPU_SR cpu_sr;
	OS_ENTER_CRITICAL();
	GPIO_SetBits(NRF24_PORT, NRF24_EN_PIN);//??????????????
	OS_EXIT_CRITICAL();
}

void NRF24_Disable(void)
{
	OS_CPU_SR cpu_sr;
	OS_ENTER_CRITICAL();
	GPIO_ResetBits(NRF24_PORT, NRF24_EN_PIN);//??????????????
	OS_EXIT_CRITICAL();
}
 



/*****************SPI读写函数*******************************************/
uint8_t NRF24_SPIReadWrite(uint8_t dat)
{
	uint8_t temp;
	OS_CPU_SR cpu_sr;
  
	while(1)
	{
		OS_ENTER_CRITICAL();
		temp=SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_TXE);
		OS_EXIT_CRITICAL();
		if(temp==RESET)
			;//OSTimeDly(1);
		else 
			break;
	}//?????SPI/I2S???????
	OS_ENTER_CRITICAL();
 	SPI_I2S_SendData(SPI3,dat);
	OS_EXIT_CRITICAL();
	while(1)
	{
		OS_ENTER_CRITICAL();
		temp=SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_RXNE);
		OS_EXIT_CRITICAL();
		if(temp==RESET)
			;//OSTimeDly(1);
		else 
			break;
	}//?????SPI/I2S???????
  OS_ENTER_CRITICAL();
	temp=SPI_I2S_ReceiveData(SPI3);
	OS_EXIT_CRITICAL();
  return temp;//????SPI/I2S???????

}



/*****************SPI读寄存器一字节函数*********************************/
uint8_t NRF24_ReadReg(uint8_t RegAddr)
{
	uint8_t BackData;
	SPI3_Select();
	NRF24_SPIReadWrite(RegAddr);//写寄存器地址
	BackData=NRF24_SPIReadWrite(0x00);//写入读寄存器指令  
	SPI3_Deselect();


   return(BackData); //返回状态
}
/*****************SPI写寄存器一字节函数*********************************/
uint8_t NRF24_WriteReg(uint8_t RegAddr,uint8_t data)
{
	uint8_t BackData;
	SPI3_Select();
	BackData=NRF24_SPIReadWrite(RegAddr);//写入地址
	NRF24_SPIReadWrite(data);//写入值
	SPI3_Deselect();

	return(BackData);
}
/*****************SPI读取RXFIFO寄存器的值********************************/
uint8_t NRF24_ReadRxData(uint8_t RegAddr,uint8_t *RxData,uint8_t DataLen)
{  //寄存器地址//读取数据存放变量//读取数据长度//用于接收
  uint8_t BackData,i;
	SPI3_Select();
  BackData=NRF24_SPIReadWrite(RegAddr);//写入要写入寄存器的地址
	for(i=0;i<DataLen;i++) //读取数据
	{
		 RxData[i]=NRF24_SPIReadWrite(0);
	} 
	SPI3_Deselect();
  return(BackData); 
}
/*****************SPI写入TXFIFO寄存器的值**********************************/
uint8_t NRF24_WriteTxData(uint8_t RegAddr,uint8_t *TxData,uint8_t DataLen)
{ //寄存器地址//写入数据存放变量//读取数据长度//用于发送
	uint8_t BackData,i;
	SPI3_Select();
	BackData=NRF24_SPIReadWrite(RegAddr);//写入要写入寄存器的地址
	for(i=0;i<DataLen;i++)//写入数据
	{
		NRF24_SPIReadWrite(*(TxData++));
	} 
	SPI3_Deselect();

	return(BackData);
}
/*****************NRF设置为发送模式并发送数据******************************/
void NRF24_SetTxMode(uint8_t *TxDate)
{  //发送模式 
	NRF24_Disable();
	SPI3_Select();
	NRF24_WriteTxData(W_REGISTER+TX_ADDR,TxAddr,TX_ADDR_WITDH);//写寄存器指令+P0地址使能指令+发送地址+地址宽度
	NRF24_WriteTxData(W_REGISTER+RX_ADDR_P0,TxAddr,TX_ADDR_WITDH);//为了应答接收设备，接收通道0地址和发送地址相同
	NRF24_WriteTxData(W_TX_PAYLOAD,TxDate,TX_DATA_WITDH);//写入数据 
	/******下面有关寄存器配置**************/
	NRF24_WriteReg(W_REGISTER+EN_AA,0x01);       // 使能接收通道0自动应答
	NRF24_WriteReg(W_REGISTER+EN_RXADDR,0x01);   // 使能接收通道0
	NRF24_WriteReg(W_REGISTER+SETUP_RETR,0x0a);  // 自动重发延时等待250us+86us，自动重发10次
	NRF24_WriteReg(W_REGISTER+RF_CH,0x40);         // 选择射频通道0x40
	NRF24_WriteReg(W_REGISTER+RF_SETUP,0x07);    // 数据传输率1Mbps，发射功率0dBm，低噪声放大器增益
	NRF24_WriteReg(W_REGISTER+CONFIG,0x0e);      // CRC使能，16位CRC校验，上电	
	NRF24_Enable();
	Delay(50);//保持10us秒以上
} 
/*****************NRF设置为接收模式并接收数据******************************/

void NRF24_FlushRXBuffer(void)
{
	SPI3_Select();
	NRF24_SPIReadWrite(FLUSH_RX);//用于清空FIFO ！！关键！！不然会出现意想不到的后果！！！大家记住！！ 
	SPI3_Deselect();
}



//接收模式
void NRF24_SetRXMode(void)
{
	NRF24_Disable();
	
	NRF24_FlushRXBuffer();
	NRF24_WriteTxData(W_REGISTER+RX_ADDR_P0,TxAddr,TX_ADDR_WITDH);  // 接收设备接收通道0使用和发送设备相同的发送地址
	NRF24_WriteReg(W_REGISTER+EN_AA,0x01);               // 使能接收通道0自动应答
	NRF24_WriteReg(W_REGISTER+EN_RXADDR,0x01);           // 使能接收通道0
	NRF24_WriteReg(W_REGISTER+RF_CH,0x40);                 // 选择射频通道0x40
	NRF24_WriteReg(W_REGISTER+RX_PW_P0,TX_DATA_WITDH);  // 接收通道0选择和发送通道相同有效数据宽度
	NRF24_WriteReg(W_REGISTER+RF_SETUP,0x07);            // 数据传输率1Mbps，发射功率0dBm，低噪声放大器增益
	NRF24_WriteReg(W_REGISTER+CONFIG,0x0f);  	// CRC使能，16位CRC校验，上电，接收模式     
	
	NRF24_Enable();
	SPI3_Deselect();

	Delay(50);    
}
/****************************检测是否有接收到数据******************************/
void NRF24_CheckACK(void)
{  //用于发射模式接收应答信号
	                    // 返回状态寄存器
	if(NRF24_ReadReg(R_REGISTER+STATUS)&0x20)
	   NRF24_WriteReg(W_REGISTER+STATUS,0xff);  // 清除TX_DS或MAX_RT中断标志
}
/*************************接收数据*********************************************/


uint8_t NRF24_GetData(uint8_t *rxdata)				 
{
//	uint8_t temp;
    //发送数据后读取状态寄存器
	if((NRF24_ReadReg(R_REGISTER+STATUS))&0x40)				// 判断是否接收到数据
	{
	  NRF24_Disable();
		//待机
		NRF24_ReadRxData(R_RX_PAYLOAD,rxdata,RX_DATA_WITDH);// 从RXFIFO读取数据 接收4位即可，后一位位结束位
		NRF24_FlushRXBuffer();
				 
		NRF24_WriteReg(W_REGISTER+STATUS,0xff); //接收到数据后RX_DR,TX_DS,MAX_PT都置高为1，通过写1来清楚中断标
		return 0;
	}
	else
		return 1;
} 



void NRF24_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;	
	
	//clock initial
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);//??????APB2????
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);//??????APB2????

  //GPIO initial
	GPIO_InitStructure.GPIO_Pin =  SPI3_SCK_PIN | SPI3_MOSI_PIN ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//??????10MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//??????
	GPIO_Init(SPI3_PORT, &GPIO_InitStructure);//?????GPIOx???
	
	GPIO_InitStructure.GPIO_Pin = SPI3_MISO_PIN | NRF24_IRQ_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//????
	GPIO_Init(SPI3_PORT, &GPIO_InitStructure);//?????GPIOx???*/
  

	GPIO_InitStructure.GPIO_Pin = NRF24_EN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//????
	GPIO_Init(NRF24_PORT, &GPIO_InitStructure);//?????GPIOx???
	
	GPIO_InitStructure.GPIO_Pin = SPI3_CSN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//????
	GPIO_Init(SPI3_CSN_PORT, &GPIO_InitStructure);//?????GPIOx???
	
	
	SPI3_Deselect();
	NRF24_Disable();

  



  //SPI mode initial
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//???????
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//????SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//8????
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;//?????
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;//??????
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSS???????
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;//?????128
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//?????MSB???
	SPI_InitStructure.SPI_CRCPolynomial = 7;//??CRC???????
	SPI_Init(SPI3,&SPI_InitStructure);//?????

  //SPI start
  SPI_Cmd(SPI3,ENABLE);//??????SPI??


	NRF24_WriteReg(W_REGISTER+STATUS,0x7f); //接收到数据后RX_DR,TX_DS,MAX_PT都置高为1，通过写1来清楚中断标
	SPI3_Select();
	NRF24_SPIReadWrite(FLUSH_RX);//用于清空FIFO ！！关键！！不然会出现意想不到的后果！！！大家记住！！ 
	SPI3_Deselect();
}





