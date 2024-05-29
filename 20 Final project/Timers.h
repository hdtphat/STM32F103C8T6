#ifndef TIMERS_H
#define TIMERS_H
#include <stdint.h>

void TIM6_Init(void);
void Delay_us(uint32_t us);
void Delay_ms(uint32_t ms);
void TIM4_Init(void);
void TIM2_Init(void);
void TIM2_IRQHandler(void);
void TIM3_Init(void);
void TIM3_IRQHandler(void);

#endif
