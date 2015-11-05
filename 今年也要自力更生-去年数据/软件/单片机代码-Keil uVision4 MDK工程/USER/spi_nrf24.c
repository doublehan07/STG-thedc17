#include "spi_nrf24.h"
#include "includes.h"


 uint8_t  TxAddr[]={0x34,0x43,0x10,0x10,0x01};//���͵�ַ

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
 



/*****************SPI��д����*******************************************/
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



/*****************SPI���Ĵ���һ�ֽں���*********************************/
uint8_t NRF24_ReadReg(uint8_t RegAddr)
{
	uint8_t BackData;
	SPI3_Select();
	NRF24_SPIReadWrite(RegAddr);//д�Ĵ�����ַ
	BackData=NRF24_SPIReadWrite(0x00);//д����Ĵ���ָ��  
	SPI3_Deselect();


   return(BackData); //����״̬
}
/*****************SPIд�Ĵ���һ�ֽں���*********************************/
uint8_t NRF24_WriteReg(uint8_t RegAddr,uint8_t data)
{
	uint8_t BackData;
	SPI3_Select();
	BackData=NRF24_SPIReadWrite(RegAddr);//д���ַ
	NRF24_SPIReadWrite(data);//д��ֵ
	SPI3_Deselect();

	return(BackData);
}
/*****************SPI��ȡRXFIFO�Ĵ�����ֵ********************************/
uint8_t NRF24_ReadRxData(uint8_t RegAddr,uint8_t *RxData,uint8_t DataLen)
{  //�Ĵ�����ַ//��ȡ���ݴ�ű���//��ȡ���ݳ���//���ڽ���
  uint8_t BackData,i;
	SPI3_Select();
  BackData=NRF24_SPIReadWrite(RegAddr);//д��Ҫд��Ĵ����ĵ�ַ
	for(i=0;i<DataLen;i++) //��ȡ����
	{
		 RxData[i]=NRF24_SPIReadWrite(0);
	} 
	SPI3_Deselect();
  return(BackData); 
}
/*****************SPIд��TXFIFO�Ĵ�����ֵ**********************************/
uint8_t NRF24_WriteTxData(uint8_t RegAddr,uint8_t *TxData,uint8_t DataLen)
{ //�Ĵ�����ַ//д�����ݴ�ű���//��ȡ���ݳ���//���ڷ���
	uint8_t BackData,i;
	SPI3_Select();
	BackData=NRF24_SPIReadWrite(RegAddr);//д��Ҫд��Ĵ����ĵ�ַ
	for(i=0;i<DataLen;i++)//д������
	{
		NRF24_SPIReadWrite(*(TxData++));
	} 
	SPI3_Deselect();

	return(BackData);
}
/*****************NRF����Ϊ����ģʽ����������******************************/
void NRF24_SetTxMode(uint8_t *TxDate)
{  //����ģʽ 
	NRF24_Disable();
	SPI3_Select();
	NRF24_WriteTxData(W_REGISTER+TX_ADDR,TxAddr,TX_ADDR_WITDH);//д�Ĵ���ָ��+P0��ַʹ��ָ��+���͵�ַ+��ַ���
	NRF24_WriteTxData(W_REGISTER+RX_ADDR_P0,TxAddr,TX_ADDR_WITDH);//Ϊ��Ӧ������豸������ͨ��0��ַ�ͷ��͵�ַ��ͬ
	NRF24_WriteTxData(W_TX_PAYLOAD,TxDate,TX_DATA_WITDH);//д������ 
	/******�����йؼĴ�������**************/
	NRF24_WriteReg(W_REGISTER+EN_AA,0x01);       // ʹ�ܽ���ͨ��0�Զ�Ӧ��
	NRF24_WriteReg(W_REGISTER+EN_RXADDR,0x01);   // ʹ�ܽ���ͨ��0
	NRF24_WriteReg(W_REGISTER+SETUP_RETR,0x0a);  // �Զ��ط���ʱ�ȴ�250us+86us���Զ��ط�10��
	NRF24_WriteReg(W_REGISTER+RF_CH,0x40);         // ѡ����Ƶͨ��0x40
	NRF24_WriteReg(W_REGISTER+RF_SETUP,0x07);    // ���ݴ�����1Mbps�����书��0dBm���������Ŵ�������
	NRF24_WriteReg(W_REGISTER+CONFIG,0x0e);      // CRCʹ�ܣ�16λCRCУ�飬�ϵ�	
	NRF24_Enable();
	Delay(50);//����10us������
} 
/*****************NRF����Ϊ����ģʽ����������******************************/

