#ifndef __MPU_SPI_H
#define __MPU_SPI_H

#include "stm32f4xx.h"

void MPU_SPI_Init(void);
void SPI1_SetSpeed(unsigned char SPI_BaudRatePrescaler);
unsigned char SPI1_ReadWriteByte(unsigned char TxData);

#endif

