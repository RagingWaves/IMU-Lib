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

#include "IMU.h"
#include <math.h>
#include <stdio.h>

#include "MPU9250.h"
#include "stm32f4xx.h"

/**
  * @brief  获取系统时间
  * @retval 距离上次调用此函数的时间差，单位为ms
  */
static float IMU_Get_Time(void) {
	float new_time;
	new_time = TIM7->CNT;
	TIM_SetCounter(TIM7, 0);
	return (new_time/1000);
}

/**
  * @brief  IMU时钟初始化
  * @param  IMU数据结构体
  * @retval void
  */
static void IMU_Init_Timer(void) {
	TIM_TimeBaseInitTypeDef timer_initstruct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

	timer_initstruct.TIM_CounterMode = TIM_CounterMode_Up;
	timer_initstruct.TIM_Period = 499999999;
	timer_initstruct.TIM_Prescaler = 84-1;

	TIM_TimeBaseInit(TIM7, &timer_initstruct);
	
	TIM_Cmd(TIM7, ENABLE);

	TIM_SetCounter(TIM7, 0);
}

/**
  * @brief  写入校准数据
  * @param  待写入的数组
  * @param  数据长度
  * @param  数据类型，
  *         0：加速度计Accel的校准数据；
  *         1：磁力计Mag的校准数据。
  * @retval void
  */
static void IMU_Flash_Write(float *data, int size, int mode) {
	int i;
	u32 write_addr = 0x080E0000;
	while(!mode) {
		FLASH_Unlock();
		FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGAERR | FLASH_FLAG_WRPERR|FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
		FLASH_EraseSector(FLASH_Sector_11,VoltageRange_2);
		
		for(i = 0; i < size; i++) {
			FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR| FLASH_FLAG_PGSERR);
			FLASH_ProgramWord(write_addr, *((u32*)&data[i]));
			write_addr += 4;
		}
		FLASH_Lock();
		break;
	}
}

/**
  * @brief  读取校准数据
  * @param  读出的数据
  * @param  数据长度
  * @param  数据类型，
  *         0：加速度计Accel的校准数据；
  *         1：磁力计Mag的校准数据。
  * @retval void
  */
static void IMU_Flash_Read(float *data, int size, int mode) {
	int i;
	u32 read_addr = 0x080E0000;
	if(!mode)
		for(i = 0; i < size; i++) {
			data[i] = *(float *)read_addr;
			read_addr += 4;
		}
}

/**
  * @brief  读写初始化
  * @retval void
  */
static void IMU_Init_Flash(void) {
	return;
}

/**
  * @brief	获取陀螺仪原始数据
  * @param  IMU数据结构体
  * @retval void
  */
static void IMU_Get_Raw_Data(IMU_Data_t *imu_data) {
	MPU9250_Get_Gyro_transform(&imu_data->gyro_x, &imu_data->gyro_y, &imu_data->gyro_z);
	MPU9250_Get_Accel_transform(&imu_data->accel_x, &imu_data->accel_y, &imu_data->accel_z);

	/*角加速度转弧度制*/
	imu_data->gyro_x *= DEG2RAD;
	imu_data->gyro_y *= DEG2RAD;
	imu_data->gyro_z *= DEG2RAD;
	
	imu_data->accel_x *= ACCEL_GRAVITY;
	imu_data->accel_y *= ACCEL_GRAVITY;
	imu_data->accel_z *= ACCEL_GRAVITY;	
	
	imu_data->mag_x = 0;
	imu_data->mag_y = 0;
	imu_data->mag_z = 0;
}

/**
  * @brief  加速度校准
  * @param  IMU数据结构体
  * @retval void
  */
static void IMU_Calibration_Accel(IMU_Data_t *imu_data) {
	float accel_raw_data[3][1];
	float temp3x1_1_data[3][1], temp3x1_2_data[3][1];
	mat accel_raw;
	mat temp3x1_1, temp3x1_2;
	mat_init(&accel_raw, 3, 1, (float *)accel_raw_data);
	mat_init(&temp3x1_1, 3, 1, (float *)temp3x1_1_data);
	mat_init(&temp3x1_2, 3, 1, (float *)temp3x1_2_data);
	accel_raw.pData[0] = imu_data->accel_x;
	accel_raw.pData[1] = imu_data->accel_y;
	accel_raw.pData[2] = imu_data->accel_z;
	mat_mult(&imu_data->imu_calibration_data.Accel_Scale, &accel_raw, &temp3x1_1);
	mat_add(&temp3x1_1, &accel_raw, &temp3x1_2);
	imu_data->accel_x = temp3x1_2.pData[0];
	imu_data->accel_y = temp3x1_2.pData[1];
	imu_data->accel_z = temp3x1_2.pData[2];
}

