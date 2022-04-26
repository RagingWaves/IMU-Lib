#include "task_led.h"
#include "delay.h"
#include "led.h"

#define LED_TASK_PRIO		2
#define LED_STK_SIZE 		50
TaskHandle_t LEDTask_Handler;
void led_task(void *pvParameters);

void task_led_Create(void)
{
	//创建LED任务
	xTaskCreate((TaskFunction_t )led_task,
							(const char*    )"led_task",
							(uint16_t       )LED_STK_SIZE,
							(void*          )NULL,
							(UBaseType_t    )LED_TASK_PRIO,
							(TaskHandle_t*  )&LEDTask_Handler);
}

//LED任务函数 
void led_task(void *pvParameters)
{
	//LED1=0;
	while(1)
	{
		LED1=~LED1;
		delay_ms(100);
	}
}   


