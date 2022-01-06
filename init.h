#ifndef INIT_H_
#define INIT_H_

#include "main.h"

#define HAL_MAX_DELAY      0xFFFFFFFFU

void Init_GPIO(void);
void Init_Clock(void);
void SYSTimer_Init(void);
uint32_t HAL_GetTick_us(void);
uint32_t HAL_GetTick_ms(void);
//void HAL_Delay_ms(uint32_t Delay_ms);
void HAL_Delay_us(uint32_t Delay_us);

#endif // INIT_H_


