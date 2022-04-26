#include "MPU9250.h"
#include "MPU_SPI.h"

/**
  * @brief  软件延时，用于陀螺仪初始化
  * @param  延时的毫秒数
  * @retval void
  * @attention  MPU9250/MPU6050初始化对软件延时要求较高，如果不能保证自己的软件
  *             延时能够正常工作（1毫秒实际只有几十微秒那种），建议取消删除头文
  *             件同名的宏定义并使用下面的函数，错误的延时可能导致初始化错误。
  *             该函数为STM32F405于168MHz的延时。
  */
//void MPU_delay_ms(unsigned int t)
//{
//	int i;
//	for( i=0;i<t;i++)
//	{
//		int a=42000; //at 168MHz 42000 is ok
//		while(a--);
//	}
//}

/**
  * @brief  连续读
  * @param  陀螺仪编号
  *         1：陀螺仪1
  *         2：陀螺仪2
  * @param  寄存器地址
  * @param  连续读长度
  * @param  读取的数据
  * @retval void
  */
void MPU9250_Read_Len(int cs_n, unsigned char reg,int len, unsigned char *buf)
{
	if(cs_n == 1)
		GPIO_ResetBits(MPU1_CS_GPIO_PORT, MPU1_CS_GPIO_PIN);
	else if (cs_n == 2)
		GPIO_ResetBits(MPU2_CS_GPIO_PORT, MPU2_CS_GPIO_PIN);
	SPI1_ReadWriteByte(reg | 0x80);
	while(len--) {
		*buf++ = SPI1_ReadWriteByte(0xFF);
		reg++;
	}
	if(cs_n == 1)
		GPIO_SetBits(MPU1_CS_GPIO_PORT, MPU1_CS_GPIO_PIN);
	else if (cs_n == 2)
		GPIO_SetBits(MPU2_CS_GPIO_PORT, MPU2_CS_GPIO_PIN);
}

/**
  * @brief  连续写
  * @param  陀螺仪编号
  *         1：陀螺仪1
  *         2：陀螺仪2
  * @param  寄存器地址
  * @param  连续写长度
  * @param  写入的数据
  * @retval void
  */
void MPU9250_Write_Len(int cs_n, unsigned char reg,int len,unsigned char *buf)
{
	int i = 0;
	
	if(cs_n == 1)
		GPIO_ResetBits(MPU1_CS_GPIO_PORT, MPU1_CS_GPIO_PIN);
	else if (cs_n == 2)
		GPIO_ResetBits(MPU2_CS_GPIO_PORT, MPU2_CS_GPIO_PIN);
	SPI1_ReadWriteByte(reg);
	for(i = 0; i < len; i++, reg++) {
		SPI1_ReadWriteByte(buf[i]);
	}
	if(cs_n == 1)
		GPIO_SetBits(MPU1_CS_GPIO_PORT, MPU1_CS_GPIO_PIN);
	else if (cs_n == 2)
		GPIO_SetBits(MPU2_CS_GPIO_PORT, MPU2_CS_GPIO_PIN);
}

/**
  * @brief  读取1字节
  * @param  陀螺仪编号
  *         1：陀螺仪1
  *         2：陀螺仪2
  * @param  寄存器地址
  * @retval 读取的数据
  */
unsigned char MPU9250_Read_Byte(int cs_n, unsigned char reg)
{
	unsigned char data;
	
	if(cs_n == 1)
		GPIO_ResetBits(MPU1_CS_GPIO_PORT, MPU1_CS_GPIO_PIN);
	else if (cs_n == 2)
		GPIO_ResetBits(MPU2_CS_GPIO_PORT, MPU2_CS_GPIO_PIN);
	SPI1_ReadWriteByte(reg | 0x80);
	data = SPI1_ReadWriteByte(0xFF);
	if(cs_n == 1)
		GPIO_SetBits(MPU1_CS_GPIO_PORT, MPU1_CS_GPIO_PIN);
	else if (cs_n == 2)
		GPIO_SetBits(MPU2_CS_GPIO_PORT, MPU2_CS_GPIO_PIN);
	
	return data;
}

/**
  * @brief  写入1字节
  * @param  陀螺仪编号
  *         1：陀螺仪1
  *         2：陀螺仪2
  * @param  寄存器地址
  * @param  写入的数据
  * @retval void
  */
