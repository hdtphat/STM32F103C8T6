#include "stm32f10x.h"                  // Device header
#include "EXTI.h"
#include <stdint.h>



void EXTI_InitPB0(uint8_t piority){	
	// configure EXTI (0: portA, 1: portB)
	AFIO->EXTICR[0] |= 0x0001;	// pin3 ~ pin0					
	// disable  EXTI mask
	EXTI->IMR |= (1U<<0);														
	// enable Rising Edge Trigger
	EXTI->RTSR |= (1U<<1); 	
	// Disable Falling Edge Trigger	
	EXTI->FTSR &= ~(1U<<1);										
	// configure piority of ext interupt
	NVIC_SetPriority (EXTI0_IRQn, piority);
	// enable interupt
	NVIC_EnableIRQ (EXTI0_IRQn); 
}


void EXTI_InitPB1(uint8_t piority){	
	// configure EXTI (0: portA, 1: portB)
	AFIO->EXTICR[0] |= 0x0010;	// pin3 ~ pin0					
	// disable  EXTI mask
	EXTI->IMR |= (1U<<1);														
	// enable Rising Edge Trigger
	EXTI->RTSR |= (1U<<1); 	
	// Disable Falling Edge Trigger	
	EXTI->FTSR &= ~(1U<<1);										
	// configure piority of ext interupt
	NVIC_SetPriority (EXTI1_IRQn, piority);
	// enable interupt
	NVIC_EnableIRQ (EXTI1_IRQn); 
}
