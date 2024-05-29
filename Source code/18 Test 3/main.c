#include "stm32f10x.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Delay.h"
#include "LCD.h"
#include "UART.h"

void Clock_Init(void);
void TIM3_Init(void);
void TIM3_IRQHandler(void);
void TIM4_Init(void);
void TIM2_Init(void);
void TIM2_IRQHandler(void);
void EXTI_Init(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);

uint8_t SPEED_MODE = 0; // 0, 20, 60, 100%
uint8_t SPEED_CHANGE = 0; // 1 -> change speed
uint8_t RHYMT_MODE = 0; // 0xFF -> change speed every 3s
uint8_t TIME = 0; // turn off 

int main (void){
	// Enable Clock
	Clock_Init();
	// GPIO configuration
	GPIOA->CRL =	0x44444444;		// PA1~3 input
	GPIOB->CRL =	0x44222222;		// PB0~PB5 output, PB6~7 PWM
	GPIOB->CRH =	0x44444244;		// PB10 output
	GPIOA->ODR =	0;
	GPIOB->ODR =	0;
	// Enable interupt
	__enable_irq();	
	// Peripherals configuration
	TIM3_Init();
	TIM2_Init();
	TIM4_Init();
	EXTI_Init();
	SysTick_Config(9000);
	
	while(1){
		// only change speed after bt1 was pushed
		if(SPEED_CHANGE !=0){
			switch (SPEED_MODE){
				case 0: // OFF
					TIM4->CCR1 = 0; // PWM = 0%
					GPIOB->ODR &= ~(1U<<10); // ENA = 0
					GPIOB->ODR &= ~(7U<<0); // turn off all speed LED
					break;
				case 1: // 20% speed
					TIM4->CCR1 = 200; // PWM = 20%
					GPIOB->ODR |= (1U<<10); // ENA = 1
					GPIOB->ODR &= ~(7U<<0); // turn off all speed LED
					GPIOB->ODR |= (1U<<0); // turn on 20% LED
					break;
				case 2: // 60% speed
					TIM4->CCR1 = 600; // PWM = 60%
					GPIOB->ODR |= (1U<<10); // ENA = 1
					GPIOB->ODR &= ~(7U<<0); // turn off all speed LED
					GPIOB->ODR |= (1U<<1); // turn on 60% LED
					break;
				case 3: // 100% speed
					TIM4->CCR1 = 999; // PWM = 100%
					GPIOB->ODR |= (1U<<10); // ENA = 1
					GPIOB->ODR &= ~(7U<<0); // turn off all speed LED
					GPIOB->ODR |= (1U<<2); // turn on 100% LED
					break;
			}
			SPEED_CHANGE = 0;
		}		
	}
}

/*========================================================================================================================================*/

void Clock_Init(void){
	// Enable clock for PortA, PortB and AFIO
	RCC->APB2ENR |= (1U<<0)|(1U<<2)|(1U<<3);
	// APB1 prescaler = 8 -> PCLK1 = 9MHz
	RCC->CFGR |= (6<<8);
	// APB2 prescaler = 2 -> PCLK2 = 36MHz
	RCC->CFGR |= (4U<<11);  
	// Enable Timer4 and  timer3 timer2
	RCC->APB1ENR |= (1U<<2) | (1U<<1) | (1U<<0);
	// TIM4 prescaler = 18 -> Ftim4 = 9x2/18 = 1MHz
	TIM4->PSC = 18-1;
	// TIM3 prescaler = 1800 -> Ftim3 = 9x2/1800 = 0.01MHz
	TIM3->PSC = 1800-1;
	// TIM2 prescaler = 3600 -> Ftim5 = 9x2/3600 = 0.005MHz
	TIM2->PSC = 3600-1;
}

void TIM4_Init(void){
	// Set ARR, CNT, CCR
	TIM4->ARR = 1000-1;
	TIM4->CNT = 0;
	// Initial duty cycle = 0
	TIM4->CCR1 = 0;   
	TIM4->CCR2 = 0;  
	// Set PWM1 Mode for channel 1 2
	TIM4->CCMR1 |= (6<<4);			// Channel 1
	TIM4->CCMR1 |= (6<<12);			// Channel 2
	// Enable Channe 1,2
	TIM4->CCER |= (1<<0)|(1<<4);	
	// Enable counter
	TIM4->CR1 |= (1<<0);
}

