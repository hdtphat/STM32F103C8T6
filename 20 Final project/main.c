#include "stm32f10x.h"
#include "Clock.h"
#include "GPIO.h"
#include "Timers.h"
#include "EXTI.h"
#include "ADC.h"
#include "NVIC.h"
#include <stdint.h>
#include <stdlib.h>

#define Sample_rate 0.1 // motor speed sample rate 100ms
#define ADC_max 4000
#define ADC_min 2050
#define PWM_max 100
#define PWM_min 0

uint32_t Dutty_cycle = 0; // speed 0%
uint32_t Encoder_count = 0; // current encoder count
uint32_t Encoder_1_round = 120; // 120 encoder pulse per round
uint32_t Motor_speed; // current motor speed (rpm)
uint32_t ADC_value = ADC_min; // PB0 ADC value
uint8_t test_mode = 0;

int main(void){
	Clock_Init();
	Enable_peripheralS();
	GPIO_Init();
	Enable_Interupt();
	TIM6_Init();
	TIM4_Init();
	TIM2_Init();
	TIM3_Init();
	EXTI_Init();
	ADC_Init();
	
	while(1);
}

void EXTI4_IRQHandler(void){
	if(EXTI->PR & (1U<<4)){
		// clear flag
		EXTI->PR |= (1U<<4);
		// increse count
		Encoder_count++;
	}
}

void EXTI9_5_IRQHandler(void){
	if(EXTI->PR & (1U<<5)){
		// clear flag
		EXTI->PR |= (1U<<5);
		// increse count
		Encoder_count++;
	}
}

void TIM2_IRQHandler(void){
//	// test mode
//	TIM2->SR = 0;
//	test_mode = ~test_mode;
//	if(test_mode == 0)GPIOA->ODR |= 1<<1;
//	else if(test_mode != 0)GPIOA->ODR &= ~(1U<<1);
	
	// clear flag
	TIM2->SR = 0;
	// calculate velocity and reset encoder counting value
	Motor_speed = (uint32_t)(Encoder_count/Encoder_1_round)*(uint32_t)(60.0/Sample_rate);
	Encoder_count = 0;
}

void TIM3_IRQHandler(void){
//	// test mode
//	TIM3->SR = 0;
//	test_mode = ~test_mode;
//	if(test_mode == 0)GPIOA->ODR |= 1<<1;
//	else if(test_mode != 0)GPIOA->ODR &= ~(1U<<1);
	
	// clear flag
	TIM3->SR = 0;
	// get ADC value
	ADC_value = ADC1->DR;
	if(ADC_value > ADC_max) ADC_value = ADC_max;
	else if(ADC_value < ADC_min) ADC_value = ADC_min;
	// calculate and config dutty cycle
	Dutty_cycle = ((ADC_value-ADC_min)*(PWM_max-PWM_min)) / (ADC_max-ADC_min);
	TIM4->CCR1 = (uint16_t)(Dutty_cycle*20000/100);
}

