#ifndef __MPU9250_H
#define __MPU9250_H

#include "stm32f4xx.h"
#include "delay.h"

#define MPU_delay_ms(n)         delay_ms(n)

#define MPU1_CS_GPIO_PIN        GPIO_Pin_4
#define MPU1_CS_GPIO_PORT       GPIOC
#define MPU1_CS_GPIO_CLK        RCC_AHB1Periph_GPIOC

#define MPU2_CS_GPIO_PIN        GPIO_Pin_4
#define MPU2_CS_GPIO_PORT       GPIOA
#define MPU2_CS_GPIO_CLK        RCC_AHB1Periph_GPIOA

#define MPU_HEAT_GPIO_PIN       GPIO_Pin_0
#define MPU_HEAT_GPIO_PORT      GPIOB
#define MPU_HEAT_GPIO_CLK       RCC_AHB1Periph_GPIOB
#define MPU_HEAT_AF_PINSOURCE   GPIO_PinSource0
#define MPU_HEAT_TIM_PSC        84 - 1
#define MPU_HEAT_TIM_ARR        1000 - 1


#define MPU_SELF_TESTX_REG      0x0D    //自检寄存器X
#define MPU_SELF_TESTY_REG      0x0E    //自检寄存器Y
#define MPU_SELF_TESTZ_REG      0x0F    //自检寄存器Z

#define MPU_SAMPLE_RATE_REG     0x19    //采样频率分频器
#define MPU_CFG_REG             0x1A    //配置寄存器
#define MPU_GYRO_CFG_REG        0x1B    //陀螺仪配置寄存器
#define MPU_ACCEL_CFG_REG       0x1C    //加速度计配置寄存器
#define MPU_MOTION_DET_REG      0x1F    //运动检测阀值设置寄存器
#define MPU_FIFO_EN_REG         0x23    //FIFO使能寄存器
#define MPU_I2CMST_CTRL_REG     0x24    //IIC主机控制寄存器
#define MPU_I2CSLV0_ADDR_REG    0x25    //IIC从机0器件地址寄存器
#define MPU_I2CSLV0_REG         0x26    //IIC从机0数据地址寄存器
#define MPU_I2CSLV0_CTRL_REG    0x27    //IIC从机0控制寄存器
#define MPU_I2CSLV1_ADDR_REG    0x28    //IIC从机1器件地址寄存器
#define MPU_I2CSLV1_REG         0x29    //IIC从机1数据地址寄存器
#define MPU_I2CSLV1_CTRL_REG    0x2A    //IIC从机1控制寄存器
#define MPU_I2CSLV2_ADDR_REG    0x2B    //IIC从机2器件地址寄存器
#define MPU_I2CSLV2_REG         0x2C    //IIC从机2数据地址寄存器
#define MPU_I2CSLV2_CTRL_REG    0x2D    //IIC从机2控制寄存器
#define MPU_I2CSLV3_ADDR_REG    0x2E    //IIC从机3器件地址寄存器
#define MPU_I2CSLV3_REG         0x2F    //IIC从机3数据地址寄存器
#define MPU_I2CSLV3_CTRL_REG    0x30    //IIC从机3控制寄存器
#define MPU_I2CSLV4_ADDR_REG    0x31    //IIC从机4器件地址寄存器
#define MPU_I2CSLV4_REG         0x32    //IIC从机4数据地址寄存器
#define MPU_I2CSLV4_DO_REG      0x33    //IIC从机4写数据寄存器
#define MPU_I2CSLV4_CTRL_REG    0x34    //IIC从机4控制寄存器
#define MPU_I2CSLV4_DI_REG      0x35    //IIC从机4读数据寄存器

#define MPU_I2CMST_STA_REG      0x36    //IIC主机状态寄存器
#define MPU_INTBP_CFG_REG       0x37    //中断/旁路设置寄存器
#define MPU_INT_EN_REG          0x38    //中断使能寄存器
#define MPU_INT_STA_REG         0x3A    //中断状态寄存器

#define MPU_ACCEL_XOUTH_REG     0x3B    //加速度值,X轴高8位寄存器
#define MPU_ACCEL_XOUTL_REG     0x3C    //加速度值,X轴低8位寄存器
#define MPU_ACCEL_YOUTH_REG     0x3D    //加速度值,Y轴高8位寄存器
#define MPU_ACCEL_YOUTL_REG     0x3E    //加速度值,Y轴低8位寄存器
#define MPU_ACCEL_ZOUTH_REG     0x3F    //加速度值,Z轴高8位寄存器
#define MPU_ACCEL_ZOUTL_REG     0x40    //加速度值,Z轴低8位寄存器

#define MPU_TEMP_OUTH_REG       0x41    //温度值高8位寄存器
#define MPU_TEMP_OUTL_REG       0x42    //温度值低8位寄存器