void MPU9250_Write_Byte(int cs_n, unsigned char reg,unsigned char data)
{
	if(cs_n == 1)
		GPIO_ResetBits(MPU1_CS_GPIO_PORT, MPU1_CS_GPIO_PIN);
	else if (cs_n == 2)
		GPIO_ResetBits(MPU2_CS_GPIO_PORT, MPU2_CS_GPIO_PIN);
	SPI1_ReadWriteByte(reg);
	SPI1_ReadWriteByte(data);
	if(cs_n == 1)
		GPIO_SetBits(MPU1_CS_GPIO_PORT, MPU1_CS_GPIO_PIN);
	else if (cs_n == 2)
		GPIO_SetBits(MPU2_CS_GPIO_PORT, MPU2_CS_GPIO_PIN);
}

/**
  * @brief  获取陀螺仪温度
  * @param  获取到的温度
  * @retval void
  */
void MPU9250_Get_Temp(float *temp) {
	unsigned char buf[2];
	short raw1, raw2;
	
	MPU9250_Read_Len(1, MPU_TEMP_OUTH_REG, 2, buf);
	raw1 = ((short)(buf[0])<<8)|buf[1];
	MPU9250_Read_Len(2, MPU_TEMP_OUTH_REG, 2, buf);
	raw2 = ((short)(buf[0])<<8)|buf[1];
	*temp = (((double)raw1 / 333.87 + 21) + ((double)raw2 / 333.87 + 21)) / 2;
}

/**
  * @brief  获取陀螺仪加速度
  * @param  获取到的x轴加速度
  * @param  获取到的y轴加速度
  * @param  获取到的z轴加速度
  * @retval void
  */
void MPU9250_Get_Accel(short *ax, short *ay, short *az) {
	unsigned char buf[6];
	short raw1[3], raw2[3];
	
	MPU9250_Read_Len(1, MPU_ACCEL_XOUTH_REG, 6, buf);
	raw1[0] = ((unsigned short)buf[0]<<8)|buf[1];
	raw1[1] = ((unsigned short)buf[2]<<8)|buf[3];
	raw1[2] = ((unsigned short)buf[4]<<8)|buf[5];
	MPU9250_Read_Len(2, MPU_ACCEL_XOUTH_REG, 6, buf);
	raw2[0] = ((unsigned short)buf[0]<<8)|buf[1];
	raw2[1] = ((unsigned short)buf[2]<<8)|buf[3];
	raw2[2] = ((unsigned short)buf[4]<<8)|buf[5];
	
	*ax = (raw1[0] + raw2[1]) / 2;
	*ay = (raw1[1] - raw2[0]) / 2;
	*az = (raw1[2] + raw2[2]) / 2;
}

/**
  * @brief  获取单位转换后的陀螺仪加速度，单位为g
  * @param  获取到的x轴加速度
  * @param  获取到的y轴加速度
  * @param  获取到的z轴加速度
  * @retval void
  */
void MPU9250_Get_Accel_transform(float *ax, float *ay, float *az) {
	short rax, ray, raz;
	
	MPU9250_Get_Accel(&rax, &ray, &raz);
	//2g量程
	*ax = (float)rax / 16384;
	*ay = (float)ray / 16384;
	*az = (float)raz / 16384;
}

/**
  * @brief  获取陀螺仪角加速度
  * @param  获取到的x轴角加速度
  * @param  获取到的y轴角加速度
  * @param  获取到的z轴角加速度
  * @retval void
  */
void MPU9250_Get_Gyro(short *gx, short *gy, short *gz) {
	unsigned char buf[6];
	short raw1[3], raw2[3];
	
	MPU9250_Read_Len(1, MPU_GYRO_XOUTH_REG, 6, buf);
	raw1[0] = ((unsigned short)buf[0]<<8)|buf[1];
	raw1[1] = ((unsigned short)buf[2]<<8)|buf[3];
	raw1[2] = ((unsigned short)buf[4]<<8)|buf[5];
	MPU9250_Read_Len(2, MPU_GYRO_XOUTH_REG, 6, buf);
	raw2[0] = ((unsigned short)buf[0]<<8)|buf[1];
	raw2[1] = ((unsigned short)buf[2]<<8)|buf[3];
	raw2[2] = ((unsigned short)buf[4]<<8)|buf[5];
	
	*gx = (raw1[0] + raw2[1]) / 2;
	*gy = (raw1[1] - raw2[0]) / 2;
	*gz = (raw1[2] + raw2[2]) / 2;
}

/**
  * @brief  获取单位转换后的陀螺仪角加速度，单位为度/秒
  * @param  获取到的x轴角加速度
  * @param  获取到的y轴角加速度
  * @param  获取到的z轴角加速度
  * @retval void
  */
void MPU9250_Get_Gyro_transform(float *gx, float *gy, float *gz) {
	short rgx, rgy, rgz;
	
	MPU9250_Get_Gyro(&rgx, &rgy, &rgz);
	//2000dps量程
	*gx = (rgx * 0.060976f);
	*gy = (rgy * 0.060976f);
	*gz = (rgz * 0.060976f);
}