void TIM2_Init(void){
	/*
		F after TIM2 prescaler = 0.005MHz
		so 1 count take T = 1/F = 200us
		15000 count would take 3s
		25000 count take 5s
		35000 count take 7s
	
		=> F=0.005 and  ARR=15000 -> interupt every 3s
		=> F=0.005 and  ARR=25000 -> interupt every 5s
		=> F=0.005 and  ARR=35000 -> interupt every 7s
	*/
	// Set ARR, CNT, CCR
	TIM2->ARR = 35000-1;
	TIM2->CNT = 0;
	// enable timer interrupt
	TIM2->DIER |= (1U<<0);
	NVIC_SetPriority (TIM2_IRQn, 4);
	NVIC_EnableIRQ (TIM2_IRQn);
	// Disable counter
	TIM2->CR1 &= ~(1U<<0);
}

void TIM2_IRQHandler(void){
	// clear interrupt flag
	TIM2->SR = 0;
	// turn off fan
	SPEED_MODE = 0;
	SPEED_CHANGE = 1;
	GPIOB->ODR &= ~(7U<<3);
}	

void TIM3_Init(void){
	/*
		F after TIM3 prescaler = 0.01MHz
		so 1 count take T = 1/F = 100us
		30000 count would take 3s
		=> F=0.01 and  ARR=30000 -> interupt every 3s
	*/
	// Set ARR, CNT, CCR
	TIM3->ARR = 30000-1;
	TIM3->CNT = 0;
	// enable timer interrupt
	TIM3->DIER |= (1U<<0);
	NVIC_SetPriority (TIM3_IRQn, 0);
	NVIC_EnableIRQ (TIM3_IRQn);
	// Disable counter
	TIM3->CR1 &= ~(1U<<0);
}

void TIM3_IRQHandler(void){
	// clear interrupt flag
	TIM3->SR = 0;
	// change speed automatically
	SPEED_MODE++;
	if(SPEED_MODE > 3){SPEED_MODE = 0;}
	SPEED_CHANGE = 1;
}

void EXTI_Init(void){
	// Configure EXTI (0: portA, 1: portB)
	AFIO->EXTICR[0] = 0x0000;	// pin3 ~ pin0, external interrupt for PA1, 2, 3
	// Disable  EXTI mask
	EXTI->IMR |= (1U<<1)|(1U<<2)|(1U<<3);														
	// Configure the Rising/Falling Edge Trigger
	EXTI->RTSR &= ~((1U<<1)|(1U<<2)|(1U<<3)); 													
	EXTI->FTSR |= (1U<<1)|(1U<<2)|(1U<<3);
	// Configure piority of ext interupt
	NVIC_SetPriority (EXTI3_IRQn, 1);
	NVIC_SetPriority (EXTI1_IRQn, 2);
	NVIC_SetPriority (EXTI2_IRQn, 3);
	// Enable interupt
	NVIC_EnableIRQ (EXTI3_IRQn);
	NVIC_EnableIRQ (EXTI1_IRQn);
	NVIC_EnableIRQ (EXTI2_IRQn);
}

void EXTI1_IRQHandler(void){
	// Clear flag by writing 1 to register
	if(EXTI->PR & (1U<<1)){
		EXTI->PR |= (1U<<1);
		// change speed
		SPEED_MODE++;
		if(SPEED_MODE > 3){SPEED_MODE = 0;}
		SPEED_CHANGE = 1;
	}
}

void EXTI2_IRQHandler(void){
	// clear flag by writing 1 to register
	if(EXTI->PR & (1U<<2)){
		EXTI->PR |= (1U<<2);
		// enable timer2
		TIME ++;
		if(TIME > 3){TIME = 0;}
		switch (TIME){
			case 0: 
				TIM2->ARR = 0;
				GPIOB->ODR &= ~(7U<<3);
			break;
				case 1: 
				TIM2->ARR = 15000;
				GPIOB->ODR &= ~(7U<<3);
				GPIOB->ODR |= (1<<3);
				break;
				case 2: 
				TIM2->ARR = 25000;
				GPIOB->ODR &= ~(7U<<3);
				GPIOB->ODR |= (1<<4);
				break;
			case 3:
				TIM2->ARR = 35000;
				GPIOB->ODR &= ~(7U<<3);
				GPIOB->ODR |= (1<<5);
				break;
		}
		TIM2->CNT = 0;
		TIM2->CR1 |= (1<<0);
	}
}

void EXTI3_IRQHandler(void){
		// clear flag by writing 1 to register
	if(EXTI->PR & (1U<<3)){
		EXTI->PR |= (1U<<3);
		// change RHYMT_MODE
		RHYMT_MODE = ~RHYMT_MODE;
		if(RHYMT_MODE == 0){ // no Rhythm
			TIM3->CR1 &= ~(1U<<0); // disable counter
			TIM3->CNT = 0; // reset counter
		} else {
			TIM3->CR1 |= (1U<<0); // enable counter
		}
	}
}
