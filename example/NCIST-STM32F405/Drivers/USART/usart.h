#ifndef __USART_H
#define __USART_H

#include "sys.h"
#include "stm32f4xx.h"

#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	

#define PC_NVIC 6	 // 防止FreeRTOS报错：Error:..\FreeRTOS\port\RVDS\ARM_CM4F\port.c,768

#define  BSP_USART6_DMA_RX_BUF_LEN               20u
#define  Revice_Len  							 50
#define  Send_Len    							 50

#define  MINIPC_FRAME_LENGTH                     12u

typedef struct
{
  u16 len;
  u8 buffer[Send_Len];
} DataSend;
typedef struct
{
  u16 len;
  u8 buffer[Revice_Len];
} DataRevice;

void USART6_Init(u32 bound);


#endif

