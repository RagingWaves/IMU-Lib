#include "IMU_task.h"
#include "MPU9250.h"
#include "Smooth_Filter.h"
#include "pid.h"

IMU_Data_t     IMU_Data;
PidTypeDef     IMU_Temp_PID;
SmoothFilter_t IMU_Temp_Filter;

//�������¶ȿ���PID
#define IMU_Temp_PID_Kp     300.0
#define IMU_Temp_PID_Ki     0.5
#define IMU_Temp_PID_Kd     0.5
#define IMU_Temp_Set        40.0    //������Ԥ���¶�

#define IMU_TASK_PRIO		30
#define IMU_STK_SIZE 		512
TaskHandle_t IMUTask_Handler;
void IMU_task(void *pvParameters);

/**
  * @brief  ������������̬��������
  * @retval void
  */
void task_IMU_Create(void) {
	xTaskCreate((TaskFunction_t )IMU_task,
	            (const char*    )"IMU_task",
	            (uint16_t       )IMU_STK_SIZE,
	            (void*          )NULL,
	            (UBaseType_t    )IMU_TASK_PRIO,
	            (TaskHandle_t*  )&IMUTask_Handler);
}

/**
  * @brief  �����������ʼ��
  * @retval void
  */
void IMU_task_Init(void) {
	float pid[3] = {IMU_Temp_PID_Kp, IMU_Temp_PID_Ki, IMU_Temp_PID_Kd};
	IMU_Init(&IMU_Data);
	SmoothFilter_Init(&IMU_Temp_Filter, 8);
	PID_Init(&IMU_Temp_PID, PID_POSITION, pid, 1000, 0);
}

/**
  * @brief  ��������̬��������
  */
void IMU_task(void *pvParameters) {
	int cali_sta = 0;
	float MPU_Temp = 0;
	while(1) {
		if(cali_sta) { //Debug�����ϵ磬������1����У׼
			cali_sta = 0;
			IMU_Get_Accel_Cali_Data(&IMU_Data);
		}
		else {
			IMU_Calc(&IMU_Data);
		}
		//������PID���µ���
		MPU9250_Get_Temp(&MPU_Temp);
		MPU_Temp = SmoothFilter_Calc(&IMU_Temp_Filter, MPU_Temp);
		MPU9250_Set_Heat_PWM((int)PID_Calc(&IMU_Temp_PID, MPU_Temp, IMU_Temp_Set));
		vTaskDelay(1);
	}
}

