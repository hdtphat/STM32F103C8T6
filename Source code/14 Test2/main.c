

// Honag Doan Tien Phat _ 20146266

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "stm32f10x.h"	
#include "Delay.h"

void EXTI_Init(void);
void EXTI9_5_IRQHandler(void);
void EXTI15_10_IRQHandler(void);
void show_number(int second, int mili_second );
void TIM4_Init(void);
void TIM4_IRQHandler(void);
int mode = 0;
uint8_t BLINKING = 0;			// 0 -> no blinking
int TIME_S = 0;
int TIME_MS = 0;
uint8_t STOP_START = 0; 		// 0 -> stop
uint8_t COUNTING = 0;		// 0 -> not counting

/*=============================================================================================================*/

int main (void){
	// Enable clock for PortA, PortB and AFIO
	RCC->APB2ENR |= (1U<<0)|(1U<<2)|(1U<<3);
	// Set APB1 prescaler = 1/8 = 72/8 = 9MHz
	RCC->CFGR |= (6<<8);
	// GPIO configuration
	GPIOA->CRL =	0x44422222;		// PA7~PA1: output
	GPIOA->CRH =	0x22222222;		// PA15~PA8: output
	GPIOB->CRL =	0x44444444;		// PB6 andPB10: input
	GPIOA->ODR =	0;
	GPIOB->ODR =	0;
	// Enable interupts
	__enable_irq();
	// Initialization
	TIM4_Init();
	EXTI_Init();
	
	// main loop
	while(1){
		
	}
}

/*=============================================================================================================*/
void TIM4_Init(void){
	// enable Timer4 clock
	RCC->APB1ENR |= (1<<2); 
	// set APB1 prescaler
	RCC->CFGR |= (7<<8);     // APB1 prescaler = 16 
	// set TIM4 prescaler to 9/900 = 0.01MHz -> 1 count = 100us = 1/10000 s
	TIM4->PSC = 900-1;
	// configure timer 4
	TIM4->ARR = 10000;			// interupt every 1s
	TIM4->CNT = 0;
	// enable timer interrupt and counter
	TIM4->DIER |= (1<<0);    	// enable timer interrupr
	TIM4->CR1 |= (1<<0);		 	// disable counter 
	// Enable NVIC for timer 4
	NVIC_EnableIRQ(TIM4_IRQn);
}

void TIM4_IRQHandler(void){
	// clear interrupt flag
	TIM4->SR = 0;
	// your code
	mode = ~mode;
	if(mode == 0){ GPIOA->ODR |= 1<<0;}
	else{ GPIOA->ODR &= ~(1<<0);}
}

void EXTI_Init(void){
//	1. configure EXTI (0: portA, 1: portB)
	AFIO->EXTICR[1] = 0x0100;	// EXTI1: PB6
	AFIO->EXTICR[2] = 0x0100;	// EXTI2: PB10
//	2. disable  EXTI mask
	EXTI->IMR |= (1U<<6) | (1U<<10);		
//	3. Configure the Rising/Falling Edge Trigger
	EXTI->RTSR &= ~((1U<<6) | (1U<<10)); 													
	EXTI->FTSR |= (1U<<6) | (1U<<10);		
//	4. configure piority of ext interupt
	NVIC_SetPriority (EXTI15_10_IRQn, 1);	// PB10
	NVIC_SetPriority (EXTI9_5_IRQn, 2);		// PB6
//	5. enable interupt
	NVIC_EnableIRQ (EXTI15_10_IRQn); 
	NVIC_EnableIRQ (EXTI9_5_IRQn); 
}

void EXTI9_5_IRQHandler(void){
//	1. clear flag by writing 1 to register
	uint8_t flag = 6;
	if(EXTI->PR & (1U<<flag)){
		EXTI->PR |= (1U<<flag);
//	2. execute code
		STOP_START = ~STOP_START;
		if(STOP_START != 0){					// START:				
			TIM4->CR1 |= (1<<0);				// start counting
			BLINKING = 0;						// no blinking
			EXTI->IMR &= ~(1U<<10);		// disable button PB10
			NVIC_DisableIRQ (EXTI15_10_IRQn);
		} else {										// STOP:				
			TIM4->CR1 &= ~(1<<0);			// stop counting
			BLINKING = 1;						// blinking
			EXTI->IMR |= (1U<<10);			// enable button PB10
			NVIC_EnableIRQ (EXTI15_10_IRQn);		
		}
	}
}

void EXTI15_10_IRQHandler(void){
//	1. clear flag by writing 1 to register
	uint8_t flag = 10;
	if(EXTI->PR & (1U<<flag)){
		EXTI->PR |= (1U<<flag);
//	2. execute code
		TIME_S = 0.0;			// reset counting time
		TIME_MS = 0.0;
		BLINKING = 0;		// stop blinking
	}
}

void show_number(int second, int mili_second ){
	// array
	uint32_t display_digits[10] = {0x00FC, 0x0060, 0x00DA, 0x00F2, 0x0066, 0x00B6, 0x00BE, 0x00E0, 0x00FE, 0x00F6};
	// float to int
	int number = second*100 + mili_second;
	// create an array which contain components of input number
	uint16_t component[4];
	for (uint16_t i = 0; i < 4; i++) {
		component[i] = number / (uint16_t)pow(10, 3 - i);
		number = number % (uint16_t)pow(10, 3 - i);
	}
	// display each component into LED screen
	for(uint16_t i=0; i<4; i++){
		// reset register
		GPIOA->ODR = 0x1E;
		// choose LED to turn on
		GPIOA->ODR &= ~(1UL<<(4-i));
		// display a digit
		GPIOA->ODR |= (uint32_t)(display_digits[component[3-i]])<<8;
		// display the dot after 2nd digit from the left
		if(i == 2){
			GPIOA->ODR |= (1U<<8);
		}
		Delay(5);
	}
}