/**
  * @brief  获取磁力计数据
  * @param  获取到的x轴磁力计数据
  * @param  获取到的y轴磁力计数据
  * @param  获取到的z轴磁力计数据
  * @retval void
  */
void MPU9250_Get_Mag(short *mx, short *my, short *mz) {
	
}

/**
  * @brief  设置加热PWM占空比
  * @param  PWM占空比，范围为0~1000
  * @retval void
  */
void MPU9250_Set_Heat_PWM(int duty) {
	//注意限幅，避免PID输出负数或超定时器计数值
	if(duty > 1000)
		duty = 1000;
	if(duty < 0)
		duty = 0;
	TIM_SetCompare3(TIM3, duty);
}

/**
  * @brief  陀螺仪1寄存器初始化
  * @retval 1：初始化失败
  *         0：初始化成功
  */
int MPU9250_1_Init(void) {
	unsigned char get_ID = 0;
	
	get_ID = MPU9250_Read_Byte(1, MPU_DEVICE_ID_REG);
	MPU_delay_ms(10);
	if(get_ID==0x70)
	{
		MPU9250_Write_Byte(1, MPU_PWR_MGMT1_REG,    0x80);
		MPU_delay_ms(10);
		MPU9250_Write_Byte(1, MPU_PWR_MGMT1_REG,    0x00);
		MPU_delay_ms(10);
		MPU9250_Write_Byte(1, MPU_SIGPATH_RST_REG,  0x07);
		MPU_delay_ms(10);
		MPU9250_Write_Byte(1, MPU_PWR_MGMT1_REG,    0x03);
		MPU_delay_ms(10);
		MPU9250_Write_Byte(1, MPU_PWR_MGMT2_REG,    0x00);
		MPU_delay_ms(10);
		MPU9250_Write_Byte(1, MPU_CFG_REG,          0x00);
		MPU_delay_ms(10);
		MPU9250_Write_Byte(1, MPU_SAMPLE_RATE_REG,  0x00);  //1K采样率
		MPU_delay_ms(10);
		MPU9250_Write_Byte(1, MPU_CFG_REG,          0x00);  //内部低通滤波频率
		MPU_delay_ms(10);
		MPU9250_Write_Byte(1, MPU_GYRO_CFG_REG,     3<<3);  //陀螺仪量程，0~3对应250、500、1k、2k（dps）
		MPU_delay_ms(10);
		MPU9250_Write_Byte(1, MPU_ACCEL_CFG_REG,    0<<3);  //加速度量程，0~3对应2g、4g、8g、16g
		MPU_delay_ms(10);
		MPU9250_Write_Byte(1, MPU_INT_EN_REG,       0x01);  //中断
		MPU_delay_ms(10);
		MPU9250_Write_Byte(1, MPU_INTBP_CFG_REG,    0x01);
		MPU_delay_ms(10);
		MPU9250_Write_Byte(1, 0x27, 0x0D);
		MPU_delay_ms(10);
		MPU9250_Write_Byte(1, 0x67, 0x0D);
		MPU_delay_ms(10);
		MPU9250_Write_Byte(1, 0x27, 0x0D);
		MPU_delay_ms(10);
		MPU9250_Write_Byte(1, MPU_PWR_MGMT1_REG,    0x01);
		MPU_delay_ms(10);
		MPU9250_Write_Byte(1, MPU_PWR_MGMT2_REG,    0x00);
		MPU_delay_ms(10);
		return 0;
	}
	else
		return 1;
}

/**
  * @brief  陀螺仪2寄存器初始化
  * @retval 1：初始化失败
  *         0：初始化成功
  */
