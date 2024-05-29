#include "stm32f10x.h"                  // Device header
#include <stdint.h>
#include "Clock.h"

void Clock_Init(void){
	/*
		system clock 72MHz
		APB1 clock 9MHz
	*/
	// Set APB1 prescaler to 1/8 
	RCC->CFGR |= (6<<8);  
}

void Enable_peripheralS(void){
	// enable GPIOA, GPIOB, AFIO
	RCC->APB2ENR |= (1U<<0) | (1U<<2) | (1U<<3);
	// enable TIM6, TIM7, TIM3 and TIM2
	RCC->APB1ENR |= (1U<<4) | (1U<<5) | (1U<<1) | (1U<<0);
	// enable ADC1
	RCC->APB2ENR |= (1U<<9);
}
