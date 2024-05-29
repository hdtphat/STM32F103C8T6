#include "stm32f10x.h"                  // Device header
#include <stdint.h>

void ADC1_2_IRQHandler (void);
void clockInit(void);
void ADC_Init(void);

uint16_t adc_value;

int main(void){
	clockInit();
	// convert PA1, PA5 to analog input (reset state)
	GPIOA->CRL &= ~((1UL<<22)|(1UL<<6)); 
	
	ADC_Init();
	 // wait until ADC ready
	
	while(1){}
}

/*======================================================================================================*/

void ADC1_2_IRQHandler (void){
	// read ADC value and clear EOC flag
	adc_value = ADC1->DR;
}
void clockInit(void){
	// enable GPIOA, AFIO, ADC clock
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_ADC1EN;
	 
}
void ADC_Init(void){	
	// configure the ADC
	ADC1->CR2 &= ~(1U<<1);							// Single mode
	ADC1->CR2 &= ~(1U<<11);						// Right alignment	
	ADC1->SMPR2 = 0;							// sampling time: 1.5 cycle 
	ADC1->SQR3 = 0x000000A1;				// Select channel IN1 and IN5
	ADC1->CR1 |= (1<<8);							// Enable scan mode
	ADC1->CR2 |= (7<<17);							// select SWSTART as external trigger
	// EOC interrupt enable
	ADC1->CR1 |= (1<<5); 
	__enable_irq();
	NVIC_EnableIRQ(ADC1_2_IRQn);
	// start calibration
	ADC1->CR2 |= (1<<2);
	while( ADC1->CR2 & (1<<2));		// wait until calibration completed
	ADC1->CR2 |= (1<<0);							// enable ADC 
}
