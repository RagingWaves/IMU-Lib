#include "sys.h"
#include "delay.h"
#include "led.h"
#include "task_led.h"
#include "usart.h"

#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"

#include "IMU_task.h"


#define START_TASK_PRIO		1				//任务优先级
#define START_STK_SIZE 		128  			//任务堆栈大小
TaskHandle_t StartTask_Handler;				//任务句柄
void start_task(void *pvParameters);		//任务函数
void TaskStart(void);

void Init(void)
{
	USART6_Init(115200);
	printf("test\r\n");
	delay_init(168);
	LED_Init();
	
	while(MPU9250_Init());
	IMU_task_Init();
	
	delay_ms(1000);
}

int main(void)
{	
	Init();
	TaskStart();
}

void TaskStart(void)
{
	//创建开始任务
	xTaskCreate((TaskFunction_t )start_task,            //任务函数
							(const char*    )"start_task",          //任务名称
							(uint16_t       )START_STK_SIZE,        //任务堆栈大小
							(void*          )NULL,                  //传递给任务函数的参数
							(UBaseType_t    )START_TASK_PRIO,       //任务优先级
							(TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
	vTaskStartScheduler();          //开启任务调度
}

//开始任务任务函数
void start_task(void *pvParameters)
{
	taskENTER_CRITICAL();           //进入临界区
	
	task_led_Create();
	task_IMU_Create();
	
	vTaskDelete(StartTask_Handler); //删除开始任务
	taskEXIT_CRITICAL();            //退出临界区
}

