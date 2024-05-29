#include "stm32f10x.h"                  // Device header
#include "EXTI.h"
#include <stdint.h>

/*
	Read encoder with x4 mode (enable both rising/falling edge)
	EXTI4 -> PB4 get data form encoder CHA
	EXTI5 -> PB5 get data from encoder CHB
*/

void EXTI_Init(void){
	// configure EXTI (0: portA, 1: portB)
	AFIO->EXTICR[1] = 0x0011;	// EXTI1: PB4 and PB5
	// disable  EXTI mask
	EXTI->IMR |= (1U<<4) | (1U<<5);		
	// enable both Rising/Falling Edge Trigger
	EXTI->RTSR |= (1U<<4) | (1U<<5); 													
	EXTI->FTSR |= (1U<<4) | (1U<<5);		
}



