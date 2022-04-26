#ifndef __IMU_TASK_H
#define __IMU_TASK_H

#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"

#include "IMU.h"
#include "MPU9250.h"

extern IMU_Data_t IMU_Data;

void task_IMU_Create(void);
void IMU_task_Init(void);

#endif

