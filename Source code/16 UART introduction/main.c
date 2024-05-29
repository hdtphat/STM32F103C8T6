#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void clock_Init(void);
void UART1_SendChar(char c);
void delay(uint32_t dlyTicks);
void SysTick_Handler(void);

volatile uint32_t msTicks = 0;

int main(void){
	// clock initialization
	clock_Init();
	
	// GPIO init 
	// PA9: Altenate function output 50MHz, PA10: Input with pull up/down
	GPIOA->CRH = 0;
	GPIOA->CRH |= (3<<4);
	GPIOA->CRH |= (2<<6);
	GPIOA->CRH &= ~(3UL<<8);
	GPIOA->CRH |= (2<<10);
	
	GPIOA->ODR = 1<<10;    // pull up for PA10 (UART1 RX)
	
	// UART configuration
	USART1->CR1 = 0;  // 8-N-1
	USART1->BRR = (234<<4) | (6<<0);
	USART1->CR1 = (3<<2);   // enable Receive/Transmit mode
	USART1->CR1 |= (1<<13); // enable USART1
	
	SysTick_Config(9000);
	
	while(1){
		UART1_SendChar('H');
		UART1_SendChar('e');
		UART1_SendChar('l');
		UART1_SendChar('l');
		UART1_SendChar('o');
		delay(1000);
	}
}

void clock_Init(void){
	// enable port A,AFIO, UART1
	RCC->APB2ENR |= (1<<2);
	RCC->APB2ENR |= (1<<0);
	RCC->APB2ENR |= (1<<14);
	// set APB2 prescaler
	RCC->CFGR |= (4<<11);     // APB2 prescaler = 2 -> PCLK2 = 36MHz
}

void UART1_SendChar(char c){
	USART1->DR = c;
	while(!(USART1->SR & (1<<6)));  // Wait until TC bit = 1
}

void SysTick_Handler(void){
	 /* Increment counter necessary in Delay()*/
		msTicks++;
}
 
void delay(uint32_t dlyTicks){
      uint32_t curTicks;
      curTicks = msTicks;
      while ((msTicks - curTicks) < dlyTicks) ;
}
