#include "sys.h"  



//取消ARM的半主机模式
#pragma import(__use_no_semihosting)        
struct __FILE{
	int handle;
};
FILE __stdout;
void _sys_exit(int x){
	x = x; 
}

//重定向printf
int fputc(int ch, FILE* stream)
{
	while((USART6->SR&0x40)==0);
	USART6->DR = (u8) ch;
	return ch;
}


__asm void WFI_SET(void)
{
    WFI;
}

__asm void INTX_DISABLE(void)
{
    CPSID I
        BX LR
}

__asm void INTX_ENABLE(void)
{
    CPSIE I
        BX LR
}

__asm void MSR_MSP(uint32_t addr)
{
    MSR MSP, r0 //set Main Stack value
                 BX r14
}

