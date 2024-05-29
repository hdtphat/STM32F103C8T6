#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "stm32f10x.h"	
#include "Delay.h"

void EXTI_Init(void);
void EXTI0_IRQHandler(void);
void EXTI9_5_IRQHandler(void);

uint8_t STOP_START = 0;
uint8_t ON_BLINK = 0;
uint8_t NEW_ROLL = 0;
int RandomNumber = 0;

/*=============================================================================================================*/

int main (void){
	// Enable clock for PortA, PortB and AFIO
	RCC->APB2ENR |= (1U<<0)|(1U<<2)|(1U<<3);
	// Set APB1 prescaler = 1/8 = 72/8 = 9MHz
	RCC->CFGR |= (6<<8);
	// GPIO configuration
	GPIOA->CRL =	0x22222224;		// PA7~PA1: LED
	GPIOB->CRL =	0x44444444;		// PB1~PB0: Button
	GPIOA->ODR =	0;
	GPIOB->ODR =	0;
	// Init systick timer
	SysTick_Config(9000);
	// Enable interupts
	__enable_irq();
	// Initialization
	EXTI_Init();
	
	// main loop
	while(1){
		if(ON_BLINK != 0){
			GPIOA->ODR = 0xFF;
			Delay(1000);
			GPIOA->ODR = 0;
			ON_BLINK = 0;
		}
		if(NEW_ROLL != 0){
			switch (RandomNumber){
				case 1:
					GPIOA->ODR |= (1U<<4);
					Delay(1000);
					GPIOA->ODR = 0;
					NEW_ROLL = 0;
					break;
				case 2:
					GPIOA->ODR |= (1U<<3) | (1U<<5);
					Delay(1000);
					GPIOA->ODR = 0;
					NEW_ROLL = 0;
					break;
				case 3:
					GPIOA->ODR |= (1U<<3) | (1U<<5) | (1U<<4);
					Delay(1000);
					GPIOA->ODR = 0;
					NEW_ROLL = 0;
					break;
				case 4:
					GPIOA->ODR |= (1U<<3) | (1U<<5) | (1U<<1) | (1U<<7);
					Delay(1000);
					GPIOA->ODR = 0;
					NEW_ROLL = 0;
					break;
				case 5:
					GPIOA->ODR |= (1U<<3) | (1U<<5) | (1U<<1) | (1U<<7) | (1U<<4);
					Delay(1000);
					GPIOA->ODR = 0;
					NEW_ROLL = 0;
					break;
				case 6:
					for(int i=0; i<3; i++){
						GPIOA->ODR |= (1U<<3) | (1U<<5) | (1U<<1) | (1U<<7) | (1U<<2) | (1U<<6);
						Delay(333);
						GPIOA->ODR = 0;
						Delay(333);
					}
					NEW_ROLL = 0;
					break;
			}
		}
	}
}

/*=============================================================================================================*/

void EXTI_Init(void){
//	1. configure EXTI (0: portA, 1: portB)
	AFIO->EXTICR[0] = 0x0011;	// EXTI0: PB0
	AFIO->EXTICR[1] = 0x1000;	// EXTI1: PB7	
//	2. disable  EXTI mask
	EXTI->IMR |= (1U<<7) | (1U<<0);		
//	3. Configure the Rising/Falling Edge Trigger
	EXTI->RTSR &= ~((1U<<7) | (1U<<0)); 													
	EXTI->FTSR |= (1U<<7) | (1U<<0);		
//	4. configure piority of ext interupt
	NVIC_SetPriority (EXTI0_IRQn, 1);
	NVIC_SetPriority (EXTI9_5_IRQn, 2);
//	5. enable interupt
	NVIC_EnableIRQ (EXTI0_IRQn); 
	NVIC_EnableIRQ (EXTI9_5_IRQn); 
}

void EXTI0_IRQHandler(void){
//	1. clear flag by writing 1 to register
	uint8_t flag = 0;
	if(EXTI->PR & (1U<<flag)){
		EXTI->PR |= (1U<<flag);}
//	2. execute code
	STOP_START = ~STOP_START;
	if(STOP_START != 0){					// STOP:
		GPIOA->ODR = 0;					// turn off all led
		EXTI->IMR &= ~(1U<<7);		// disable button PB1
		NVIC_DisableIRQ (EXTI9_5_IRQn);
	} else {										// START:
		ON_BLINK = ~ON_BLINK;			// blink all led 1 time
		EXTI->IMR |= (1U<<7);			// enable button PB1
		NVIC_EnableIRQ (EXTI9_5_IRQn);
		
	}
}

void EXTI9_5_IRQHandler(void){
//	1. clear flag by writing 1 to register
	uint8_t flag = 7;
	if(EXTI->PR & (1U<<flag)){
		EXTI->PR |= (1U<<flag);
//	2. execute code
	RandomNumber = (rand() % 6) + 1;
	NEW_ROLL = ~NEW_ROLL;
	}
}
