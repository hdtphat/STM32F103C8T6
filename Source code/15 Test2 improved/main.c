/*
||	Hoang Doan Tien Phat _ 20146266
||
||	PA1~PA4: output - LED cathode
||	PA8~PA15: output - LED anode
||	PB6: input - start/stop button
||	PB10: input - reset button
||
||	TIM4 is used for counting - interrupt every 10ms
||	TIM3 is used for blinking - interrupt every 250ms
||	EXTI6 -> PB6 start/stop button
||	EXTI10 -> PB10 reset button
*/
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
void TIM3_Init(void);
void TIM3_IRQHandler(void);

uint8_t TIME_S = 0;
uint8_t TIME_MS = 0;
uint8_t BLINKING = 0; // 0 -> no blinking
uint8_t STOP_START = 0; // 0 -> stop
uint8_t COUNTING = 0; // 0 -> not counting
uint8_t BLINK_MODE = 0; // 0x00 -> LED on, 0xFF -> LED off

/*=============================================================================================================*/

int main (void){
	// Enable clock for PortA, PortB and AFIO
	RCC->APB2ENR |= (1U<<0)|(1U<<2)|(1U<<3);
	// Set APB1 prescaler = 1/8 = 72/8 = 9MHz
	RCC->CFGR |= (6<<8);
	// GPIO configuration
	GPIOA->CRL =	0x44422224; // PA4~PA1: output
	GPIOA->CRH = 0x22222222; // PA15~PA8: output
	GPIOB->CRL =	0x44444444; // PB6 and PB10: input
	GPIOA->ODR = 0;
	GPIOB->ODR = 0;
	// Enable interupts
	__enable_irq();
	// Initialization
	TIM3_Init();
	TIM4_Init();
	SysTick_Config(9000);
	EXTI_Init();
	
	// main loop
	while(1){
		if(BLINKING == 0){
			show_number(TIME_S, TIME_MS);
		} else {
			switch (BLINK_MODE){
				case 0x00: // LED on
					show_number(TIME_S, TIME_MS);
					break;
				case 0xFF: // LED off
					GPIOA->ODR = 0;
					break;
			}
		}
	}
}

/*=============================================================================================================*/

void TIM3_Init(void){
	// enable Timer3 clock
	RCC->APB1ENR |= (1U<<1); 
	// set APB1 prescaler
	RCC->CFGR |= (6U<<8);     // APB1 prescaler = 8
	// set TIM3 prescaler to 1/18 => clock to timer = (9x2)/18 = 1MHz => 1 count cost 1us => 1000 count = 10ms
	TIM3->PSC = 18-1;
	// configure timer 3
	TIM3->ARR = 25000;	// 1000 count = 10ms => interupt every 250ms
	TIM3->CNT = 0;
	// enable timer interrupt and counter
	TIM3->DIER |= (1U<<0); // enable timer interrupr
	TIM3->CR1 |= (1U<<0); // disable counter 
	// set interrupt piority
	NVIC_SetPriority (TIM3_IRQn, 4); 
	// Enable NVIC for timer 4
	NVIC_EnableIRQ(TIM3_IRQn);
}

void TIM3_IRQHandler(void){
	// clear interrupt flag
	TIM3->SR = 0;
	// your code
	BLINK_MODE = ~BLINK_MODE;
}

void TIM4_Init(void){
	// enable Timer4 clock
	RCC->APB1ENR |= (1U<<2); 
	// set APB1 prescaler
	RCC->CFGR |= (6U<<8); // APB1 prescaler = 8
	// set TIM4 prescaler to 1/18 => clock to timer = (9x2)/18 = 1MHz => 1 count cost 1us => 1000 count = 10ms
	TIM4->PSC = 18-1;
	// configure timer 4
	TIM4->ARR = 1000; // interupt every 10ms
	TIM4->CNT = 0;
	// enable timer interrupt and counter
	TIM4->DIER |= (1U<<0); // enable timer interrupr
	TIM4->CR1 &= ~(1U<<0); // disable counter 
	// set interrupt piority
	NVIC_SetPriority (TIM4_IRQn, 3); 
	// Enable NVIC for timer 4
	NVIC_EnableIRQ(TIM4_IRQn);
}

void TIM4_IRQHandler(void){
	// clear interrupt flag
	TIM4->SR = 0;
	// your code
	TIME_MS += 1;
	if(TIME_MS > 99){
		TIME_MS = 0;
		TIME_S += 1;}
	if(TIME_S > 99){
		TIME_S = 0;}
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
	NVIC_SetPriority (EXTI9_5_IRQn, 2); // PB6
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
		} else {									// STOP:				
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
