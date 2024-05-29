#ifndef DELAY_H
#define DELAY_H

void SysTick_Handler(void);
void Delay(uint32_t dlyTicks);

void TIM6_Init(void);
void Delay_us(uint32_t us);
void Delay_ms(uint32_t ms);
void Delay_s(uint32_t s);

#endif
