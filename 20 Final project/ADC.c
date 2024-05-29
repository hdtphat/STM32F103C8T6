#include "stm32f10x.h"                  // Device header
#include "ADC.h"
#include <stdint.h>

void ADC_Init(void) {
	// configure the ADC
	ADC1->CR2 |= 1 << 1;  		// Continuous mode
	ADC1->CR2 &= ~(1U << 11);	// Right alignment
	ADC1->SMPR2 |= (2 << 0);	// sampling time: 13.5 cycle for PB0
	ADC1->SQR3 |= (8 << 0);  	// Select channel IN8 (corresponding to PB0)
	ADC1->CR2 |= (1 << 20);  	// External trigger conversion mode for regular channels
	ADC1->CR2 |= (7 << 17);  	// select SWSTART as external trigger
	// EOC interrupt disable
	ADC1->CR1 &= ~(1UL << 5);
	// enable ADC
	ADC1->CR2 |= (1 << 0);
	// start calibration
	ADC1->CR2 |= (1 << 2);
	while (ADC1->CR2 & (1 << 2)); // wait until calibration completed
	// Start ADC
	ADC1->CR2 |= (1 << 22);
}
