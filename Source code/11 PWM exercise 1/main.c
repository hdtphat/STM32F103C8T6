#include "stm32f10x.h"
#include <stdint.h>

/*
	create 3 chanels of PWM using timer 2, each has a ddiffferent duty cycle
*/

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
	TIM2->ARR = 500;
	TIM2->CNT = 0;
	// set chanel 2 CCR (for PWM) -> PWM as 50% of TIM2_ARR
	TIM2->CCR2 = 250; 
	// set chanel 3 CCR (for PWM) -> PWM as 40% of TIM2_ARR
	TIM2->CCR3 = 250; 
	// set chanel 4 CCR (for PWM) -> PWM as 30% of TIM2_ARR
	TIM2->CCR4 = 250; 
	// set to PWM1 mode to chanel 2, 3, 4
	TIM2->CCMR1 |= (6U<<12);
	TIM2->CCMR2 |= (6U<<1);
	TIM2->CCMR2 |= (6U<<12);
	// enable chanel 2, 3, 4
	TIM2->CCER |= (1U<<4) | (1U<<8) | (1U<<12);
	// enable counter to start counting
	TIM2->CR1 |= (1U<<0);
}
