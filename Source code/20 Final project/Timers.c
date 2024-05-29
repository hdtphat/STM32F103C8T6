#include "stm32f10x.h"                  // Device header
#include "Timers.h"
#include <stdint.h>

/*
	APB1 clock PCLK1 = 9MHz

	TIM6 -> delay functions
	Ftim6 = 1MHz, ARR = 65535, no interupt
	
	TIM4 -> PWM CH1 with PB6 
	Ftim4 = 1MHz, ARR = 20000, no interupt
	
	TIM2 -> get motor speed every 100ms
	Ftim2 = 0.5MHz, ARR = 50000, interupt every 100ms
*/

void TIM6_Init(void){
	// enable clock to TIM6
	RCC->APB1ENR |= (1U<<4);
	// set TIM4 prescaler to 1/18 -> Ftim6 = 1MHz
	TIM6->PSC = 18-1;
	// config max counting value
	TIM6->ARR = 0xFFFF;
	// reset counter
	TIM6->CNT = 0;
	// enable counter 
	TIM6->CR1 |= (1U<<0);	
}

void Delay_us(uint32_t us){
	TIM6->CNT = 0;
	while(TIM6->CNT <us){}
}

void Delay_ms(uint32_t ms){
	for(uint32_t i=0; i<ms; i++) Delay_us(1000);
}

void TIM4_Init(void){
	// enable clock to TIM4
	RCC->APB1ENR |= (1U<<2);
	// Set TIM4 prescaler to 1/18 -> Ftim4 = 1MHz
	TIM4->PSC = 18-1;     
	// Set ARR, CNT, CCR
	TIM4->ARR = 20000;
	// reset counter
	TIM4->CNT = 0;
	// config dutty cycle
	TIM4->CCR1 = 1000; // dutty cycle 5%
	// Set PWM1 mode1 to CH1
	TIM4->CCMR1 |= (6U<<4);
	// Enable CH1
	TIM4->CCER |= (1U<<0);
	// Enable counter
	TIM4->CR1 |= (1U<<0);
}

void TIM2_Init(void){
	// enable clock to TIM2
	RCC->APB1ENR |= (1U<<0);
	// set TIM2 prescaler to 1/36 -> Ftim2 = 0.5MHz
	TIM2->PSC = 36-1;
	// config max count value -> interupt every 100ms
	TIM2->ARR = 50000;
	// reset counter
	TIM2->CNT = 0;
	// enable timer interrupt
	TIM2->DIER |= (1U<<0);
	// enable counter 
	TIM2->CR1 |= (1<<0);	
}

void TIM3_Init(void){
	// enable clock to TIM3
	RCC->APB1ENR |= (1U<<1);
	// set TIM3 prescaler to 1/18 -> Ftim3 = 1MHz
	TIM3->PSC = 18-1;
	// config max count value -> interupt every 10ms
	TIM3->ARR = 10000;
	// reset counter
	TIM3->CNT = 0;
	// enable timer interrupt
	TIM3->DIER |= (1U<<0);	
	// enable counter 
	TIM3->CR1 |= (1<<0);	
}
