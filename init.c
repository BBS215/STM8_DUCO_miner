#include "init.h"

volatile uint32_t g_uwTick;

void Init_GPIO(void)
{
	GPIO_DeInit(GPIOA);
	GPIO_DeInit(GPIOB);
	GPIO_DeInit(GPIOC);
	GPIO_DeInit(GPIOD);
}

void Init_Clock(void)
{
	CLK->CKDIVR = 0; // HSIDIV = 0; CPUDIV = 0; 16 MHz HSI
}

void SYSTimer_Init(void)
{
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, ENABLE);
	TIM2_DeInit();
	TIM2_TimeBaseInit(TIM2_PRESCALER_16, 1000); // 16 MHz / 16 / 1000 = 1000 Hz
	TIM2_ClearFlag(TIM2_FLAG_UPDATE);
	TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
	TIM2_Cmd(ENABLE);
}

uint32_t HAL_GetTick_us(void)
{
    uint16_t tmpcntr = 0;
    tmpcntr = ((uint16_t)TIM2->CNTRH << 8);
  return (g_uwTick + (uint32_t)(tmpcntr | (uint16_t)(TIM2->CNTRL)));
}

uint32_t HAL_GetTick_ms(void)
{
  return (g_uwTick / 1000);
}
/*
void HAL_Delay_ms(uint32_t Delay_ms)
{
    uint32_t tickstart = HAL_GetTick_ms();
    Delay_ms++; // Add a period to guaranty minimum wait

  while((HAL_GetTick_ms() - tickstart) < Delay_ms) { nop(); }
}
*/
void HAL_Delay_us(uint32_t Delay_us)
{
    uint32_t tickstart = HAL_GetTick_us();
    Delay_us++; // Add a period to guaranty minimum wait

  while((HAL_GetTick_us() - tickstart) < Delay_us) { nop(); }
}