/**
  * @brief  陀螺仪校准
  * @param  IMU数据结构体
  * @retval void
  */
static void IMU_Calibration_Gyro(IMU_Data_t *imu_data) {
	imu_data->gyro_x -= imu_data->imu_calibration_data.gyro_x;
	imu_data->gyro_y -= imu_data->imu_calibration_data.gyro_y;
	imu_data->gyro_z -= imu_data->imu_calibration_data.gyro_z;
}

/**
  * @brief  磁力计校准
  * @param  IMU数据结构体
  * @retval void
  */
static void IMU_Calibration_Mag(IMU_Data_t *imu_data) {
	
}

/**
  * @brief  死区函数，当绝对值小于死区阈值时返回0，否则返回原数据
  * @param  原始数据
  * @retval 计算结果
  */
static float IMU_Deadline(float value, float deadline) {
	if(fabs(value) < deadline)
		return 0;
	return value;
}

/**
  * @brief  加速度滤波
  * @param  IMU数据结构体
  * @retval void
  */
static void IMU_Filter_Accel(IMU_Data_t *imu_data) {
	imu_data->accel_x = IIR_Filter_Calc(&imu_data->imu_filter.IMU_AccelX_Filter, imu_data->accel_x);
	imu_data->accel_y = IIR_Filter_Calc(&imu_data->imu_filter.IMU_AccelY_Filter, imu_data->accel_y);
	imu_data->accel_z = IIR_Filter_Calc(&imu_data->imu_filter.IMU_AccelZ_Filter, imu_data->accel_z);
}

/**
  * @brief  陀螺仪滤波
  * @param  IMU数据结构体
  * @retval void
  */
static void IMU_Filter_Gyro(IMU_Data_t *imu_data) {
	imu_data->gyro_x = IIR_Filter_Calc(&imu_data->imu_filter.IMU_GyroX_Filter, imu_data->gyro_x);
	imu_data->gyro_y = IIR_Filter_Calc(&imu_data->imu_filter.IMU_GyroY_Filter, imu_data->gyro_y);
	imu_data->gyro_z = IIR_Filter_Calc(&imu_data->imu_filter.IMU_GyroZ_Filter, imu_data->gyro_z);
	imu_data->gyro_x = IMU_Deadline(imu_data->gyro_x, imu_data->imu_calibration_data.deadline_gyro_x);
	imu_data->gyro_y = IMU_Deadline(imu_data->gyro_y, imu_data->imu_calibration_data.deadline_gyro_y);
	imu_data->gyro_z = IMU_Deadline(imu_data->gyro_z, imu_data->imu_calibration_data.deadline_gyro_z);
}

/**
  * @brief  磁力计滤波
  * @param  IMU数据结构体
  * @retval void
  */
static void IMU_Filter_Mag(IMU_Data_t *imu_data) {
#if IMU_ENABLE_MAG
	imu_data->mag_x = SmoothFilter_Calc(&imu_data->imu_filter.IMU_MagX_Filter, imu_data->mag_x);
	imu_data->mag_y = SmoothFilter_Calc(&imu_data->imu_filter.IMU_MagY_Filter, imu_data->mag_y);
	imu_data->mag_z = SmoothFilter_Calc(&imu_data->imu_filter.IMU_MagZ_Filter, imu_data->mag_z);
#endif
}

/**
  * @brief  计算加速度校准矩阵时获取原始数据
  * @param  IMU数据结构体
  * @retval void
  */
static void IMU_Calibration_Accel_Get_Raw(IMU_Data_t *imu_data) {
	int i;
	for(i = 0; i < 200; i++) {
		IMU_Get_Raw_Data(imu_data);
		imu_data->imu_calibration_data.accel_x += imu_data->accel_x;
		imu_data->imu_calibration_data.accel_y += imu_data->accel_y;
		imu_data->imu_calibration_data.accel_z += imu_data->accel_z;
		IMU_Task_Delay_ms(3);
	}
	imu_data->imu_calibration_data.accel_x /= i;
	imu_data->imu_calibration_data.accel_y /= i;
	imu_data->imu_calibration_data.accel_z /= i;
}

