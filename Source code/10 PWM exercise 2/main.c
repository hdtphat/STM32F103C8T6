#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "stm32f10x.h"		// Device header
#include "Delay.h"
#include "LCD.h"

void PWM_Init(void);
void EXTI_Init(void);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
uint32_t current_speed = 50;
uint32_t last_speed = 0;
uint8_t START_STOP = 0;
uint8_t LCD_display[10] = {0x05, 0x03, 0x05, 0x00, 0x04, 0x05, 0x04, 0x05, 0x04, 0x04};

/*=============================================================================================================*/

int main (void){
	// Enable clock for PortA, PortB and AFIO
	RCC->APB2ENR |= (1U<<0)|(1U<<2)|(1U<<3);
	// Set APB1 prescaler = 1/8 = 72/8 = 9MHz
	RCC->CFGR |= (6<<8);
	// GPIO configuration
	GPIOA->CRL =	0x2444BBBB;		// PA7~PA0
	GPIOA->CRH =	0x44444444;		// PA15~PA8
	GPIOB->CRL =	0x44444444;		// PB7~PB0
	GPIOB->CRH =	0x44222222;		// PB15~PB8
	GPIOA->ODR =	0;
	GPIOB->ODR =	0;
	// Init systick timer
	SysTick_Config(9000);
	// Enable interupts
	__enable_irq();
	// Initialization
	PWM_Init();
	LCD_Init();
	EXTI_Init();
		
	// main loop
	while(1){
		// refresh LCD
		LCD_ClearAll();
		LCD_DisplayArray(LCD_display, 10);
		LCD_MoveCursor(8, 0);
		LCD_DisplayFloat((float)current_speed);
		Delay(500);
	}
}

/*=============================================================================================================*/

void PWM_Init(void){
/*
	fcnt 	= fAHB x APB1presc x 2 x TIM2presc
			= 72 x (1/8) x 2 x (1/18) = 1MHz
	
	fPWM = fCNT/(ARR)
	target: fPWM = 4kHz
	so: ARR must be 
*/
	RCC->CFGR |= (6<<8);			// APB1 prescaler = 8	
	// enable clock to TIM2
	RCC->APB1ENR |= (1U<<0);
	// Set TIM2 prescaler = 1/18
	TIM2->PSC = 18-1;     
	// Set ARR, CNT, CCR
	TIM2->ARR = 1000 - 1;
	TIM2->CNT = 0;
	TIM2->CCR1 = (50 *1000)/100;
	TIM2->CCR2 = (50 *1000)/100;   
	TIM2->CCR3 = (50 *1000)/100; 
	TIM2->CCR4 = (50 *1000)/100;  
	// Set PWM1 Mode for channel 1,2,3,4 (6: mode1, 7:mode2)
	TIM2->CCMR1 |= (6<<4) | (6<<12);	// chanel 1, 2
	TIM2->CCMR2 |= (6<<4) | (6<<12);	// chanel 3, 4
	// Enable Channel 1,2,3,4
	TIM2->CCER |= (1<<0)|(1<<4)|(1<<8)|(1<<12);
	// Enable counter
	TIM2->CR1 |= (1<<0);
}

void EXTI_Init(void){
/*	
	============================================
	||								STEPS FOLLOWED										||
	||																								||
	||	1. Enable the SYSCFG/AFIO bit in RCC register 							||
	||	2. Configure the EXTI configuration Register in the SYSCFG/AFIO	||
	||	3. Disable the EXTI Mask using Interrupt Mask Register (IMR)			||
	||	4. Configure the Rising Edge / Falling Edge Trigger						||
	||	5. Set the Interrupt Priority														||
	||	6. Enable the interrupt																||
	============================================
*/
//	1. configure EXTI (0: portA, 1: portB)
	AFIO->EXTICR[0] = 0x0111;	// EXTI1: pin3 ~ pin0				
//	2. disable  EXTI mask
	EXTI->IMR |= (1U<<1) | (1U<<0) | (1U<<2);		
//	3. Configure the Rising/Falling Edge Trigger
	EXTI->RTSR &= ~((1U<<1) | (1U<<0) | (1U<<2)); 													
	EXTI->FTSR |= (1U<<1) | (1U<<0) | (1U<<2);		
//	4. configure piority of ext interupt
	NVIC_SetPriority (EXTI0_IRQn, 1);
	NVIC_SetPriority (EXTI1_IRQn, 2);
	NVIC_SetPriority (EXTI2_IRQn, 3);
//	5. enable interupt
	NVIC_EnableIRQ (EXTI0_IRQn); 
	NVIC_EnableIRQ (EXTI1_IRQn); 
	NVIC_EnableIRQ (EXTI2_IRQn);
}

void EXTI0_IRQHandler(void){
//	1. clear flag by writing 1 to register
	uint8_t flag = 0;
	if(EXTI->PR & (1U<<flag)){
		EXTI->PR |= (1U<<flag);}
//	2. execute code: START/STOP
	START_STOP = ~START_STOP;
	if(START_STOP != 0){	//STOP
		last_speed = current_speed;
		current_speed = 0;
		TIM2->CCR1 = 0;
		TIM2->CCR2 = 0;   
		TIM2->CCR3 = 0; 
		TIM2->CCR4 = 0;  
	} else {						//START
		current_speed = last_speed;
		TIM2->CCR1 = last_speed*400;
		TIM2->CCR2 = last_speed*400;   
		TIM2->CCR3 = last_speed*400; 
		TIM2->CCR4 = last_speed*400;  
	}
}

void EXTI1_IRQHandler(void){
//	1. clear flag by writing 1 to register
	if(EXTI->PR & (1U<<1)){
		EXTI->PR |= (1U<<1);}
//	2. execute code: SPEED +
	current_speed += 5;
	TIM2->CCR1 += 2000;
	TIM2->CCR2 += 2000;   
	TIM2->CCR3 += 2000; 
	TIM2->CCR4 += 2000;  
}

void EXTI2_IRQHandler(void){
//	1. clear flag by writing 1 to register
	if(EXTI->PR & (1U<<2)){
		EXTI->PR |= (1U<<2);}
//	2. execute code: SPEED - 
	current_speed -= 5;
	TIM2->CCR1 -= 2000;
	TIM2->CCR2 -= 2000;   
	TIM2->CCR3 -= 2000; 
	TIM2->CCR4 -= 2000;  
}
