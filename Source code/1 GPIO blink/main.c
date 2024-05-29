#include "stm32f10x.h"                  // Device header
#include <stdint.h>

void delay(uint32_t dlyTicks);
/* Counts 1ms timeTicks */
volatile uint32_t msTicks = 0;
void SysTick_Handler(void);

int main(void){
	// enable clock for Port B
	RCC->APB2ENR |= (1<<3) | (1<<2);
	// setup PB15 as output
	GPIOB->CRH = 0x24444444;
	GPIOA->CRL = 0x44444222;
	// turn off all led
	GPIOA->ODR = 0;
	// set systick timer, interrupt each 1ms
	SysTick_Config(9000);
	
	// main loop
	while(1){
		GPIOA->ODR ^= 0x02;
		delay(1000);
		
	}
}

void SysTick_Handler(void){
	 /* Increment counter necessary in Delay()*/
		msTicks++;
}
 
void delay(uint32_t dlyTicks){
      uint32_t curTicks;
      curTicks = msTicks;
      while ((msTicks - curTicks) < dlyTicks) ;
}