/**
  * @brief  计算加速度校准矩阵
  * @param  IMU数据结构体
  * @retval void
  */
static void IMU_Calibration_Accel_Calc_CaliMat(IMU_Data_t *imu_data) {
	float Accel_X_cali_data[4][3];
	float Accel_Y_pos_data[6][3] = {
		       0      ,        0      ,  ACCEL_GRAVITY,
		       0      ,        0      , -ACCEL_GRAVITY,
		       0      ,  ACCEL_GRAVITY,        0      ,
		       0      , -ACCEL_GRAVITY,        0      ,
		 ACCEL_GRAVITY,        0      ,        0      ,
		-ACCEL_GRAVITY,        0      ,        0      ,
	};
	float Accel_W_raw_T_data[4][6];
	mat Accel_X_cali;
	mat Accel_Y_pos;
	mat Accel_W_raw, Accel_W_raw_T;
	
	float temp4x4_1_data[4][4], temp4x4_2_data[4][4];
	float temp4x6_data[4][6];
	mat temp4x4_1, temp4x4_2, temp4x6;
	
	mat_init(&Accel_X_cali,  4, 3, (float *)Accel_X_cali_data);
	mat_init(&Accel_Y_pos,   6, 3, (float *)Accel_Y_pos_data);
	mat_init(&Accel_W_raw,   6, 4, (float *)imu_data->imu_calibration_data.Accel_W_raw_data);
	mat_init(&Accel_W_raw_T, 4, 6, (float *)Accel_W_raw_T_data);
	mat_init(&temp4x4_1,     4, 4, (float *)temp4x4_1_data);
	mat_init(&temp4x4_2,     4, 4, (float *)temp4x4_2_data);
	mat_init(&temp4x6,       4, 6, (float *)temp4x6_data);
	
	while(mat_trans(&Accel_W_raw, &Accel_W_raw_T));
	while(mat_mult(&Accel_W_raw_T, &Accel_W_raw, &temp4x4_1));
	while(mat_inv(&temp4x4_1, &temp4x4_2));
	while(mat_mult(&temp4x4_2, &Accel_W_raw_T, &temp4x6));
	while(mat_mult(&temp4x6, &Accel_Y_pos, &Accel_X_cali));
	IMU_Flash_Write((float *)Accel_X_cali.pData, 12, 0);
}

/**
  * @brief  获取六面加速度数据，用于校准，详细步骤查阅文档
  * @param  IMU数据结构体
  * @retval void
  */
void IMU_Get_Accel_Cali_Data(IMU_Data_t *imu_data) {
	static int Cali_Sta = 0, Pos_N = 0;
	while(!Cali_Sta) {
		if((Pos_N > 0) && (Pos_N <= 6)) {
			IMU_Calibration_Accel_Get_Raw(imu_data);
			imu_data->imu_calibration_data.Accel_W_raw_data[Pos_N-1][0] = imu_data->imu_calibration_data.accel_x;
			imu_data->imu_calibration_data.Accel_W_raw_data[Pos_N-1][1] = imu_data->imu_calibration_data.accel_y;
			imu_data->imu_calibration_data.Accel_W_raw_data[Pos_N-1][2] = imu_data->imu_calibration_data.accel_z;
			imu_data->imu_calibration_data.Accel_W_raw_data[Pos_N-1][3] = 1;
			Pos_N = 0;
		}
		else if(Pos_N == 7) {
			IMU_Calibration_Accel_Calc_CaliMat(imu_data);
			Pos_N = 0;
		}
		IMU_Task_Delay_ms(1);
	}
}

/**
  * @brief  快速开方
  * @param  原始数据
  * @retval 开方后的倒数
  */
static float invSqrt(float x) {
  float halfx = 0.5f * x;
  float y = x;
  long i = *(long*)&y;

  i = 0x5f3759df - (i>>1);
  y = *(float*)&i;
  y = y * (1.5f - (halfx * y * y));

  return y;
}

/**
  * @brief  姿态解算，使用Mahony算法
  * @param  IMU数据结构体
  */
