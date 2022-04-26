#include "IIC.h"


void IIC_Delay0(unsigned int n)
{
	unsigned int i;
	for ( i=0;i<n;++i);
}


void IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(IIC_SDA_GPIO_CLK		, ENABLE);
	RCC_APB2PeriphClockCmd(IIC_SCL_GPIO_CLK		, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin		= IIC_SDA_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed	= IIC_SDA_GPIO_SPEED;
	GPIO_Init(IIC_SDA_GPIO_PORT		, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin		= IIC_SCL_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed	= IIC_SCL_GPIO_SPEED;
	GPIO_Init(IIC_SCL_GPIO_PORT		, &GPIO_InitStructure);
	
	GPIO_SetBits(IIC_SDA_GPIO_PORT	, IIC_SDA_GPIO_PIN);
	GPIO_SetBits(IIC_SCL_GPIO_PORT	, IIC_SCL_GPIO_PIN);
}

void IIC_SDA_InputMode(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin		= IIC_SDA_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed	= IIC_SDA_GPIO_SPEED;
	GPIO_Init(IIC_SDA_GPIO_PORT		, &GPIO_InitStructure);
}

void IIC_SDA_OutputMode(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin		= IIC_SDA_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed	= IIC_SDA_GPIO_SPEED;
	GPIO_Init(IIC_SDA_GPIO_PORT		, &GPIO_InitStructure);
}

//SDA输出电平
void IIC_SDA_Output(u8 sta)
{
	if(sta)
		GPIO_SetBits(IIC_SDA_GPIO_PORT		, IIC_SDA_GPIO_PIN);
	else
		GPIO_ResetBits(IIC_SDA_GPIO_PORT	, IIC_SDA_GPIO_PIN);
}
//SDA输入电平
u8 IIC_SDA_Input(void)
{
	return GPIO_ReadInputDataBit(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN);
}

//SCL输出电平
void IIC_SCL_Output(u8 sta)
{
	if(sta)
		GPIO_SetBits(IIC_SCL_GPIO_PORT		, IIC_SCL_GPIO_PIN);
	else
		GPIO_ResetBits(IIC_SCL_GPIO_PORT	, IIC_SCL_GPIO_PIN);
}

void IIC_Start(void)
{
	IIC_SDA_OutputMode();
	IIC_SDA_Output(1);
	IIC_SCL_Output(1);IIC_Delay(4);
	IIC_SDA_Output(0);IIC_Delay(4);
	IIC_SCL_Output(0);
}

void IIC_Stop(void)
{
	IIC_SDA_OutputMode();
	IIC_SCL_Output(0);
	IIC_SDA_Output(0);IIC_Delay(4);
	IIC_SCL_Output(1);IIC_Delay(4);
	IIC_SDA_Output(1);
}

u8 IIC_Wait_Ack(void)
{
	int ErrTime=0;
	
	IIC_SDA_InputMode();
	IIC_SDA_Output(1);IIC_Delay(1);
	IIC_SCL_Output(1);IIC_Delay(1);
	while(IIC_SDA_Input())
	{
		ErrTime++;
		if(ErrTime>250)
		{
			IIC_SDA_OutputMode();
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL_Output(0);
	return 0;
}

void IIC_Ack(void)
{
	
	IIC_SCL_Output(0);
	IIC_SDA_OutputMode();
	IIC_SDA_Output(0);IIC_Delay(2);
	IIC_SCL_Output(1);IIC_Delay(2);
	IIC_SCL_Output(0);
}

void IIC_NAck(void)
{
	
	IIC_SCL_Output(0);
	IIC_SDA_OutputMode();
	IIC_SDA_Output(1);IIC_Delay(2);
	IIC_SCL_Output(1);IIC_Delay(2);
	IIC_SCL_Output(0);
}

//发送数据
//参数1：需要发送的数据
void IIC_Write(u8 TxData)
{
	u8 i;
	IIC_SDA_OutputMode();
	IIC_SCL_Output(0);
	for(i=0; i<8; i++)
	{
		if((TxData&0x80 )>> 7)
			IIC_SDA_Output(1);
		else
			IIC_SDA_Output(0);
		TxData <<= 1;
		IIC_SCL_Output(1);IIC_Delay(2);
		IIC_SCL_Output(0);IIC_Delay(2);
	}
}

//接收数据
//返回值：接收到的数据
u8 IIC_Read(u8 ack)
{
	u8 i, RxData=0;
	IIC_SDA_InputMode();
	for(i=0; i<8; i++)
	{
		IIC_SCL_Output(0);IIC_Delay(2);
		IIC_SCL_Output(1);
		RxData <<= 1;
		if(IIC_SDA_Input())
			RxData |= 0x01;
		IIC_Delay(1);
	}
	if(ack)
		IIC_Ack();
	else
		IIC_NAck();
	return RxData;
}