#define MPU_GYRO_XOUTH_REG      0x43    //陀螺仪值,X轴高8位寄存器
#define MPU_GYRO_XOUTL_REG      0x44    //陀螺仪值,X轴低8位寄存器
#define MPU_GYRO_YOUTH_REG      0x45    //陀螺仪值,Y轴高8位寄存器
#define MPU_GYRO_YOUTL_REG      0x46    //陀螺仪值,Y轴低8位寄存器
#define MPU_GYRO_ZOUTH_REG      0x47    //陀螺仪值,Z轴高8位寄存器
#define MPU_GYRO_ZOUTL_REG      0x48    //陀螺仪值,Z轴低8位寄存器

#define MPU_I2CSLV0_DO_REG      0x63    //IIC从机0数据寄存器
#define MPU_I2CSLV1_DO_REG      0x64    //IIC从机1数据寄存器
#define MPU_I2CSLV2_DO_REG      0x65    //IIC从机2数据寄存器
#define MPU_I2CSLV3_DO_REG      0x66    //IIC从机3数据寄存器

#define MPU_I2CMST_DELAY_REG    0x67    //IIC主机延时管理寄存器
#define MPU_SIGPATH_RST_REG     0x68    //信号通道复位寄存器
#define MPU_MDETECT_CTRL_REG    0x69    //运动检测控制寄存器
#define MPU_USER_CTRL_REG       0x6A    //用户控制寄存器
#define MPU_PWR_MGMT1_REG       0x6B    //电源管理寄存器1
#define MPU_PWR_MGMT2_REG       0x6C    //电源管理寄存器2
#define MPU_FIFO_CNTH_REG       0x72    //FIFO计数寄存器高八位
#define MPU_FIFO_CNTL_REG       0x73    //FIFO计数寄存器低八位
#define MPU_FIFO_RW_REG         0x74    //FIFO读写寄存器
#define MPU_DEVICE_ID_REG       0x75    //器件ID寄存器0x71

#define MPU_ADDR                0x68    //ADO接GND，最低位为0
#define	GYRO_ADDRESS            0xD0    //陀螺仪地址
#define ACCEL_ADDRESS           0xD0   	//加速度计地址

#define MAG_ADDRESS             0x18   	//磁力计地址
#define AKM8963_DEVICE_ID_REG   0x00    //AKM8963器件ID寄存器  0x48
#define AKM8963_INFO_REG        0x01    //AKM8963器件信息寄存器
#define AKM8963_STATUS1_REG     0x02    //AKM8963状态寄存器1

#define AKM8963_MAG_XOUTL_REG   0x03    //AKM8963磁力计值,X轴低8位寄存器
#define AKM8963_MAG_XOUTH_REG   0x04    //AKM8963磁力计值,X轴高8位寄存器
#define AKM8963_MAG_YOUTL_REG   0x05    //AKM8963磁力计值,Y轴低8位寄存器
#define AKM8963_MAG_YOUTH_REG   0x06    //AKM8963磁力计值,Y轴高8位寄存器
#define AKM8963_MAG_ZOUTL_REG   0x07    //AKM8963磁力计值,Z轴低8位寄存器
#define AKM8963_MAG_ZOUTH_REG   0x08    //AKM8963磁力计值,Z轴高8位寄存器

#define AKM8963_STATUS2_REG     0x09    //AKM8963状态寄存器2
#define	AKM8963_CNTL1_REG       0x0A    //AKM8963控制寄存器1
#define	AKM8963_CNTL2_REG       0x0B    //AKM8963控制寄存器2
#define AKM8963_SELF_TEST_REG   0x0C    //AKM8963自检控制寄存器
#define AKM8963_TEST1_REG       0x0D    //AKM8963测试寄存器1
#define AKM8963_TEST2_REG       0x0E    //AKM8963测试寄存器2
#define AKM8963_I2C_DISABLE_REG 0x0F    //AKM8963-I2C失能寄存器
#define AKM8963_ASAX_REG        0x10    //AKM8963磁力计X轴灵敏度校正寄存器
#define AKM8963_ASAY_REG        0x11    //AKM8963磁力计Y轴灵敏度校正寄存器
#define AKM8963_ASAZ_REG        0x12    //AKM8963磁力计Z轴灵敏度校正寄存器

void MPU9250_Get_Temp(float *temp);
void MPU9250_Get_Accel(short *ax, short *ay, short *az);
void MPU9250_Get_Accel_transform(float *ax, float *ay, float *az);
void MPU9250_Get_Gyro(short *gx, short *gy, short *gz);
void MPU9250_Get_Gyro_transform(float *gx, float *gy, float *gz);
void MPU9250_Set_Heat_PWM(int duty);
int MPU9250_Init(void);

#endif

