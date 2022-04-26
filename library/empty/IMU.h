/***//****************************************
      https://github.com/RagingWaves
----------------------------------------------
 * @file        IMU.c/h
 * @dependent   1.  Matrix.c/h 矩阵运算（V1.2.1）
 *              2.  IIR_Filter.c/h IIR滤波器（V1.0.2）
 *              3.  Smooth_Filter.c/h 滑动平均滤波器（V1.0.0）
 * @brief       通用六轴陀螺仪姿态结算库
 * @mark        1.  STM32F1、STM32F4、MPU6050测试通过--22.4.8
                2.  开放IMU_Calc--22.4.9
 * @version     V1.0.1
 * @auther      ZYuan
 * @url         
----------------------------------------------
****************************************//***/

#ifndef __IMU_H
#define __IMU_H

/*以下宏定义无需修改*/
#ifndef PI
#define PI                          3.14159265358979f
#endif
#define DEG2RAD                     (PI / 180.0f)
#define RAD2DEG                     (180.0f / PI)
#define Mahony_Kp                   2.0f   // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Mahony_Ki                   0.0f   // integral gain governs rate of convergence of gyroscope biases

/*以下宏定义视情况修改*/
#define GYROX_DEADLINE              0.002
#define GYROY_DEADLINE              0.002
#define GYROZ_DEADLINE              0.002
#define IMU_Init_Delay_ms(n)        delay_ms(n)
#define IMU_Task_Delay_ms(n)        delay_ms(n)
#define ACCEL_GRAVITY               9.80665f
#define IMU_CALC_INIT_TIME          1000
#define IMU_ENABLE_MAG              0    //是否使用磁力计
#define IMU_ENABLE_ACCEL_CALI_DATA  1    //是否进行加速度校准

#if IMU_ENABLE_MAG
#include "Smooth_Filter.h"
#endif
#include "IIR_Filter.h"
#include "Matrix.h"

typedef struct {
	float gyro_x;
	float gyro_y;
	float gyro_z;
	float accel_x;
	float accel_y;
	float accel_z;
	
	float deadline_gyro_x;
	float deadline_gyro_y;
	float deadline_gyro_z;
	float Accel_W_raw_data[6][4];
	float Accel_Scale_data[3][3];
	float Accel_Bias_data[3][1];
	mat Accel_Scale;
	mat Accel_Bias;
#if IMU_ENABLE_MAG
	float mag_x;
	float mag_y;
	float mag_z;
#endif
} IMU_Calibration_Data_t;

typedef struct {
	IIR_Filter_t IMU_GyroX_Filter;
	IIR_Filter_t IMU_GyroY_Filter;
	IIR_Filter_t IMU_GyroZ_Filter;
	IIR_Filter_t IMU_AccelX_Filter;
	IIR_Filter_t IMU_AccelY_Filter;
	IIR_Filter_t IMU_AccelZ_Filter;
#if IMU_ENABLE_MAG
	SmoothFilter_t IMU_MagX_Filter;
	SmoothFilter_t IMU_MagY_Filter;
	SmoothFilter_t IMU_MagZ_Filter;
#endif
} IMU_Filter_t;

typedef struct {
	IMU_Calibration_Data_t imu_calibration_data;
	IMU_Filter_t imu_filter;
	
	float gyro_x;
	float gyro_y;
	float gyro_z;
	float accel_x;
	float accel_y;
	float accel_z;
	float mag_x;
	float mag_y;
	float mag_z;
	
	double q0;
	double q1;
	double q2;
	double q3;
	float exInt;
	float eyInt;
	float ezInt;
	
	float pitch;
	float roll;
	float yaw;
	float yaw_last;
	float yaw_new;
	float yaw_count;
} IMU_Data_t;

void IMU_Init(IMU_Data_t *imu_data);
void IMU_Get_Accel_Cali_Data(IMU_Data_t *imu_data);
void IMU_Calc(IMU_Data_t *imu_data);
void IMU_Get_Data(IMU_Data_t *imu_data, float *pitch, float *roll, float *yaw);

#endif

