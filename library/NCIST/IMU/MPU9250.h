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


#define MPU_SELF_TESTX_REG      0x0D    //�Լ�Ĵ���X
#define MPU_SELF_TESTY_REG      0x0E    //�Լ�Ĵ���Y
#define MPU_SELF_TESTZ_REG      0x0F    //�Լ�Ĵ���Z

#define MPU_SAMPLE_RATE_REG     0x19    //����Ƶ�ʷ�Ƶ��
#define MPU_CFG_REG             0x1A    //���üĴ���
#define MPU_GYRO_CFG_REG        0x1B    //���������üĴ���
#define MPU_ACCEL_CFG_REG       0x1C    //���ٶȼ����üĴ���
#define MPU_MOTION_DET_REG      0x1F    //�˶���ֵⷧ���üĴ���
#define MPU_FIFO_EN_REG         0x23    //FIFOʹ�ܼĴ���
#define MPU_I2CMST_CTRL_REG     0x24    //IIC�������ƼĴ���
#define MPU_I2CSLV0_ADDR_REG    0x25    //IIC�ӻ�0������ַ�Ĵ���
#define MPU_I2CSLV0_REG         0x26    //IIC�ӻ�0���ݵ�ַ�Ĵ���
#define MPU_I2CSLV0_CTRL_REG    0x27    //IIC�ӻ�0���ƼĴ���
#define MPU_I2CSLV1_ADDR_REG    0x28    //IIC�ӻ�1������ַ�Ĵ���
#define MPU_I2CSLV1_REG         0x29    //IIC�ӻ�1���ݵ�ַ�Ĵ���
#define MPU_I2CSLV1_CTRL_REG    0x2A    //IIC�ӻ�1���ƼĴ���
#define MPU_I2CSLV2_ADDR_REG    0x2B    //IIC�ӻ�2������ַ�Ĵ���
#define MPU_I2CSLV2_REG         0x2C    //IIC�ӻ�2���ݵ�ַ�Ĵ���
#define MPU_I2CSLV2_CTRL_REG    0x2D    //IIC�ӻ�2���ƼĴ���
#define MPU_I2CSLV3_ADDR_REG    0x2E    //IIC�ӻ�3������ַ�Ĵ���
#define MPU_I2CSLV3_REG         0x2F    //IIC�ӻ�3���ݵ�ַ�Ĵ���
#define MPU_I2CSLV3_CTRL_REG    0x30    //IIC�ӻ�3���ƼĴ���
#define MPU_I2CSLV4_ADDR_REG    0x31    //IIC�ӻ�4������ַ�Ĵ���
#define MPU_I2CSLV4_REG         0x32    //IIC�ӻ�4���ݵ�ַ�Ĵ���
#define MPU_I2CSLV4_DO_REG      0x33    //IIC�ӻ�4д���ݼĴ���
#define MPU_I2CSLV4_CTRL_REG    0x34    //IIC�ӻ�4���ƼĴ���
#define MPU_I2CSLV4_DI_REG      0x35    //IIC�ӻ�4�����ݼĴ���

#define MPU_I2CMST_STA_REG      0x36    //IIC����״̬�Ĵ���
#define MPU_INTBP_CFG_REG       0x37    //�ж�/��·���üĴ���
#define MPU_INT_EN_REG          0x38    //�ж�ʹ�ܼĴ���
#define MPU_INT_STA_REG         0x3A    //�ж�״̬�Ĵ���

#define MPU_ACCEL_XOUTH_REG     0x3B    //���ٶ�ֵ,X���8λ�Ĵ���
#define MPU_ACCEL_XOUTL_REG     0x3C    //���ٶ�ֵ,X���8λ�Ĵ���
#define MPU_ACCEL_YOUTH_REG     0x3D    //���ٶ�ֵ,Y���8λ�Ĵ���
#define MPU_ACCEL_YOUTL_REG     0x3E    //���ٶ�ֵ,Y���8λ�Ĵ���
#define MPU_ACCEL_ZOUTH_REG     0x3F    //���ٶ�ֵ,Z���8λ�Ĵ���
#define MPU_ACCEL_ZOUTL_REG     0x40    //���ٶ�ֵ,Z���8λ�Ĵ���

#define MPU_TEMP_OUTH_REG       0x41    //�¶�ֵ��8λ�Ĵ���
#define MPU_TEMP_OUTL_REG       0x42    //�¶�ֵ��8λ�Ĵ���

