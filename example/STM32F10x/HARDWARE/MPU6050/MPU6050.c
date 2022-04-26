#include "MPU6050.h"

float gxos = 0, gyos = 0, gzos = 0;

unsigned char MPU6050_Write(unsigned char reg, unsigned char data)
{
	IIC_Start();
	IIC_Write(MPU6050_WADDRESS);
	if(IIC_Wait_Ack())return 1;
	IIC_Write(reg);
	if(IIC_Wait_Ack())return 1;
	IIC_Write(data);
	if(IIC_Wait_Ack())return 1;
	IIC_Stop();
	return 0;
}

unsigned char MPU6050_Read(unsigned char reg)
{
	unsigned char data;
	IIC_Start();
	IIC_Write(MPU6050_WADDRESS);
	IIC_Wait_Ack();
	IIC_Write(reg);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Write(MPU6050_RADDRESS);
	IIC_Wait_Ack();
	data = IIC_Read(0);
	IIC_Stop();
	return data;		
}

unsigned char MPU6050_Write_Len(unsigned char reg, unsigned char len, unsigned char *buf)
{
	IIC_Start();
	IIC_Write(MPU6050_WADDRESS);
	if(IIC_Wait_Ack())return 1;
	IIC_Write(reg);
	if(IIC_Wait_Ack())return 1;
	while(len--)
	{
		IIC_Write(*buf++);
		if(IIC_Wait_Ack())return 1;
	}
	IIC_Stop();
	return 0;
} 

unsigned char MPU6050_Read_Len(unsigned char reg, unsigned char len, unsigned char *buf)
{ 
	IIC_Start();
	IIC_Write(MPU6050_WADDRESS);
	if(IIC_Wait_Ack())return 1;
	IIC_Write(reg);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Write(MPU6050_RADDRESS);
	IIC_Wait_Ack();
	while(len--)
	{
		if(len)*buf=IIC_Read(1);
		else *buf=IIC_Read(0);
		buf++;
	}
	IIC_Stop();
	return 0;
}

//设置MPU6050的数字低通滤波器
//lpf:1,188Hz; 2,98Hz; 3,42Hz; 4,20Hz; 5,10Hz; 6,5Hz
//返回值:0,设置成功
//    其他,设置失败
unsigned char MPU6050_Set_LPF(unsigned char lpf)
{
	return MPU6050_Write(MPU6050_REG_CFG, lpf);
}

//设置MPU6050的采样频率分频器 1000Hz
//返回值:0,设置成功
//    其他,设置失败
unsigned char MPU6050_Set_SampleRate(unsigned char rate)
{
	return MPU6050_Write(MPU6050_REG_SAMPLE_RATE, rate);
}

//陀螺仪传感器满量程范围
//fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
//返回值:0,设置成功
//    其他,设置失败
unsigned char MPU6050_Set_GyroFsr(unsigned char fsr)
{
	return MPU6050_Write(MPU6050_REG_GYRO_CFG, fsr<<3);
}

//设置MPU6050加速度传感器满量程范围
//fsr:0,±2g;1,±4g;2,±8g;3,±16g
//返回值:0,设置成功
//    其他,设置失败
unsigned char MPU6050_Set_AccelFsr(unsigned char fsr)
{
	return MPU6050_Write(MPU6050_REG_ACCEL_CFG, fsr<<3);
}




short MPU6050_Get_Temp(void)
{
	unsigned char buf[2];
	short raw;
	float temp;
	MPU6050_Read_Len(MPU6050_REG_TEMP_OUTH, 2, buf);
    raw=((u16)buf[0]<<8)|buf[1];
    temp=36.53+((float)raw)/340;
    return temp*100;
}

//得到陀螺仪值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
unsigned char MPU6050_Get_RawGyro(short *rgx,short *rgy,short *rgz)
{
	unsigned char buf[6],res = 0;
	res=MPU6050_Read_Len(MPU6050_REG_GYRO_XOUTH,6,buf);
	if(res)return res;
	*rgx=((unsigned short)buf[0]<<8)|buf[1];
	*rgy=((unsigned short)buf[2]<<8)|buf[3];
	*rgz=((unsigned short)buf[4]<<8)|buf[5];
	return res;
}

//得到加速度值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
unsigned char MPU6050_Get_RawAccel(short *rax,short *ray,short *raz)
{
	unsigned char buf[6],res = 0;
	res=MPU6050_Read_Len(MPU6050_REG_ACCEL_XOUTH,6,buf);
	if(res)return res;
	*rax=((unsigned short)buf[0]<<8)|buf[1];
	*ray=((unsigned short)buf[2]<<8)|buf[3];
	*raz=((unsigned short)buf[4]<<8)|buf[5];
	return res;
}



unsigned char MPU6050_Get_Gyro(float *gx,float *gy,float *gz)
{
	unsigned char res = 0;
	short rgx, rgy, rgz;
	
	res = MPU6050_Get_RawGyro(&rgx, &rgy, &rgz);
	if(res)return res;
	*gx = ((float)rgx / 16.384);
	*gy = ((float)rgy / 16.384);
	*gz = ((float)rgz / 16.384);
	
	return res;
}

unsigned char MPU6050_Get_Accel(float *ax,float *ay,float *az)
{
	unsigned char res = 0;
	short rax, ray, raz;
	
	res = MPU6050_Get_RawAccel(&rax, &ray, &raz);
	if(res)return res;
	*ax = (float)rax / 16384;
	*ay = (float)ray / 16384;
	*az = (float)raz / 16384;
	return res;
}



unsigned char MPU6050_Init(void)
{
	IIC_Init();
	
	MPU6050_Write(MPU6050_REG_PWR_MGM1, 	0x80);		//复位内部寄存器
delay_ms(100);
	MPU6050_Write(MPU6050_REG_PWR_MGM1, 	0x00);		//唤醒MPU6050
	
	MPU6050_Set_GyroFsr(3);			//角加速度分辨率2000
	MPU6050_Set_AccelFsr(0);		//加速度分辨率2
	MPU6050_Set_SampleRate(0);
	MPU6050_Set_LPF(0);
	
	MPU6050_Write(MPU6050_REG_INT_EN, 		0x00);		//关闭所有中断
	MPU6050_Write(MPU6050_REG_USER_CTRL,	0x00);		//用户配置
	MPU6050_Write(MPU6050_REG_FIFO_EN,		0x00);		//关闭FIFO
	MPU6050_Write(MPU6050_REG_INTPB_CFG,	0x80);		//INT引脚配置
delay_ms(100);
	MPU6050_Write(MPU6050_REG_PWR_MGM1, 	0x01);
	MPU6050_Write(MPU6050_REG_PWR_MGM2, 	0x00);
	
	return MPU6050_Read(0x75);
}