int MPU9250_2_Init(void) {
	unsigned char get_ID = 0;
	
	get_ID = MPU9250_Read_Byte(2, MPU_DEVICE_ID_REG);
	MPU_delay_ms(10);
	if(get_ID==0x70)
	{
		MPU9250_Write_Byte(2, MPU_PWR_MGMT1_REG,    0x80);
		MPU_delay_ms(10);
		MPU9250_Write_Byte(2, MPU_PWR_MGMT1_REG,    0x00);
		MPU_delay_ms(10);
		MPU9250_Write_Byte(2, MPU_SIGPATH_RST_REG,  0x07);
		MPU_delay_ms(10);
		MPU9250_Write_Byte(2, MPU_PWR_MGMT1_REG,    0x03);
		MPU_delay_ms(10);
		MPU9250_Write_Byte(2, MPU_PWR_MGMT2_REG,    0x00);
		MPU_delay_ms(10);
		MPU9250_Write_Byte(2, MPU_CFG_REG,          0x00);
		MPU_delay_ms(10);
		MPU9250_Write_Byte(2, MPU_SAMPLE_RATE_REG,  0x00);  //1K采样率
		MPU_delay_ms(10);
		MPU9250_Write_Byte(2, MPU_CFG_REG,          0x00);  //内部低通滤波频率
		MPU_delay_ms(10);
		MPU9250_Write_Byte(2, MPU_GYRO_CFG_REG,     3<<3);  //陀螺仪量程，0~3对应250、500、1k、2k（dps）
		MPU_delay_ms(10);
		MPU9250_Write_Byte(2, MPU_ACCEL_CFG_REG,    0<<3);  //加速度量程，0~3对应2g、4g、8g、16g
		MPU_delay_ms(10);
		MPU9250_Write_Byte(2, MPU_INT_EN_REG,       0x01);  //中断
		MPU_delay_ms(10);
		MPU9250_Write_Byte(2, MPU_INTBP_CFG_REG,    0x01);
		MPU_delay_ms(10);
		MPU9250_Write_Byte(2, 0x27, 0x0D);
		MPU_delay_ms(10);
		MPU9250_Write_Byte(2, 0x26, 0x0D);
		MPU_delay_ms(10);
		MPU9250_Write_Byte(2, 0x27, 0x0D);
		MPU_delay_ms(10);
		MPU9250_Write_Byte(2, MPU_PWR_MGMT1_REG,    0x01);
		MPU_delay_ms(10);
		MPU9250_Write_Byte(2, MPU_PWR_MGMT2_REG,    0x00);
		MPU_delay_ms(10);
		return 0;
	}
	else
		return 1;
}

/**
  * @brief  陀螺仪初始化
  * @retval 1：初始化失败
  *         0：初始化成功
  * @mark   两个陀螺仪的初始化设置应当一样
  */
int MPU9250_Init(void) {
	{  //使能引脚初始化
		GPIO_InitTypeDef  GPIO_InitStructure;
		
		RCC_AHB1PeriphClockCmd(MPU1_CS_GPIO_CLK,  ENABLE);
		RCC_AHB1PeriphClockCmd(MPU2_CS_GPIO_CLK,  ENABLE);
		
		GPIO_InitStructure.GPIO_Pin   = MPU1_CS_GPIO_PIN;
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_Init(MPU1_CS_GPIO_PORT, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin   = MPU2_CS_GPIO_PIN;
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_Init(MPU2_CS_GPIO_PORT, &GPIO_InitStructure);

		GPIO_SetBits(MPU1_CS_GPIO_PORT, MPU1_CS_GPIO_PIN);
		GPIO_SetBits(MPU2_CS_GPIO_PORT, MPU2_CS_GPIO_PIN);
	}
	{  //SPI初始化
		MPU_SPI_Init();
		GPIO_SetBits(MPU1_CS_GPIO_PORT, MPU1_CS_GPIO_PIN);
		GPIO_SetBits(MPU2_CS_GPIO_PORT, MPU2_CS_GPIO_PIN);
	}
	{  //加热引脚初始化//加热PWM初始化
		GPIO_InitTypeDef         GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef        TIM_OCInitStructure;
		
		RCC_AHB1PeriphClockCmd(MPU_HEAT_GPIO_CLK,   ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		
		GPIO_PinAFConfig(MPU_HEAT_GPIO_PORT, MPU_HEAT_AF_PINSOURCE, GPIO_AF_TIM3);
		
		GPIO_InitStructure.GPIO_Pin   = MPU_HEAT_GPIO_PIN;
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
		GPIO_Init(MPU_HEAT_GPIO_PORT, &GPIO_InitStructure);
		
		GPIO_ResetBits(MPU_HEAT_GPIO_PORT, MPU_HEAT_GPIO_PIN);
		
		TIM_TimeBaseStructure.TIM_Prescaler         = MPU_HEAT_TIM_PSC;
		TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
		TIM_TimeBaseStructure.TIM_Period            = MPU_HEAT_TIM_ARR;
		TIM_TimeBaseStructure.TIM_ClockDivision     = 0;
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
		
		TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_High;
		TIM_OC3Init(TIM3, &TIM_OCInitStructure);
		
		TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
		
		TIM_ARRPreloadConfig(TIM3, ENABLE);
		
		TIM_Cmd(TIM3, ENABLE);
		
		TIM_SetCompare3(TIM3, 0);
	}
	{
		if(MPU9250_1_Init())
			return 1;
		if(MPU9250_2_Init())
			return 1;
	}
	return 0;
}

