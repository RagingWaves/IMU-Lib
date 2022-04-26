#ifndef __LED_H
#define __LED_H

#include "stm32f4xx.h"
#include "sys.h"

#define LED1_GPIO_PIN	GPIO_Pin_10
#define LED1_GPIO_PORT	GPIOC
#define LED1_GPIO_CLK	RCC_AHB1Periph_GPIOC

#define LED2_GPIO_PIN	GPIO_Pin_11
#define LED2_GPIO_PORT	GPIOC
#define LED2_GPIO_CLK	RCC_AHB1Periph_GPIOC

#define LED3_GPIO_PIN	GPIO_Pin_12
#define LED3_GPIO_PORT	GPIOC
#define LED3_GPIO_CLK	RCC_AHB1Periph_GPIOC

#define LED4_GPIO_PIN	GPIO_Pin_2
#define LED4_GPIO_PORT	GPIOD
#define LED4_GPIO_CLK	RCC_AHB1Periph_GPIOD

#define LED1	PCout(10)
#define LED2	PCout(11)
#define LED3	PCout(12)
#define LED4	PDout(2)

void LED_Init(void);

#endif