static void IMU_MahonyAHRSupdate(IMU_Data_t *imu_data) {
	float norm;
	float hx, hy, hz, bx, bz;
	float vx, vy, vz;
	float wx, wy, wz;
	float ex, ey, ez;
	float temp0, temp1, temp2, temp3;
	double halfT;

	float q0q0 = imu_data->q0*imu_data->q0;
	float q0q1 = imu_data->q0*imu_data->q1;
	float q0q2 = imu_data->q0*imu_data->q2;
	float q0q3 = imu_data->q0*imu_data->q3;
	float q1q1 = imu_data->q1*imu_data->q1;
	float q1q2 = imu_data->q1*imu_data->q2;
	float q1q3 = imu_data->q1*imu_data->q3;
	float q2q2 = imu_data->q2*imu_data->q2;
	float q2q3 = imu_data->q2*imu_data->q3;
	float q3q3 = imu_data->q3*imu_data->q3;
	
	halfT = IMU_Get_Time() / 2000;
	
	norm = invSqrt(imu_data->accel_x*imu_data->accel_x + imu_data->accel_y*imu_data->accel_y + imu_data->accel_z*imu_data->accel_z);
	imu_data->accel_x = imu_data->accel_x*norm;
	imu_data->accel_y = imu_data->accel_y*norm;
	imu_data->accel_z = imu_data->accel_z*norm;
	
#if IMU_ENABLE_MAG
	norm = invSqrt(imu_data->mag_x*imu_data->mag_x + imu_data->mag_y*imu_data->mag_y + imu_data->mag_z*imu_data->mag_z);          
	imu_data->mag_x = imu_data->mag_x*norm;
	imu_data->mag_y = imu_data->mag_y*norm;
	imu_data->mag_z = imu_data->mag_z*norm;
#else
	imu_data->mag_x = 0;
	imu_data->mag_y = 0;
	imu_data->mag_z = 0;
#endif
	
	// compute reference direction of flux
	hx = 2.0f*imu_data->mag_x*(0.5f - q2q2 - q3q3) + 2.0f*imu_data->mag_y*(q1q2 - q0q3) + 2.0f*imu_data->mag_z*(q1q3 + q0q2);
	hy = 2.0f*imu_data->mag_x*(q1q2 + q0q3) + 2.0f*imu_data->mag_y*(0.5f - q1q1 - q3q3) + 2.0f*imu_data->mag_z*(q2q3 - q0q1);
	hz = 2.0f*imu_data->mag_x*(q1q3 - q0q2) + 2.0f*imu_data->mag_y*(q2q3 + q0q1) + 2.0f*imu_data->mag_z*(0.5f - q1q1 - q2q2);         
	bx = sqrt((hx*hx) + (hy*hy));
	bz = hz; 	
	// estimated direction of gravity and flux (v and w)
	vx = 2.0f*(q1q3 - q0q2);
	vy = 2.0f*(q0q1 + q2q3);
	vz = q0q0 - q1q1 - q2q2 + q3q3;
	wx = 2.0f*bx*(0.5f - q2q2 - q3q3) + 2.0f*bz*(q1q3 - q0q2);
	wy = 2.0f*bx*(q1q2 - q0q3) + 2.0f*bz*(q0q1 + q2q3);
	wz = 2.0f*bx*(q0q2 + q1q3) + 2.0f*bz*(0.5f - q1q1 - q2q2);  
	// error is sum of cross product between reference direction of fields and direction measured by sensors
	ex = (imu_data->accel_y*vz - imu_data->accel_z*vy) + (imu_data->mag_y*wz - imu_data->mag_z*wy);
	ey = (imu_data->accel_z*vx - imu_data->accel_x*vz) + (imu_data->mag_z*wx - imu_data->mag_x*wz);
	ez = (imu_data->accel_x*vy - imu_data->accel_y*vx) + (imu_data->mag_x*wy - imu_data->mag_y*wx);

	if(ex != 0.0f && ey != 0.0f && ez != 0.0f) {
		imu_data->exInt = imu_data->exInt + ex * Mahony_Ki * halfT;
		imu_data->eyInt = imu_data->eyInt + ey * Mahony_Ki * halfT;	
		imu_data->ezInt = imu_data->ezInt + ez * Mahony_Ki * halfT;		
		imu_data->gyro_x = imu_data->gyro_x + Mahony_Kp*ex + imu_data->exInt;
		imu_data->gyro_y = imu_data->gyro_y + Mahony_Kp*ey + imu_data->eyInt;
		imu_data->gyro_z = imu_data->gyro_z + Mahony_Kp*ez + imu_data->ezInt;
	}
	
	// integrate quaternion rate and normalise
	temp0 = imu_data->q0 + (-imu_data->q1 * imu_data->gyro_x - imu_data->q2 * imu_data->gyro_y - imu_data->q3 * imu_data->gyro_z) * halfT;
	temp1 = imu_data->q1 + ( imu_data->q0 * imu_data->gyro_x + imu_data->q2 * imu_data->gyro_z - imu_data->q3 * imu_data->gyro_y) * halfT;
	temp2 = imu_data->q2 + ( imu_data->q0 * imu_data->gyro_y - imu_data->q1 * imu_data->gyro_z + imu_data->q3 * imu_data->gyro_x) * halfT;
	temp3 = imu_data->q3 + ( imu_data->q0 * imu_data->gyro_z + imu_data->q1 * imu_data->gyro_y - imu_data->q2 * imu_data->gyro_x) * halfT;
	
	// normalise quaternion
	norm = invSqrt(temp0*temp0 + temp1*temp1 + temp2*temp2 + temp3*temp3);
	imu_data->q0 = temp0 * norm;
	imu_data->q1 = temp1 * norm;
	imu_data->q2 = temp2 * norm;
	imu_data->q3 = temp3 * norm;

	imu_data->yaw_last = imu_data->yaw_new;
	imu_data->yaw_new = -atan2(2.0f * imu_data->q1 * imu_data->q2 + 2.0f * imu_data->q0 * imu_data->q3, -2.0f * imu_data->q2 * imu_data->q2 - 2.0f * imu_data->q3 * imu_data->q3 + 1.0f)* RAD2DEG;
	imu_data->pitch = -asin(-2.0f * imu_data->q1 * imu_data->q3 + 2.0f * imu_data->q0 * imu_data->q2)* RAD2DEG;
	imu_data->roll = atan2(2.0f * imu_data->q2 * imu_data->q3 + 2 * imu_data->q0 * imu_data->q1, -2.0f * imu_data->q1 * imu_data->q1 - 2.0f * imu_data->q2 * imu_data->q2 + 1.0f)* RAD2DEG;
}

