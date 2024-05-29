#include "stm32f10x.h"                  // Device header
#include <stdint.h>

void TIM2_IRQHandler(void);
void delay_ms(uint32_t ms);
uint8_t LED_mode = 0x00;

int main (void){
	// enable clock to portA, portB, AFIO
	RCC->APB2ENR |= (1U<<0) | (1U<<2) | (1U<<3);
	// Set APB1 prescaler to 1/8 => clock before timer = 72/8x2 = 18MHz
	RCC->CFGR |= (6<<8);
	
	// set PA1 as output
	GPIOA->CRL = 0x44444424;
	GPIOA->ODR = 0;
	GPIOA->ODR |= 1<<1;
	
//	// enable clock to TIM4
//	RCC->APB1ENR |= 1<<2;
//	// set TIM4 prescaler to 1/180 => clock to timer = 18/180 = 0.1MHz => 1 count cost 10us => 50000 count = 500ms
//	TIM4->PSC = 180-1;
//	// config max count value = 50000 => timer interrupt every 500ms
//	TIM4->ARR = 50000;
//	// reset counter
//	TIM4->CNT = 0;
//	// enable timer interrupt
//	TIM4->DIER |= (1<<0);
//	// Enable NVIC for timer 4
//	__enable_irq();
//	NVIC_EnableIRQ(TIM4_IRQn);	
//	// enable counter 
//	TIM4->CR1 |= (1<<0);	


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
	NVIC_EnableIRQ(TIM2_IRQn);	
	// enable counter 
	TIM2->CR1 |= (1<<0);	
	
	while(1){
		
	}
}

void TIM2_IRQHandler(void){
	// clear interrupt flag
	TIM2->SR = 0;
	// execute code 
	LED_mode = ~LED_mode;
	if(LED_mode == 0){
			GPIOA->ODR |= 1<<1;
		} else {
			GPIOA->ODR &= ~(1U<<1);
		}
}
