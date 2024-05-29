#include "stm32f10x.h"                  // Device header
#include <stdint.h>
#include <math.h>

void delay(uint32_t dlyTicks);
volatile uint32_t msTicks = 0;
void SysTick_Handler(void);

int main(void){
	// enable clock for Port A, Port B
	RCC->APB2ENR |= (3<<2);
	// setup PA1 as output PA2 as input
	GPIOA->CRL = 0x44444824;
	GPIOA->ODR = 0;
	SysTick_Config(9000);

	while(1){
		if((GPIOA->IDR & 0x4) == 0x4){
			GPIOA->ODR &= ~(1U<<1);
		}
		else{
			GPIOA->ODR |= 1U<<1;
		}
	}
}

void SysTick_Handler(void){
		msTicks++;
}

 
void delay(uint32_t dlyTicks){
      uint32_t curTicks;
      curTicks = msTicks;
      while ((msTicks - curTicks) < dlyTicks) ;
}