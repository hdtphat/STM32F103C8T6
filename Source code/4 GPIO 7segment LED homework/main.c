#include "stm32f10x.h"
#include <stdint.h>
#include <math.h>
#include <stdlib.h>

/*
			  15
			===
	10	  ||   9  ||	14
			===
	11	  ||  12  ||	13
			===		o
						8

*/


void delay(uint32_t dlyTicks);
volatile uint32_t msTicks = 0;
void SysTick_Handler(void);

void show_number(int number){
	// array
	uint32_t display_digits[10] = {0x00FC, 0x0060, 0x00DA, 0x00F2, 0x0066, 0x00B6, 0x00BE, 0x00E0, 0x00FE, 0x00F6};
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
		GPIOA->ODR |= (uint32_t)(display_digits[component[3-i]])<<5;
		delay(5);
	}
}

int main(void){
	// enable clock for Port A
	RCC->APB2ENR |= (1<<2);
	// setup PA1~PA4 and PA8~PA15 as output
	GPIOA->CRL = 0x22222222;
	GPIOA->CRH = 0x22222222;
	// turn off all led
	GPIOA->ODR = 0;   			
	// set systick timer, interrupt each 1ms
	SysTick_Config(9000);
	
	// main loop
	while(1){
		int randomNumber = rand() % 9999 + 1;
		for(int i=0; i<500; i++) show_number(randomNumber);
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
