#include "spi_memory.h"
#include "public.h"
#include "comunication.h"

#define SPI3_PORT       GPIOB
#define SPI3_MOSI_PIN   GPIO_Pin_5
#define SPI3_MISO_PIN   GPIO_Pin_4
#define SPI3_SCK_PIN    GPIO_Pin_3

#define SPI3_CS_PORT		GPIOA
#define	SPI3_CS_PIN     GPIO_Pin_15


void SPI3_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	//clock initialize
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//??????APB2????
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//??????APB2????
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);//??????APB1????

	//gpio initialize
	GPIO_InitStructure.GPIO_Pin = SPI3_MISO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//??????50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//????
	GPIO_Init(SPI3_PORT, &GPIO_InitStructure);//?????GPIOx???

	GPIO_InitStructure.GPIO_Pin = SPI3_MOSI_PIN | SPI3_SCK_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//????
	GPIO_Init(SPI3_PORT, &GPIO_InitStructure);//?????GPIOx???
	
	GPIO_InitStructure.GPIO_Pin = SPI3_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//????
	GPIO_Init(SPI3_CS_PORT, &GPIO_InitStructure);//?????GPIOx???

	//SPI3_initialize
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//???????
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//????SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//8????
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//?????
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//??????
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//??NSS???SSI???
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;//?????2
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//?????MSB???
	SPI_InitStructure.SPI_CRCPolynomial = 7;//??CRC???????
	SPI_Init(SPI3,&SPI_InitStructure);//?????

	SPI_Cmd(SPI3,ENABLE);//??????SPI??

}

void SPI3_Select(void)
{
	GPIO_ResetBits(SPI3_CS_PORT,SPI3_CS_PIN);
}

void SPI3_Deselect(void)
{
	GPIO_SetBits(SPI3_CS_PORT,SPI3_CS_PIN);
}


uint8_t SPI3_ReadWriteByte(uint8_t data)
{
	while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_TXE)==0);
	SPI_I2S_SendData(SPI3,data);//????SPI/I2S??????
	while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_RXNE)==0);//?????SPI/I2S???????
	return SPI_I2S_ReceiveData(SPI3);//????SPI/I2S???????
}

////////////////////////////////////////////////////////////////////////////
 
//ָ���
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 


uint16_t SPIFlash_ReadDeviceID(void)
{
	uint16_t temp;
	SPI3_Select();
	SPI3_ReadWriteByte(0x90); //read device id command
	SPI3_ReadWriteByte(0x00);
	SPI3_ReadWriteByte(0x00);
	SPI3_ReadWriteByte(0x00);
	temp=SPI3_ReadWriteByte(0x00);
	temp=temp<<8;
	temp+=SPI3_ReadWriteByte(0x00);
	SPI3_Deselect();
	return temp;
}


//��ȡSPI_FLASH��״̬�Ĵ���
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
uint8_t SPIFlash_ReadSR(void)   
{  
	uint8_t byte=0;   
	SPI3_Select();                            //ʹ������   
	SPI3_ReadWriteByte(W25X_ReadStatusReg);    //���Ͷ�ȡ״̬�Ĵ�������    
	byte=SPI3_ReadWriteByte(0Xff);             //��ȡһ���ֽ�  
	SPI3_Deselect();                            //ȡ��Ƭѡ     
	return byte;   
} 
//дSPI_FLASH״̬�Ĵ���
//ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
void SPIFLASH_WriteSR(uint8_t sr)   
{   
	SPI3_Select();                            //ʹ������   
	SPI3_ReadWriteByte(W25X_WriteStatusReg);   //����дȡ״̬�Ĵ�������    
	SPI3_ReadWriteByte(sr);               //д��һ���ֽ�  
	SPI3_Deselect();                            //ȡ��Ƭѡ     	      
}   
//SPI_FLASHдʹ��	
//��WEL��λ   
void SPIFLASH_WriteEnable(void)   
{
	SPI3_Select();                            //ʹ������   
  SPI3_ReadWriteByte(W25X_WriteEnable);      //����дʹ��  
	SPI3_Deselect();                            //ȡ��Ƭѡ     	      
} 
//SPI_FLASHд��ֹ	
//��WEL����  
void SPIFLASH_WriteDisable(void)   
{  
	SPI3_Select();                            //ʹ������   
  SPI3_ReadWriteByte(W25X_WriteDisable);     //����д��ָֹ��    
	SPI3_Deselect();                            //ȡ��Ƭѡ     	      
} 			    


