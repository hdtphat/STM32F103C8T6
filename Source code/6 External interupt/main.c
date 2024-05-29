#include "stm32f10x.h"                  // Device header
#include <stdint.h>

void EXTI1_IRQHandler(void);
void EXTI3_IRQHandler(void);
uint8_t count = 0xFF;
int test = 0;
// button1 PB1
// button2 PB3
// LED PA7

int main (void){
	// enable clock to port A, port B and AFIO
	RCC->APB2ENR |= (1U<<3) | (1U<<2) | (1U<<0);
	// set PB3 as input, PA7 as output
	GPIOB->CRL = 0x44444444;
	GPIOA->CRL = 0x24444444;
	GPIOB->ODR = 0;
	GPIOA->ODR = 0;
	
	// configure EXTI (0: portA, 1: portB) => PB3
	AFIO->EXTICR[0] = 0x1010;	// pin3 ~ pin0					
	// disable  EXTI mask
	EXTI->IMR |= (1U<<3) | (1U<<1);														
	// Configure the Rising/Falling Edge Trigger
	EXTI->RTSR |= (1U<<3) | (1U<<1); 													
	EXTI->FTSR &= ~((1U<<3) | (1U<<1));										
	// configure piority of ext interupt
	NVIC_SetPriority (EXTI3_IRQn, 1);
	NVIC_SetPriority (EXTI1_IRQn, 2);
	// enable interupt
	NVIC_EnableIRQ (EXTI3_IRQn); 
	NVIC_EnableIRQ (EXTI1_IRQn); 
	
	while(1){
	}
}

void EXTI3_IRQHandler(void){
	// clear flag by writing 1 to register
	if(EXTI->PR & (1U<<3)){
		EXTI->PR |= (1U<<3);
	}
	// execute code
	GPIOA->ODR |= 1<<7;	// LED on
	test++;
}

void EXTI1_IRQHandler(void){
	// clear flag by writing 1 to register
	if(EXTI->PR & (1U<<1)){
		EXTI->PR |= (1U<<1);
	}
	// execute code
	GPIOA->ODR &= ~(1U<<7);	// LED off
}
