#include "sys.h"
#include "delay.h"
#include "led.h"
#include "task_led.h"
#include "usart.h"

#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"

#include "IMU_task.h"


#define START_TASK_PRIO		1				//�������ȼ�
#define START_STK_SIZE 		128  			//�����ջ��С
TaskHandle_t StartTask_Handler;				//������
void start_task(void *pvParameters);		//������
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
	//������ʼ����
	xTaskCreate((TaskFunction_t )start_task,            //������
							(const char*    )"start_task",          //��������
							(uint16_t       )START_STK_SIZE,        //�����ջ��С
							(void*          )NULL,                  //���ݸ��������Ĳ���
							(UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
							(TaskHandle_t*  )&StartTask_Handler);   //������              
	vTaskStartScheduler();          //�����������
}

//��ʼ����������
void start_task(void *pvParameters)
{
	taskENTER_CRITICAL();           //�����ٽ���
	
	task_led_Create();
	task_IMU_Create();
	
	vTaskDelete(StartTask_Handler); //ɾ����ʼ����
	taskEXIT_CRITICAL();            //�˳��ٽ���
}

