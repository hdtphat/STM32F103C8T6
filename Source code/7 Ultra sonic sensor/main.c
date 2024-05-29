#include "stm32f10x.h"
#include <stdint.h>

uint8_t echo = 0;

void EXTI4_IRQHandler(void){
	EXTI->PR |= (1U<<4);
	if(echo == 0){
		// reset counter value
		TIM4->CNT = 0;
		// enable tim4
		TIM4->CR1 |= (1U<<1);
	} else {
		// disable tim4
		TIM4->CR1 &= ~(1U<<1);
		// calculate distance
		uint16_t timer_value = TIM4->CNT;
		
	}
	echo= ~echo;
}

int main (void){
	// enable clock to port B and AFIO
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPBEN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	// set pb4 as input and pb5 as output
	GPIOB->CRL= 0x44244444;
	// configure EXTI 
	AFIO->EXTICR[1] |= (1U<<0);			// PB4 is selected as source for EXTI
	EXTI->IMR |= (1U<<1);					// enable interupt for EXTI4
	EXTI->RTSR |= (1U<<4);					// detect rising edge
	EXTI->FTSR |= (1U<<4);					// detect falling edge
	NVIC_EnableIRQ(EXTI4_IRQn);
	// configure Timer 4
	RCC->CFGR |= (6<<8);     				// APB1 prescaler = 8
	TIM4->PSC = 35;          					// TIM4 prescaler = 36, TIM4 clock: 0.5MHz
	TIM4->ARR = 65535;
	TIM4->CNT = 0;
	// LCD init
	
	
	while(1){}
}
