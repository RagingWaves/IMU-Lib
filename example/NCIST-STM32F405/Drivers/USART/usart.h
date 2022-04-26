#ifndef __USART_H
#define __USART_H

#include "sys.h"
#include "stm32f4xx.h"

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	

#define PC_NVIC 6	 // ��ֹFreeRTOS����Error:..\FreeRTOS\port\RVDS\ARM_CM4F\port.c,768

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