/**
  * @brief  IMU数据校准
  * @param  IMU数据结构体
  * @retval void
  */
static void IMU_Calibration(IMU_Data_t *imu_data) {
	IMU_Calibration_Accel(imu_data);
	IMU_Calibration_Gyro(imu_data);
	IMU_Calibration_Mag(imu_data);
}

/**
  * @brief  IMU数据滤波
  * @param  IMU数据结构体
  * @retval void
  */
static void IMU_Filter(IMU_Data_t *imu_data) {
	IMU_Filter_Accel(imu_data);
	IMU_Filter_Gyro(imu_data);
	IMU_Filter_Mag(imu_data);
}

/**
  * @brief  IMU姿态解算
  * @param  IMU数据结构体
  * @retval void
  */
void IMU_Calc(IMU_Data_t *imu_data) {
	IMU_Get_Raw_Data(imu_data);
	IMU_Calibration(imu_data);
	IMU_Filter(imu_data);
	IMU_MahonyAHRSupdate(imu_data);
	if((imu_data->yaw_new - imu_data->yaw_last) >= 330) {
		imu_data->yaw_count--;
	}
	else if((imu_data->yaw_new - imu_data->yaw_last) <= -330) {
		imu_data->yaw_count++;
	}
	imu_data->yaw = imu_data->yaw_count*360 + imu_data->yaw_new;
}

/**
  * @brief  加速度校准初始化
  * @param  IMU数据结构体
  * @retval void
  */
static void IMU_Init_Calibration_Accel(IMU_Data_t *imu_data) {
	int i, j;
	float flash_read_data[4][3] = {
		1, 0, 0,
		0, 1, 0,
		0, 0, 1,
		0, 0, 0
	};
#if IMU_ENABLE_ACCEL_CALI_DATA
	IMU_Flash_Read((float *)flash_read_data, 12, 0);
#endif
	for(i = 0; i < 3; i++)
		for(j = 0; j < 3; j++)
			imu_data->imu_calibration_data.Accel_Scale_data[i][j] = flash_read_data[i][j];
	for(i = 0; i < 3; i++)
			imu_data->imu_calibration_data.Accel_Bias_data[i][0] = flash_read_data[3][i];
	mat_init(&imu_data->imu_calibration_data.Accel_Scale, 3, 3, (float *)imu_data->imu_calibration_data.Accel_Scale_data);
	mat_init(&imu_data->imu_calibration_data.Accel_Bias, 3, 3, (float *)imu_data->imu_calibration_data.Accel_Bias_data);
}

