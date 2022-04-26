#include "delay.h"

#if __SUPPORT_FREERTOS
#include "FreeRTOS.h"
#include "task.h"

extern void xPortSysTickHandler(void);

void SysTick_Handler(void)
{	
    if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)//系统已经运行
    {
        xPortSysTickHandler();	
    }
}
#endif


static uint8_t	fac_us = 0;
static uint32_t fac_ms = 0;

void delay_init(uint32_t SYSCLK)
{
	uint32_t reload;
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	fac_us = SYSCLK;
	reload = SYSCLK; 
	reload *= 1000000 / configTICK_RATE_HZ;
	
	fac_ms = 1000 / configTICK_RATE_HZ; 
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
    SysTick->LOAD = reload;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  
}

void delay_us(uint32_t nus)
{
    uint32_t ticks = 0;
    uint32_t told = 0;
    uint32_t tnow = 0;
    uint32_t tcnt = 0;
    uint32_t reload = 0;
    reload = SysTick->LOAD;
    ticks = nus * fac_us;
    told = SysTick->VAL;
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
                tcnt += told - tnow;
            else
                tcnt += reload - tnow + told;
            told = tnow;
            if (tcnt >= ticks)
                break;
        }
    }
}

void delay_ms(uint32_t nms)
{
	/*-----兼容FreeRTOS-----*/
	if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)//系统已经运行
	{
		if(nms >= fac_ms)
			vTaskDelay(nms/fac_ms);
		if(!(nms%=fac_ms))
			return;
	}
    delay_us((u32)(nms*1000));
}

