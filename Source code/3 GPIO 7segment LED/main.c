#include "stm32f10x.h"                  // Device header
#include <stdint.h>
#include <math.h>
#include <stdlib.h>


uint16_t display_digits [10] = {0x01F8, 0x00C0, 0x01B4, 0x01E4, 0x00CC, 0x016C, 0x017C, 0x01C0, 0x01FC, 0x01EC};

void delay(uint32_t dlyTicks);
volatile uint32_t msTicks = 0;
void SysTick_Handler(void);

void show_number(int number){
	// create an array which contain components of input number
	uint16_t component[4];
	for (uint16_t i = 0; i < 4; i++) {
		component[i] = number / (uint16_t)pow(10, 3 - i);
		number = number % (uint16_t)pow(10, 3 - i);
	}
	// display each component into LED screen
	for(int i=0; i<4; i++){
		GPIOB->ODR = ~(1UL<<i);
		GPIOA->ODR |= display_digits[component[3-i]]; // chua sua
		delay(5);
		GPIOA->ODR &= 0;
	}
}

int main(void){
	// enable clock for Port A, Port B
	RCC->APB2ENR |= (3<<2);
	// setup PA1~PA3 as output (2 out, 4 in)
	GPIOA->CRL = 0x22222224; // PA0~PA7 
	GPIOA->CRH = 0x44444442; // PA8~PA15 
	// setup PB0~PB3 as output
	GPIOB->CRL = 0x44442222; // P
	// turn off all led
	GPIOA->ODR = 0;   			
	// set systick timer, interrupt each 1ms
	SysTick_Config(9000);
	
	// main loop
	while(1){
		// create an array contain number's components
		int a = rand()%10000;
		for(int i=0; i<50; i++){ // 1000ms
			show_number(a); // 20ms
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