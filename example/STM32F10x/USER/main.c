#include "sys.h"
#include "MPU6050.h"
#include "usart.h"
#include "IMU.h"

IMU_Data_t IMU_Data;

float p, r, y;

void Init(void)
{
	uart_init(115200);
	printf("\r\nIMU test\r\n");
	printf("Initializing...\r\n");
	MPU6050_Init();
	IMU_Init(&IMU_Data);
	printf("Start￡o\r\n");
	delay_ms(500);
}

int main(void)
{
	Init();
	while(1)
	{
		//方法1
		IMU_Get_Data(&IMU_Data, &p, &r, &y);
		printf("pitch:%.5f\traw:%.5f\tyaw%.5f\t\r\n", p, r, y);
		delay_ms(1);
		
		//方法2
		IMU_Calc(&IMU_Data);
		printf("pitch:%.5f\traw:%.5f\tyaw%.5f\t\r\n", IMU_Data.pitch, IMU_Data.roll, IMU_Data.yaw);
		delay_ms(1);
	}
}