void NRF24_FlushRXBuffer(void)
{
	SPI3_Select();
	NRF24_SPIReadWrite(FLUSH_RX);//�������FIFO �����ؼ�������Ȼ��������벻���ĺ����������Ҽ�ס���� 
	SPI3_Deselect();
}



//����ģʽ
void NRF24_SetRXMode(void)
{
	NRF24_Disable();
	
	NRF24_FlushRXBuffer();
	NRF24_WriteTxData(W_REGISTER+RX_ADDR_P0,TxAddr,TX_ADDR_WITDH);  // �����豸����ͨ��0ʹ�úͷ����豸��ͬ�ķ��͵�ַ
	NRF24_WriteReg(W_REGISTER+EN_AA,0x01);               // ʹ�ܽ���ͨ��0�Զ�Ӧ��
	NRF24_WriteReg(W_REGISTER+EN_RXADDR,0x01);           // ʹ�ܽ���ͨ��0
	NRF24_WriteReg(W_REGISTER+RF_CH,0x40);                 // ѡ����Ƶͨ��0x40
	NRF24_WriteReg(W_REGISTER+RX_PW_P0,TX_DATA_WITDH);  // ����ͨ��0ѡ��ͷ���ͨ����ͬ��Ч���ݿ��
	NRF24_WriteReg(W_REGISTER+RF_SETUP,0x07);            // ���ݴ�����1Mbps�����书��0dBm���������Ŵ�������
	NRF24_WriteReg(W_REGISTER+CONFIG,0x0f);  	// CRCʹ�ܣ�16λCRCУ�飬�ϵ磬����ģʽ     
	
	NRF24_Enable();
	SPI3_Deselect();

	Delay(50);    
}
/****************************����Ƿ��н��յ�����******************************/
void NRF24_CheckACK(void)
{  //���ڷ���ģʽ����Ӧ���ź�
	                    // ����״̬�Ĵ���
	if(NRF24_ReadReg(R_REGISTER+STATUS)&0x20)
	   NRF24_WriteReg(W_REGISTER+STATUS,0xff);  // ���TX_DS��MAX_RT�жϱ�־
}
/*************************��������*********************************************/


uint8_t NRF24_GetData(uint8_t *rxdata)				 
{
//	uint8_t temp;
    //�������ݺ��ȡ״̬�Ĵ���
	if((NRF24_ReadReg(R_REGISTER+STATUS))&0x40)				// �ж��Ƿ���յ�����
	{
	  NRF24_Disable();
		//����
		NRF24_ReadRxData(R_RX_PAYLOAD,rxdata,RX_DATA_WITDH);// ��RXFIFO��ȡ���� ����4λ���ɣ���һλλ����λ
		NRF24_FlushRXBuffer();
				 
		NRF24_WriteReg(W_REGISTER+STATUS,0xff); //���յ����ݺ�RX_DR,TX_DS,MAX_PT���ø�Ϊ1��ͨ��д1������жϱ�
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


	NRF24_WriteReg(W_REGISTER+STATUS,0x7f); //���յ����ݺ�RX_DR,TX_DS,MAX_PT���ø�Ϊ1��ͨ��д1������жϱ�
	SPI3_Select();
	NRF24_SPIReadWrite(FLUSH_RX);//�������FIFO �����ؼ�������Ȼ��������벻���ĺ����������Ҽ�ס���� 
	SPI3_Deselect();
}





