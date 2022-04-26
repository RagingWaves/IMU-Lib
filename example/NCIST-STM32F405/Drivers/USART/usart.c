#include "usart.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "string.h"


unsigned char fff[10];

void USART6_Init(u32 bound)
{
	GPIO_InitTypeDef		GPIO_InitStructure;
	USART_InitTypeDef		USART_InitStructure;
	USART_ClockInitTypeDef	USART_ClockInitStruct;

	USART_StructInit(&USART_InitStructure);
	USART_ClockStructInit(&USART_ClockInitStruct);
	
	{
		//开启USART6时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC ,ENABLE);
		//开启USART6时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC ,ENABLE);

		GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);

		GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_6|GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_UP ;
		GPIO_Init(GPIOC, &GPIO_InitStructure);

		USART_InitStructure.USART_BaudRate				= bound;//波特率
		USART_InitStructure.USART_WordLength			= USART_WordLength_8b;//字长为 8 位数据格式
		USART_InitStructure.USART_StopBits				= USART_StopBits_1;//一个停止位
		USART_InitStructure.USART_Parity				= USART_Parity_No;//无奇偶校验位
		USART_InitStructure.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode					= USART_Mode_Rx | USART_Mode_Tx; //收发模式
		USART_Init(USART6, &USART_InitStructure); //初始化串口
	}
	{
		USART_ITConfig(USART6, USART_IT_IDLE, ENABLE);        //usart IDLE line interrupt  enabled
		USART_ClockInitStruct.USART_Clock	= USART_Clock_Disable;  //时钟低电平活动
		USART_ClockInitStruct.USART_CPOL	= USART_CPOL_Low;  //SLCK引脚上时钟输出的极性->低电平
		USART_ClockInitStruct.USART_CPHA	= USART_CPHA_2Edge;  //时钟第二个边沿进行数据捕获
		USART_ClockInitStruct.USART_LastBit	= USART_LastBit_Disable; //最后一位数据的时钟脉冲不从SCLK输出
		USART_ClockInit(USART6, &USART_ClockInitStruct);
	}
	USART_Cmd(USART6, ENABLE);
}

//PC数据校验函数
uint8_t PcDataCheck( uint8_t *pData )
{
//	if((pData[0]==0xA5)&&(pData[13]==0x5A))

	if((pData[0]==0xA5)&&(pData[11]==0x5A))
	{
		return 1;
	}
	else 
	{
		return 0;
	}
}

//void USART6_IRQHandler()
//{
//	static uint32_t this_time_rx_len = 0;
//	DataRevice Buffer;
//	BaseType_t  TaskWoken;
//	
//	if(USART_GetITStatus(USART6, USART_IT_IDLE) != RESET)
//	{

//		(void)USART6->SR;
//		(void)USART6->DR;
////Target is Memory0
//		if(DMA_GetCurrentMemoryTarget(DMA2_Stream1) == 0)
//		{
//			DMA_Cmd(DMA2_Stream1, DISABLE);
//			this_time_rx_len = BSP_USART6_DMA_RX_BUF_LEN - DMA_GetCurrDataCounter(DMA2_Stream1);
//			DMA2_Stream1->NDTR = (uint16_t)BSP_USART6_DMA_RX_BUF_LEN;     //relocate the dma memory pointer to the beginning position
//			DMA2_Stream1->CR |= (uint32_t)(DMA_SxCR_CT);                  //enable the current selected memory is Memory 1
//			DMA_Cmd(DMA2_Stream1, ENABLE);
//			if(this_time_rx_len == MINIPC_FRAME_LENGTH)
//			{ 			
//			 Buffer.len=MINIPC_FRAME_LENGTH;
//			 memcpy(Buffer.buffer,_USART6_DMA_RX_BUF[0],MINIPC_FRAME_LENGTH);
//			 if(PcDataCheck(Buffer.buffer)==1)
//				{
////				if(verify_crc8_check_sum(&Buffer.buffer[1],10)!=NULL)
////				 
////					{
////						xQueueSendFromISR(TxCOM6,&Buffer,&TaskWoken);	
////					}
//					minipc_Get_Message(&Buffer);
//					DetectHook(TX2DataTOE);
//				}
//				else 
//				{
//					return; 
//				}
//			}
//		}
////Target is Memory1
//		else  
//		{
//			DMA_Cmd(DMA2_Stream1, DISABLE);
//			this_time_rx_len = BSP_USART6_DMA_RX_BUF_LEN - DMA_GetCurrDataCounter(DMA2_Stream1);
//			DMA2_Stream1->NDTR = (uint16_t)BSP_USART6_DMA_RX_BUF_LEN;      //relocate the dma memory pointer to the beginning position
//			DMA2_Stream1->CR &= ~(uint32_t)(DMA_SxCR_CT);                  //enable the current selected memory is Memory 0
//			DMA_Cmd(DMA2_Stream1, ENABLE);
//			if(this_time_rx_len == MINIPC_FRAME_LENGTH)
//			{	
//			Buffer.len=MINIPC_FRAME_LENGTH;
//			memcpy(Buffer.buffer,_USART6_DMA_RX_BUF[1],MINIPC_FRAME_LENGTH);
//				if(PcDataCheck(Buffer.buffer)==1)
//				{
////				if(verify_crc8_check_sum(&Buffer.buffer[1],10)!=NULL)
////				 
////					{
////						xQueueSendFromISR(TxCOM6,&Buffer,&TaskWoken);
////					}
//				minipc_Get_Message(&Buffer);
//				 DetectHook(TX2DataTOE);

//				}
//				else 
//				{
//					return; 
//				}
//			}
//		}
//	}
//	USART_ClearITPendingBit(USART6, USART_IT_IDLE);	
//}

