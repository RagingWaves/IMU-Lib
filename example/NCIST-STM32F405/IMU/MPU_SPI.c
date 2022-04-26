/***//****************************************
             NCIST     创新实验室
----------------------------------------------
 * @file        MPU_SPI.c/h
 * @brief       陀螺仪SPI初始化
 * @mark        1.  STM32F4、MPU9250测试通过--22.4.8
 * @version     V1.0.0
 * @auther      Luo Z.Y.
 * @url         
----------------------------------------------
****************************************//***/

#include "MPU_SPI.h"

/**
  * @brief  陀螺仪SPI初始化
  * @param  SPI速度
  * @retval void
  * @mark   硬件SPI1
  */
void MPU_SPI_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,  ENABLE);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7, GPIO_AF_SPI1);
	
	SPI_Cmd(SPI1, DISABLE);
	
	SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL              = SPI_CPOL_Low;
	SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial     = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	
	SPI_Cmd(SPI1, ENABLE);
	
	SPI1_ReadWriteByte(0xFF);
}

/**
  * @brief  SPI速度设置
  * @param  SPI速度
  * @retval void
  */
void SPI1_SetSpeed(unsigned char SPI_BaudRatePrescaler) {
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI1->CR1&=0XFFC7;
	SPI1->CR1|=SPI_BaudRatePrescaler;
	SPI_Cmd(SPI1,ENABLE);  
} 

/**
  * @brief  SPI读写
  * @param  要发送的字节
  * @retval 读取到的字节
  */
unsigned char SPI1_ReadWriteByte(unsigned char TxData) {		 			 
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}
		SPI_I2S_SendData(SPI1, TxData);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){}
	return SPI_I2S_ReceiveData(SPI1);
}

