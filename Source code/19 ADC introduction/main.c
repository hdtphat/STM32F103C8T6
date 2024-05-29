#include "stm32f10x.h"                  // Device header
#include <stdint.h>

void Clock_init(void);
void Enable_peripherals(void);
void GPIO_init(void);
void ADC_Init (void);

int main(void){
	Clock_init();
	Enable_peripherals();
	GPIO_init();
	ADC_Init();
	
	while(1){
		
	}
}

void Clock_init(void){
	// system clock 72MHz
	// APB2 clock = 72/1 = 72MHz
	RCC->CFGR &= ~(7U<<11);
	// ADC clock must not exceed 14MHz
	// ADC clock = 72/6 = 12MHz
	RCC->CFGR |= (2U<<14);
}

void Enable_peripherals(void){
	// enable clock to GPIOA, GPIOB, AFIO
	RCC->APB2ENR |= (1U<<0) | (1U<<2) | (1U<<3);
	// enable clock to ADC1, ADC2, ADC3
	RCC->APB2ENR |= (1U<<9) | (1U<<10) | (1U<<15);
}

void GPIO_init(void){
	
}

void ADC_Init (void){
	// ADC prescaler = PCLK2/6 = 72/6 = 12MHz
	RCC->CFGR |= (2U<<14);
	// Set the Scan Mode and Resolution in the Control Register 1 (CR1)	
	ADC1->CR1 = (1<<8);    // SCAN mode enabled
	ADC1->CR1 &= ~(1U<<24);   // 12 bit RES
	// Set the Continuous Conversion, EOC, and Data Alignment in Control Reg 2 (CR2)
	ADC1->CR2 |= (1<<1);     // enable continuous conversion mode
	ADC1->CR2 |= (1<<10);    // EOC after each conversion
	ADC1->CR2 &= ~(1U<<11);   // Data Alignment RIGHT
	// Set the Sampling Time for the channels	
	ADC1->SMPR2 &= ~((1U<<3) | (1U<<12));  // Sampling time of 3 cycles for channel 1 and channel 4
	// Set the Regular channel sequence length in ADC_SQR1
	ADC1->SQR1 |= (1<<20);   // SQR1_L =1 for 2 conversions
}
