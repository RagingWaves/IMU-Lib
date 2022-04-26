#ifndef __IIC_H
#define __IIC_H

#include "sys.h"

#define IIC_SDA_GPIO_PIN	GPIO_Pin_10
#define IIC_SDA_GPIO_PORT	GPIOB
#define IIC_SDA_GPIO_CLK	RCC_APB2Periph_GPIOB
#define IIC_SDA_GPIO_SPEED	GPIO_Speed_2MHz

#define IIC_SCL_GPIO_PIN	GPIO_Pin_11
#define IIC_SCL_GPIO_PORT	GPIOB
#define IIC_SCL_GPIO_CLK	RCC_APB2Periph_GPIOB
#define IIC_SCL_GPIO_SPEED	GPIO_Speed_2MHz


#define IIC_Delay(n)	IIC_Delay0(n*0)//100


void IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
u8 IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_Write(u8 TxData);
u8 IIC_Read(u8 ack);


#endif

