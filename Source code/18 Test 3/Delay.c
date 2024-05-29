#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include <stdint.h>

volatile uint32_t msTicks = 0;
volatile uint16_t Tick_Counter = 0;

void SysTick_Handler(void){
	 /* Increment counter necessary in Delay()*/
		msTicks++;
}

void Delay(uint32_t dlyTicks){
      uint32_t curTicks;
      curTicks = msTicks;
      while ((msTicks - curTicks) < dlyTicks) ;
}

void TIM6_Init(void){
	// enable clock to TIM6
	RCC->APB1ENR |= 1<<4;
	// set TIM4 prescaler to 1/18 => clock to timer = (9x2)/18 = 1MHz => 1 count cost 1us => 1000 count = 1ms
	TIM6->PSC = 18-1;
	// config max count value
	TIM6->ARR = 0xFFFF;
	// reset counter
	TIM6->CNT = 0;
	// enable counter 
	TIM6->CR1 |= (1<<0);
}

void Delay_us(uint32_t us){
	TIM6->CNT = 0;
	while(TIM6->CNT < us);
}

void Delay_ms(uint32_t ms){
	for(uint32_t i=0; i<ms; i++){
		Delay_us(1000);
	}
}

void Delay_s(uint32_t s){
	for(uint32_t i=0; i<s; i++){
		Delay_ms(1000);
	}
}