//�ȴ�����
void SPIFlash_WaitBusy(void)   
{   
	while ((SPIFlash_ReadSR()&0x01)==0x01);   // �ȴ�BUSYλ���
}  

   		    
//��ȡSPI FLASH  
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
void SPIFlash_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint32_t NumByteToRead)   
{ 
	SPI3_Select();                            //ʹ������   
	SPI3_ReadWriteByte(W25X_ReadData);         //���Ͷ�ȡ����   
	SPI3_ReadWriteByte((uint8_t)((ReadAddr)>>16));  //����24bit��ַ    
	SPI3_ReadWriteByte((uint8_t)((ReadAddr)>>8));   
	SPI3_ReadWriteByte((uint8_t)ReadAddr);   
  for(; NumByteToRead!=0; NumByteToRead--)
	{ 
    *(pBuffer++)=SPI3_ReadWriteByte(0XFF);   //ѭ������  
  }
	SPI3_Deselect();                            //ȡ��Ƭѡ     	      
}  
//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!	 
void SPIFlash_WritePage(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
	SPIFLASH_WriteEnable();                  //SET WEL 
	SPI3_Select();                            //ʹ������   
	SPI3_ReadWriteByte(W25X_PageProgram);      //����дҳ����   
	SPI3_ReadWriteByte((uint8_t)((WriteAddr)>>16)); //����24bit��ַ    
	SPI3_ReadWriteByte((uint8_t)((WriteAddr)>>8));   
	SPI3_ReadWriteByte((uint8_t)WriteAddr);   
  for(; NumByteToWrite!=0; NumByteToWrite--)
		SPI3_ReadWriteByte(*(pBuffer++));//ѭ��д��  
	SPI3_Deselect();                            //ȡ��Ƭѡ 
	SPIFlash_WaitBusy();					   //�ȴ�д�����
} 
//�޼���дSPI FLASH 
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ���� 
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void SPIFlash_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint32_t NumByteToWrite)   
{ 			 		 
	uint16_t pageremain;	   
	pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
	while(1)
	{	   
		SPIFlash_WritePage(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//д�������
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256)pageremain=256; //һ�ο���д��256���ֽ�
			else pageremain=NumByteToWrite; 	  //����256���ֽ���
		}
	};	    
} 

//����һ������
//Dst_Addr:������ַ 0~511 for w25x16
//����һ��ɽ��������ʱ��:150ms
void SPIFlash_EraseSector(uint32_t Dst_Addr)   
{   
	Dst_Addr*=4096;
	SPIFLASH_WriteEnable();                  //SET WEL 	 
	SPIFlash_WaitBusy();   
	SPI3_Select();                            //ʹ������   
	SPI3_ReadWriteByte(W25X_SectorErase);      //������������ָ�� 
	SPI3_ReadWriteByte((uint8_t)((Dst_Addr)>>16));  //����24bit��ַ    
	SPI3_ReadWriteByte((uint8_t)((Dst_Addr)>>8));   
	SPI3_ReadWriteByte((uint8_t)Dst_Addr);  
	SPI3_Deselect();                            //ȡ��Ƭѡ     	      
	SPIFlash_WaitBusy();   				   //�ȴ��������
}



//дSPI FLASH  
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)  		   
uint8_t SPIFlash_Buf[4096];
void SPIFlash_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint32_t NumByteToWrite)   
{ 
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;	   
 	uint16_t i;    

	secpos=WriteAddr/4096;//������ַ 0~511 for w25x16
	secoff=WriteAddr%4096;//�������ڵ�ƫ��
	secremain=4096-secoff;//����ʣ��ռ��С   

	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
	while(1) 
	{	
		SPIFlash_Read(SPIFlash_Buf,secpos*4096,4096);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(SPIFlash_Buf[secoff+i]!=0XFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			SPIFlash_EraseSector(secpos);//�����������
			for(i=0;i<secremain;i++)	   //����
			{
				SPIFlash_Buf[i+secoff]=pBuffer[i];	  
			}
			SPIFlash_Write_NoCheck(SPIFlash_Buf,secpos*4096,4096);//д����������  

		}
		else 
			SPIFlash_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumByteToWrite==secremain)
			break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 

		   	pBuffer+=secremain;  //ָ��ƫ��
			WriteAddr+=secremain;//д��ַƫ��	   
		   	NumByteToWrite-=secremain;				//�ֽ����ݼ�
			if(NumByteToWrite>4096)
				secremain=4096;	//��һ����������д����
			else 
				secremain=NumByteToWrite;			//��һ����������д����
		}	 
	};	 	 
}
//��������оƬ
//��Ƭ����ʱ��:
//W25X16:25s 
//W25X32:40s 
//W25X64:40s 
//�ȴ�ʱ�䳬��...
void SPIFlash_EraseChip(void)   
{                                             
	SPIFLASH_WriteEnable();                  //SET WEL 
	SPIFlash_WaitBusy();   
	SPI3_Select();                            //ʹ������   
	SPI3_ReadWriteByte(W25X_ChipErase);        //����Ƭ��������  
	SPI3_Deselect();                            //ȡ��Ƭѡ     	      
	SPIFlash_WaitBusy();   				   //�ȴ�оƬ��������
}   
  
//�������ģʽ
void SPIFlash_PowerDown(void)   
{ 
	SPI3_Select();                            //ʹ������   
	SPI3_ReadWriteByte(W25X_PowerDown);        //���͵�������  
	SPI3_Deselect();                            //ȡ��Ƭѡ     	      
	Delay_Simple(30);                               //�ȴ�TPD  
}   
//����
void SPIFlash_WAKEUP(void)   
{  
	SPI3_Select();                            //ʹ������   
	SPI3_ReadWriteByte(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
	SPI3_Deselect();                            //ȡ��Ƭѡ     	      
	Delay_Simple(30);                               //�ȴ�TRES1
}  

uint8_t SPIFlash_Init(void)
{
	uint8_t count=10;
	SPI3_Init();
	while(SPIFlash_ReadDeviceID()!=0xef16)
	{
		count--;
		if(count==0)
		{
			USARTSendString("\n SPIFlash Error");
			return 1;
		}
	}
	USARTSendString("\n SPIFlash Success");
	return 0;
}



