#ifndef _SPI_MEMORY_H
#define _SPI_MEMORY_H


#include "stm32f10x.h"

void SPIFlash_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint32_t NumByteToRead);   //��ȡflash
void SPIFlash_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint32_t NumByteToWrite);//д��flash
uint8_t SPIFlash_Init(void);



#endif


