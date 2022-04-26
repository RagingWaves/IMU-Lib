#include "sys.h"

#if __SUPPORT_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
extern void xPortSysTickHandler(void);
//OSÐèÒª
void SysTick_Handler(void)
{	
    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)
        xPortSysTickHandler();	
}
#endif

void delay_us(u32 us)
{
	uint32_t i;
	SysTick_Config(72);
	for(i=0; i<us; i++)
		while(!((SysTick->CTRL) & (1<<16)));
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void delay_ms(u32 ms)
{
	uint32_t i;
	SysTick_Config(72000);
	for(i=0; i<ms; i++)
		while(!((SysTick->CTRL) & (1<<16)));
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