/**
  * @brief  角加速度校准初始化
  * @param  IMU数据结构体
  * @retval void
  */
static void IMU_Init_Calibration_Gyro(IMU_Data_t *imu_data) {
	int i;
	imu_data->imu_calibration_data.gyro_x = 0;
	imu_data->imu_calibration_data.gyro_y = 0;
	imu_data->imu_calibration_data.gyro_z = 0;
	imu_data->imu_calibration_data.deadline_gyro_x = GYROX_DEADLINE;
	imu_data->imu_calibration_data.deadline_gyro_y = GYROY_DEADLINE;
	imu_data->imu_calibration_data.deadline_gyro_z = GYROZ_DEADLINE;
	for(i = 0; i < 200; i++) {
		IMU_Get_Raw_Data(imu_data);
		imu_data->imu_calibration_data.gyro_x += imu_data->gyro_x;
		imu_data->imu_calibration_data.gyro_y += imu_data->gyro_y;
		imu_data->imu_calibration_data.gyro_z += imu_data->gyro_z;
		IMU_Init_Delay_ms(3);
	}
	imu_data->imu_calibration_data.gyro_x /= i;
	imu_data->imu_calibration_data.gyro_y /= i;
	imu_data->imu_calibration_data.gyro_z /= i;
}

/**
  * @brief  磁力计校准初始化
  * @param  IMU数据结构体
  * @retval void
  */
static void IMU_Init_Calibration_Mag(IMU_Data_t *imu_data) {
	
}

/**
  * @brief  IMU滤波器初始化
  * @param  IMU数据结构体
  * @retval void
  */
static void IMU_Init_Filter(IMU_Data_t *imu_data) {
	IIR_Filter_Init(&imu_data->imu_filter.IMU_GyroX_Filter);
	IIR_Filter_Init(&imu_data->imu_filter.IMU_GyroY_Filter);
	IIR_Filter_Init(&imu_data->imu_filter.IMU_GyroZ_Filter);
	IIR_Filter_Init(&imu_data->imu_filter.IMU_AccelX_Filter);
	IIR_Filter_Init(&imu_data->imu_filter.IMU_AccelY_Filter);
	IIR_Filter_Init(&imu_data->imu_filter.IMU_AccelZ_Filter);
#if IMU_ENABLE_MAG
	SmoothFilter_Init(&imu_data->imu_filter.IMU_MagX_Filter, 8);
	SmoothFilter_Init(&imu_data->imu_filter.IMU_MagY_Filter, 8);
	SmoothFilter_Init(&imu_data->imu_filter.IMU_MagZ_Filter, 8);
#endif
}

/**
  * @brief  姿态解算预运算
  * @param  IMU数据结构体
  * @retval void
  */
static void IMU_Init_Calc(IMU_Data_t *imu_data) {
	int i;
	for(i = 0; i < 10; i++) {
		IMU_Get_Raw_Data(imu_data);
		IMU_MahonyAHRSupdate(imu_data);
		IMU_Init_Delay_ms(3);
	}
	for(i = 0; i < IMU_CALC_INIT_TIME; i++) {
		IMU_Calc(imu_data);
		IMU_Init_Delay_ms(1);
	}
}

/**
  * @brief  IMU初始化
  * @param  IMU数据结构体
  * @retval void
  */
void IMU_Init(IMU_Data_t *imu_data) {
	imu_data->q0 = 1;
	imu_data->q1 = 0;
	imu_data->q2 = 0;
	imu_data->q3 = 0;
	imu_data->yaw_count = 0;
	imu_data->yaw_last  = 0;
	
	IMU_Init_Timer();
	IMU_Init_Flash();

	IMU_Init_Calibration_Gyro(imu_data);
	IMU_Init_Calibration_Accel(imu_data);
	IMU_Init_Calibration_Mag(imu_data);
	
	IMU_Init_Filter(imu_data);
	
	IMU_Init_Calc(imu_data);
}

/**
  * @brief  获取IMU数据
  * @param  IMU数据结构体
  * @param  姿态结算后的pitch数据
  * @param  姿态解算后的row数据
  * @param  姿态解算后的yaw数据
  * @retval void
  */
void IMU_Get_Data(IMU_Data_t *imu_data, float *pitch, float *roll, float *yaw) {
	IMU_Calc(imu_data);
	
	*pitch = imu_data->pitch;
	*roll  = imu_data->roll;
	*yaw   = imu_data->yaw;
}

