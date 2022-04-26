#ifndef __DELAY_H
#define __DELAY_H

#include "sys.h"

extern void delay_init(uint32_t SYSCLK);
extern void delay_us(uint32_t nus);
extern void delay_ms(uint32_t nms);

#endif

