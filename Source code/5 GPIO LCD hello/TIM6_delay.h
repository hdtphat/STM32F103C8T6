#ifndef TIM6_delay_H
#define TIM6_delay_H

void TIM6delay_Init(uint8_t APB1_CLK);
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);
void delay_s(uint32_t s);

#endif