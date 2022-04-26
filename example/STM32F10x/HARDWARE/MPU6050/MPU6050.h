#ifndef __MPU6050_H
#define __MPU6050_H

#include "sys.h"
#include "IIC.h"

#define MPU6050_WADDRESS	0xD0
#define MPU6050_RADDRESS	0xD1

//#define PI 3.1415926
//#define GYRO_CALIBRATION_COFF_2 0.060976f  //2000
//#define GYRO_CALIBRATION_COFF   0.030488f    //1000
#define DEG2RAD         (PI / 180.0f)
#define RAD2DEG         (180.0f / PI)

/*-----Register Map-----*/
#define MPU6050_REG_SAMPLE_RATE		0x19 //25	采样频率分频器
#define MPU6050_REG_CFG				0x1A //26	配置
#define MPU6050_REG_GYRO_CFG		0x1B //27	陀螺仪配置
#define MPU6050_REG_ACCEL_CFG		0x1C //28	加速度计配置


#define MPU6050_REG_ACCEL_XOUTH		0x3B //59	X加速度高8位
#define MPU6050_REG_ACCEL_XOUTL		0x3C
#define MPU6050_REG_ACCEL_YOUTH		0x3D
#define MPU6050_REG_ACCEL_YOUTL		0x3E
#define MPU6050_REG_ACCEL_ZOUTH		0x3F
#define MPU6050_REG_ACCEL_ZOUTL		0x40

#define MPU6050_REG_TEMP_OUTH		0x41 //65	温度高8位
#define MPU6050_REG_TEMP_OUTL		0x42 //66	温度低8位

#define MPU6050_REG_GYRO_XOUTH		0x43 //67	X陀螺仪高8位
#define MPU6050_REG_GYRO_XOUTL		0x44
#define MPU6050_REG_GYRO_YOUTH		0x45
#define MPU6050_REG_GYRO_YOUTL		0x46
#define MPU6050_REG_GYRO_ZOUTH		0x47
#define MPU6050_REG_GYRO_ZOUTL		0x48

#define MPU6050_REG_FIFO_EN			0x23 //35	FIFO使能
#define MPU6050_REG_INTPB_CFG		0x37 //55	INT引脚/旁路使能
#define MPU6050_REG_INT_EN			0x38 //56	中断使能
#define MPU6050_REG_USER_CTRL		0x6A //106	用户配置
#define MPU6050_REG_PWR_MGM1		0x6B //107	电源管理1
#define MPU6050_REG_PWR_MGM2		0x6C //108	电源管理2


unsigned char MPU6050_Init(void);
short MPU6050_Get_Temp(void);
unsigned char MPU6050_Get_RawGyro(short *rgx,short *rgy,short *rgz);
unsigned char MPU6050_Get_RawAccel(short *rax,short *ray,short *raz);
unsigned char MPU6050_Get_Gyro(float *gx,float *gy,float *gz);
unsigned char MPU6050_Get_Accel(float *ax,float *ay,float *az);

unsigned char MPU6050_GyroOffset(void);


#endif

