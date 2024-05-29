#include "stm32f10x.h"                  // Device header
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "UART.h"

void delay(uint32_t dlyTicks);
void SysTick_Handler(void);

volatile uint32_t msTicks = 0;
uint8_t rx_buffer[20]; //R233G100B230

int main (void){
	// enable clock to GPIOA
	RCC->APB2ENR |= (1U<<2);
	// enable clock to AFIO and USART1
	RCC->APB2ENR |= (1U<<0) | (1U<<14);
	// APB2 prescaler = 2 -> PCLK2 = 36MHz
	RCC->CFGR |= (4U<<11);     
	
	UART1_PortInit();
	UART1_Setup();
	SysTick_Config(9000);
	
	UART1_SerialPrintln("hello world!");
	while(1){
		UART1_SerialRead(&rx_buffer, 12);
		UART1_SerialPrintln("input: ");
		UART1_SerialPrintln(&rx_buffer);
		memset(rx_buffer, 0, sizeof(rx_buffer));
	}
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
