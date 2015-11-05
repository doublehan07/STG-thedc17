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
 
//指令表
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


//读取SPI_FLASH的状态寄存器
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00
uint8_t SPIFlash_ReadSR(void)   
{  
	uint8_t byte=0;   
	SPI3_Select();                            //使能器件   
	SPI3_ReadWriteByte(W25X_ReadStatusReg);    //发送读取状态寄存器命令    
	byte=SPI3_ReadWriteByte(0Xff);             //读取一个字节  
	SPI3_Deselect();                            //取消片选     
	return byte;   
} 
//写SPI_FLASH状态寄存器
//只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
void SPIFLASH_WriteSR(uint8_t sr)   
{   
	SPI3_Select();                            //使能器件   
	SPI3_ReadWriteByte(W25X_WriteStatusReg);   //发送写取状态寄存器命令    
	SPI3_ReadWriteByte(sr);               //写入一个字节  
	SPI3_Deselect();                            //取消片选     	      
}   
//SPI_FLASH写使能	
//将WEL置位   
void SPIFLASH_WriteEnable(void)   
{
	SPI3_Select();                            //使能器件   
  SPI3_ReadWriteByte(W25X_WriteEnable);      //发送写使能  
	SPI3_Deselect();                            //取消片选     	      
} 
//SPI_FLASH写禁止	
//将WEL清零  
void SPIFLASH_WriteDisable(void)   
{  
	SPI3_Select();                            //使能器件   
  SPI3_ReadWriteByte(W25X_WriteDisable);     //发送写禁止指令    
	SPI3_Deselect();                            //取消片选     	      
} 			    


//等待空闲
void SPIFlash_WaitBusy(void)   
{   
	while ((SPIFlash_ReadSR()&0x01)==0x01);   // 等待BUSY位清空
}  

   		    
//读取SPI FLASH  
//在指定地址开始读取指定长度的数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(24bit)
//NumByteToRead:要读取的字节数(最大65535)
void SPIFlash_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint32_t NumByteToRead)   
{ 
	SPI3_Select();                            //使能器件   
	SPI3_ReadWriteByte(W25X_ReadData);         //发送读取命令   
	SPI3_ReadWriteByte((uint8_t)((ReadAddr)>>16));  //发送24bit地址    
	SPI3_ReadWriteByte((uint8_t)((ReadAddr)>>8));   
	SPI3_ReadWriteByte((uint8_t)ReadAddr);   
  for(; NumByteToRead!=0; NumByteToRead--)
	{ 
    *(pBuffer++)=SPI3_ReadWriteByte(0XFF);   //循环读数  
  }
	SPI3_Deselect();                            //取消片选     	      
}  
//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!	 
void SPIFlash_WritePage(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
	SPIFLASH_WriteEnable();                  //SET WEL 
	SPI3_Select();                            //使能器件   
	SPI3_ReadWriteByte(W25X_PageProgram);      //发送写页命令   
	SPI3_ReadWriteByte((uint8_t)((WriteAddr)>>16)); //发送24bit地址    
	SPI3_ReadWriteByte((uint8_t)((WriteAddr)>>8));   
	SPI3_ReadWriteByte((uint8_t)WriteAddr);   
  for(; NumByteToWrite!=0; NumByteToWrite--)
		SPI3_ReadWriteByte(*(pBuffer++));//循环写数  
	SPI3_Deselect();                            //取消片选 
	SPIFlash_WaitBusy();					   //等待写入结束
} 
//无检验写SPI FLASH 
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能 
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
void SPIFlash_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint32_t NumByteToWrite)   
{ 			 		 
	uint16_t pageremain;	   
	pageremain=256-WriteAddr%256; //单页剩余的字节数		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
	while(1)
	{	   
		SPIFlash_WritePage(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//写入结束了
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite>256)pageremain=256; //一次可以写入256个字节
			else pageremain=NumByteToWrite; 	  //不够256个字节了
		}
	};	    
} 

//擦除一个扇区
//Dst_Addr:扇区地址 0~511 for w25x16
//擦除一个山区的最少时间:150ms
void SPIFlash_EraseSector(uint32_t Dst_Addr)   
{   
	Dst_Addr*=4096;
	SPIFLASH_WriteEnable();                  //SET WEL 	 
	SPIFlash_WaitBusy();   
	SPI3_Select();                            //使能器件   
	SPI3_ReadWriteByte(W25X_SectorErase);      //发送扇区擦除指令 
	SPI3_ReadWriteByte((uint8_t)((Dst_Addr)>>16));  //发送24bit地址    
	SPI3_ReadWriteByte((uint8_t)((Dst_Addr)>>8));   
	SPI3_ReadWriteByte((uint8_t)Dst_Addr);  
	SPI3_Deselect();                            //取消片选     	      
	SPIFlash_WaitBusy();   				   //等待擦除完成
}



//写SPI FLASH  
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)  		   
uint8_t SPIFlash_Buf[4096];
void SPIFlash_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint32_t NumByteToWrite)   
{ 
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;	   
 	uint16_t i;    

	secpos=WriteAddr/4096;//扇区地址 0~511 for w25x16
	secoff=WriteAddr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小   

	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
	while(1) 
	{	
		SPIFlash_Read(SPIFlash_Buf,secpos*4096,4096);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(SPIFlash_Buf[secoff+i]!=0XFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			SPIFlash_EraseSector(secpos);//擦除这个扇区
			for(i=0;i<secremain;i++)	   //复制
			{
				SPIFlash_Buf[i+secoff]=pBuffer[i];	  
			}
			SPIFlash_Write_NoCheck(SPIFlash_Buf,secpos*4096,4096);//写入整个扇区  

		}
		else 
			SPIFlash_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumByteToWrite==secremain)
			break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

		   	pBuffer+=secremain;  //指针偏移
			WriteAddr+=secremain;//写地址偏移	   
		   	NumByteToWrite-=secremain;				//字节数递减
			if(NumByteToWrite>4096)
				secremain=4096;	//下一个扇区还是写不完
			else 
				secremain=NumByteToWrite;			//下一个扇区可以写完了
		}	 
	};	 	 
}
//擦除整个芯片
//整片擦除时间:
//W25X16:25s 
//W25X32:40s 
//W25X64:40s 
//等待时间超长...
void SPIFlash_EraseChip(void)   
{                                             
	SPIFLASH_WriteEnable();                  //SET WEL 
	SPIFlash_WaitBusy();   
	SPI3_Select();                            //使能器件   
	SPI3_ReadWriteByte(W25X_ChipErase);        //发送片擦除命令  
	SPI3_Deselect();                            //取消片选     	      
	SPIFlash_WaitBusy();   				   //等待芯片擦除结束
}   
  
//进入掉电模式
void SPIFlash_PowerDown(void)   
{ 
	SPI3_Select();                            //使能器件   
	SPI3_ReadWriteByte(W25X_PowerDown);        //发送掉电命令  
	SPI3_Deselect();                            //取消片选     	      
	Delay_Simple(30);                               //等待TPD  
}   
//唤醒
void SPIFlash_WAKEUP(void)   
{  
	SPI3_Select();                            //使能器件   
	SPI3_ReadWriteByte(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
	SPI3_Deselect();                            //取消片选     	      
	Delay_Simple(30);                               //等待TRES1
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



