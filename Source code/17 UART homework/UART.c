#include "stm32f10x.h"                  // Device header
#include <stdint.h>
#include "UART.h"
#include <string.h>
#include <stdio.h>

/*
	===================
	||			Pin map					||
	||	PA9			UART1_Tx		||
	||	PA10			UART1_Rx		||
	||	PA2			UART2_Tx		||
	||	PA3			UART2_Rx		||
	===================
*/

void UART1_PortInit(void){
	// enable clock to GPIOA
	RCC->APB2ENR |= (1U<<2);
	// clear pins PA9 and PA10 to 0
	GPIOA->CRH &= ~(0xFFU<<4);
	// PA9 - Tx
	GPIOA->CRH |= (3U<<4); // MODE: output with max speed 50MHz
	GPIOA->CRH |= (2U<<6); // CNF: alternate func push-pull
	// PA10 - Rx
	GPIOA->CRH &= ~(3U<<8); // MODE: input mode
	GPIOA->CRH |= (2U<<10); // CNF: input with pull-up/pull-down
	// Pull Up for PA10
	GPIOA->ODR |= 1<<10;  
}

void UART1_Setup(void){
	// enable clock to AFIO and USART1
	RCC->APB2ENR |= (1U<<0) | (1U<<14);
	// disable UART1
	USART1->CR1 &= ~(1U<<13); 
	// word length: 1 Start bit, 8 Data bits, n Stop bit
	USART1->CR1 &= ~(1U<<12);
	// Parity control disable
	USART1->CR1 &= ~(1U<<10);
	// STOP bits: 1 stop bit
	USART1->CR2 &= ~(3U<<12);
	//  desired baud rate: 9600
	USART1->BRR = (234<<4) | (6<<0);
	// enable Receive/Transmit mode
	USART1->CR1 = (3<<2); 
	// enable UART1
	USART1->CR1 |= (1U<<13); 
}

void UART1_SendChar (char c){
	 // Load the Data
	USART1->DR = c;  
	// Wait for TC to SET.. This indicates that the data has been transmitted
	while (!(USART1->SR & (1<<6)));  
}

uint8_t UART1_GetChar (void){
	uint8_t Temp;
	// Wait for RXNE to SET.. This indicates that the data has been Received
	while (!(USART1->SR & (1<<5)));  
	// Read the data. 
	Temp = USART1->DR;  
	return Temp;
}

void UART1_SerialPrint(char *input) {
    int length = strlen(input);
    for (int i = 0; i < length; i++) {
        if (input[i] != '\0') {
            UART1_SendChar(input[i]);
        }
    }
}

void UART1_SerialPrintln(char *input) {
    int length = strlen(input);
    for (int i = 0; i < length; i++) {
        if (input[i] != '\0') {
            UART1_SendChar(input[i]);
        }
    }
	UART1_SendChar('\n');
}

void UART1_SerialRead(char rx_buffer[], int buffer_lenght){
	uint32_t text_len = 0; // current text lenght
	char ch; // current character
	do {
		ch = UART1_GetChar();
		if((text_len<buffer_lenght) & ((ch>=0x20) & (ch<0x7F))){
			rx_buffer[text_len] = ch;
			text_len++;
		}
	}
	while((ch!=13) && (ch!=27));
	text_len = 0;
}

void UART2_PortInit(void){
	// enable clock to GPIOA
	RCC->APB2ENR |= (1U<<2);
	// clear pins PA2 and PA3 to 0
	GPIOA->CRL &= ~(0xFFU<<8);
	// PA2 - Tx
	GPIOA->CRL |= (3U<<8); // MODE: output with max speed 50MHz
	GPIOA->CRL |= (2U<<10); // CNF: alternate func push-pull
	// PA3 - Rx
	GPIOA->CRL &= ~(3U<<12); // MODE: input mode
	GPIOA->CRL |= (2U<<14); // CNF: input with pull-up/pull-down
	// Pull Up for PA3: set Rx to high
	GPIOA->ODR |= 1<<3;  
}

void UART2_Setup(void){
	// enable clock to AFIO and USART2
	RCC->APB2ENR |= (1U<<0);
	RCC->APB1ENR |= (1U<<17);
	// disable UART2
	USART2->CR1 &= ~(1U<<13); 
	// word length: 1 Start bit, 8 Data bits, n Stop bit
	USART2->CR1 &= ~(1U<<12);
	// Parity control disable
	USART2->CR1 &= ~(1U<<10);
	// STOP bits: 1 stop bit
	USART2->CR2 &= ~(3U<<12);
	//  desired baud rate: 9600
	USART2->BRR = (234<<4) | (6<<0);
	// enable Receive/Transmit mode
	USART2->CR1 = (3<<2); 
	// enable UART2
	USART2->CR1 |= (1U<<13); 
}

void UART2_SendChar (char c){
	 // Load the Data
	USART2->DR = c;  
	// Wait for TC to SET.. This indicates that the data has been transmitted
	while (!(USART2->SR & (1<<6)));  
}

uint8_t UART2_GetChar (void){
	uint8_t Temp;
	// Wait for RXNE to SET.. This indicates that the data has been Received
	while (!(USART2->SR & (1<<5)));  
	// Read the data. 
	Temp = USART2->DR;  
	return Temp;
}

void UART2_SerialPrint(char *input) {
    int length = strlen(input);
    for (int i = 0; i < length; i++) {
        if (input[i] != '\0') {
            UART2_SendChar(input[i]);
        }
    }
}

void UART2_SerialPrintln(char *input) {
    int length = strlen(input);
    for (int i = 0; i < length; i++) {
        if (input[i] != '\0') {
            UART2_SendChar(input[i]);
        }
    }
	UART2_SendChar('\n');
}
