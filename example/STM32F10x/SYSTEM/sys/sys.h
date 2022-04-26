/**
	*
	*	编写：RagingWaves
	*
	*	编写日期：2021/3/8
	*
	*/

#ifndef __SYS_H 
#define __SYS_H

#define __SUPPORT_FREERTOS		1

#include "stm32f10x.h"

//IO口地址映射
#define GPIOA_ODR_Addr (GPIOA_BASE + 0x0C) //0x4001080C
#define GPIOB_ODR_Addr (GPIOB_BASE + 0x0C) //0x40010C0C
#define GPIOC_ODR_Addr (GPIOC_BASE + 0x0C) //0x4001100C
#define GPIOD_ODR_Addr (GPIOD_BASE + 0x0C) //0x4001140C
#define GPIOE_ODR_Addr (GPIOE_BASE + 0x0C) //0x4001180C
#define GPIOF_ODR_Addr (GPIOF_BASE + 0x0C) //0x40011A0C
#define GPIOG_ODR_Addr (GPIOG_BASE + 0x0C) //0x40011E0C
#define GPIOA_IDR_Addr (GPIOA_BASE + 0x08) //0x40010808
#define GPIOB_IDR_Addr (GPIOB_BASE + 0x08) //0x40010C08
#define GPIOC_IDR_Addr (GPIOC_BASE + 0x08) //0x40011008
#define GPIOD_IDR_Addr (GPIOD_BASE + 0x08) //0x40011408
#define GPIOE_IDR_Addr (GPIOE_BASE + 0x08) //0x40011808
#define GPIOF_IDR_Addr (GPIOF_BASE + 0x08) //0x40011A08
#define GPIOG_IDR_Addr (GPIOG_BASE + 0x08) //0x40011E08

//IO口操作
#define PAout(n)   *(unsigned int*)((GPIOA_ODR_Addr & 0xF0000000)+0x02000000+((GPIOA_ODR_Addr &0x00FFFFF)<<5)+(n<<2))  //输出
#define PAin(n)    *(unsigned int*)((GPIOA_IDR_Addr & 0xF0000000)+0x02000000+((GPIOA_IDR_Addr &0x00FFFFF)<<5)+(n<<2))  //输入
#define PBout(n)   *(unsigned int*)((GPIOB_ODR_Addr & 0xF0000000)+0x02000000+((GPIOB_ODR_Addr &0x00FFFFF)<<5)+(n<<2))  //输出
#define PBin(n)    *(unsigned int*)((GPIOB_IDR_Addr & 0xF0000000)+0x02000000+((GPIOB_IDR_Addr &0x00FFFFF)<<5)+(n<<2))  //输入
#define PCout(n)   *(unsigned int*)((GPIOC_ODR_Addr & 0xF0000000)+0x02000000+((GPIOC_ODR_Addr &0x00FFFFF)<<5)+(n<<2))  //输出
#define PCin(n)    *(unsigned int*)((GPIOC_IDR_Addr & 0xF0000000)+0x02000000+((GPIOC_IDR_Addr &0x00FFFFF)<<5)+(n<<2))  //输入
#define PDout(n)   *(unsigned int*)((GPIOD_ODR_Addr & 0xF0000000)+0x02000000+((GPIOD_ODR_Addr &0x00FFFFF)<<5)+(n<<2))  //输出
#define PDin(n)    *(unsigned int*)((GPIOD_IDR_Addr & 0xF0000000)+0x02000000+((GPIOD_IDR_Addr &0x00FFFFF)<<5)+(n<<2))  //输入
#define PEout(n)   *(unsigned int*)((GPIOE_ODR_Addr & 0xF0000000)+0x02000000+((GPIOE_ODR_Addr &0x00FFFFF)<<5)+(n<<2))  //输出
#define PEin(n)    *(unsigned int*)((GPIOE_IDR_Addr & 0xF0000000)+0x02000000+((GPIOE_IDR_Addr &0x00FFFFF)<<5)+(n<<2))  //输入
#define PFout(n)   *(unsigned int*)((GPIOF_ODR_Addr & 0xF0000000)+0x02000000+((GPIOF_ODR_Addr &0x00FFFFF)<<5)+(n<<2))  //输出
#define PFin(n)    *(unsigned int*)((GPIOF_IDR_Addr & 0xF0000000)+0x02000000+((GPIOF_IDR_Addr &0x00FFFFF)<<5)+(n<<2))  //输入
#define PGout(n)   *(unsigned int*)((GPIOG_ODR_Addr & 0xF0000000)+0x02000000+((GPIOG_ODR_Addr &0x00FFFFF)<<5)+(n<<2))  //输出
#define PGin(n)    *(unsigned int*)((GPIOG_IDR_Addr & 0xF0000000)+0x02000000+((GPIOG_IDR_Addr &0x00FFFFF)<<5)+(n<<2))  //输入

#define Write		0x00
#define Read		0x01

//软件延时函数
void delay_us(u32 us);
void delay_ms(u32 ms);

#endif

