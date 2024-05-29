#include "stm32f10x.h"                  // Device header
#include <stdint.h>
#include "LCD.h"
#include "RCC_conf.h"
#include "TIM6_delay.h"

int main(void){
	// setup system clock as 72MHz
	SystemCLK_Init();
	// set APB2 = 72/2 = 36MHz
	APB2CLK_Init(2);
	// enable clock for Port A, Port B
	RCC->APB2ENR |= (3<<2);
	// enable AFIO and TIM6
	RCC->APB2ENR |= (1<<0) | (1U<<4);
	
	// GPIO_configuration
	LCD_PortConf();
	// setup TIM6 for delay_ms()
	TIM6delay_Init(36);
	// setup LCD
	LCD_Init();
	
	// display something with LCD
	LCD_DisplayString("Hello world!");
	
	
	while(1){

	}
}


