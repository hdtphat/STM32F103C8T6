#include "stm32f10x.h"                  // Device header
#include <stdint.h>

void clock_init(void);
void timer2_init(void);

int main(void){
	// enable clock and set prescalers
	clock_init();
	// setup timer 2
	timer2_init();
	
	
	while(1){}	
}

void clock_init(void){
	// enable clock to port A, AFIO and timer 2
	RCC->APB2ENR |= (1U<<2) | (1U<<0);
	RCC->APB1ENR |= (1U<<0);
	// set APB1 prescaler -> devided by 8
	RCC->CFGR |= (6U<<8);
	// set timer 2 prescaler -> devided by 18
	TIM2->PSC = 17;
}

void timer2_init(void){
	// set ARR (total count), CNT (counter)
	TIM2->ARR = 200;
	TIM2->CNT = 0;
	// set chanel 2 CCR (for PWM) -> PWM as 50% of TIM2_ARR
	TIM2->CCR2 = 100; 
	// set to PWM1 mode
	TIM2->CCMR1 |= (6U<<12);
	// enable chanel 2
	TIM2->CCER |= (1U<<4);
	// enable counter to start counting
	TIM2->CR1 |= (1U<<0);
}
