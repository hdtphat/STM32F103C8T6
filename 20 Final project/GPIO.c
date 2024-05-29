#include "stm32f10x.h"                  // Device header
#include "GPIO.h"
#include <stdint.h>

/*
	PA1~12 general output -> Segments LED
	PB6 alternate func ouput -> TIM4 CH1 PWM
	PB4~5 external interupt input -> Encoder CHA and CHB
	PB0 analog input -> Rheostat 
*/

void GPIO_Init(void){
	GPIOA->CRL = 0x22222224; // PA0~7
	GPIOA->CRH = 0x44422222; // PA8~15
	GPIOB->CRL = 0x4B444440; // PB0~7
	GPIOB->CRH = 0x44444444; // PB8~15
	GPIOA->ODR = 0;
	GPIOB->ODR = 0;
}

void SegLED_Display(uint32_t number){
	uint32_t display_digits[10] = {0xFC,0x60,0xDA,0xF2,0x66,0xB6,0xBE,0xE0,0xFE,0xF6};
	// create an array which contain components of input number
	uint16_t component[4];
	component[0] = (uint16_t)(number / 1000);
	component[1] = (uint16_t)(number%1000/100);
	component[2] = (uint16_t)(number%100/10);
	component[3] = (uint16_t)(number%10);
	// display each component into LED screen
	for(uint16_t i=0; i<4; i++){
		// PA1~4 high, PA5~12 low
		GPIOA->ODR |= (0x0F<<1);
		GPIOA->ODR &= ~(0xFFU<<5);
		// choosing which LED to turn on
		GPIOA->ODR &= ~(1UL<<(4-i));
		// display a digit on that LED
		GPIOA->ODR |= (uint32_t)(display_digits[component[3-i]])<<5;
		Delay_ms(5);
	}
}