#define MPU_GYRO_XOUTH_REG      0x43    //������ֵ,X���8λ�Ĵ���
#define MPU_GYRO_XOUTL_REG      0x44    //������ֵ,X���8λ�Ĵ���
#define MPU_GYRO_YOUTH_REG      0x45    //������ֵ,Y���8λ�Ĵ���
#define MPU_GYRO_YOUTL_REG      0x46    //������ֵ,Y���8λ�Ĵ���
#define MPU_GYRO_ZOUTH_REG      0x47    //������ֵ,Z���8λ�Ĵ���
#define MPU_GYRO_ZOUTL_REG      0x48    //������ֵ,Z���8λ�Ĵ���

#define MPU_I2CSLV0_DO_REG      0x63    //IIC�ӻ�0���ݼĴ���
#define MPU_I2CSLV1_DO_REG      0x64    //IIC�ӻ�1���ݼĴ���
#define MPU_I2CSLV2_DO_REG      0x65    //IIC�ӻ�2���ݼĴ���
#define MPU_I2CSLV3_DO_REG      0x66    //IIC�ӻ�3���ݼĴ���

#define MPU_I2CMST_DELAY_REG    0x67    //IIC������ʱ����Ĵ���
#define MPU_SIGPATH_RST_REG     0x68    //�ź�ͨ����λ�Ĵ���
#define MPU_MDETECT_CTRL_REG    0x69    //�˶������ƼĴ���
#define MPU_USER_CTRL_REG       0x6A    //�û����ƼĴ���
#define MPU_PWR_MGMT1_REG       0x6B    //��Դ����Ĵ���1
#define MPU_PWR_MGMT2_REG       0x6C    //��Դ����Ĵ���2
#define MPU_FIFO_CNTH_REG       0x72    //FIFO�����Ĵ����߰�λ
#define MPU_FIFO_CNTL_REG       0x73    //FIFO�����Ĵ����Ͱ�λ
#define MPU_FIFO_RW_REG         0x74    //FIFO��д�Ĵ���
#define MPU_DEVICE_ID_REG       0x75    //����ID�Ĵ���0x71

#define MPU_ADDR                0x68    //ADO��GND�����λΪ0
#define	GYRO_ADDRESS            0xD0    //�����ǵ�ַ
#define ACCEL_ADDRESS           0xD0   	//���ٶȼƵ�ַ

#define MAG_ADDRESS             0x18   	//�����Ƶ�ַ
#define AKM8963_DEVICE_ID_REG   0x00    //AKM8963����ID�Ĵ���  0x48
#define AKM8963_INFO_REG        0x01    //AKM8963������Ϣ�Ĵ���
#define AKM8963_STATUS1_REG     0x02    //AKM8963״̬�Ĵ���1

#define AKM8963_MAG_XOUTL_REG   0x03    //AKM8963������ֵ,X���8λ�Ĵ���
#define AKM8963_MAG_XOUTH_REG   0x04    //AKM8963������ֵ,X���8λ�Ĵ���
#define AKM8963_MAG_YOUTL_REG   0x05    //AKM8963������ֵ,Y���8λ�Ĵ���
#define AKM8963_MAG_YOUTH_REG   0x06    //AKM8963������ֵ,Y���8λ�Ĵ���
#define AKM8963_MAG_ZOUTL_REG   0x07    //AKM8963������ֵ,Z���8λ�Ĵ���
#define AKM8963_MAG_ZOUTH_REG   0x08    //AKM8963������ֵ,Z���8λ�Ĵ���

#define AKM8963_STATUS2_REG     0x09    //AKM8963״̬�Ĵ���2
#define	AKM8963_CNTL1_REG       0x0A    //AKM8963���ƼĴ���1
#define	AKM8963_CNTL2_REG       0x0B    //AKM8963���ƼĴ���2
#define AKM8963_SELF_TEST_REG   0x0C    //AKM8963�Լ���ƼĴ���
#define AKM8963_TEST1_REG       0x0D    //AKM8963���ԼĴ���1
#define AKM8963_TEST2_REG       0x0E    //AKM8963���ԼĴ���2
#define AKM8963_I2C_DISABLE_REG 0x0F    //AKM8963-I2Cʧ�ܼĴ���
#define AKM8963_ASAX_REG        0x10    //AKM8963������X��������У���Ĵ���
#define AKM8963_ASAY_REG        0x11    //AKM8963������Y��������У���Ĵ���
#define AKM8963_ASAZ_REG        0x12    //AKM8963������Z��������У���Ĵ���

void MPU9250_Get_Temp(float *temp);
void MPU9250_Get_Accel(short *ax, short *ay, short *az);
void MPU9250_Get_Accel_transform(float *ax, float *ay, float *az);
void MPU9250_Get_Gyro(short *gx, short *gy, short *gz);
void MPU9250_Get_Gyro_transform(float *gx, float *gy, float *gz);
void MPU9250_Set_Heat_PWM(int duty);
int MPU9250_Init(void);

#endif

