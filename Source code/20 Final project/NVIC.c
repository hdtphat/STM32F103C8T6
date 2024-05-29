#include "stm32f10x.h"                  // Device header
#include "NVIC.h"

/*
	TIM2 -> calculate motor speed 
	TIM3 -> calculate dutty cycle based on ADC value
	EXTI4 -> PB4 get signal from encoder CHA
	EXTI9_5 -> PB5 get signal from encoder CHB
*/

void Enable_Interupt (void){
	__enable_irq();
	// config interupt piority
	NVIC_SetPriority (TIM2_IRQn, 1); 
	NVIC_SetPriority (EXTI4_IRQn, 2);	
	NVIC_SetPriority (EXTI9_5_IRQn, 2); 
	NVIC_SetPriority (TIM3_IRQn, 3); 
	// enable interupt
	NVIC_EnableIRQ(TIM2_IRQn);	
	NVIC_EnableIRQ(TIM3_IRQn);
	NVIC_EnableIRQ (EXTI4_IRQn); 
	NVIC_EnableIRQ (EXTI9_5_IRQn); 
}
