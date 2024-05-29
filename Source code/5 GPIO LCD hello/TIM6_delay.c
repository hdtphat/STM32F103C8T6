#include "stm32f10x.h"                  // Device header
#include "TIM6_delay.h"
#include <stdint.h>

void TIM6delay_Init(uint8_t APB1_CLK){
/*
	APB1_clk = System_clk / APB1_prescaler
	
	if APB1 prescaler is not divided by 1:
	clock before TIM6 prescaler = 2 * APB1_clk 
	else: clock before TIM6 prescaler = APB1_clk 
	
	clock after TIM6 prescaler = clock before TIM6 prescaler / TIM6 prescaler
*/
	// enable clock to AFIO
	RCC->APB2ENR |= (1U<<0);
	// enable clock to TIM6
	RCC->APB1ENR |= (1U<<4);
	// set TIM6 prescaler to 1/(2*APB1_CLK) => TIM6 clock = (2*APB1_CLK)/(2*APB1_CLK) = 1MHz
	// 1 count cost 1us => 1000 count = 1000us = 1ms
	TIM6->PSC = (APB1_CLK*2)-1;
	// config max counting value
	TIM6->ARR = 0xFFFF;
	// reset counter
	TIM6->CNT = 0;
	// enable counter 
	TIM6->CR1 |= (1<<0);	
}

void delay_us(uint32_t us){
	// reset counter
	TIM6->CNT = 0;
	// wait for counter to reach the setpoint
	while(TIM6->CNT <us){}
}

void delay_ms(uint32_t ms){
	for(uint32_t i=0; i<ms; i++){
		// delay 1000us = 1ms
		delay_us(1000); 
	}
}

void delay_s(uint32_t s){
	for(uint32_t i=0; i<s; i++){
		// delay 1000ms = 1s
		delay_ms(1000); 
	}
}
